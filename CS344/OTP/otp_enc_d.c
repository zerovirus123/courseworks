//
//  OTP_enc_d.c
//  description: Program that runs as a background daemon. This program does the actual encryption of the plaintext. From the port it is assigned, it will receive a plaintext and key. It then encrypts the information, and sends it back to the sender program through the same port. Key should have the same length as the plaintext.
//
//  Created by Brian Sia on 8/7/16.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //BSD sockets
#include <sys/types.h>
#include <unistd.h> //for fork() and close() functions
#include <arpa/inet.h> //manipulates numeric IP addresses
#include <netinet/in.h> //for constants and structs needed for internet domain addresses
#include "otp_util.h" //to use common utility functions
//#include "OTP_util.c"

#define SERVER 1 //1 for encryption, and 0 for decryption

//main()
//receives an encryption requests, then encrypts the data and send it back to the requesting program
//1.) Create a socket.
//2.) Bind the socket.
//3.) Listen for incoming connections.
//4.) Accept new client connections
//5.) Read/write to client connections
//6.) Shut down connections
//preconditions: creates a socket and accepts plaintext and key of equal length
//postoconditions: encrypts the plaintext, and then sends the ciphertext back to requesting program

int main(int argc, char **argv){

    int sockFD, newsockFD, portNum; //socket file descriptors and port number
    socklen_t clientLen; //length of data sent from client
    struct sockaddr_in serverAddr, clientAddr; //client and server address
    int optionVal; //to set socket's optional value
    int status; //process status
    
    pid_t pid; //using this instead of int so that it is platform compatible
    
    int serverType = SERVER;	// encrypt == 1; decrypt == 0
    int clientType;
    int sendBytes, receiveBytes;
    
    long ptextSize, keySize; //to verify that key and plaintext have same length
    char buffer[BUFFSIZE];
    bzero(buffer, sizeof(buffer));
    
    // check for correct number of arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s (listening_port)\n", argv[0]);
        exit(0);
    }
    
    // create the socket and get file descriptor
    // use AF_INET for IPV4 domain, SOCK_STREAM for TCP type socket
    // use 0 for IP protocol (default protocol)
    if ((sockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Error: server socket creation failed\n");
        exit(1);
    }
    
    optionVal = 1;
    //pointed by the socket file descriptor, at the socket layer/level, set the socket
    //option so that ports can be reused, as pointed by the option value. the option size is an optional
    //argument
    setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &optionVal, sizeof(int));
    
    // set up socket address struct and accept server connections
    portNum = atoi(argv[1]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // bind server socket to a port
    if (bind(sockFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
    {
        fprintf(stderr, "Error: server bind invocation failed\n");
        exit(1);
    }
    
    // listen for connections; max connections = 5. this allows five encryptions to execute
    // concurrently
    if (listen(sockFD, 5) == -1)
    {
        fprintf(stderr, "Error: server listen call failed\n");
        exit(1);
    }
    
    //socket creation and binding ends
    //in a while loop, wait for client request and accepts it
    while(1){
    
        clientLen = sizeof(clientAddr);
        newsockFD = accept(sockFD, (struct sockaddr *) &clientAddr, &clientLen);
        
        if (newsockFD == -1)
        {
            fprintf(stderr, "Error: server accept call failed\n");
        }
        
        pid = fork();	// fork the server process. the actual encryption will take place
        //in the child process
        
        if (pid == -1)	// fork failure
        {
            close(newsockFD);
            newsockFD = -1;
            fprintf(stderr, "Error: server fork failed\n");
        }
        
        if (pid == 0)	// child process successfully created
        {
            
            // check that both server and client are of encryption type
            // both client and server do this check, but server sends its
            //	response to the client
            // client is the one to close the connection if there is a type mismatch
            // verify that serverType == clientType
            // receive client type from client
            receiveBytes = recv(newsockFD, &clientType, sizeof(clientType), 0);
            
            if (receiveBytes == 0)
            {
                printReceiveError(argv[0], "client");
            }
            
            // send server type to client
            sendBytes = send(newsockFD, &serverType, sizeof(serverType), 0);
            if (sendBytes == 0)
            {
                printSendError(argv[0], "client");
            }
            
            // server at port is not encrypt type
            if (clientType != serverType || clientType != 1 || serverType != 1)
            {
                continue; //nothing hapense
            }
            
            else
            {
                // send response to client; client will check response
                char response[] = "type match";
                sendBytes = send(newsockFD, response, sizeof(response), 0);
                if (sendBytes < 0)
                {
                    printSendError(argv[0], "client");
                }
            }
            
            // check key file size
            // both client and server do this check, but server sends its response to the client
            // client closes the connection if there is a size issue
            // receive plaintext and key file sizes for server confirmation
            receiveBytes = recv(newsockFD, &ptextSize, sizeof(ptextSize), 0);
            if (receiveBytes < 0)
            {
                printReceiveError(argv[0], "client");
            }
            
            receiveBytes = recv(newsockFD, &keySize, sizeof(keySize), 0);
            if (receiveBytes < 0)
            {
                printReceiveError(argv[0], "client");
            }
            
            // key size too small for plaintext
            if (keySize < ptextSize)
            {
                continue; //nothing happens
            }
            else
            {
                // send repsonse to client; client will check response
                char response[] = "key ok";
                sendBytes = send(newsockFD, response, sizeof(response), 0);
                if (sendBytes < 0)
                {
                    printSendError(argv[0], "client");
                }
            }
            
            // passed both encryption type and key size check
            
            // now receive plaintext from client
            char plaintext[ptextSize];
            bzero(plaintext, sizeof(plaintext));
            receiveBytes = recv(newsockFD, plaintext, sizeof(plaintext), MSG_WAITALL);
            
            if (receiveBytes < 0)
            {
                printReceiveError(argv[0], "client");
            }
            
            // now receive key from client
            char key[keySize];
            bzero(key, sizeof(key));
            receiveBytes = recv(newsockFD, key, sizeof(key), MSG_WAITALL);
            
            if (receiveBytes < 0)
            {
                printReceiveError(argv[0], "client");
            }
            
            // now encrypt the plaintext using the key
            char ciphertext[ptextSize];
            bzero(ciphertext, sizeof(ciphertext));
            encryption(plaintext, key, ciphertext);
            
            // send ciphertext to client
            sendBytes = send(newsockFD, ciphertext, sizeof(ciphertext), 0);
            if (sendBytes < 0)
            {
                printSendError(argv[0], "client");
            }
            
            // close connection to client
            close(newsockFD);
            newsockFD = -1;
            
            exit(0);	// zombies
        }
        
    }
    
    //shut down connections and exit the daemon
    close(sockFD);
    return 0;
}
