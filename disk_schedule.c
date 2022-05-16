#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


//int nums[] ={55,58,39,18,90,160,150,38,184};	//Example from lecture 13
int nums[] ={110,22,3,11,75,180,100,85,60,23};	//Example from bonus assignment
int temp_nums[sizeof(nums)];	//nums to manipulate as to not affect originals
int serviced_requests[sizeof(nums)];	//array to show traversal path of each algorithm
int find_min(int *arr, int location, int arr_size);	//find shortest service time to travel
float FCFC(int *arr, int start_point, int arr_size);	//First come first serve
float SSTF(int *arr, int start_point, int arr_size);	//Shortest service time first
float SCAN(int *arr, int start_point, int arr_size);	//SCAN
float C_SCAN(int *arr, int start_point, int arr_size);//C-SCAN
void print_serviced_requests(int arr_size);		//print our traveresed numbers in order 
void reset_arrs(int arr_size);//reset our temporary nums that we have to mess with, I could have just had 4 temporary arrays but I wanted to save memory, it also resets our serviced nums to appropriatley show the traversals of the different algorithms
void s_sort(int *arr_to_modify, int start_value, int arr_size);	//Special sort algorithm for SCAN
void c_sort(int *arr_to_modify, int start_value, int arr_size);	//Special sort algorithm for C-SCAN



int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Usage: %s n where n is starting point for head\n", argv[0]);
	}
	int size=sizeof(nums)/sizeof(nums[0]);	//size of num array
	for(int i=0; i<size; i++){			
		temp_nums[i] = nums[i];
	}
	int start_point = atoi(argv[1]);
	float fcfc=FCFC(nums, start_point, size);
	printf("Algorithm\t\tAverage seek length\n");
	printf("FCFC\t\t\t%.2f\n",fcfc);
	print_serviced_requests(size);
	reset_arrs(size);
	float sstf=SSTF(temp_nums, start_point, size);
	printf("SSTF\t\t\t%.2f\n",sstf);
	print_serviced_requests(size);
	reset_arrs(size);
	float scan = SCAN(temp_nums, start_point, size);
	printf("SCAN\t\t\t%.2f\n",scan);
	print_serviced_requests(size);
	reset_arrs(size);
	float c_scan=C_SCAN(temp_nums, start_point, size);
	printf("C-SCAN\t\t\t%.2f\n",c_scan);
	print_serviced_requests(size);


}

float FCFC(int *arr, int start_point, int arr_size){
	float average_seek_length;		//average seek length
	float distance_traveled=0.0;		//total distance traveled
	int current_location = start_point;	//current position of head
	int new_value;				//value to 
	for(int i=0; i<arr_size; i++){
		new_value=arr[i]-current_location;	//Calculate difference from current position to next position
		if(new_value < 0){
			new_value = new_value * -1;
		}
		distance_traveled+=new_value;
		current_location=arr[i];
		serviced_requests[i]=arr[i];
		//printf("Next Track acessed: %d\t\t# of tracks traversed:%d\n",arr[i], new_value);
		//fflush(stdout);	
	}
	average_seek_length=(distance_traveled/arr_size);
	return(average_seek_length);
}
float SSTF(int *arr, int start_point, int arr_size){
	float average_seek_length;	//average seek length
	int new_value;			//value to add to distance traveled
	float distance_traveled=0.0;	//total distance we have covered during serive
	int current_location=start_point;	//current position in array
	int position_to_go;	
	//find shortest service time
	for(int i=0; i<arr_size; i++){
		position_to_go = find_min(arr, current_location, arr_size);	//find posiiton to go based on distance
		new_value=(arr[position_to_go]-current_location);	//calculate travel time
		if(new_value < 0){new_value=new_value*-1;};		
		distance_traveled+=new_value;
		current_location=arr[position_to_go];			//adjust current location
		serviced_requests[i]=arr[position_to_go];	//adjust serviced rquests
		arr[position_to_go] = -1;
	}
	average_seek_length=(distance_traveled/arr_size);
	return(average_seek_length);

}
float SCAN(int *arr, int start_point, int arr_size){
	float average_seek_length;	//return value
	int new_value;		//value to add to distance traveled
	float distance_traveled=0.0;	//total distance we have covered during service
	int current_location=start_point;
	s_sort(arr, start_point, arr_size);
	for(int i=0; i<arr_size; i++){
		new_value=arr[i]-current_location;
		if(new_value < 0){
			new_value = new_value * -1;
		}
		distance_traveled+=new_value;
		current_location=arr[i];
		serviced_requests[i]=arr[i];
	}
	average_seek_length=(distance_traveled/arr_size);
	return(average_seek_length);
}

float C_SCAN(int *arr, int start_point, int arr_size){
	float average_seek_length;	//return value
	int new_value;		//value to add to distance traveled
	float distance_traveled=0.0;	//total distance we have covered during service
	int current_location=start_point;
	c_sort(arr, start_point, arr_size);
	for(int i=0; i<arr_size; i++){
		new_value=arr[i]-current_location;
		if(new_value < 0){
			new_value = new_value * -1;
		}
		distance_traveled+=new_value;
		current_location=arr[i];
		serviced_requests[i]=arr[i];
	}
	average_seek_length=(distance_traveled/arr_size);
	return(average_seek_length);

}



void print_serviced_requests(int arr_size){
	printf("[ ");
	for(int i=0; i<arr_size; i++){
		if(i==arr_size-1){printf("%d", serviced_requests[i]);}
		else{printf("%d, ", serviced_requests[i]);}
	}
	printf(" ]");
	printf("\n\n");
	fflush(stdout);

}
void reset_arrs(int arr_size){
	for(int i=0; i<arr_size; i++){
		serviced_requests[i]=-1;
		temp_nums[i] = nums[i];
	}
}
int find_min(int *arr, int location, int arr_size){
	int position_to_go;
	int min = -1;
	int temp;
	for(int i=0; i<arr_size; i++){
		if(arr[i] == -1){;}
		else if(arr[i] > -1){
			temp=(arr[i]-location);
			if(temp<0){temp=temp*-1;}
			//Check to see if we have a base case
			if(min==-1){
				min=temp;
				position_to_go=i;
			}
			//else compare the new value to the minimum value
			else{
				if(temp<=min){
					min=temp;	//get new shortest distance
					position_to_go=i; //set shortest position in array to go to
				}
			}
		}
		
	}
	return position_to_go;
}
//sort function for SCAN algorithm
void s_sort(int *arr_to_modify, int start_value, int arr_size){
	int temp=0;
	int cutoff=0;		//numbers of items larger than our starting position
	//basic descending sort
	for(int i=0; i<arr_size; ++i){
		if(nums[i] > start_value){cutoff++;}
		for(int j=i+1; j<arr_size; ++j){
			if(arr_to_modify[i] < arr_to_modify[j]){
				temp=arr_to_modify[i];
				arr_to_modify[i] = arr_to_modify[j];
				arr_to_modify[j]=temp;
			}
			
		}
	
	}
	//now ascending sort for items larger than the start position
	for(int i=0; i<cutoff; i++){
		for(int j=i+1; j<cutoff; ++j){
			if(arr_to_modify[i] > arr_to_modify[j]){
				temp=arr_to_modify[i];
				arr_to_modify[i] = arr_to_modify[j];
				arr_to_modify[j]=temp;
			}
			
		}
	
	}
}
//sort algorithm for C-SCAN
void c_sort(int *arr_to_modify, int start_value, int arr_size){
	int temp=0;
	int cutoff=0;		//numbers of items larger than our starting position
	//basic ascending sort
	for(int i=0; i<arr_size; ++i){
		if(nums[i] > start_value){cutoff++;}
		for(int j=i+1; j<arr_size; ++j){
			if(arr_to_modify[i] > arr_to_modify[j]){
				temp=arr_to_modify[i];
				arr_to_modify[i] = arr_to_modify[j];
				arr_to_modify[j]=temp;
			}	
		}
	}
	int temp_arr[arr_size];
	int temp_count=0;
	for(int i=0; i<arr_size; i++){
		if(arr_to_modify[i] > start_value){
			temp_arr[temp_count]=arr_to_modify[i];
			temp_count++;
		}
	}
	for(int i=0; i<arr_size-cutoff; i++){
		temp_arr[i+cutoff]=arr_to_modify[i]; 
	}
	for(int i=0; i<arr_size; i++){
		arr_to_modify[i]=temp_arr[i];
	}
}
