//
//  OTP_dec.c
//  description: sends a request to OTP_dec_d for text decryption. sends a ciphertext and a key
//  for the daemon to decipher the text. should not connect to OTP_enc_d under any circumstances.
//  The program also checks the ciphertext and key for validity, and verifies that the server it connects
//  to is a decryption server
//  Created by Brian Sia on 8/7/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include "otp_util.h"
//#include "OTP_util.c"

#define CLIENT 0 //1 for encryption, and 0 for decryption

//main()
//establish a connection and sends a request to OTP_dec_d.c for text decryption, with the ciphertext and key
//The following shows the abstraction of how a client establishes a connection with the server.
//1.) Create a socket.
//2.) Set up the server address.
//3.) Connect to the server.
//4.) Read/write data.
//5.) Shut down connetion
//preconditions:
//postconditions: receives a plaintext from the decrypting daemon
int main(int argc, char **argv)
{
	int sockFD;				
	int portNum;		
	struct sockaddr_in servAddr;
	struct hostent *server;

	struct stat ciphertextStat, keyStat;
	// file variable for checking length
	long ctextSize = 0;		
	long keySize = 0;	
	int ctextFD, keyFD;// file descriptors for ciphertext and key files
	int readCtext, readKey;	// verify that read works

	int clientType = CLIENT;// encrypt is 1, decrypt is 0
	int serverType;
	int sendBytes, receiveBytes;

	char buffer[BUFFSIZE];
	bzero(buffer, sizeof(buffer));

	// check for correct number of arguments
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s ciphertext key port\n", argv[0]);
		exit(0);
	}

	// check file sizes for ciphertext file and key
	stat(argv[1], &ciphertextStat);
	ctextSize = ciphertextStat.st_size;
	stat(argv[2], &keyStat);
	keySize = keyStat.st_size;

	if (keySize < ctextSize)//if key is too short
	{
		fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
		exit(1);
	}

	// open ciphertext file
	ctextFD = open(argv[1], O_RDONLY);
	if (ctextFD == -1)
	{
		fprintf(stderr, "Error: cannot open ciphertext file '%s'\n", argv[1]);
		exit(1);
	}

	// verify contents of ciphertext file
	char ciphertext[ctextSize];
	bzero(ciphertext, sizeof(ciphertext));
	readCtext = read(ctextFD, ciphertext, ctextSize);
	if (readCtext < 0)
	{
		fprintf(stderr, "Error: cannot read ciphertext file '%s'\n", argv[1]);
		exit(1);
	}

	ciphertext[ctextSize - 1] = '\0';
		// replace trailing newline with null char
	if (checkString(ciphertext) != 1)
	{
		fprintf(stderr, "Error: '%s' contains invalid characters\n", argv[1]);
		exit(1);
	}

	// open key file
	keyFD = open(argv[2], O_RDONLY);
	if (keyFD == -1)
	{
		fprintf(stderr, "Error: cannot open key file '%s'\n", argv[2]);
		exit(1);
	}

	// verify contents of key file
	char key[keySize];
	bzero(key, sizeof(key));
	readKey = read(keyFD, key, keySize);
	if (readKey < 0)
	{
		fprintf(stderr, "Error: cannot read key file '%s'\n", argv[2]);
		exit(1);
	}
	key[keySize - 1] = '\0';// replace trailing newline with null char
	if (checkString(key) != 1)
	{
		fprintf(stderr, "Error: '%s' contains invalid characters\n", argv[2]);
		exit(1);
	}

	// create the socket and get file descriptor
	// use AF_INET for IPv4 domain
	// use SOCK_STREAM for TCP
	// use 0 for IP protocol (default protocol)
	if ((sockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "Error: %s socket creation failed\n", argv[0]);
		exit(1);
	}

	// set up server address, and connect client to server
	server = gethostbyname("localhost");
	if (server == NULL)
	{
		fprintf(stderr, "Error: %s could not resolve server host name\n", argv[0]);
		exit(1);
	}

	// set up socket address for client-server connection
	portNum = atoi(argv[3]);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portNum);
	memcpy(&servAddr.sin_addr, server->h_addr_list[0], server->h_length);

	// connect the socket to the server
	if (connect(sockFD, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	{
		//if OTP_dec_d.c daemon is not found
		fprintf(stderr, "Error: %s could not find otp_dec_d on port %d\n", argv[0], portNum);
		exit(2);
	}

	// check that both server and client are decryption type
	// both sides do this check, but server sends its response to the client
	// client is the one to close the connection if there is a type mismatch
	// verify that serverType == clientType
	sendBytes = send(sockFD, &clientType, sizeof(clientType), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], "server", 1);
	}

	// receive server type from server
	receiveBytes = recv(sockFD, &serverType, sizeof(serverType), 0);
	if (receiveBytes < 0)
	{
		receiveError(argv[0], "server", 1);
	}
	// server at port is not decrypt type
	if (clientType != serverType || clientType != 0 || serverType != 0)
	{
		fprintf(stderr, "Error: %s cannot find otp_dec_d on port %d\n", argv[0], portNum);
		close(sockFD);
		exit(2);
	}
	// receive server reponse to type check
	bzero(buffer, sizeof(buffer));
	receiveBytes = recv(sockFD, buffer, sizeof(buffer), 0);
	if (receiveBytes < 0)
	{
		receiveError(argv[0], "server", 1);
	}
	if (strcmp(buffer, "type match") != 0)
	{
		fprintf(stderr, "Error: %s cannot find otp_dec_d on port %d\n", argv[0], portNum);
		close(sockFD);
		exit(2);
	}

	// file sizes verified and file contents verified by otp_enc
	// connected and both client and server are decrypt type

	// server will do another file size check on its end and send response back
	// client is the one to close connection if key size is too small
	// send ciphertext file size
	sendBytes = send(sockFD, &ctextSize, sizeof(ctextSize), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], "server", 1);
	}
	// send key file size
	sendBytes = send(sockFD, &keySize, sizeof(keySize), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], "server", 1);
	}
	
	// pass both client and server decrypt type and key size checks

	// send ciphertext to server
	sendBytes = send(sockFD, ciphertext, sizeof(ciphertext), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], "server", 1);
	}

	// send key to server
	sendBytes = send(sockFD, key, sizeof(key), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], "server", 1);
	}

	// server decrypts ciphertext and sends plaintext back
	char plaintext[ctextSize];
	bzero(plaintext, sizeof(plaintext));
	receiveBytes = recv(sockFD, plaintext, sizeof(plaintext), MSG_WAITALL);
	if (receiveBytes < 0)
	{
		receiveError(argv[0], "server", 1);
	}

	// print plaintext to stdout, and add trailing newline
	fprintf(stdout, "%s\n", plaintext);

	close(sockFD);

	return 0;
}
