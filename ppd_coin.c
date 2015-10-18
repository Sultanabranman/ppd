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
#include "ppd_coin.h"
#include "ppd_stock.h"

 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/
BOOLEAN initialise_cash_register(struct ppd_system * system)
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
	return TRUE;
}

enum denomination convert_denom(int value)
{
	/** array to match denomination to value **/
	const int denom_value[NUM_DENOMS] = {5, 10, 20, 50, 100, 200, 500, 1000};
	/** loop counter **/
	enum denomination i;		
	
	for(i = FIVE_CENTS; i <= TEN_DOLLARS; ++i)
	{
		if(value == denom_value[i])
		{
			return i;
		}
	}
	
	/** if matching value not found **/
	fprintf(stderr, "Error loading coin file: denom token "
			    "is not valid\n");
	exit(EXIT_FAILURE);	
}

BOOLEAN check_price_input(int price)
{
	int i;	
	const int denom_value[NUM_DENOMS] = {5, 10, 20, 50, 100, 200, 500, 1000};	
	
	/** check price against all valid denominations **/
	for(i = 0; i < NUM_DENOMS; i++)
	{
		/** if price matches **/
		if(price == denom_value[i])
		{
			return TRUE;
		}
		/** if price doesn't match **/
		else if(price != denom_value[i] && i == NUM_DENOMS-1)
		{
			return FALSE;
		}
	}
	
	return FALSE;
}