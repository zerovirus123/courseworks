//
//  OTP_dec_d.c
//  description: background process that does the actual decryption of the ciphertext into plaintext
//  ,if a key is supplied. The process will listen to and accept a ciphertext and key from a port. It writes
//  the plaintext back to the requesting program via the port as well. Key must be as long as the ciphertext
//  Created by Brian Sia on 8/7/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "otp_util.h"
//#include "OTP_util.c"

#define SERVER 0

//main()
//receives decryption requests, then decrypts data and send it back to requesting program.
//1.) Create a socket.
//2.) Bind the socket.
//3.) Listen for incoming connections.
//4.) Accept new client connections.
//5.) Read/write client connections
//6.) Shut down connections.
//preconditions: creates a socket that accepts ciphertext and key of equal length
//postconditions: decrypts the ciphertext, then sends plaintext back to requesting program
int main(int argc, char **argv)
{
	int sockFD, newsockFD;
	int portNum;
	socklen_t clientLen;			// size of address of client for accept()
	struct sockaddr_in serverAddr, clientAddr;	// internet address struct
	int optionValue; //setsockopt() optional argument

	int status;			
	pid_t pid;			

	int serverType = SERVER; //1 for encryption, and 0 for decryption
	int clientType;
	int sendBytes, receiveBytes;

	long ctextSize, keySize;
	char buffer[BUFFSIZE];
	bzero(buffer, sizeof(buffer));

	// check for correct number of arguments
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s listening_port\n", argv[0]);
		exit(0);
	}

	// create the socket and get file descriptor
	// use AF_INET for IPV4 domain
	// use SOCK_STREAM for TCP
	// use 0 for IP (default protocol)
	if ((sockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "Error: server socket creation failed\n");
		exit(1);
	}

	// allow UNIX to reuse ports
	optionValue = 1;
	setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(int));

	// set up socket address structm and let server accept connections
	portNum = atoi(argv[1]);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind server socket to a port
	if (bind(sockFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
	{
		fprintf(stderr, "Error: server bind call failed\n");
		exit(1);
	}

	// listen for up to 5 connections. encryption should be able to run concurrently on all five sockets
	if (listen(sockFD, 5) == -1)
	{
		fprintf(stderr, "Error: server listen call failed\n");
		exit(1);
	}

	//socket creation and binding successful
	//infinite loop and accept client requests
	while (1)
	{
		clientLen = sizeof(clientAddr);

		newsockFD = accept(sockFD, (struct sockaddr *) &clientAddr, &clientLen);

		if (newsockFD == -1)
		{
			fprintf(stderr, "Error: server accept call failed\n");
		}

		pid = fork();	// fork the server process. actual decryption takes place in the child process

		if (pid == -1)	// failure to fork
		{
			close(newsockFD);
			newsockFD = -1;
			fprintf(stderr, "Error: server fork failed\n");
		}

		if (pid == 0)	// child process
		{
			char clientString[] = "client";		// for error messages

			// check that both server and client are decrypt type
			// both client and server do this check, but server sends its response to the client
			// client is the one to close the connection if there is a type mismatch
			// verify that serverType == clientType
			// receive client type from client
			receiveBytes = recv(newsockFD, &clientType, sizeof(clientType), 0);
			if (receiveBytes == 0)
			{
				printReceiveError(argv[0], clientString);
			}
			// send server type to client
			sendBytes = send(newsockFD, &serverType, sizeof(serverType), 0);
			if (sendBytes == 0)
			{
				printSendError(argv[0], clientString);
			}
			// server at port is not decrypt type
			if (clientType != serverType || clientType != 0 || serverType != 0)
			{
				continue;	
			}
			else
			{
				// send response to client; client will check response
				char response[] = "type match";
				sendBytes = send(newsockFD, response, sizeof(response), 0);
				if (sendBytes < 0)
				{
					printSendError(argv[0], clientString);
				}
			}

			// check key file size
			// both client and server do this check, but server sends its response to the client
			// client is the one to close the connection if there is a size	issue
			// receive ciphertext and key file sizes for server confirmation
			receiveBytes = recv(newsockFD, &ctextSize, sizeof(ctextSize), 0);
			if (receiveBytes < 0)
			{
				printReceiveError(argv[0], clientString);
			}

			receiveBytes = recv(newsockFD, &keySize, sizeof(keySize), 0);
			if (receiveBytes < 0)
			{
				printReceiveError(argv[0], clientString);
			}

			// passed both decrypt type and key size check
			// now receive ciphertext from client
			char ciphertext[ctextSize];
			bzero(ciphertext, sizeof(ciphertext));
			receiveBytes = recv(newsockFD, ciphertext, sizeof(ciphertext), MSG_WAITALL);

			if (receiveBytes < 0)
			{
				printReceiveError(argv[0], clientString);
			}

			// now receive key from client
			char key[keySize];
			bzero(key, sizeof(key));
			receiveBytes = recv(newsockFD, key, sizeof(key), MSG_WAITALL);
			if (receiveBytes < 0)
			{
				printReceiveError(argv[0], clientString);
			}

			// now decrypt the ciphertext using the key
			char plaintext[ctextSize];
			bzero(plaintext, sizeof(plaintext));
			decryption(ciphertext, key, plaintext);

			// send plaintext to client
			sendBytes = send(newsockFD, plaintext, sizeof(plaintext), 0);
			if (sendBytes < 0)
			{
				printSendError(argv[0], clientString);
			}

			// close connection to client
			close(newsockFD);
			newsockFD = -1;

			exit(0);
		}

		else// parent process
		{
			// close client socket
			close(newsockFD);
			newsockFD = -1;
			// deal with zombies. wait until the zombie processes are terminated
			wait(NULL);
		}
	}

	close(sockFD);

	return 0;
}
