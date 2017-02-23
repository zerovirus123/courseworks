/******************************************************************************
otp_enc_d.c
CS344-400
Program 4: OTP
OTP is a client/server simulation of a one-time pad encryption system.
	otp_enc_d is a program that runs in the background as a daemon. It performs
	the actual encryption of the plaintext. It will listen on a particular
	port, which it is assigned to when first run, and receives plaintext and a
	key via that port when a connection to it is made. It will then write back
	the ciphertext to the process that it is connected to via the same port.
	The key passed in must be at least as big as the plaintext.
	Supports up to 5 concurrent socket connections.
	Usage: otp_enc_d listening_port
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "otp_common.h"

#define SERVER_TYPE 1		// encrypt == 1; decrypt == 0

int main(int argc, char **argv)
{
	int sockfd, newsockfd;		// socket file descriptors
	int portNum;				// port number
	socklen_t clilen;			// size of address of client for accept()
	struct sockaddr_in servAddr, cliAddr;	// internet address struct
	int optval;					// for setsockopt()

	int status;			// process status
	pid_t pid;			// process id

	int serverType = SERVER_TYPE;	// encrypt == 1; decrypt == 0
	int clientType;
	int sendBytes, recvBytes;

	long ptextSize, keySize;
	char buffer[BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));

	// check for correct number of arguments
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s listening_port\n", argv[0]);
		exit(0);
	}

	// create the socket and get file descriptor
	// use AF_INET for IP
	// use SOCK_STREAM for TCP
	// use 0 for IP
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "Error: server socket creation failed\n");
		exit(1);
	}

	// allow UNIX to reuse ports
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

	// set up socket address struct
	// server accepting connections
	portNum = atoi(argv[1]);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portNum);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind server socket to a port
	if (bind(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) == -1)
	{
		fprintf(stderr, "Error: server bind call failed\n");
		exit(1);
	}

	// listen for connections; max connections = 5
	if (listen(sockfd, 5) == -1)
	{
		fprintf(stderr, "Error: server listen call failed\n");
		exit(1);
	}

	// loop and accept, wait for clients to contact
	while (1)
	{
		clilen = sizeof(cliAddr);

		newsockfd = accept(sockfd, (struct sockaddr *) &cliAddr,
			&clilen);
		if (newsockfd == -1)
		{
			fprintf(stderr, "Error: server accept call failed\n");
		}

		pid = fork();	// fork the server process

		if (pid == -1)	// failure to fork
		{
			close(newsockfd);
			newsockfd = -1;
			fprintf(stderr, "Error: server fork failed\n");
		}

		if (pid == 0)	// child process
		{
			char clientString[] = "client";		// for error messages

			// check that both server and client are encrypt type
			// both client and server do this check, but server sends its
			//	response to the client
			// client is the one to close the connection if there is a type
			//	mismatch
			// verify that serverType == clientType
			// receive client type from client
			recvBytes = recv(newsockfd, &clientType, sizeof(clientType), 0);
			if (recvBytes == 0)
			{
				receiveErrorNoExit(argv[0], clientString);
			}
			// send server type to client
			sendBytes = send(newsockfd, &serverType, sizeof(serverType), 0);
			if (sendBytes == 0)
			{
				sendErrorNoExit(argv[0], clientString);
			}
			// server at port is not encrypt type
			if (clientType != serverType || clientType != 1 || serverType != 1)
			{
				continue;	// client has probably already closed connection
				// // send response to client, client will close connection
				// char response[] = "type mismatch";
				// sendBytes = send(newsockfd, response, sizeof(response), 0);
				// if (sendBytes < 0)
				// {
				// 	sendErrorNoExit(argv[0], clientString);
				// }
			}
			else
			{
				// send response to client; client will check response
				char response[] = "type match";
				sendBytes = send(newsockfd, response, sizeof(response), 0);
				if (sendBytes < 0)
				{
					sendErrorNoExit(argv[0], clientString);
				}
			}

			// check key file size
			// both client and server do this check, but server sends its
			//	response to the client
			// client is the one to close the connection if there is a size
			//	issue
			// receive plaintext and key file sizes for server confirmation
			recvBytes = recv(newsockfd, &ptextSize, sizeof(ptextSize), 0);
			if (recvBytes < 0)
			{
				receiveErrorNoExit(argv[0], clientString);
			}
			recvBytes = recv(newsockfd, &keySize, sizeof(keySize), 0);
			if (recvBytes < 0)
			{
				receiveErrorNoExit(argv[0], clientString);
			}
			// key size too small for plaintext
			if (keySize < ptextSize)
			{
				continue;	// client has probably already closed connection
				// // send response to client, client will close connection
				// char response[] = "key too short";
				// sendBytes = send(newsockfd, response, sizeof(response), 0);
				// if (sendBytes < 0)
				// {
				// 	sendErrorNoExit(argv[0], clientString);
				// }
			}
			else
			{
				// send repsonse to client; client will check response
				char response[] = "key ok";
				sendBytes = send(newsockfd, response, sizeof(response), 0);
				if (sendBytes < 0)
				{
					sendErrorNoExit(argv[0], clientString);
				}
			}

			// pass both encrypt type and key size check

			// now receive plaintext from client
			char plaintext[ptextSize];
			bzero(plaintext, sizeof(plaintext));
			recvBytes = recv(newsockfd, plaintext, sizeof(plaintext),
				MSG_WAITALL);
			if (recvBytes < 0)
			{
				receiveErrorNoExit(argv[0], clientString);
			}

			// now receive key from client
			char key[keySize];
			bzero(key, sizeof(key));
			recvBytes = recv(newsockfd, key, sizeof(key), MSG_WAITALL);
			if (recvBytes < 0)
			{
				receiveErrorNoExit(argv[0], clientString);
			}

			// now encrypt the plaintext using the key
			char ciphertext[ptextSize];
			bzero(ciphertext, sizeof(ciphertext));
			encrypt(plaintext, key, ciphertext);

			// send ciphertext to client
			sendBytes = send(newsockfd, ciphertext, sizeof(ciphertext), 0);
			if (sendBytes < 0)
			{
				sendErrorNoExit(argv[0], clientString);
			}

			// close connection to client
			close(newsockfd);
			newsockfd = -1;

			exit(0);	// zombies
		}
		else			// parent process
		{
			// close client socket
			close(newsockfd);
			newsockfd = -1;
			// deal with zombies
			wait(NULL);
		}
	}

	close(sockfd);

	return 0;
}