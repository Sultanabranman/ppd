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
	
	switch(load_coin_data(system, coins_name))
	{
		/** return false **/
		case FALSE:
		{
			return FALSE;
		}
		/** continue as normal **/
		case TRUE:
		{
			;
		}		
	}
	
	
	/** load stock data into system struct **/	
	switch(load_stock_data(system, stock_name))
	{
		/** return false **/
		case FALSE:
		{
			return FALSE;
		}
		/** continue as normal **/
		case TRUE:
		{
			;
		}		
	}
	
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
	
	/** open coin file for reading **/
	coin_file = fopen(coins_name, "r");
	
	/** Check if coin file opened correctly **/
	if(coin_file == NULL)
	{
		fprintf(stderr, "Error: coin file failed to open.\n");
		return FALSE;
	}		
	
	/** run through coin file line by line and store data in 
	  * ppd_coin struct 
	 **/
	while(fgets(line, LINELEN + EXTRACHARS, coin_file))
	{
		/** get denomination from file **/ 
		token = strtok(line, COIN_DELIM);
		
		/** check if denomination is valid **/
		if(token == NULL)
		{
			fprintf(stderr, "Error loading coin file: denomination token "
			    "is not valid\n");
			return FALSE;
		}
		
		/** assign denomination to new coin struct **/
		
		
        /** get count from file **/ 	
		token = strtok(NULL, COIN_DELIM);
		
		/** check if count is valid **/
		if(token == NULL)
		{
			fprintf(stderr, "Error loading coin file: count token "
			    "is not valid\n");
			return FALSE;
			
		}
		
		/** assign count to new coin struct **/
		new_coin.count = strtol(token, &end, 0);
					
				
		/** check if there is more data in the line, and if there is, 
		  * treat as fatal error 
		 **/
		if((strtok(NULL, COIN_DELIM) != NULL))
		{
			fprintf(stderr, "Error loading coin file: too many arguments\n");
			return FALSE;
		}
		
		/** assign new coin struct to system cash_register array **/
		system -> cash_register[count] = new_coin;		
		
		/** increment loop counter **/
		++count;
	}
	
	/** check if there was a total of NUM_DENOMS entries in coin file **/ 
	if(count != NUM_DENOMS)
		{
			fprintf(stderr, "Error loading coin file: incorrect number of " 
			    "denominations.\n");
			return FALSE;
		}
	
	fclose(coin_file);
	
	return TRUE;
}

/** load data from stock file into system struct **/
BOOLEAN load_stock_data(struct ppd_system * system, const char * stock_name)
{
	FILE * stock_file;
	char * token;	
	char * end;	
    char * price_token;	
	char line[MAX_LINE_LEN];
	struct ppd_stock new_stock_item;
	int on_hand = 0;
	
	/** open stock file for reading **/
	stock_file = fopen(stock_name, "r");
	
	/** Check if stock file opened correctly **/
	if(stock_file == NULL)
	{
		fprintf(stderr, "Error: stock file failed to open.\n");
		return FALSE;
	}
	
	/** run through stock file line by line and store data in 
	  * ppd_stock struct 
	 **/
	while(fgets(line, MAX_LINE_LEN + EXTRACHARS, stock_file))
	{
		/** get id from stock file **/
		token = strtok(line, STOCK_DELIM);		
		/** check if id token is valid **/
		if(token == NULL || (strlen(token)) > (IDLEN + EXTRACHARS) || token[FIRSTCHAR] != 'I')
		{
			fprintf(stderr, "Error loading stock file: the id token "
			    "is not valid.\n");
			return FALSE;
		}		
		/** assign id data to new_stock_item struct **/
		strcpy(new_stock_item.id, token);

		
		/** get name from stock file **/
		token = strtok(NULL, STOCK_DELIM);		
		/** check if name is valid **/
		if(token == NULL || (strlen(token)) > (NAMELEN + EXTRACHARS))
		{
			fprintf(stderr, "Error loading stock file: the name token "
			    "is not valid.\n");
			return FALSE;
		}		
		/** assign name data to new_stock_item struct */
		strcpy(new_stock_item.name, token);
		
		
		/** get product description from stock file **/ 
		token = strtok(NULL, STOCK_DELIM);
		/** check if name token is valid **/
		if(token == NULL || (strlen(token)) > (DESCLEN + EXTRACHARS)) 
		{
			fprintf(stderr, "Error loading stock file: the description token "
			    "is not valid.\n");
			return FALSE;
		}
		/** assign description data to new_stock_item struct **/
		strcpy(new_stock_item.desc, token);
		
		
		/** get price token from stock file **/
		price_token = strtok(NULL, STOCK_DELIM);
		/** check if price token exists **/
        if(price_token == NULL)
		{
			fprintf(stderr, "Error loading stock file: the price token "
			    "is not valid.\n");
			return FALSE;
		}		
		
		
		/** get stock on_hand from stock file **/
		token = strtok(NULL, STOCK_DELIM);		
		/** check if on_hand data exists **/
		if(token == NULL)
		{
			fprintf(stderr, "Error loading stock file: the on_hand token "
			    "is not valid.\n");
			return FALSE;
		}
		/** remove \n character from end of string **/
		token[strlen(token)-1] = 0;
		/** convert on_hand data to long **/
		on_hand = strtol(token, &end, 0);		
		/** check if on_hand data is valid **/
		if(*end)
		{
			fprintf(stderr, "Error loading stock file: the on_hand token "
			    "is not valid.\n");
			return FALSE;
		}
		/** assign on_hand data to new_stock_item struct **/
		new_stock_item.on_hand = on_hand;		
		
		
		/** check if there is more data in the line, and if there is, 
		  * treat as fatal error 
		 **/
		if((strtok(NULL, STOCK_DELIM) != NULL))
		{
			fprintf(stderr, "Error loading stock file: too many arguments\n");
			return FALSE;
		}
		
		
		/** load the price data into struct at the end to avoid 
		  * interfering with strtok 
		 **/
		switch(load_price_data(&new_stock_item, price_token))
	    {
		    /** return false **/
		    case FALSE:
		    {
		    	return FALSE;
		    }
		    /** continue as normal **/
		    case TRUE:
		    {
		    	;
		    }		
	    }
		
		printf("%s, %s , %s, $%d.%d, %d\n", new_stock_item.id, new_stock_item.name, new_stock_item.desc, new_stock_item.price.dollars, new_stock_item.price.cents, new_stock_item.on_hand);
	}
	
	fclose(stock_file);
	
	return TRUE;
}

BOOLEAN load_price_data(struct ppd_stock * new_stock_item, char * token)
{
	char * end;
	char * price_token;
	int dollars = 0;
	int cents = 0;
	
	/** get dollar data from token **/
	price_token = strtok(token, PRICE_DELIM);
	/** check if dollar token exists **/
	if(price_token == NULL)
	{
		fprintf(stderr, "Error loading stock file: price token not valid\n");
		return FALSE;
	}
	/** convert price token to long **/
	dollars = strtol(price_token, &end, 0);
	/** check if dollar data is valid **/
	if(*end)
	{
		fprintf(stderr, "Error loading stock file: the price token "
		    "is not valid.\n");
		return FALSE;
	}
	/** assign dollar data to new_stock_item struct **/
	new_stock_item->price.dollars = dollars;
	
	/** get cent data from token **/
	price_token = strtok(NULL, PRICE_DELIM);
	/** check if price token exists **/
	if(price_token == NULL)
	{
		fprintf(stderr, "Error loading stock file: price token not valid\n");
		return FALSE;
	}
	/** convert price token to long **/
	cents = strtol(price_token, &end, 0); 
	/** check if cent data is valid **/
	if(*end || cents < MIN_CENTS || cents > MAX_CENTS)
	{
		fprintf(stderr, "Error loading stock file: the price token "
		   "is not valid.\n");
		return FALSE;
	}
	/** assign cent data to new_stock_item struct **/
	new_stock_item->price.cents = cents;
	
	/** check if there is more data in the token, and if there is, 
	  * treat as fatal error 
	 **/
	if((strtok(NULL, STOCK_DELIM) != NULL))
	{
		fprintf(stderr, "Error loading stock file: the price token "
		   "is not valid.\n");
		return FALSE;
	}
	
	return TRUE;
}
