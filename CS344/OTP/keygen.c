//
//  keygen.c
//  Description: Creates a one-time pad keyfile of specific length, determined by user input
//  Characters used as key include the 26 English alphabet, and the space character.
//  Created by Brian Sia on 8/4/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//main()
//intializes random seed, checks for valid user input, and then generate the ASCII to integer conversion
//for the key. then add trailing newspace to end the file.
//Preconditions: length of key supplied as an argument
//Postconditions: generates a one-time pad key made up of the 26 English alphabet and the space character
int main(int argc, char **argv){

    //random num gen seed
    srand(time(NULL));
    
    if(argc !=2){
        fprintf(stderr, "Usage: %s keylength", argv[0]);
        exit(1);
    }
    
    int randChar, index, keylength = atoi(argv[1]);
    
    //A to Z lies between decimal 65-90 in ASCII. 91 corresponds to [ character
    //which will be converted to the space character if encountered
    for(index = 0; index < keylength; index++){
        
        randChar = rand()%27 + 'A';
        
        if (randChar == 26 + 'A')// '[' character encounted
        {
            randChar = 32; //ASCII for space
        }
        
        fprintf(stdout, "%c", randChar);
    
    }

    //newline ending the keygen file
    fprintf(stdout, "\n");
    return 0;

}