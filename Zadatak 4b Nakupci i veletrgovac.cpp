#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

pthread_mutex_t kljuc;
pthread_cond_t redCekanja;

pthread_mutex_t kljuc2;
pthread_cond_t redCekanja2;

bool uzeto = true;
int postavljeno = 0;


void Ispis_V(int a){
	
	switch(postavljeno){
		case 1:
			cout << "Veletrgovac stavio monitor i racunalo" << endl;
			
			break;
			
		case 2: 
			cout << "Veletrgovac stavio racunalo i tipkovnicu" << endl;
			
			break;
			
		case 3: 
			cout << "Veletrgovac stavio monitor i tipkovnicu" << endl;
			
			break;
	}
	sleep(1);
	
}

void Ispis_N(int a){
	
	switch(a){
		case 1:
			cout << "Nakupac s tipkovnicom uzeo monitor i racunalo" << endl;
			
			break;
			
		case 2: 
			cout << "Nakupac s monitorom uzeo racunalo i tipkovnicu" << endl;
			
			break;
			
		case 3: 
			cout << "Nakupac s racunalom uzeo monitor i tipkovnicu" << endl;
			
			break;
	}
	sleep(1);
	
}

void Ulaz_u_ko_V(){
	
	pthread_mutex_lock(&kljuc2);
	
	while(!uzeto){
		pthread_cond_wait(&redCekanja2, &kljuc2);
	}

	pthread_mutex_unlock(&kljuc2);

}

void Izlaz_iz_ko_V(){
	
	pthread_mutex_lock(&kljuc2);
	uzeto = false;
	pthread_cond_broadcast(&redCekanja);
	
	pthread_mutex_unlock(&kljuc2);
}



void Ulaz_u_ko_N(int brDretve){
	
	pthread_mutex_lock(&kljuc);
	
	while(postavljeno != brDretve || uzeto){
		pthread_cond_wait(&redCekanja, &kljuc);
	}
	
	pthread_mutex_unlock(&kljuc);
	
}

void Izlaz_iz_ko_N(){
	
	pthread_mutex_lock(&kljuc);
	
	uzeto = true;
	pthread_cond_broadcast(&redCekanja2);
	
	pthread_mutex_unlock(&kljuc);
	
}

void *Veletrgovac(void *nekoristena){
	srand(pthread_self());
	
	while(1){
		Ulaz_u_ko_V();
		
		postavljeno = rand() % 3 + 1;
		Ispis_V(postavljeno);
		
		Izlaz_iz_ko_V();
	
	}
}

void *Nakupci(void *red_br){
	int broj_dretve = *(int *)red_br + 1;
	while(1){
		
		Ulaz_u_ko_N(broj_dretve);
		
		Ispis_N(broj_dretve);
		
		Izlaz_iz_ko_N();
	}
}

int main(){
	
	pthread_t dretve [4];
	
	pthread_mutex_init(&kljuc, NULL);
	pthread_cond_init(&redCekanja, NULL);
	
	pthread_mutex_init(&kljuc2, NULL);
	pthread_cond_init(&redCekanja2, NULL);
	
	
	int polje_i [4];
	int j=0;
	pthread_create(&dretve[3], NULL, Veletrgovac, &j);

	for(int i=0; i<3; i++){
		polje_i[i] = i;
		pthread_create(&dretve[i], NULL, Nakupci, &polje_i[i]);

	}
	
	
	
	for(int i=0; i<4; i++){
		
		pthread_join(dretve[i], NULL);
		
	}
	
	
	pthread_mutex_destroy(&kljuc);
	pthread_cond_destroy(&redCekanja);
	
	pthread_mutex_destroy(&kljuc2);
	pthread_cond_destroy(&redCekanja2);
		
	return 0;
}