/**Linked List to Implement LRU/OPT/CLOCK/FIFO Page Replacement Algorithms**
 *************************Author: Jeff Godfrey*****************************/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Elements of list
typedef struct ELEMENT_T{
	struct ELEMENT_T *next;
	bool opt_flag;
	int data;
	int LRU_counter;
	bool clock_fresh;
}element_t;

//List
typedef struct{
	element_t *head;
	element_t *tail;
	element_t *clock;
	long size;
}list_t;

//This function adds a new elements to the list, which will now be the tail
element_t* newnode(list_t *ptr, int data)
{
	element_t* newnode = (element_t*) malloc(sizeof(element_t));
	newnode->data = data;

	newnode->next = NULL;

	if(ptr->head == NULL)
	{
		ptr->head = newnode;

	}
	else
	{
		ptr->tail->next = newnode;
	}
	ptr->tail = newnode;
	ptr->size++;
	
	return newnode;
}

//This function initializes size of list to zero, and dereferences head and tail
void list_init(list_t *ptr)
{
	ptr->head = NULL;
	ptr->tail = NULL;
	ptr->clock = NULL;
	ptr->size = 0;
}

//List function removes an element from the head of the list
void list_remove(list_t *ptr)
{
	if(ptr->head == NULL)
		return;
	if(ptr->tail == ptr->head)
		ptr->tail = NULL;

	element_t *newhead = ptr->head->next;
	free(ptr->head);
	ptr->head = newhead;
	ptr->size--;
}

//This function returns true if value is found in list, false otherwise
bool page_table_search(list_t *ptr, int input)
{
	element_t *temp = ptr->head;
	
	while(temp != NULL)
	{
		if(temp->data == input)
			return true;
		temp = temp->next;
	}

	return false;
}

//This function advances the clock pointer, used in the Clock replacement algorithm
element_t *clock_advance(list_t *ptr)
{
	if(ptr->clock->next != NULL)
		ptr->clock = ptr->clock->next;
	else
		ptr->clock = ptr->head;
	return(ptr->clock);
}

/*This function calculates how many unflagged elements remain in the page table,
 * used for the OPT replacement algorithm*/
int opt_unflagged(list_t *ptr)
{
	int unflagged_count = 0;
	element_t *element = ptr->head;

	while(element != NULL)
	{
		if(element->opt_flag == false)
			unflagged_count++;

		element = element->next;
	}

	return unflagged_count;
}

