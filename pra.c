#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int rnums[100];	//random numbers
//TEST NUMBERS FROM EXAMPLE
//TO USE THIS ARRAY YOU WILL HAVE TO CHANGE THE ARRAY THAT IS PASSED INTO EACH ALGORITHMS FUNCTION
//int nums2[] = {2,8,7,2,0,3,1,7,3,1,9,3,6,1,8,4,5,1,8,8,3,4,4,3,4,2,0,6,6,7,0,1,0,9,3,6,4,
//6,8,0,4,6,2,3,5,7,8,0,3,2,0,0,0,4,6,9,1,4,3,8,8,0,0,9,7,0,7,0,9,7,7,3,8,8,
//9,2,7,2,1,2,0,9,1,1,1,5,0,7,1,4,9,1,9,5,8,4,4,7,9,6};

int lower=0;	//lower bound
int upper=9;	//upper bound
int* frames;	//frames

void get_rands();	//generate random numbers with no seed
void get_rands_s(int seed);	//generate random numbers with seed provided
void reset(int size); 	//Function to reset frames to -1 as to not affect further calcualtions

int FIFO(int *nums, int *frames, int size);	//FIFO
int LRU(int *nums, int *frames, int size);	//LRU
int OPT(int *nums, int*frames, int size);	//OPT
int check(int *arr, int element, int size);	//function to check if we have a page break
int calculate_opt(int *nums,int count, int size);	//Function to find optimal frame to replace


int main(int argc, char *argv[]){
	if(argc < 2 || argc > 3){
	printf("Usage: %s n s where n is number of frames and s is an OPTIONAL seed for random function\n", argv[0]);
	exit(1);}
	int frame_nums = atoi(argv[1]);	//number of frames provided
	int seed;				//place holder for a seed if provided
	frames = (int*)malloc(frame_nums*4);
	//Check if a seed was provided 
	if (argc == 3){
		seed = atoi(argv[2]);
		get_rands_s(seed);		
	}
	//if no seed provided use the basic random function
	if (argc == 2){
		get_rands();
	}
	reset(frame_nums);	//set all frames to -1
	int fifo, lru, opt;
	printf("Algorithm\t#Page Faults\n\n");
	fifo=FIFO(rnums, frames, frame_nums);		//FIFO
	reset(frame_nums);
	lru=LRU(rnums, frames, frame_nums);		//LRU
	reset(frame_nums);
	opt=OPT(rnums, frames, frame_nums);		//OPT
	printf("FIFO:\t\t%d\n", fifo);
	printf("LRU:\t\t%d\n", lru);
	printf("OPT:\t\t%d\n",opt);
	exit(0);
	
}
int FIFO(int *nums, int *frames, int size){
	int PageBreaks=0;	//# of page breaks
	int count=0;		//# counter to iterate over our random numbers
	int frame_position=0;	//Array position of oldest item
	int status;		//status int to tell us if we have a page break
	while(count < 100){
		status = check(frames, nums[count], size);
		//If we have a page break
		if (status == -1){
			PageBreaks++;	//Increase number of page breaks
			frames[frame_position] = nums[count];	//Alerter frame apporpriatley
			if (frame_position < 4 && frame_position >= 0){
				frame_position++;
			}
			else if(frame_position == 4){
				frame_position=0;	
			}
		}
		count++;
	}
	return PageBreaks;
}
int LRU(int *nums, int *frames, int size){
	int PageBreaks=0;	//# of page breaks
	int count=0;		//# counter to iterate over our random numbers
	int status;		//status int to tell us if we have a page break
	int frame_position=0;
	int first_five=0;	//variable to allow us to fill up the frames before using the LRT algorithm
	int oldest=0;
	int newest;
	while(count < 100){
		status = check(frames, nums[count], size);
		
		//we have a page break
		if(status == -1){
			PageBreaks++;
			//first check if we have filled the array complelty
			if(frames[size-1] == -1){
				frames[first_five] = nums[count];
				first_five++;
				newest=first_five;
			}
			else{
				frames[frame_position] = nums[count];
				newest=frame_position;
				if(frame_position==4){
					frame_position=0;
				}
				else{frame_position++;}
			}
		}
		//If the item already exists 
		//alter position of lru as necessary
		else{
			if(status==oldest){
				if(newest==oldest){
					oldest++;
				}
				if(oldest+1 == newest){
					if(oldest+1 == 4){
						oldest=0;
					}
					else{oldest--;}
				}
			}
			frame_position=oldest;
		}
		count++;
	}
	return PageBreaks;
}
int OPT(int *nums, int *frames, int size){
	int PageBreaks=0;	//# of page breaks
	int count=0;		//# counter to iterate over our random numbers
	int first_five=0;	//variable to help occupy the first frames before using optimal calculations
	int frame_position=0;	//Array position of oldest item for first 5 items
	int status;		//status int to tell us if we have a page break
	while(count < 100){
		status = check(frames, nums[count], size);
		//if we have a page break
		if (status == -1){
			PageBreaks++;	//Increase number of page breaks
			if(frames[size-1] == -1){
				frames[first_five] = nums[count];
				first_five++;
			}
			else{
				frame_position = calculate_opt(nums, count, size);//find optimal frame to replace
				frames[frame_position] = nums[count];
			}
		}
		count++;
	}
	return PageBreaks;
}

//Function to check if the next element is already in frames
//If it is in the array return the array position
//If it is not in the array it return -1
int check(int *arr, int element, int size){
	int return_value = -1;
	for(int i=0; i<size; i++){
		if (arr[i] == element){
			return_value=i; //element is in frames
			break;
		}
	}
	return return_value;
}
//function to set all frame position to -1 so they dont interfere with other calculations
void reset(int size){
	for(int i=0; i<size; i++){
		frames[i] = -1;
	}
}
//return position of optimal element to replace	
int calculate_opt(int* nums, int count, int size){
	int position=0;	//position in frames to replace
	int farthest_elements[size];
	for(int i=0; i<size; i++){
		farthest_elements[i] = 0;
	}
	//loop through each number in the frames
	for(int i=0; i<size; i++){
		int no_repeats=0;
		for(int j=count; j<100; j++){
			if(frames[i] == nums[j]){
				farthest_elements[i]=j;
				no_repeats=1;
				break;
			}
		}
		if(no_repeats==0){
			farthest_elements[i]=0;
		}
	}
	//find the farthest element or the element that is not seen again with the fifo rule in place for
	//ties
	int farthest=farthest_elements[0];	//base case to compare
	for(int i=0; i<size; i++){
	
		//first check for both zero and zero tie
		if(farthest_elements[i]==0){
			position=i;
			break;
		}	
		else if(farthest_elements[i] !=0 && farthest_elements[i] > farthest){
			farthest=farthest_elements[i];
			position=i;
		}
	}
return position;	
}
//Get randome numbers without a provided seed value
void get_rands(){
int count = 0;
	srand(time(NULL));
	while (count < 100){
	int num = (rand() % (upper - lower + 1)) + lower;
	rnums[count] = num;
	count++;
	}
}
//Get randome numbers with a provided seed value
void get_rands_s(int seed){
	int count = 0;
	srand(seed);
	while (count < 100){
	int num = (rand() % (upper - lower + 1)) + lower;
	rnums[count] = num;
	count++;	
	}
}
