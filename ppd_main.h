/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : CHRISTOPHER ROBERT SALTER
 * Student Number   : s3334875
 * Course Code      : COSC1076
 * Program Code     : BP162
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#ifndef PPD_MAIN
#define PPD_MAIN
/**
 * @file ppd_main.h this file holds the rest of the application together.
 * From these header files, main() can call all functions refered to 
 * in the header files included. You might also want to insert here
 * any functions for handling and validation of command line arguments
 **/
 
 #define STOCKFILE 1
 #define COINSFILE 2

/**
 * datatype to represent a boolean value within the system
 **/
typedef enum truefalse
{
    /**
     * the constant for false
     **/
    FALSE, 
    /**
     * the constant for true
     **/
    TRUE
} BOOLEAN;


#endif
