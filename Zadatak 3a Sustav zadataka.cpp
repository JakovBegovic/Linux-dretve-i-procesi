#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>


using namespace std;

sem_t sem1;
sem_t sem2;
sem_t sem3;
sem_t sem4;
sem_t sem5;
sem_t sem6;
sem_t sem7;

int* polje_i;
pthread_t* polje_dretvi;

int n = 7; // broj dretvi
	

void kraj(int sig){
	
	for(int i=0; i<n; i++){
		pthread_join(polje_dretvi[i], NULL);
	}
	
	sem_destroy(&sem1);
	sem_destroy(&sem2);
	sem_destroy(&sem3);
	sem_destroy(&sem4);
	sem_destroy(&sem5);
	sem_destroy(&sem6);
	sem_destroy(&sem7);
	
	delete [] polje_i;
	delete [] polje_dretvi;
	
	exit(sig);
}

void Ispis(int br_proc){
	srand(pthread_self());
	int br_ponavljanja = rand()%10;
	for(int i=0; i<br_ponavljanja; i++){
		sleep(1);
		printf("Izvodim zadatak %d: %d/%d\n", br_proc, i+1, br_ponavljanja);
	}
}

void *Dretva(void* x){
	int j = *(int*)(x);
	
	switch(j){ // semafori prije izvadjanja
		case 0:
			break;
			
		case 1:
			break;
			
		case 2:
			sem_wait(&sem1);
			break;
		
		case 3:
			sem_wait(&sem2);
			break;
			
		case 4:
			sem_wait(&sem3);
			break;
			
		case 5:
			sem_wait(&sem4);
			break;
			
		case 6:
			sem_wait(&sem5);
			sem_wait(&sem6);
			sem_wait(&sem7);
			break;
	}
	
 	Ispis(j+1);
 	
 	switch(j){ // semafori nakon izvadjanja
		case 0:
			sem_post(&sem3);
 			break;
 			
		case 1:
 			sem_post(&sem1);
 			sem_post(&sem2);
 			break;
 			
 		case 2: 
 			sem_post(&sem4);
 			break;
 			
		case 3:
			sem_post(&sem5);
			break;
			
		case 4:
			sem_post(&sem6);
			break;
			
		case 5:
			sem_post(&sem7);
			break;
			
		case 6:
			break;
	}
 	
}


int main(void){


    /* initializing semaphore */
    int pshared = 0; // !=0 for processes, =0 for threads
    int value = 0; // pocetna vrijednost semafora
    sem_init(&sem1, pshared, value); // initialize the semaphore
    sem_init(&sem2, pshared, value); // initialize the semaphore
    sem_init(&sem3, pshared, value); // initialize the semaphore
    sem_init(&sem4, pshared, value); // initialize the semaphore
    sem_init(&sem5, pshared, value); // initialize the semaphore
    sem_init(&sem6, pshared, value); // initialize the semaphore
    sem_init(&sem7, pshared, value); // initialize the semaphore

	polje_dretvi = new pthread_t[n];
	polje_i = new int [n];
	
	for(int i=0; i<n; i++){
		polje_i[i] = i;
		
		if (pthread_create(&polje_dretvi[i], NULL, Dretva, &polje_i[i]) != 0) {
      		cout <<"Greska pri stvaranju dretve!\n";
      		exit(1);
    	}
		
	}
   
   	

	kraj(0);
}