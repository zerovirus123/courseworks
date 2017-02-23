#File: mypython.py
#Author: Jing Guang Sia
#Description:
#Assignment 3 for CS344. Creates 3 files in same directory as script, 3 files create 10 random characters from lowercase alphabet, eleventh character is a newline. Then randomly chooses two integers and
#print their products
#Date: 19th July 2016

import os
import sys
import random
import string

#creates a random string of 10 lowercase characters. elegant way of doing it
def random_string():
    return ''.join(random.choice(string.ascii_lowercase) for i in range(10))

if __name__ == "__main__":

    #opens 3 files in same directory as this script, 'w' option erases existing file with same name
    file1 = open("file1.txt", 'w+r')
    file2 = open("file2.txt", 'w+r')
    file3 = open("file3.txt", 'w+r')

    #10 random lowercase plus a newline character
    file1.write( random_string() + '\n')
    file2.write( random_string() + '\n')
    file3.write( random_string() + '\n')
    
    file1.close()
    file2.close()
    file3.close()

    print "Contents of the 3 files are: "
    
    with open( "file1.txt" ) as f:
        print f.read()
    
    with open( "file2.txt" ) as f:
        print f.read()

    with open( "file3.txt" ) as f:
        print f.read()

    rand_int1 = random.randint(1,42)
    rand_int2 = random.randint(1,42)
    
    print "Random int 1: " + str(rand_int1) + "\nRandom int 2: " + str(rand_int2) + "\n"
    print "Product of two random integers: " + str(rand_int1 * rand_int2) + "\n"

