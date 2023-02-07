//Author: Noah Jones
//Program: Dinner Philosopher problem
//CSC-345

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct{
	int position;	//position of Philosopher
	int arr_size;	//size of philosopher thread array
	int times_to_eat;	//times philisopher must eat
	int R;	//Philosophers right mutex/chopstick
	int L;	//Philosophers left mutex/chopstick
} Ppos;

void init(Ppos *pp, int pos, int size, int eat); 	//data structure
void *live(void *); 					//Main function for our philosphers
pthread_mutex_t *Chopsticks;				//Our mutexes
Ppos *phils;						//Array of philosophers data structures

int main(int argc, char* argv[]){
	if (argc != 3){
		printf("Usage: %s n t where n is number of philosphers and t is times each philospher must eat\n", argv[0]);
		exit(1);
	}
	int P = atoi(argv[1]);
	int T = atoi(argv[2]);
	pthread_t Pthreads[P]; //Philosopher threads
	Chopsticks = (pthread_mutex_t*)malloc(P*sizeof(pthread_mutex_t)); //Metaphorical mutex chopsticks
	phils = (Ppos*)malloc(P*sizeof(Ppos)); //array of philosophers data structures
	
	//Initializing each philosophers data structures
	for(int i = 0; i < P; i++){
		init(&phils[i], i, P, T);
	}
	//Initialize mutexs
	for(int i=0; i<P; i++){
		pthread_mutex_init(&Chopsticks[i], NULL);
	}
	//Loop to start threads
	for(int i=0; i<P; ++i){
		pthread_create(&Pthreads[i], NULL, live, &phils[i]);
		sleep(1);
	}
	//Loop to join threads
	for(int i=0; i<P; ++i){
		pthread_join(Pthreads[i], NULL);
	}
}
void *live(void *param){
	Ppos *pp = (Ppos*)param;
	//int Etimes = pp->times_to_eat;
	while(pp->times_to_eat > 0){
		sleep(1);
		printf("Philosopher %d is thinking\n", pp->position);
		//ASYMMETIRC SOLUTION
		//Odd position
		if(pp->position%2!=0){
			//Acquire left first
			pthread_mutex_lock(&Chopsticks[pp->L]);
			//Then the right
			pthread_mutex_lock(&Chopsticks[pp->R]);
			//deincrement times to eat
			pp->times_to_eat--;
			printf("Philosopher %d is eating\n", pp->position);
			fflush(stdout);
			//unlock held mutexes
			pthread_mutex_unlock(&Chopsticks[pp->R]);
			pthread_mutex_unlock(&Chopsticks[pp->L]);
			sleep(1);}
		//Even Position
		if(pp->position%2==0){
			//Acquire right first
			pthread_mutex_lock(&Chopsticks[pp->R]);
			//Then try the left
			pthread_mutex_lock(&Chopsticks[pp->L]);
			//deincrement times to eat
			pp->times_to_eat--;
			printf("Philosopher %d is eating\n", pp->position);
			fflush(stdout);
			//unlock held mutexes
			pthread_mutex_unlock(&Chopsticks[pp->R]);
			pthread_mutex_unlock(&Chopsticks[pp->L]);
			sleep(1);}
		printf("Philosopher %d is thinking\n", pp->position);
	}
}

//Function to initialize our data structure and set Philosophers right and left chopsticks
void init(Ppos *pp, int pos, int size, int eat ){
	pp->position = pos;
	pp->arr_size = size;
	pp->times_to_eat = eat;
	if (pp->position == 0){	//Incase philosopher position in array is 0
		pp->R = 0;
		pp->L = pp->arr_size-1;} 
	else if(pp->position==pp->arr_size-1){	//If philsopher is last in the array
		pp->R=pp->position;
		pp->L=pp->position-1;}
	else{				//if philosopher psition is not 0 or the last in the array
		pp->R=pp->position;
		pp->L=pp->position-1;}
}
