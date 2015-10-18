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
	char id[IDLEN+EXTRACHARS];
	char price_entered[PRICELEN+EXTRACHARS];
	struct ppd_stock * selected_item = NULL;
	struct price amount_owing;
	BOOLEAN valid = FALSE;
	int price = 0;
	int amount_remaining = 0;
	char * end;
	int amount_inserted[1999] = {0};
	int i = 0;
	int k;
	enum denomination current_amount;	
	
    printf("Purchase Item\n");
	printf("-------------\n");
	
	while(valid != TRUE)
	{
		printf("Please enter the id of the item you wish to purchase: ");
	
	    /** get id **/
	    switch(get_input(id, IDLEN))
	    {
		    case TRUE:
		    {
			    if(id[1] == '\0')
			    {				
			    	return FALSE;
			    }
			
			    /** search item_list for id **/
	            switch(search_for_id(system, id))
	            {
		            case TRUE:
		            {
						selected_item = get_selected_item(system, id);
						if(selected_item->on_hand == 0)
						{
							fprintf(stderr, "Error: selected item is out of "
								"stock\n");
							continue;
						}
			            valid = TRUE;
						break;
		            }
		            case FALSE:
		            {
			            fprintf(stderr, "Error: the id you entered was not "
						    "valid. Please try again.\n");
						continue;
		            }
	            }
				break;
		    }
		    case FALSE:
		    {
			    fprintf(stderr, "Error: you did not enter a valid id. " 
				    "Please try again.\n");
			    continue;
		    }
	    }
	}
	
	printf("You have selected %s - %s. This will cost you $%d.%02d\n", 
		selected_item->name, selected_item->desc, selected_item->price.dollars
		, selected_item->price.cents);
	
	printf("Please hand over the money - type in the value of each note/coin" 
		"in cents.\n");
	printf("Press enter or ctrl-d on a new line to cancel this purchase:\n");
	
	/** set amount owing to price of item_selected **/
	amount_owing.dollars = selected_item->price.dollars;
	amount_owing.cents = selected_item->price.cents;
	
	/** convert price to cents **/
	amount_remaining = (amount_owing.dollars * 100) + amount_owing.cents;
	
	/** loop until price has been filled **/
	while(amount_remaining > 0)
	{
		printf("You still need to give us $%d.%02d: ", amount_owing.dollars,
			amount_owing.cents);
		
		/** get user input **/
		switch(get_input(price_entered, PRICELEN))
		{
			case TRUE:
			{
				/** if RTM **/
				if(price_entered[1] == '\0')
				{
					return FALSE;
				}
				
				/** convert input to integer **/
				price = strtol(price_entered, &end, 0);
				
				/** if an integer wasn't entered **/
				if(*end)
				{
					fprintf(stderr, "Error: you did not enter a valid integer."
						" Please try again.\n");
					continue;
				}
				/** check input is a valid denomination **/
				switch(check_price_input(price))
				{
					case TRUE:
					{
						break;
					}
					case FALSE:
					{
						fprintf(stderr, "Error: valid denomination was not "
							"entered. Please try again.\n");
						continue;
					}
				}
				
				break;
			}
			case FALSE:
			{
				continue;
			}
		}
		/** reset loop counter **/
		i = 0;
		
		
		/** store denominations currently entered into array **/
		while(TRUE)
		{
			if(amount_inserted[i] == 0)
			{
				amount_inserted[i] = price;
				break;
			}
			
			i++;
		}		
		
		/** deduct value from amount owing **/
		amount_remaining -= (price);	
		
		/** get dollar value from amount remaining **/
		amount_owing.dollars = (amount_remaining/100);		
		
		/** get cents value from amount remaining **/
		amount_owing.cents = (amount_remaining % 100);	
		
	}
	
	
	
	/** give any change required **/
	
	/** deduct item from stock **/
	deduct_from_stock(id, system);
	
	/** deduct coins from register **/
	
	
	
	/** reset loop counter **/
	i = 0;
	
	/** add coins to register **/
	while(amount_inserted[i] != 0)
	{
		current_amount = convert_denom(amount_inserted[i]);
		
		for(k = 0; k < NUM_DENOMS; k++)
		{
			if(system->cash_register[k].denom == current_amount 
				&& current_amount == FIVE_CENTS)
			{
				system->cash_register[k].count++;
				break;
			}
			else if(system->cash_register[k].denom == current_amount 
				&& current_amount == TEN_CENTS)
			{
				system->cash_register[k].count++;
				break;
			}
			else if(system->cash_register[k].denom == current_amount 
				&& current_amount == TWENTY_CENTS)
			{
				system->cash_register[k].count++;
				break;
			}
			else if(system->cash_register[k].denom == current_amount 
				&& current_amount == FIFTY_CENTS)
			{
				system->cash_register[k].count++;
				break;
			}
			else if(system->cash_register[k].denom == current_amount 
				&& current_amount == ONE_DOLLAR)
			{
				system->cash_register[k].count++;
				break;
			}
			else if(system->cash_register[k].denom == current_amount 
				&& current_amount == TWO_DOLLARS)
			{
				system->cash_register[k].count++;
				break;
			}
			else if(system->cash_register[k].denom == current_amount 
				&& current_amount == FIVE_DOLLARS)
			{
				system->cash_register[k].count++;
				break;
			}
			else if(system->cash_register[k].denom == current_amount 
				&& current_amount == TEN_DOLLARS)
			{
				system->cash_register[k].count++;
				break;
			}
		}
		
		i++;
			
	}
	
	printf("Here is your %s and your change of ", selected_item->name);
	printf("Please come again soon\n");
	
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    FILE * coin_file;
	FILE * stock_file;
	int i;
	enum denomination j;
	
	struct ppd_node * current = system->item_list->head;
		
	/** open coin file in write mode **/
	coin_file = fopen(system->coin_file_name, "w");
	
	/** if coin file failed to open **/
	if(coin_file == NULL)
	{
		fprintf(stderr, "Error: coin file failed to open.\n");
		return FALSE;
	}	
	
	for(i = 0; i < NUM_DENOMS; i++)
	{
		for(j = FIVE_CENTS; j <= TEN_DOLLARS; j++)
		{
			if(system->cash_register[i].denom == FIVE_CENTS 
				&& j == FIVE_CENTS)
			{
				fprintf(coin_file, "%d,%d\n", 5, system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == TEN_CENTS 
				&& j == TEN_CENTS)
			{
				fprintf(coin_file, "%d,%d\n", 10, system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == TWENTY_CENTS 
				&& j == TWENTY_CENTS)
			{
				fprintf(coin_file, "%d,%d\n", 20, system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == FIFTY_CENTS 
				&& j == FIFTY_CENTS)
			{
				fprintf(coin_file, "%d,%d\n", 50, system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == ONE_DOLLAR 
				&& j == ONE_DOLLAR)
			{
				fprintf(coin_file, "%d,%d\n", 100, system->cash_register[i]
					.count);
			}
			else if(system->cash_register[i].denom == TWO_DOLLARS 
				&& j == TWO_DOLLARS)
			{
				fprintf(coin_file, "%d,%d\n", 200, system->cash_register[i]
					.count);
			}
			else if(system->cash_register[i].denom == FIVE_DOLLARS 
				&& j== FIVE_DOLLARS)
			{
				fprintf(coin_file, "%d,%d\n", 500, system->cash_register[i]
					.count);
			}
			else if(system->cash_register[i].denom == TEN_DOLLARS 
				&& j == TEN_DOLLARS)
			{
				fprintf(coin_file, "%d,%d\n", 1000, system->cash_register[i]
					.count);
			}
		}
	}
	
	/** open stock file in write mode **/
	stock_file = fopen(system->stock_file_name, "w");
	
	/** if stock file failed to open **/
	if(stock_file == NULL)
	{
		fprintf(stderr, "Error: stock file failed to open.\n");
		return FALSE;
	}
	
	/** store all stock list data into stock file **/
	while(current->next != NULL)
	{
		fprintf(stock_file, "%s|%s|%s|%d.%d|%d\n", current->data->id,
			current->data->name, current->data->desc, current->data->price
			.dollars, current->data->price.cents, current->data->on_hand);
		
		current = current->next;
	}
	
	/** close both files **/
	fclose(coin_file);
	fclose(stock_file);
	
	/** free all malloc'ed memory **/
	system_free(system);
	
	/** close the program **/
	exit(EXIT_SUCCESS);	
	
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
	
	/** increment id **/
	id_num++;
	
	/** append I to start of id **/
	sprintf(id, "I%04d", id_num);
	
	/** store id in new item struct **/
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
        if(name[1] == '\0')	
		{
			printf("\nOperation cancelled at the user's request.\n");
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
	    if(desc[1] == '\0')
	    {
		    printf("\nOperation cancelled at the user's request.\n");
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
	    if(price[1] == '\0')
	    {
		    printf("\nOperation cancelled at the user's request.\n");
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
	    if(id[1] == '\0')
	    {
		    printf("\nOperation cancelled at the user's request.\n");
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
	struct ppd_node * current = system->item_list->head;
	
	/** Set all stock items to default stock level **/
	while(current->next != NULL)
	{
		current->data->on_hand = DEFAULT_STOCK_LEVEL;
		current = current->next;
	}
    
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    int i;
	
	for(i = 0; i < NUM_DENOMS; i++)
	{
		system->cash_register[i].count = DEFAULT_COIN_COUNT;
	}
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
	/** loop counters **/
	int i;
	enum denomination j;
	
    printf("Coins Summary\n");
	printf("-------------\n");
	printf("%-13s | %10s\n", "Denomination", "Count");
	printf("--------------------------\n");
	
	for(j = FIVE_CENTS; j <= TEN_DOLLARS; j++)
	{
		for(i = 0; i < NUM_DENOMS; i++)
		{
			if(system->cash_register[i].denom == FIVE_CENTS 
				&& j == FIVE_CENTS)
			{
				printf("%-13s | %10d\n", "5 Cents", system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == TEN_CENTS 
				&& j == TEN_CENTS)
			{
				printf("%-13s | %10d\n", "10 Cents", system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == TWENTY_CENTS 
				&& j == TWENTY_CENTS)
			{
				printf("%-13s | %10d\n", "20 Cents", system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == FIFTY_CENTS 
				&& j == FIFTY_CENTS)
			{
				printf("%-13s | %10d\n", "50 Cents", system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == ONE_DOLLAR 
				&& j == ONE_DOLLAR)
			{
				printf("%-13s | %10d\n", "1 Dollar", system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == TWO_DOLLARS 
				&& j == TWO_DOLLARS)
			{
				printf("%-13s | %10d\n", "2 Dollars", system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == FIVE_DOLLARS 
				&& j== FIVE_DOLLARS)
			{
				printf("%-13s | %10d\n", "5 Dollars", system->cash_register[i].count);
			}
			else if(system->cash_register[i].denom == TEN_DOLLARS 
				&& j == TEN_DOLLARS)
			{
				printf("%-13s | %10d\n", "10 Dollars", system->cash_register[i].count);
			}
		}		
	}	
	
    return TRUE;
}

BOOLEAN abort_program(struct ppd_system * system)
{
	/** free system memory **/
	system_free(system);
	
	/** exit program **/
	exit(EXIT_SUCCESS);
	
	return FALSE;
}
