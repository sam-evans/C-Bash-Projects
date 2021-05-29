// Name: Samuel Evans
//Jimmie Gann
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int comparePages(int *a, int a_size, int ref_num);
int FIFO(int *r, int r_size, int f_size);
int LRU(int *r, int r_size, int f_size);
int OPT(int *r, int r_size, int f_size);

// main
int main(int argc, char *argv[])
{
	int f_size = atoi(argv[1]);
	int r_size = 100; 
	int ref[r_size];
	//example ref string
	//int ref[100]={2, 8, 7, 2, 0, 3, 1, 7, 3, 1, 9, 3, 6, 1, 8, 4, 5, 1, 8, 8, 3, 4, 4, 3, 4, 2, 0, 6, 6, 7, 0, 1, 0, 9, 3, 6, 4, 6, 8, 0, 4, 6, 2, 3, 5, 7, 8, 0, 3, 2, 0, 0, 0, 4, 6, 9, 1, 4, 3, 8, 8, 0, 0, 9, 7, 0, 7, 0, 9, 7, 7, 3, 8, 8, 9, 2, 7, 2, 1, 2, 0, 9, 1, 1, 1, 5, 0, 7, 1, 4, 9, 1, 9, 5, 8, 4, 4, 7, 9, 6};	

	// generates random array
	srand(time(NULL));
	for (int i = 0; i < r_size; i++)
	{
		ref[i] = rand() % 10;
		printf("%d ",ref[i] );
	}
	printf("\nFIFO page faults: %i\n", FIFO(ref, r_size, f_size));
	printf("\nLRU page faults: %i\n", LRU(ref, r_size, f_size));
	printf("\nOPT page faults: %i\n", OPT(ref, r_size, f_size));
}


//checks if a number is in the array *a
int comparePages(int *a, int a_size, int ref_num)
{
	for (int i = 0; i < a_size; i++)
	{
		if (*a == ref_num){
			return 1;
		break;
		}
		a++;
	}
	return 0;
}

// FIFO 
int FIFO(int *r, int r_size, int f_size)
{
	int frames[f_size];
	int ref[r_size];
	int faults = 0;
	
	for (int i = 0; i < r_size; i++)
	{
		ref[i] = *r;
		r++;
	}
	
	int hit = 0;
	int count = 0;
	int r_index = 0;
	
	while (count < f_size && (r_index < r_size))	
	{
		for (int b = 0; b < f_size; b++)
		{
			if (frames[b] == ref[r_index])
			{
				hit = 1; //no page fault
			}
		}
		if (hit == 0)
		{
			frames[count] = ref[r_index];//page fault swap takes place
			faults++;
			count++; 
		}
		
		hit = 0;
		r_index++;
	}

	
//After filling the blanks represented by -1, to implement FIFO its safe to assume the first number in the array is the oldest 
//because of this the second is the second oldest and so on.
//So, when swapping pages i move the every element up one position so the oldest element is kept track of in the first position
	for (int i = 0; i < r_size; i++)
	{
		if (comparePages(frames, f_size, ref[i]) == 0)
		{
			faults++;

			int x = 0;
			while(1)
			{
				frames[x] = frames[x+1]; //gets rid of the oldest number and moves each number down one position 
				if (x == f_size-2)
				{
					break;
				}
				x++;
			}
			frames[f_size-1] = ref[i];//placing the newest page at the end of the array
			
			
		}
		
	}
	return faults;
}

// LRU function
int LRU(int *r, int r_size, int f_size)
{
	int frames[f_size][2];
	int ref[r_size];
	int faults = 0;
	
	// makes a copy of the reference list for the LRU function
	for (int i = 0; i < r_size; i++)
	{
		ref[i] = *r;
		r++;
	}

	for (int r = 0; r < f_size; r++)
	{
		frames[r][0] = -1;
	} 

	int isFound = 0;
	int count = 0;
	int r_index = 0;
	while (count < f_size)
	{
		// for loop that checks if the current number from the reference list is already in the frames array or not
		for (int z = 0; z < f_size; z++)
		{
			if (frames[z][0] == ref[r_index])
			{
				isFound = 1;
			}
		}
		// adds the current the current reference number if it was not found in the frames array and increments faults 
		if (isFound == 0)
		{	
			frames[count][0] = ref[r_index];
			frames[count][1] += 1;
			faults++;
			count++;
		}
		
		isFound = 0;
		r_index++;
		
	}

	for (int i = 0; i < r_size; i++)
	{	
		int isThere = 0;
		// checks if the current reference number is in the array
		for (int z = 0; z < f_size; z++)
		{
			if (frames[z][0] == ref[i])
			{
				isThere = 1;
			}
		}
		if (isThere == 0)
		{
			faults++;
			int index = 0;
			int last_used = frames[0][1];
			for (int y = 0; y < f_size; y++)
			{
				if (frames[y][1] > last_used)
				{
					index = y;
					last_used = frames[y][1];
				}
			}		
			frames[index][0] = ref[i];
			frames[index][1] = 0;
		}
		int index2;
		for (int x = 0; x < f_size; x++)
		{
			frames[x][1] += 1;
			if (frames[x][0] == ref[i])
			{
				index2 = x;
			}
		}
		frames[index2][1] = 0;			
	}
	return faults;
}

// OPT 
int OPT(int *r, int r_size, int f_size)
{
	int frames[f_size][2];
	int ref[r_size];
	int faults = 0;

	for (int i = 0; i < r_size; i++)
	{
		ref[i] = *r;
		r++;
	}
	for (int r = 0; r < f_size; r++)
	{
		frames[r][0] = -1;
	}
	int hit = 0;
	int count = 0;
	int r_index = 0;
	
	while (count < f_size)
	{
		
		for (int z = 0; z < f_size; z++)
		{
			if (frames[z][0] == ref[r_index])
			{
				hit = 1;
			}
		}
		
		if (hit == 0)
		{	
			frames[count][0] = ref[r_index];
			frames[count][1] = 0;
			faults++;
			count++;
		}
		hit = 0;
		r_index++;
	}

	
	for (int i = 0; i < r_size; i++)
	{	
		int isThere = 0;
		// checks if the current reference number is in the array
		for (int z = 0; z < f_size; z++)
		{
			if (frames[z][0] == ref[i])
			{
				isThere = 1;
			}
		}
		
		if (isThere == 0)
		{
			faults++;
			int furthest = frames[0][1];
			
			for (int y = 0; y < f_size; y++)
			{
				int counter = 0;
				// for loop to find the next occurance for a specific index
				for (int w = i ; w < r_size; w++)
				{
					if (ref[w] == frames[y][0])
					{					
						frames[y][1] += counter;
						break;
					}
					counter++;
					if (w == r_size-1)
					{
						frames[y][1] += r_size;
					}
				}
			}
			// determines what frames need to be replaced
			int index;
			int biggest_gap = -1;
			for (int z = 0; z < f_size; z++)
			{
				if (frames[z][1] > biggest_gap)
				{
					biggest_gap = frames[z][1];
					index = z;
				}
			}
			// replaces the frame
			frames[index][0] = ref[i];
			
			// resets the frames back to zero
			for (int t = 0; t < f_size; t++)
				frames[t][1] = 0;
		}
		
	}
	return faults;
}
