/********LRU/OPT/CLOCK/FIFO Page Replacement Simulation********
 ********************Author: Jeff Godfrey*********************/

#pragma once
#include<stdio.h>
#include<stdlib.h>
#include "Linked_list.h"


//FIFO Page Replacement Algorithm
void FIFO(list_t *page_table, int page_address, int frame_size)
{
	extern int page_faults;

	//If page already exists in page table
	if(page_table_search(page_table, page_address))
		return;

	//If page table not yet full
	if(page_table->size < frame_size)
	{
		newnode(page_table, page_address);
		page_faults++;
		return;
	}

	//If page table already full
	if(page_table->size == frame_size)
	{
		list_remove(page_table);
		newnode(page_table, page_address);
		page_faults++;
		return;
	}
}

//LRU Page Replacement Algorithm
void LRU(list_t *page_table, int page_address, int frame_size)
{
	extern int page_faults;
	int temp_counter;

	//If page already exists in page table
	if(page_table_search(page_table, page_address))
	{
		element_t *addr = page_table->head;
		
		while(addr != NULL)
		{
			if(addr->data == page_address)
				temp_counter = addr->LRU_counter;
			addr = addr->next;
		}

		addr = page_table->head;

		while(addr != NULL)
		{
			if(addr->data == page_address)
				addr->LRU_counter = 0;
			else if(addr->LRU_counter < temp_counter)
				addr->LRU_counter++;
			addr = addr->next;
		}

		return;
	}
	
	//If page table is not yet full
	if(page_table->size < frame_size)
	{
		element_t *addr = page_table->head;
		
		newnode(page_table, page_address);
		
		while(addr != NULL)
		{
			addr->LRU_counter++;
			addr = addr->next;
		}

		page_table->tail->LRU_counter = 0;
		page_faults++;

		return;
	}

	//If page table is full
	if(page_table->size == frame_size)
	{
		element_t *addr = page_table->head;

		while(addr != NULL)
		{
			if(addr->LRU_counter == (frame_size - 1))
			{
				addr->data = page_address;
				addr->LRU_counter = 0;
			}
			else
				addr->LRU_counter++;
			
			addr = addr->next;
		}

		page_faults++;
	}
}

//CLOCK Page Replacement Algorithm
void Clock(list_t *page_table, int page_address, int frame_size)
{
	extern int page_faults;

	//If page already exists in page table
	if(page_table_search(page_table, page_address))
	{
		element_t *addr = page_table->head;

		while(addr != NULL)
		{
			if(addr->data == page_address)
				addr->clock_fresh = true;

			addr = addr->next;
		}

		return;
	}

	//If page table is not yet full
	if(page_table->size < frame_size)
	{
		element_t *addr = newnode(page_table, page_address);
		addr->clock_fresh = true;
		page_table->clock = addr;
		
		page_faults++;

		return;
	}

	//If page table is full
	if(page_table->size == frame_size)
	{
		while(1)
		{
			if(page_table->clock->clock_fresh == false)
			{
				page_table->clock->data = page_address;
				page_table->clock->clock_fresh = true;
			}
			else
				clock_advance(page_table);

			page_faults++;

			return;
		}
	}
}

//OPT Page Replacement Algorithm
void OPT(list_t *page_table, list_t *address_stream, int page_address, int frame_size)
{
	extern int page_faults;

	//If page already exists in page table
	if(page_table_search(page_table, page_address))
		return;

	//If page table is not yet full
	else if(page_table->size < frame_size)
	{
		newnode(page_table, page_address);
		
		page_faults++;

		return;
	}

	//If page table is full
	else if(page_table->size == frame_size)
	{	
		element_t *addr = page_table->head;

		while(addr != NULL)
		{
			addr->opt_flag = false;
			addr = addr->next;
		}
		
		addr = address_stream->head;

		while(addr != NULL)
		{
			if(opt_unflagged(page_table) == 1)
				break;

			element_t *page_entry = page_table->head;

			while(page_entry != NULL && (opt_unflagged(page_table) > 1))
			{
				if(page_entry->data == addr->data)
				{
					page_entry->opt_flag = true;
					break;
				}
				
				page_entry = page_entry->next;
			}
			
			addr = addr->next;
		}

		addr = page_table->head;

		while(addr != NULL)
		{
			if(addr->opt_flag == false)
			{
				addr->data = page_address;
				break;
			}

			addr = addr->next;
		}

		page_faults++;
	}	
	else
	{
		printf("ErrorOPT");
		exit(EXIT_FAILURE);
	}	
}

