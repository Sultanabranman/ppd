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
#include "ppd_stock.h"

 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/

BOOLEAN initialise_stock_list(struct ppd_system * system)
{
	struct ppd_stock empty_stock_item;
	struct price empty_price;
	struct ppd_node * empty_node = NULL;
	struct ppd_list * item_list = NULL;
	
	/** create empty price struct **/
	empty_price.dollars = 0;
	empty_price.cents = 0;
	
	/** create empty ppd_stock struct **/
	strcpy(empty_stock_item.id, "");
	strcpy(empty_stock_item.name, "");
	strcpy(empty_stock_item.desc, "");
	empty_stock_item.price = empty_price;
	empty_stock_item.on_hand = 0;
	
	/** create empty node and assign empty ppd_stock struct **/
	empty_node = malloc(sizeof(struct ppd_node));
	/** check if malloc was successful **/
	if(empty_node == NULL)
	{
		fprintf(stderr, "Error: malloc failed");
		return FALSE;
	}
	empty_node -> data = &empty_stock_item;
	empty_node -> next = NULL;
	
	/** create empty item_list struct **/
	item_list = malloc(sizeof(struct ppd_list));
	/** check if malloc was successful **/
	if(item_list == NULL)
	{
		fprintf(stderr, "Error: malloc failed");
		return FALSE;
	}
	item_list -> head = empty_node;
	item_list -> count = 0;
	
	/** assign pointer to list in system struct **/
	system -> item_list = item_list;
	
	return TRUE;
}

BOOLEAN add_new_node(struct ppd_system * system, struct ppd_stock stock_item)
{
	struct ppd_node * current = system->item_list->head;
	struct ppd_stock * new_stock_item = NULL; 
	
	/** create new memory space for ppd_stock struct **/
	new_stock_item = malloc(sizeof(struct ppd_stock));
	
	/** check if malloc was successful **/
	if(new_stock_item == NULL)
	{
		fprintf(stderr, "Error: malloc failed");
		return FALSE;
	}
	
	/** store stock item in new memory address **/
	*new_stock_item = stock_item;
	
	/** if head node has no data **/
	if(current->next == NULL)
	{
		/** point head node to new stock item **/
		current->data = new_stock_item;
		
		/** create new memory address for next node in list **/
		current->next = malloc(sizeof(struct ppd_node));
		
		/** check if malloc was successful **/
		if(current->next == NULL)
		{
			fprintf(stderr, "Error: malloc failed");
			return FALSE;
		}
		/** assign next node as end of list **/
		current->next->next = NULL;
		
		/** increment item_list count **/
		system -> item_list -> count++;
		
		return TRUE;
	}
	
	/** iterate over list until it reaches end of list **/
	while(current -> next != NULL)
	{					
		current = current->next;
	}
		
	current -> next = malloc(sizeof(struct ppd_node));
	/** check if malloc was successful **/
	if(current -> next == NULL)
	{
		fprintf(stderr, "Error: malloc failed");
		return FALSE;
	}
	
	current -> data = new_stock_item;
	current -> next -> next = NULL;
		
	system -> item_list -> count++;
		
	return TRUE;
	
}

