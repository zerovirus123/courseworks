//
//  OTP_util.c
//  Description: Contains common routines used by OTP_enc.c, OTP_enc_d.c,
//  OTP_dec.c, OTP_dec_d.c
//  Created by Brian Sia on 8/4/16.
//

#include "otp_util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

//checkString()
//check that the input string has no invalid characters (anything not
//A-Z or the space character
//preconditions: receives string to verify
//postconditions: 1 if string is valid, 0 if string is invalid
int checkString(char *string){

    long index;
    long length = strlen(string);
    
    for(index = 0; index <length; index++){
    
        //if character does not fall within the legal ASCII
        if( string[index] > 90 || (string[index] < 65 && string[index] != 32) ){
            return 0;
        }
        
    }
    return 1;
}

//sendError()
//print an error to stdout, then exits with the appropriate error number
//preconditions: a send error is invoked. Also takes in sender and receiver process
//as well as error number
//postconditions: prints error message to stderr, then exit with the correct
//error number
void sendError(char *sender, char *receiver, int errNum){

    fprintf(stderr, "Error: %s could not send data from %s", sender, receiver);
    exit(errNum);
    
}

//receiveError()
//print an error to stdout, then exits with appropriate error number
//preconditions: a receive error is invoked. Also takes in sender and receiver
//process, as well as the error number
//postconditions: prints error message to stderr, then exit with the correct
//error number
void receiveError(char *receiver, char *sender, int errNum){

    fprintf(stderr, "Error: %s could not receive data from %s", receiver, sender);
    exit(errNum);
    
}

//encryption()
//using the key, encrypt the plaintext into ciphertext
//preconditions: key length longer than plaintext length
//postconditions: ciphertext length same length as plaintext length. ciphertext is illegible. saves result into
//string called ciphertext
void encryption(char *plaintext, char *key, char *ciphertext){

    long index;
    int plainIndex, keyIndex, cipherIndex;
    
    //for traversing the plaintext
    long ptextSize = strlen(plaintext);
    
    //user for encryption arithmetic
    char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    
    //for each character in plaintext, covert them to appropriate index
    //from 0-26, including space bar
    for (index = 0; index < ptextSize; index++)
    {
        
        if (plaintext[index] == ' ')
        {
            plainIndex = 26;
        }
        
        //since A starts at 65, offset it by 65 so that A-Z goes from 0-25
        else
        {
            plainIndex = plaintext[index] - 65;
        }
        
        if (key[index] == ' ')
        {
            keyIndex = 26;
        }
        
        else
        {
            keyIndex = key[index] - 65;
        }
        
        // add plaintext to key, then convert to ciphertext
        cipherIndex = plainIndex + keyIndex;
        cipherIndex %= 27; //any rollovers just restarts at 0. this is modular arithmetic
        ciphertext[index] = characters[cipherIndex];
    }
}

//decryption()
//using the key, decipher the ciphertext into plaintext, saving result into plaintext
//preconditions: key length is longer than ciphertext
//postconditions: plaintext and ciphertext has same length. result saved in ciphertext
void decryption(char *ciphertext, char *key, char *plaintext){

    long index, ctextSize;
    int cipherIndex, keyIndex, plainIndex;
    
    ctextSize = strlen(ciphertext);
    
    char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    
    for (index = 0; index < ctextSize; index++)
    {
        if (ciphertext[index] == ' ')
        {
            cipherIndex = 26;
        }
        else
        {
            cipherIndex = ciphertext[index] - 65;
        }
        
        // convert key to corresponding index in characters
        if (key[index] == ' ')
        {
            keyIndex = 26;
        }
        else
        {
            keyIndex = key[index] - 65;
        }
        
        // subtract key from ciphertext, and then convert result to plaintext
        plainIndex = cipherIndex - keyIndex;
        if (plainIndex < 0)
        {
            plainIndex += 27; //rollover
        }
        plainIndex %= 27;
        plaintext[index] = characters[plainIndex];
    }
}

//()printSendError
//prints out send error msg to stderr
//preconditions: takes in a sender and receiver processes
//postconditions: prints out the error message to stderr
void printSendError(char *sender, char *receiver){

    fprintf(stderr, "Error: %s could not send data to %s\n", sender, receiver);
}

//printReceiveError()
//prints out receive error message to stderr
//preconditions: takes in a sender and receiver processes
//postconditions: prints out the error message to stderr
void printReceiveError(char *receiver, char *sender){
    fprintf(stderr, "Error: %s could not receive data from %s\n", receiver, sender);
}


