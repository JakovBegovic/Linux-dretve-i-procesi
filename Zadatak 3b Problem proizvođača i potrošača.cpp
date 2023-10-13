#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>

using namespace std;

struct slog_dijeljena{
	int ulaz;
	int izlaz;
	int ukupno;
	int polje[5];
};
key_t id_dij_mem;
slog_dijeljena* zajednicka;

int SemId;
int brProiz;
int brSluc;
int zbroj;

sembuf slog_semafori;

void PostaviSemafor(int IDSem){
	slog_semafori.sem_num = IDSem;
	slog_semafori.sem_op = 1; // -1 za ispitivanje, 1 za postavljanje
	semop(SemId, &slog_semafori, 1); // sad napravi operaciju sa tim parametrima
}

void IspitajSemafor(int IDSem){
	slog_semafori.sem_num = IDSem;
	slog_semafori.sem_op = -1; // -1 za ispitivanje, 1 za postavljanje
	semop(SemId, &slog_semafori, 1); // sad napravi operaciju sa tim parametrima
	
}

void kraj(int sig){
	semctl(SemId, IPC_RMID, 0);
	
	for(int i=0; i<brProiz; i++){
		wait(NULL);
	}
	
	cout << zbroj << endl;
	
	shmdt(zajednicka);
	shmctl(id_dij_mem, IPC_RMID, NULL);
	
	exit(sig);
}

void Proizvodjac(int redBr){
	srand(getpid());
	for(int i=0; i<brSluc; i++){
		IspitajSemafor(1);
		IspitajSemafor(0);
		
		zajednicka->polje[zajednicka->ulaz] = rand()%1000;
		
		usleep(500000);
		printf("Proizvodac %d salje \"%d\" \n", redBr, zajednicka->polje[zajednicka->ulaz]);
		zajednicka->ulaz = 	(zajednicka->ulaz + 1) % 5;
		
		PostaviSemafor(0);
		PostaviSemafor(2);
	}
	
	exit(0);
}

int main(int argc, char** argv){
	
	brProiz = atoi(argv[1]);
	brSluc = atoi(argv[2]);
	
	slog_semafori.sem_flg = 0;
	SemId = semget(IPC_PRIVATE, 3, 0600);
	semctl(SemId, 0, SETVAL, 1); // PISI
	semctl(SemId, 1, SETVAL, 1); // PUN
	semctl(SemId, 2, SETVAL, 0); // PRAZAN
	
	
	id_dij_mem = shmget(IPC_PRIVATE, sizeof(slog_dijeljena), 0600);
	zajednicka = (slog_dijeljena *) shmat(id_dij_mem, NULL, 0);
	
	
	zajednicka->ulaz = 0;
	zajednicka->izlaz = 0;
	
	for(int i=0; i<brProiz; i++){
		switch(fork()){
			case 0:
				Proizvodjac(i+1);
				
			case -1:
				cout << "Dijete se nije stvorilo." << endl;
		}
	}
	
	zajednicka->ukupno = brSluc*brProiz;
	zbroj = 0;
	for(int i=0; i<zajednicka->ukupno; i++){
		IspitajSemafor(2);
		
		zbroj += zajednicka->polje[zajednicka->izlaz];
		usleep(500000);
		printf("Potrosac prima %d \n", zajednicka->polje[zajednicka->izlaz]);
		
		zajednicka->izlaz = (zajednicka->izlaz + 1) % 5;
		
		PostaviSemafor(1);
	}
	
	
	kraj(0);
}