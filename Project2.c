/******************************LRU/OPT/CLOCK/FIFO Page Replacement Simulation******************************
 ******************************************Author: Jeff Godfrey*******************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/queue.h>
#include <stdbool.h>
#include "Linked_list.h"
#include "Algorithms.h"

int page_faults;

void main(int argc, char *argv[])
{
	page_faults = 0;

	//If incorrect usage, exit program
	if(argc != 5)
	{
		printf("Usage : ./<program name> <input file name> <beginning frame> <ending frame>\n");
		exit(EXIT_FAILURE);
	}	

	int frame_start = atoi(argv[2]);    //Allocate beginning frame to variable frame_start
	int frame_end = atoi(argv[3]);	    //Allocate end frame to variable frame_end
	int number_pages = atoi(argv[4]);   //Allocate number of pages in process to variable number_pages
	FILE *inputFile;	//File type to open and read from data file
	
	//If file doesn't exist, exit program
	if((inputFile = fopen(argv[1], "r")) == 0)
	{
		printf("Unable to open file, check to see if it exists\n");
		exit(EXIT_FAILURE);
	}

	//The below counts the number of lines, and therefore addresses, in the input file
	int line_count = 0;
	char ch;

	while(!feof(inputFile))
	{
		ch = fgetc(inputFile);
		if(ch == '\n')
		{
			line_count++;
		}
	}

	//Reset file pointer
	rewind(inputFile);

	//Determine maximum number of digits to be scanned in per line
	int n = number_pages;
	int digit_count = 0;

	while(n != 0)
	{
		n /= 10;
		digit_count++;
	}	


	list_t memory;		//List of page address references
	list_init(&memory);	//Initialize list head, tail, and size

	//Populate list with address stream
	for(int i = 0; i < line_count; i++)
	{
		int data;
		fscanf(inputFile, "%d", &data);

		//If file contains address larger than number_pages-1, exit program
		if(data >= number_pages)
		{
			printf("File contains page addresses outside of maximum. Please modify file, or increase page number in process\n");
			exit(EXIT_FAILURE);
		}
		newnode(&memory, data);
	}
	
	//Preformatting FIFO data run
	printf("POLICY\tPAGES\tFRAMES:\t");

	for(int i = frame_start; i <= frame_end; i++)
		printf("%d\t", i);

	printf("\nFIFO\t%d\tFAULTS\t", number_pages);
	//Finished preformatting

	int frame_range = frame_end - frame_start + 1;	//Initialize range of frames

	list_t page_table;
	list_init(&page_table);	//Initialize page table

	//Run of FIFO from frame_start to frame_end
	for(int i = frame_start; i <= frame_end; i++)
	{
		element_t *addr = memory.head;	//Initialize addr with the beginning of the page address stream

		while(addr != NULL)	//While there are still items in the list
		{
			FIFO(&page_table, addr->data, i);
			addr = addr->next;
		}

		printf("%d\t", page_faults);	//Output page fault data
		page_faults = 0;
		
		while(page_table.size)		//Remove all elements from page table
			list_remove(&page_table);
	}

	//Preformatting LRU data run
	printf("\nLRU\t%d\tFAULTS\t", number_pages);

	list_init(&page_table); //Initialize page table

	//Run of LRU from frame_start to frame_end
	for(int i = frame_start; i <= frame_end; i++)
	{
		element_t *addr = memory.head;	//Initialize addr with the beginning of the page address stream

		while(addr != NULL)	//While there are still items in the list
		{
			LRU(&page_table, addr->data, i);
			addr = addr->next;
		}

		printf("%d\t", page_faults);	//Output page fault data
		page_faults = 0;

		while(page_table.size)		//Remove all elements from page table
			list_remove(&page_table);
	}

	//Preformatting Clock data run
	printf("\nClock\t%d\tFAULTS\t", number_pages);

	list_init(&page_table);	//Initialize page table

	//Run of Clock from frame_start to frame_end
	for(int i = frame_start; i <= frame_end; i++)
	{
		element_t *addr = memory.head;	//Initialize addr with the beginning of the page address stream

		while(addr != NULL)	//While there are still items in the list
		{
			Clock(&page_table, addr->data, i);
			addr = addr->next;
		}
		
		printf("%d\t", page_faults);	//Output page fault data
		page_faults = 0;

		while(page_table.size)		//Remove all elements from page table
			list_remove(&page_table);
	}

	//Preformatting OPT data run
	printf("\nOPT\t%d\tFAULTS\t", number_pages);

	list_init(&page_table);	//Initialize page table

	//Run of OPT from frame_start to frame_end
	for(int i = frame_start; i <= frame_end; i++)
	{
		element_t *addr = memory.head;	//Initialize addr with the beginning of the page address stream

		while(addr != NULL)
		{
			list_t temp;
			temp.head = addr->next;
			temp.tail = memory.tail;
			OPT(&page_table, &temp, addr->data, i);
		
			addr = addr->next;
		}	
	
		printf("%d\t", page_faults);
		page_faults = 0;

		while(page_table.size)
			list_remove(&page_table);
	}

	printf("\n");


	return;

}
