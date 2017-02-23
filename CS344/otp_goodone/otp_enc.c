/******************************************************************************
otp_enc.c
CS344-400
Program 4: OTP
OTP is a client/server simulation of a one-time pad encryption system.
	otp_enc is a program that connects to otp_enc_d and asks it to encrypt
	plaintext, using a passed-in plaintext and key. It uses the same syntax and
	usage as otp_dec. It should not be able to connect to otp_dec_d.
	otp_enc checks if the key or plaintext files have bad characters, and
	verifies that the server it is connecting to is an encryption server.
	Usage: otp_enc plaintext key port
******************************************************************************/

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
#include "otp_common.h"

#define CLIENT_TYPE 1		// encrypt == 1; decrypt == 0

int main(int argc, char **argv)
{
	int sockfd;						// socket file descriptor
	int portNum;					// port number
	struct sockaddr_in servAddr;	// internet address struct
	struct hostent *server;			// host computer

	struct stat plaintextStat, keyStat;	// file statistics for checking length
	long ptextSize, keySize;		// for storing file lengths
	int ptextfd, keyfd;		// file descriptors for plaintext and key files
	int readPtext, readKey;			// verify that read works

	int clientType = CLIENT_TYPE;	// encrypt == 1; decrypt == 0
	int serverType;
	int sendBytes, recvBytes;

	char buffer[BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));

	// check for correct number of arguments
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s plaintext key port\n", argv[0]);
		exit(0);
	}

	// check file sizes for plaintext file and key
	stat(argv[1], &plaintextStat);
	ptextSize = plaintextStat.st_size;
	stat(argv[2], &keyStat);
	keySize = keyStat.st_size;
	// fprintf(stderr, "%ld\n", ptextSize);
	if (keySize < ptextSize)	// key too short
	{
		fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
		exit(1);
	}

	// open plaintext file
	ptextfd = open(argv[1], O_RDONLY);
	if (ptextfd == -1)
	{
		fprintf(stderr, "Error: cannot open plaintext file '%s'\n", argv[1]);
		exit(1);
	}

	// verify contents of plaintext file
	char plaintext[ptextSize];
	bzero(plaintext, sizeof(plaintext));
	readPtext = read(ptextfd, plaintext, ptextSize);
	if (readPtext < 0)
	{
		fprintf(stderr, "Error: cannot read plaintext file '%s'\n", argv[1]);
		exit(1);
	}
	plaintext[ptextSize - 1] = '\0';
		// replace trailing newline with null char
	if (verifyInput(plaintext) != 1)
	{
		fprintf(stderr, "Error: '%s' contains invalid characters\n", argv[1]);
		exit(1);
	}

	// open key file
	keyfd = open(argv[2], O_RDONLY);
	if (keyfd == -1)
	{
		fprintf(stderr, "Error: cannot open key file '%s'\n", argv[2]);
		exit(1);
	}

	// verify contents of key file
	char key[keySize];
	bzero(key, sizeof(key));
	readKey = read(keyfd, key, keySize);
	if (readKey < 0)
	{
		fprintf(stderr, "Error: cannot read key file '%s'\n", argv[2]);
		exit(1);
	}
	key[keySize - 1] = '\0';		// replace trailing newline with null char
	if (verifyInput(key) != 1)
	{
		fprintf(stderr, "Error: '%s' contains invalid characters\n", argv[2]);
		exit(1);
	}

	// create the socket and get file descriptor
	// use AF_INET for IP
	// use SOCK_STREAM for TCP
	// use 0 for IP
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "Error: %s socket creation failed\n", argv[0]);
		exit(1);
	}

	// set up address
	// client connecting to server
	server = gethostbyname("localhost");
	if (server == NULL)
	{
		fprintf(stderr, "Error: %s could not resolve server host name\n",
			argv[0]);
		exit(1);
	}

	// set up socket address struct
	// client connecting to server
	portNum = atoi(argv[3]);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portNum);
	memcpy(&servAddr.sin_addr, server->h_addr_list[0], server->h_length);

	// connect the socket to the server
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	{
		// no encrypt server on port
		fprintf(stderr, "Error: %s could not contact otp_enc_d on port %d\n",
			argv[0], portNum);
		exit(2);
	}

	char serverString[] = "server";		// for error messages

	// check that both server and client are encrypt type
	// both sides do this check, but server sends its response to the client
	// client is the one to close the connection if there is a type mismatch
	// verify that serverType == clientType
	// send client type to server
	sendBytes = send(sockfd, &clientType, sizeof(clientType), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], serverString, 1);
	}
	// receive server type from server
	recvBytes = recv(sockfd, &serverType, sizeof(serverType), 0);
	if (recvBytes < 0)
	{
		receiveError(argv[0], serverString, 1);
	}
	// server at port is not encrypt type
	if (clientType != serverType || clientType != 1 || serverType != 1)
	{
		fprintf(stderr, "Error: %s could not contact otp_enc_d on port %d\n",
			argv[0], portNum);
		close(sockfd);
		exit(2);
	}
	// receive server reponse to type check
	bzero(buffer, sizeof(buffer));
	recvBytes = recv(sockfd, buffer, sizeof(buffer), 0);
	if (recvBytes < 0)
	{
		receiveError(argv[0], serverString, 1);
	}
	if (strcmp(buffer, "type match") != 0)
	{
		fprintf(stderr, "Error: %s cannot find otp_enc_d on port %d\n",
			argv[0], portNum);
		close(sockfd);
		exit(2);
	}

	// file sizes verified and file contents verified by otp_enc
	// connected and both client and server are encrypt type

	// server will do another file size check on its end and send response back
	// client is the one to close connection if key size is too small
	// send plaintext file size
	sendBytes = send(sockfd, &ptextSize, sizeof(ptextSize), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], serverString, 1);
	}
	// send key file size
	sendBytes = send(sockfd, &keySize, sizeof(keySize), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], serverString, 1);
	}
	// receive server response to key size check
	bzero(buffer, sizeof(buffer));
	recvBytes = recv(sockfd, buffer, sizeof(buffer), 0);
	if (recvBytes < 0)
	{
		receiveError(argv[0], serverString, 1);
	}
	if (strcmp(buffer, "key ok") != 0)
	{
		fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
		close(sockfd);
		exit(1);
	}

	// pass both client and server encrypt type and key size checks

	// send plaintext to server
	sendBytes = send(sockfd, plaintext, sizeof(plaintext), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], serverString, 1);
	}

	// send key to server
	sendBytes = send(sockfd, key, sizeof(key), 0);
	if (sendBytes < 0)
	{
		sendError(argv[0], serverString, 1);
	}

	// server encrypts plaintext and sends ciphertext back
	char ciphertext[ptextSize];
	bzero(ciphertext, sizeof(ciphertext));
	recvBytes = recv(sockfd, ciphertext, sizeof(ciphertext), MSG_WAITALL);
	if (recvBytes < 0)
	{
		receiveError(argv[0], serverString, 1);
	}

	// print ciphertext to stdout, and add trailing newline
	fprintf(stdout, "%s\n", ciphertext);

	close(sockfd);

	return 0;
}