#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<cstdlib>       
#include<unistd.h>      
#include<sys/wait.h> 
#include <csignal>


using namespace std;

int n;

struct slog_zajednicka{
	int* trazim;
	int* broj;
};

slog_zajednicka* zajednicka;

key_t id_dij_mem[3];

int najveci(){
	int najveci = 0;
	for(int i=1; i<n; i++){
		if(zajednicka->broj[i]>zajednicka->broj[najveci]){
			najveci = i;
		}
	}
	
	return najveci;
}

void u_ko(int i){
	
	zajednicka->trazim[i] = 1;
	zajednicka->broj[i] = zajednicka->broj[najveci()] + 1;
	zajednicka->trazim[i] = 0;
	
	for(int j=0; j<n; j++){
		while(zajednicka->trazim[j]!=0){}
		
		while( (zajednicka->broj[j]!=0) && ((zajednicka->broj[j] < zajednicka->broj[i]) || 
		((zajednicka->broj[j] == zajednicka->broj[i]) && (j < i))) ){}
	}

}


void van_ko(int i){
	zajednicka->broj[i] = 0;
}


void Dijete(int i){
	
	for(int k=1; k<6; k++){
		u_ko(i);
		for(int m=1; m<6; m++){
			cout <<"Dretva: " << i+1 << ", K.O. br: " << k << " (" << m << "/5)" << endl;
			sleep(1);
		}
		van_ko(i);
	}
	
}

void kraj(int sig){
	
	for (int i = 0; i < n; i++){ 
		wait(NULL);
	}
	
	shmdt(zajednicka->broj);
	shmdt(zajednicka->trazim);
	shmdt(zajednicka);
	shmctl(id_dij_mem[0], IPC_RMID, NULL);
	shmctl(id_dij_mem[1], IPC_RMID, NULL);
	shmctl(id_dij_mem[2], IPC_RMID, NULL);
	
	exit(0);
}

int main(int argc, char** argv){
		
	n = atoi(argv[1]);
	
	sigset(SIGINT, kraj);
	
	id_dij_mem[0] = shmget(IPC_PRIVATE, sizeof(slog_zajednicka), 0600);
	zajednicka = (slog_zajednicka *) shmat(id_dij_mem[0], NULL, 0);
	
	id_dij_mem[1] = shmget(IPC_PRIVATE, n*sizeof(int), 0600);
	zajednicka->broj = (int*)shmat(id_dij_mem[1], NULL, 0);
	
	id_dij_mem[2] = shmget(IPC_PRIVATE, n*sizeof(int), 0600);
	zajednicka->trazim = (int*)shmat(id_dij_mem[2], NULL, 0);
	
	for(int i=0; i<n; i++){
		zajednicka->broj[i] =0;
		zajednicka->trazim[i] =0;
	}
	
	for (int i = 0; i < n; i++){
		switch (fork()) {
		    case 0:
		     	 Dijete(i);
		     	 exit(0);
		    case -1:
		     	 cout << "Dijete nije stvoreno";
		     	 exit(1);
		    }
	}
	   

	kraj(0);
}