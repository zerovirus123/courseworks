/******************************************************************************
otp_common.c
CS344-400
Program 4: OTP
OTP is a client/server simulation of a one-time pad encryption system.
	Implementation file for common functions for the otp programs.
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include "otp_common.h"

/* verifyInput() is used to verify that a string contains no invalid
	characters. Valid characters are A-Z and the space character.
	Preconditions: none
	Postconditions: none
	Receives: string as char array
	Returns: 0 if invalid character present; 1 if string is valid */
int verifyInput(char *string)
{
	long i;							// loop counter
	long length = strlen(string);	// length of input string

	for (i = 0; i < length; i++)
	{
		// using ASCII codes to check string characters
		// 65-90 is A-Z, and space is 32
		if (string[i] > 90 || (string[i] < 65 && string[i] != 32))
		{
			return 0;	// invalid character in string
		}
	}
	return 1;	// no invalid characters in string
}

/* sendError() is used to print an error to stdout, then exits with a supplied
	integer value. Used in otp_enc and otp_dec.
	Preconditions: error on send()
	Postconditions: prints message to stderr and program exits with supplied
		exit value
	Receives: sender process as char array, receiver as char array, exit value
		as int
	Returns: none */
void sendError(char *sender, char *receiver, int errorNum)
{
	fprintf(stderr, "Error: %s could not send to %s\n", sender, receiver);
	exit(errorNum);
}

/* receiveError() is used to print an error to stdout, then exits with a
	supplied integer value. Used in otp_enc and otp_dec.
	Preconditions: error on recv()
	Postconditions: prints message to stderr and program exits with supplied
		exit value
	Receives: receiver process as char array, sender as char array, exit value
		as int
	Returns: none */
void receiveError(char *receiver, char *sender, int errorNum)
{
	fprintf(stderr, "Error: %s could not receive from %s\n", receiver, sender);
	exit(errorNum);
}

/* sendErrorNoExit() is used to print an error to stdout, but does not exit.
	Used in otp_enc_d and otp_dec_d.
	Preconditions: error on send()
	Postconditions: prints message to stderr
	Receives: sender process as char array, receiver as char array
	Returns: none */
void sendErrorNoExit(char *sender, char *receiver)
{
	fprintf(stderr, "Error: %s could not send to %s\n", sender, receiver);
}

/* receiveErrorNoExit() is used to print an error to stdout, but does not exit.
	Used in otp_enc_d and otp_dec_d.
	Preconditions: error on recv()
	Postconditions: prints message to stderr
	Receives: receiver process as char array, sender as char array
	Returns: none */
void receiveErrorNoExit(char *receiver, char *sender)
{
	fprintf(stderr, "Error: %s could not receive from %s\n", receiver, sender);
}

/* encrypt() performs the encryption of a plaintext string using a key, and
	saves the result in the ciphertext string. Used by otp_enc_d.
	Preconditions: key length is longer than plaintext length, and ciphertext
		length is same as plaintext
	Postconditions: plaintext is encrypted using key and saved in ciphertext
	Receives: plaintext, key, and ciphertext all as char arrays
	Returns: none */
void encrypt(char *plaintext, char *key, char *ciphertext)
{
	long i;
	long ptextSize;
	int x, y, z;

	ptextSize = strlen(plaintext);

	char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

	for (i = 0; i < ptextSize; i++)
	{
		// convert plaintext to corresponding index in characters
		if (plaintext[i] == ' ')
		{
			x = 26;
		}
		else
		{
			x = plaintext[i] - 65;
		}

		// convert key to corresponding index in characters
		if (key[i] == ' ')
		{
			y = 26;
		}
		else
		{
			y = key[i] - 65;
		}

		// add plaintext to key, then convert to ciphertext
		z = x + y;
		z %= 27;
		ciphertext[i] = characters[z];
	}
}

/* decrypt() performs the decryption of a ciphertext string using a key, and
	saves the result in the plaintext string. Used by otp_dec_d.
	Preconditions: key length is longer than ciphertext length, and plaintext
		length is same as ciphertext
	Postconditions: ciphertext is decrypted using key and saved in plaintext
	Receives: ciphertext, key, and plaintext all as char arrays
	Returns: none */
void decrypt(char *ciphertext, char *key, char *plaintext)
{
	long i;
	long ctextSize;
	int x, y, z;

	ctextSize = strlen(ciphertext);

	char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

	for (i = 0; i < ctextSize; i++)
	{
		// convert ciphertext to corresponding index in characters
		if (ciphertext[i] == ' ')
		{
			x = 26;
		}
		else
		{
			x = ciphertext[i] - 65;
		}

		// convert key to corresponding index in characters
		if (key[i] == ' ')
		{
			y = 26;
		}
		else
		{
			y = key[i] - 65;
		}

		// subtract key from ciphertext, then convert to plaintext
		z = x - y;
		if (z < 0)
		{
			z += 27;
		}
		z %= 27;
		plaintext[i] = characters[z];
	}
}