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

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name, 
const char * stock_name)
{
    /** load coin data into system struct **/
	load_coin_data(system, coins_name);
	
	/** load stock data into system struct **/
	load_stock_data(system, stock_name);
	
	/** set coin file name **/
	system -> coin_file_name = coins_name;
	
	/** set stock file name **/
	system -> stock_file_name = stock_name;
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
	/** loop counter **/
	int i;
	
	/** creates an empty coin struct initialised to a safe value **/
	struct coin empty_coin;
	empty_coin.denom = 0;
	empty_coin.count = 0;
	
	/** initialises cash_register array to safe values **/
	for(i = 0; i < NUM_DENOMS; i++)
	{
		system -> cash_register[i] = empty_coin;
	}
	
    /** initialises system struct to safe values **/    
	system -> item_list = NULL;
	system -> coin_file_name = "";
	system -> stock_file_name = "";
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
	free(system);
	return;
}

/** load data from coin file into system struct **/
BOOLEAN load_coin_data(struct ppd_system * system, const char * coins_name)
{
	FILE * coin_file;
	char * token;
	char * end;
	struct coin new_coin;
	int count = 0;
	
	char line[LINELEN];
	
	coin_file = fopen(coins_name, "r");
	
	if(coin_file == NULL)
	{
		fprintf(stderr, "Error: coin file failed to open.\n");
		exit(EXIT_FAILURE);
	}		
	
	while(fgets(line, LINELEN + EXTRACHARS, coin_file))
	{
		/** get denomination from file and assign to new coin **/
		token = strtok(line, COIN_DELIM);
		if(token != NULL)
		{
			
		}
		else
		{
			fprintf(stderr, "Error: coin file not a valid format\n");
			exit(EXIT_FAILURE);
		}
		
        /** get count from file and assign to new coin **/		
		token = strtok(NULL, COIN_DELIM);
		
		if(token != NULL)
		{
			new_coin.count = strtol(token, &end, 0);
		}
		else
		{
			fprintf(stderr, "Error: coin file not a valid format\n");
			exit(EXIT_FAILURE);
		}				
				
		/** check if there is more data in the line, and if there is, 
		  * treat as fatal error 
		 **/
		if((strtok(NULL, COIN_DELIM) != NULL))
		{
			fprintf(stderr, "Error: coin file not a valid format\n");
			exit(EXIT_FAILURE);
		}
		
		/** assign new coin struct to system cash_register array **/
		system -> cash_register[count] = new_coin;		
		
		/** increment loop counter **/
		++count;
	}
	
	/** check if there was a total of NUM_DENOMS entries in coin file **/ 
	if(count != NUM_DENOMS)
		{
			fprintf(stderr, "Error: coin file has incorrect number of " 
			    "denominations. Please use file of valid format\n");
			exit(EXIT_FAILURE);
		}
	
	fclose(coin_file);
	
	return TRUE;
}

/** load data from stock file into system struct **/
BOOLEAN load_stock_data(struct ppd_system * system, const char * stock_name)
{
	return FALSE;
}