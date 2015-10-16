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

#include "ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{
	struct ppd_node * current = system->item_list->head;
	char * id;
	char * name;
	int available = 0;
	int dollars = 0;
	int cents = 0;
	
	/** print out formatted title information to console **/
    printf("Items Menu\n");
	printf("----------\n");
	printf("%-5s |%-40s |%-10s |%s\n", "ID,", "Name", "Available", "Price");
	printf("------------------------------------------------------------------"
		"-\n");
	
	/** print stock data for each item in system item_list **/
	while(current -> next != NULL)
	{	
		id = current->data->id;
		name = current->data->name;
		available = current->data->on_hand;
		dollars = current->data->price.dollars;
		cents = current->data->price.cents;
		
		printf("%-5s |%-40s |%-10d |$ %d.%02d\n", id , name, available,
			dollars, cents);
		
		/**	set current to point to next node in linked list **/	
		current = current->next;
	}
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
    static int id_num = 0;
	char id[IDLEN + EXTRACHARS];
	char name[NAMELEN + EXTRACHARS];
	char desc[DESCLEN + EXTRACHARS];
	char price[PRICELEN + EXTRACHARS];
	struct ppd_stock new_item;	
	BOOLEAN valid = FALSE;
	
	if(id_num == 0)
	{
		id_num = get_latest_id(system);
	}
	
	id_num++;
	
	sprintf(id, "I%04d", id_num);
	
	strcpy(new_item.id, id);
	
	printf("Item id will be %s\n", id);
	
	/** get item name **/
	while(valid != TRUE)
	{
		printf("Enter the item name: ");
		
        if(get_input(name, NAMELEN) == FALSE)
		{
			continue;
		}
        if(name[0] == '\n')	
		{
			printf("Operation cancelled at the user's request.\n");
		    return FALSE;
		}		
		
		/** input was valid **/
		valid = TRUE;

		/** copy user input into new item struct **/
		strcpy(new_item.name, name);
	}		
	/** reset valid to false **/
	valid = FALSE;
	
	/** get item description **/
	while(valid != TRUE)
	{
		printf("Enter the item description: ");
		if(get_input(desc, DESCLEN) == FALSE)
		{
			continue;
		}
	    if(desc[0] == '\n')
	    {
		    printf("Operation cancelled at the user's request.\n");
		    return FALSE;
	    }		
		
		/** input was valid **/
		valid = TRUE;
		
        /** copy user input into new item struct **/
		strcpy(new_item.desc, desc);	    
	}	
	
	/** reset valid to false **/
    valid = FALSE;	
	
	/** get item price **/
	while(valid != TRUE)
	{
		printf("Enter the item price: ");
		if(get_input(price, PRICELEN) == FALSE)
		{
			continue;
		}
	    if(price[0] == '\n')
	    {
		    printf("Operation cancelled at the user's request.\n");
		    return FALSE;
	    }		
		/** validate price data **/
		if(load_price_data(&new_item, price) == FALSE)
		{
			fprintf(stderr, "Error: Price is not valid, please try again\n");
			continue;
		}		
		
		/** input was valid **/
		valid = TRUE;	    
	}	
	/** set new item stock level to default **/
	new_item.on_hand = DEFAULT_STOCK_LEVEL;
	
	/** add new item to item list **/
	add_new_node(system, new_item);
	
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
	char id[IDLEN + EXTRACHARS];
	BOOLEAN valid = FALSE;
	
	/** while input is not valid **/
	while(valid != TRUE)
	{
		printf("Enter the item id of the item to remove from the menu: ");
		/** get user input and check if blank**/	
		fgets(id, IDLEN + EXTRACHARS, stdin);
	    if(id[0] == '\n')
	    {
		    printf("Operation cancelled at the user's request.\n");
		    return FALSE;
	    }
		
		/** check if input was too long **/
	    if(id[strlen(id)-1] != '\n')
	    {
		    read_rest_of_line();
		    fprintf(stderr, "Error: line entered was too long. " 
			    "Please try again\n");
		    continue;
	    }
		
		/** input was valid **/
		valid = TRUE;
	    
	}
	
	/** remove \n from user input **/
	    id[strlen(id)-1] = 0;
	
	/** search item_list for id and if match found remove from list **/
    switch(remove_node(system, id))
	{
		/** if match was found **/
		case TRUE:
		{
			return TRUE;
		}
		/** if no match found **/
		case FALSE:
		{
			fprintf(stderr, "Error: desired id was not found.\n");
			return FALSE;
		}
	}
	/** if for some reason previous code is skipped **/
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

BOOLEAN abort_program(struct ppd_system * system)
{
	/* code to free memory and close program without saving */
	return FALSE;
}
