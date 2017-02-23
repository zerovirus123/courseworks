/******************************************************************************
keygen.c
CS344-400
Program 4: OTP
OTP is a client/server simulation of a one-time pad encryption system.
	keygen.c creates a key file of specified length.
	Requires a supplied numeric argument as the key length.
	The allowed characters are capital letters and the space character. Ends
	the generated keyfile with a newline character.
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
	// check for correct number of arguments
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s key_length\n", argv[0]);
		exit(0);
	}

	// seed random number generator
	srand(time(NULL));

	int randChar, i;
	int keyLength = atoi(argv[1]);	// get key length and convert to int

	// selects from ASCII 65 to 91 ('A' to '['), and converts and '[' to space
	for (i = 0; i < keyLength; i++)
	{
		randChar = rand() % 27 + 'A';	// select char as int
		if (randChar == 26 + 'A')		// convert '[' to space
		{
			randChar = 32;
		}
		fprintf(stdout, "%c", randChar);
	}

	// add trailing newline character to file
	fprintf(stdout, "\n");

	return 0;
}
