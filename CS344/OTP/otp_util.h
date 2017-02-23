//
//  OTP_util.h
//  Description: Header file for OTP_util.c, which contains common routines for OTP_enc.c, OTP_enc_d.c, OTP_dec.c, OTP_dec_d.c
//
//  Created by Brian Sia on 8/4/16.
//
//

#ifndef ____OTP_util__
#define ____OTP_util__

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define BUFFSIZE 131072

int checkString(char *string);
void sendError(char *sender, char *receiver, int errNum);
void receiveError(char *receiver, char *sender, int errNum);
void encryption(char *plaintext, char *key, char *ciphertext);
void decryption(char *ciphertext, char *key, char *plaintext);
void printSendError(char *sender, char *receiver);
void printReceiveError(char *receiver, char *sender);


#endif /* defined(____OTP_util__) */
