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
	struct ppd_node * temp_node = NULL;
	struct ppd_stock * new_stock_item = NULL; 
	int string_compare = 0;
	
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
		/** compare the names of new item and current item **/
		string_compare = strcmp(new_stock_item->name, current->data->name);
		
		/** if new item has higher alphabetical order **/
		if(string_compare < 0)
		{
			break;
		}

		current = current->next;
	}		
	
	/** if node is to be inserted at end of list **/
	if(current -> next == NULL)
	{
		/** add node to end of the list **/
		if(add_to_end_of_list(current, new_stock_item))
		{
			/** increment item_list count **/
			system -> item_list -> count++;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	/** allocate space for new node position **/
	temp_node = malloc(sizeof(struct ppd_node));
	/** check if malloc was successful **/
	if(temp_node == NULL)
	{
		fprintf(stderr, "Error: malloc failed");
		return FALSE;
	}
	/** store data in current node in temp **/
	*temp_node = *current;
	/** store new stock item data in current position **/
	current -> data = new_stock_item;
	
	/** point current node to node that was replaced **/
	current -> next = temp_node;
	
    /** increment item_list count **/	
	system -> item_list -> count++;
		
	return TRUE;
	
}

BOOLEAN add_to_end_of_list(struct ppd_node * current, struct ppd_stock * 
	new_stock_item)
{
	/** allocate space for next postition in the list **/
		current -> next = malloc(sizeof(struct ppd_node));
		/** check if malloc was successful **/
		if(current -> next == NULL)
		{
			fprintf(stderr, "Error: malloc failed");
		    return FALSE;
		}
		
		/** insert data into new node **/
		current -> data = new_stock_item;
		/** point new allocated space to NULL **/
	    current -> next -> next = NULL;
	    
		return TRUE;
}

BOOLEAN remove_node(struct ppd_system * system, char * id)
{
	struct ppd_node * current = system->item_list->head;
	struct ppd_node * previous = system->item_list->head;
	
	/** iterate through item_list **/
	while(current->next != NULL)
	{
		/** check if id matches stock item **/
		if((strcmp(current->data->id, id)) == 0)
		{
			/** if head node **/
			if(current == system->item_list->head)
			{
				system->item_list->head = current->next;
				free(current);
				return TRUE;
			}
			
			/**assign previous item to point to new next item **/
			previous->next = current->next;
			
			/**remove current node from list **/
			free(current);
			
			return TRUE;
			
		}
		/** maintain pointer to previous node **/
		previous = current;
		/** step to next node **/
		current = current->next;
	}
	
	/** if matching id not found **/
	return FALSE;
}

