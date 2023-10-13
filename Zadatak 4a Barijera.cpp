#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

using namespace std;

int n;

sem_t semPocetni;

int brBr =0;
pthread_mutex_t kljuc;
pthread_cond_t redCekanja;

void *FunkcijaDretve(void *nest){
	
	int moj_broj = *(int *)nest + 1;
	
	pthread_mutex_lock(&kljuc);
	sem_post(&semPocetni);
	int broj;
	
	
	cout << "Dretva br. " << moj_broj << ". Unesi broj: ";
	cin >> broj;
	
	brBr++;
	
	if(brBr<n){
		pthread_cond_wait(&redCekanja, &kljuc);
		pthread_cond_signal(&redCekanja);
		
	} else{
		pthread_cond_signal(&redCekanja);
		pthread_cond_wait(&redCekanja, &kljuc);
		
	}
	
	cout << "Dretva br. " << moj_broj << ": Uneseni broj iznosi: " << broj << endl;
	pthread_mutex_unlock(&kljuc);

}


int main(int argc, char** argv){
	
	n = atoi(argv[1]);
	pthread_t dretve [n];
	
	sem_init(&semPocetni, 0, 1);
	
	pthread_mutex_init(&kljuc, NULL);
	pthread_cond_init(&redCekanja, NULL);
	
	int *polje_i = new int [n];
	
	for(int i=0; i<n; i++){
		polje_i[i] = i;
		pthread_create(&dretve[i], NULL, FunkcijaDretve, &polje_i[i]);
		sem_wait(&semPocetni);
		
	}
	for(int i=0; i<n; i++){
		
		pthread_join(dretve[i], NULL);
		
	}
	
	sem_destroy(&semPocetni);
	pthread_mutex_destroy(&kljuc);
	pthread_cond_destroy(&redCekanja);
	
	delete [] polje_i;
	
	return 0;
}