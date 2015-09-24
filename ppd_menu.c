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

#include "ppd_menu.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu(struct menu_item* menu)
{
	/** loop counter **/
	int i;
	struct menu_item temp_menu_item;
	
	/** Array of menu option names **/
	const char * menu_options[NUM_MENU_CHOICES] =
	{
		"Display Items",
		"Purchase Items",
		"Save and Exit",
		"Add Item",
		"Remove Item",
		"Display Coins",
		"Reset Stock",
		"Reset Coins",
		"Abort Program"
	};
	
	/** Array of menu option function pointers **/
	options menu_functions[NUM_MENU_CHOICES] =
	{
		display_items,
		purchase_item,
		save_system,
		add_item,
		remove_item,
		display_coins,
		reset_stock,
		reset_coins,
		abort_program
	};
	
	/** Loop to store menu items into passed in array **/
	for(i = 0; i < NUM_MENU_CHOICES; i++)
	{
		strcpy(temp_menu_item.name, menu_options[i]);		
		temp_menu_item.function = menu_functions[i];		
		menu[i] = temp_menu_item;
	}
	
	return;
}

/** Displays the menu items from menu array **/
enum menu_input display_menu(struct menu_item* menu)
{
	/** loop counter **/
	int i;
	char line[LINELEN + EXTRACHARS];
	char * end;
	enum menu_input input;
	
	
	for(i = 0; i < NUM_MENU_CHOICES; i++)
	{
		if(i == 0)
		{
			printf("Main Menu:\n");
		}
		if(i == 4)
		{
			printf("Administrator-Only Menu:\n");
		}
		printf("   %d: %s\n", i+1, menu[i].name);
	}
	printf("Select your option (1-9):\n");
	if((fgets(line, LINELEN + EXTRACHARS, stdin)) == NULL)
	{
		fprintf(stderr, "Error: Input was blank\n");
		return INVALID;
	}
		
	if(line[strlen(line)-1] != '\n')
	{
		read_rest_of_line();
		fprintf(stderr, "Error: line entered was too long. " 
			"Please try again\n");
		return INVALID;
	}
	
	line[strlen(line)-1] = 0;
	input = strtol(line, &end, 0);
	
	if(*end)
	{
		fprintf(stderr, "Error: Input was not a number. Please try again\n");
		return INVALID;
	}
	
	--input;
	
	if(input < 0 || input >= INVALID)
	{
		fprintf(stderr, "Error: Not a valid selection\n");
		return INVALID;
	}
	return input;
}