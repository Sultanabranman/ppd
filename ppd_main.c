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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"
#include "ppd_stock.h"
#include "ppd_coin.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{	
    /* validate command line arguments */
	if(argc != 3)
	{
		fprintf(stderr, "Error: invalid arguments passed in.\n"
		    "Correct arguments are:\n"
			"\t./ppd <stockfile> <coinfile>\n"
			"Where <stockfile> and <coinfile> are two valid files in the "
			"expected format.\n");
		return EXIT_FAILURE;
	}
	
	const char * stockfile = argv[STOCKFILE];
	const char * coinfile = argv[COINSFILE];

    /* represents the data structures to manage the system */
    struct ppd_system system;
	struct menu_item menu[NUM_MENU_CHOICES];
	
    /* init the system */
	system_init(&system);

    /* load data */
	
	switch(load_data(&system, coinfile, stockfile))
	{
		/** fatal error occured when loading data from files **/
		case FALSE:
		{
			fprintf(stderr, "Correct arguments are:\n"
			"\t./ppd <stockfile> <coinfile>\n"
			"Where <stockfile> and <coinfile> are two valid files in the "
			"expected format.\n");
			return EXIT_FAILURE;
		}
		/** continue as normal **/
		case TRUE:
		{
			;
		}		
	}

    /* test if everything has been initialised correctly */

    /* initialise the menu system */
	init_menu(menu);

    /* loop, asking for options from the menu */
	display_menu(menu);

    /* run each option selected */
	display_items(&system);

    /* until the user quits */

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */
	 
    return EXIT_SUCCESS;
}
