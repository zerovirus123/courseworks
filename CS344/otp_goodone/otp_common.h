/******************************************************************************
otp_common.h
CS344-400
Program 4: OTP
OTP is a client/server simulation of a one-time pad encryption system.
	Header file for common functions for the otp programs.
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define BUFFER_SIZE 131072	// 2^17

/* verifyInput() is used to verify that a string contains no invalid
	characters. Valid characters are A-Z and the space character.
	Preconditions: none
	Postconditions: none
	Receives: string as char array
	Returns: 0 if invalid character present; 1 if string is valid */
int verifyInput(char *string);

/* sendError() is used to print an error to stdout, then exits with a supplied
	integer value. Used in otp_enc and otp_dec.
	Preconditions: error on send()
	Postconditions: prints message to stderr and program exits with supplied
		exit value
	Receives: sender process as char array, receiver as char array, exit value
		as int
	Returns: none */
void sendError(char *sender, char *receiver, int errorNum);

/* receiveError() is used to print an error to stdout, then exits with a
	supplied integer value. Used in otp_enc and otp_dec.
	Preconditions: error on recv()
	Postconditions: prints message to stderr and program exits with supplied
		exit value
	Receives: receiver process as char array, sender as char array, exit value
		as int
	Returns: none */
void receiveError(char *receiver, char *sender, int errorNum);

/* sendErrorNoExit() is used to print an error to stdout, but does not exit.
	Used in otp_enc_d and otp_dec_d.
	Preconditions: error on send()
	Postconditions: prints message to stderr
	Receives: sender process as char array, receiver as char array
	Returns: none */
void sendErrorNoExit(char *sender, char *receiver);

/* receiveErrorNoExit() is used to print an error to stdout, but does not exit.
	Used in otp_enc_d and otp_dec_d.
	Preconditions: error on recv()
	Postconditions: prints message to stderr
	Receives: receiver process as char array, sender as char array
	Returns: none */
void receiveErrorNoExit(char *receiver, char *sender);

/* encrypt() performs the encryption of a plaintext string using a key, and
	saves the result in the ciphertext string. Used by otp_enc_d.
	Preconditions: key length is longer than plaintext length, and ciphertext
		length is same as plaintext
	Postconditions: plaintext is encrypted using key and saved in ciphertext
	Receives: plaintext, key, and ciphertext all as char arrays
	Returns: none */
void encrypt(char *plaintext, char *key, char *ciphertext);

/* decrypt() performs the decryption of a ciphertext string using a key, and
	saves the result in the plaintext string. Used by otp_dec_d.
	Preconditions: key length is longer than ciphertext length, and plaintext
		length is same as ciphertext
	Postconditions: ciphertext is decrypted using key and saved in plaintext
	Receives: ciphertext, key, and plaintext all as char arrays
	Returns: none */
void decrpyt(char *ciphertext, char *key, char *plaintext);