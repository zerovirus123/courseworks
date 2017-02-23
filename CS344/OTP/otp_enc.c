//
//  file: OTP_enc.c
//  description: connects to OTP_enc_d, and requests for an encryption service
//  with plaintext and key. program should not be able to connect to OTP_dec_d
//  usage: OTP_enc (plaintext) (key) (port)
//  Created by Brian Sia on 8/5/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //holds a list of data types
#include <sys/stat.h> //file info methods
#include <unistd.h>
#include <sys/socket.h> //BSD sockets
#include <arpa/inet.h> //manipulates numeric IP addresses
#include <fcntl.h> //used to manipulate file descriptors
#include <string.h>
#include <netdb.h> //network database ops methods
#include "otp_util.h" //to use the common utility functions
//#include "OTP_util.c"

#define CLIENT 1 //1 for encryption, and 0 for decryption

//main()
//sends a request to OTP_enc_d for encryption, using plaintext and key
//the following steps shows the general procedure for a client to establish a connection
//1.) Create a socket.
//2.) Setup the server address.
//3.) Connect to server.
//4.) Read/write data.
//5.) Shut down connection.
//preconditions:
//postconditions: receives a ciphertext from the encrypting daemon
int main(int argc, char **argv){

    char buffer[BUFFSIZE];
    bzero(buffer, sizeof(buffer)); //set zero bytes as poited by the buffer string
    int sockFD, portNum; //socket file descriptor and port number
    struct sockaddr_in serverAddr; //used to connect to server address
    struct hostent *server; //used to store information about host
    
    int clientType = CLIENT;
    int serverType, sendBytes, receiveBytes;
    
    struct stat plainStat, keyStat; //file statistics for verifying plaintext/ciphertext length
    long ptextSize,keySize;
    int keyFD, plainFD;
    int readPtext, readKey;
    
    // check for correct usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s plaintext key port\n", argv[0]);
        exit(0);
    }
    
    // check file sizes for plaintext file and key. exit program if they do not match
    stat(argv[1], &plainStat);
    ptextSize = plainStat.st_size;
    stat(argv[2], &keyStat);
    keySize = keyStat.st_size;

    if (keySize < ptextSize)	// key too short
    {
        fprintf(stderr, "Error: key '%s' length do not match\n", argv[2]);
        exit(1);
    }
    
    //open plaintext, exits if it fails to open
    plainFD = open(argv[1], O_RDONLY);
    if (plainFD == -1)
    {
        fprintf(stderr, "Error: cannot open plaintext file '%s'\n", argv[1]);
        exit(1);
    }
    
    // verify contents of plaintext file
    char plaintext[ptextSize];
    bzero(plaintext, sizeof(plaintext));
    readPtext = read(plainFD, plaintext, ptextSize);
    if (readPtext < 0)
    {
        fprintf(stderr, "Error: cannot read plaintext file '%s'\n", argv[1]);
        exit(1);
    }
    
    plaintext[ptextSize - 1] = '\0';
    
    // replace trailing newline with null char
    if (checkString(plaintext) != 1)
    {
        fprintf(stderr, "Error: '%s' contains invalid characters\n", argv[1]);
        exit(1);
    }
    
    //open key file
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
    
    key[keySize - 1] = '\0'; // replace trailing newline with null char
    if (checkString(key) != 1)
    {
        fprintf(stderr, "Error: '%s' contains invalid characters\n", argv[2]);
        exit(1);
    }
    
    // create the socket and get file descriptor
    // use AF_INET for IPV4, and AF_INET6 for IPV6
    // use SOCK_STREAM for TCP, and 0 for IP
    if ((sockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Error: %s socket creation failed\n", argv[0]);
        exit(1);
    }
    
    //set up connection address
    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr, "Error: %s could not resolve host name\n", argv[0]);
        exit(1);
    }
    
    // set up socket address
    portNum = atoi(argv[3]);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    memcpy(&serverAddr.sin_addr, server->h_addr_list[0], server->h_length);
    
    // connect the socket to the server
    if (connect(sockFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
        // no encrypt server on port
        fprintf(stderr, "Error: %s could not connect with OTP_enc_d on port %d\n", argv[0], portNum);
        exit(2);
    }
    
    char serverString[] = "server";		// for error messages
    
    // check that both server and client are of encrypt type
    // both sides verify, but server sends its response to the client
    // client closes the connection if there is a type mismatch
    // verify that both client and server has the same type
    // send client type to server
    sendBytes = send(sockFD, &clientType, sizeof(clientType), 0);
    if (sendBytes < 0)
    {
        sendError(argv[0], "server", 1);
    }
    
    // receive server type from server
    receiveBytes = recv(sockFD, &serverType, sizeof(serverType), 0);
    
    if (receiveBytes < 0)
    {
        receiveError(argv[0], serverString, 1);
    }
    
    if (clientType != serverType || clientType != 1 || serverType != 1)
    {
        fprintf(stderr, "Error: %s could not connect with OTP_enc_d on port %d\n",
                argv[0], portNum);
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
        fprintf(stderr, "Error: %s cannot find OTP_enc_d on port %d\n",
                argv[0], portNum);
        close(sockFD);
        exit(2);
    }
    
    //once file size ands file contents are A ok, connect client and server
    
    sendBytes = send(sockFD, &ptextSize, sizeof(ptextSize), 0);
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
    
    // receive server response to key size check
    bzero(buffer, sizeof(buffer));
    receiveBytes = recv(sockFD, buffer, sizeof(buffer), 0);
    
    //error check for returned results
    if (receiveBytes < 0)
    {
        receiveError(argv[0], "server", 1);
    }
    
    if (strcmp(buffer, "key ok") != 0)
    {
        fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
        close(sockFD);
        exit(1);
    }

    // send plaintext to server
    sendBytes = send(sockFD, plaintext, sizeof(plaintext), 0);
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
    
    // server encrypts plaintext and sends ciphertext back
    char ciphertext[ptextSize];
    bzero(ciphertext, sizeof(ciphertext));
    receiveBytes = recv(sockFD, ciphertext, sizeof(ciphertext), MSG_WAITALL);
    
    if (receiveBytes < 0)
    {
        receiveError(argv[0], "server", 1);
    }
    
    // print ciphertext to stdout, and add trailing newline
    fprintf(stdout, "%s\n", ciphertext);
    
    close(sockFD);
    
    return 0;

    
}
