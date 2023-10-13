#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

using namespace std;

int M = 3, N, A, B;

void prekid(int sig){
	
	if(sig==SIGINT){
		cout << endl << "Zigica igrac A uzeo sa stola: ";
		cin >> A;
		
		while(A>M){
			
			cout << "Morate unijeti broj manji ili jednak broju " << M << "." << endl;
			cout << "Zigica igrac A uzeo sa stola: ";
			cin >> A;
			
		}
		
	} else if(sig==SIGQUIT){
		cout << endl << "Zigica igrac B uzeo sa stola: ";
		cin >> B;
		
		while(B>M){
		
			cout << "Morate unijeti broj manji ili jednak broju " << M << "." << endl;
			cout << "Zigica igrac B uzeo sa stola: ";
			cin >> B;
	
		}
	} else if (sig==SIGTSTP){
		raise(SIGKILL);
		
	}
	
}


int main(int argc, char** argv){
	
	sigset(SIGINT, prekid);
	sigset(SIGQUIT, prekid);
	sigset(SIGTSTP, prekid);
	
	sscanf(argv[1], "%d", &N);
	
	while(N<=M){
		cout << "Morate unijeti broj veci od " << M << ": ";
		cin >> N;
	}
	
	cout << "M = " << M << ". N = " << N << endl;
	cout << "Parametri su ispravni. Zapocinjem igru." << endl;
	
	while(N>0){
		
		cout << "Zigica na stolu: " << N << endl;
		cout << "Na redu igrac: A" << endl;
		
		pause();
	
		N = N - A;
		
		if(N>0){
			cout << "Zigica na stolu: " << N << endl;
			cout << "Na redu igrac: B" << endl;
			
			pause();
						
			N = N - B;
			
			if(N<=0){
				cout << "Zigica na stolu: 0. ";
				cout << "Igrac A je pobjedio." << endl;
			}
			
		} else {
			cout << "Zigica na stolu: 0. ";
			cout << "Igrac B je pobjedio." << endl;
		}
		
	}
	
	return 0;
}