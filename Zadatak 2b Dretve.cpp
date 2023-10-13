#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <unistd.h>
#include <math.h>
#include <csignal>
#include <cstdlib>
#include <pthread.h>

using namespace std;

int l, m, n;
long double *a;
long double *b;
pthread_t* polje_dretvi;
int br_elem_po_dretvi;

long long faktorijela(int br){
	long long rez=1;
	for(int i = 1; i <= br; ++i) {
            rez *= i;
        }
	return rez;
}


void *Izracun(void* x){
	int red_br_dr = *(int*)(x);
	
	int kraj = br_elem_po_dretvi*(red_br_dr+1);
	
	if(kraj>l){
		kraj =l;
	}
	
	long double rez;
	long double y;
	
	for(int j=br_elem_po_dretvi*red_br_dr; j<kraj; j++){
		y = a[j];
		rez=0;
		for(int i=0; i<=m; i++){
			rez += pow(y, i)/faktorijela(i);	
		}
		
		b[j] = rez;
	}
}

int main(int argc, char** argv) {
		
	l = atoi(argv[1]);
	m = atoi(argv[2]);
	n = atoi(argv[3]);
	
	
	a = new long double [l];
	b = new long double [l]; 
	
	srand(time(NULL));
	
	long double nasumicni;
	
	for(int i=0;i<l;i++) {
		nasumicni = rand();
		
		if(nasumicni==RAND_MAX){
			nasumicni= nasumicni -1;
		}
		
		a[i]=(nasumicni / RAND_MAX)*10;
	}
	
	cout << setprecision(10);
	cout << "Eksponenti =" << endl;
	
    for(int i=0;i<l;i++){
		cout<<a[i]<<endl;
	}

    polje_dretvi = new pthread_t[n];
	br_elem_po_dretvi = ceil(float(l)/n);
	
	int *polje_i = new int [n];
	for(int i =0; i<n; i++){
		polje_i[i] = i;
		if (pthread_create(&polje_dretvi[i], NULL, Izracun, &polje_i[i]) != 0) {
      		cout <<"Greska pri stvaranju dretve!\n";
      		exit(1);
    	}
	}
			
	cout << "Eksponencijale =" << endl;
	
   	
   	for(int i=0; i<n; i++){
			pthread_join(polje_dretvi[i], NULL);
	}
	
	for(int i=0;i<l;i++){
   		cout<<b[i]<<endl;
	}
	
	delete [] a;
	delete [] b;
	
   	return 0;
}