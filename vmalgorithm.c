/*
 *  Implementation of FIFO and LRU page replacement algorithm
 *  Please add appropriate level of comments in this file 
 */

#include <stdio.h>
#include "vmalgorithm.h"

int *accessPattern;
int ReferenceSZ; 
int FrameNR;
PageFrame memory;

/* Generate an access pattern
 * Example: 3, 2, 2, 1, 1  indicates the page 3, 2, 2, 1, 1 in order
 */
void generateAccessPattern()
{
   int i;
   srand(time(0));
   accessPattern = (int *)malloc( sizeof(int) * AccessPatternLength);   
   printf("The randomized Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
		     accessPattern[i] = rand() % ReferenceSZ;
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");
}

/*
 * Initialize the parameters of simulated memory
 */
void initializePageFrame()
{
   int i;
   memory.PageFrameList = (int *)malloc( sizeof(int)* FrameNR );    // dynamic allocated FrameNR frames to be used in memory
   memory.nextToReplaced =0;          // indicate the new frame to be replaced as 0
   for(i=0; i< FrameNR; i++)
   {
			memory.PageFrameList[i] = -1;  // initialization to -1 indicating all frames are unused 
   }

}

// Print the pages loaded in memory
void printPageFrame()
{
   int i;
   for(i=0; i< FrameNR; i++)
   {
			printf("%2d ",memory.PageFrameList[i]);       
   }
   printf("\n");
}


/*
 *  Print the access pattern in order
 */
void printAccessPattern()
{
   int i;
   printf("The Same Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");

}


/*
 * Return: number of the page fault using FIFO page replacement algorithm
 */
int FIFO() {
   //TODO: fill the code to implement FIFO replacement algorithm
   int page_fault;
   int page;
   int i, j;

   // Iterate through all the values in memory access pattern
   for (i = 0; i < AccessPatternLength; i++) {
       page = 0; // Reset to 0 for each iteration of values

       // Iterate through the number of page frames in memory
       // If page is in the memory, it is a page hit
       for (j = 0; j < FrameNR; j++) {
           if (accessPattern[i] == memory.PageFrameList[j]) {
               page = 1;
           }
       }
       // Keep record of the number of page faults
       if (page == 0) {
           page_fault = page_fault + 1;
           memory.PageFrameList[memory.nextToReplaced] = accessPattern[i]; // Load into memory
           printPageFrame();

           memory.nextToReplaced++; // Keep record of the next frame
           if (memory.nextToReplaced == FrameNR) {
               // Once the size of the number of page frames in memory is reached, reset to 0
               // Begin again from the beginning of the memory pages
               memory.nextToReplaced = 0;
           }
       }
       else if (page == 1) {
           printPageFrame();
       }
   }
   return page_fault;
}



/*
 * Return: number of the page fault using LRU page replacement algorithm
 */

int LRU() {
    //TODO: fill the code to implement LRU replacement algorithm
    int page_fault = 0;
    int page;
    int index;
    int frame;
    int *reg;
    int value;
    int clock;
    int i, j, k;

    // This is just to make sure we do not go past the range of pages that can be accessed
    if (FrameNR > ReferenceSZ) {
        frame = ReferenceSZ;
    }
    else {
        frame = FrameNR;
    }

    // Add a register to each frame
    // It will contain the last time that the frame was accessed
    reg = (int *) calloc(frame, sizeof (int));
    for (i = 0; i < frame; i++) {
        reg[i] = 0;
    }

    clock = 0;
    for (i = 0; i < AccessPatternLength; i++) {
        clock = clock + 1; // It increases by 1 each time a memory reference is made
        page = accessPattern[i]; // Obtain the number of the page

        // Find the physical page
        index = -1;
        for (j = 0; j < frame; j++) {
            if (page == memory.PageFrameList[j]) {
                index = j;
                break;
            }
        }

        // If the physical page cannot be found, we have a page fault
        if (index < 0) {
            page_fault = page_fault + 1;
            memory.nextToReplaced = 0; // Find the least recently used slot

            // Cannot exceed the length of the memory access pattern
            value = AccessPatternLength;
            for (j = 0; j < frame; j++) {
                if (reg[j] < value) {
                    memory.nextToReplaced = j;
                    value = reg[j];
                }
            }
            memory.PageFrameList[memory.nextToReplaced] = page; // Update slot
            reg[memory.nextToReplaced] = clock; // Update clock
        }
        else {
            reg[index] = clock; // Update logical time of current slot
        }

        // Print the page frame list
        for (j = 0; j < frame; j++) {
            printf("%3d", memory.PageFrameList[j]);
        }
        printf("\n");
    }
    free(reg); // Deallocate memory
    return page_fault;
}

