#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int calculaAleatorios(int min, int max);
void manejadoraFarmaceutico(int signal);
void manejadoraMedico(int signal);

int main(int argc, char* argv[]){

	if(argc!=2){ //comprobacion de los argumentos
		perror("Numero de argumentos invalido\n");
	}	
	

	struct sigaction mFarmaceutico;
	struct sigaction mMedico;
	struct sigaction mHijos;

	srand (time(NULL)); //semilla para que funcione bien el aleatorio

	printf(": ...Arrancando...\nInicializando hijos\n");	

	int posicion = atoi(argv[1]);
	
	pid_t farmaceutico, medico, hijos[posicion];
	
	farmaceutico = fork(); //Creacion del farmaceutico

	if(farmaceutico == -1){
		perror("Error en la creacion del farmaceutico\n");
	}else if(farmaceutico == 0){ //codigo del farmaceutico;	
				
		mFarmaceutico.sa_handler = manejadoraFarmaceutico;
		
		sigaction(SIGUSR1, &mFarmaceutico, NULL); //mandamos la senyal a la manejadoraFarmaceutico	

		for(;;) pause();
		
	}

	medico = fork(); //creacion del medico

	if(medico == -1){ //comprobacion de que el medico se cree bien
		perror("Erros en la ceracion del medico\n");
	}else if(medico == 0){ //codigo del medico
		int i = 0;		
		for(i=0; i<posicion; i++){
			hijos[i] = fork();
			if(hijos[i] == -1){ //comprobacion de que los pinches se creen bien
				perror("Error en la creacion del hijo\n");
			}else if(hijos[i] == 0){ //codigo de los hijos
				for(;;) pause();
			}
		}

		mMedico.sa_handler = manejadoraMedico;
		sigaction(SIGUSR1, &mMedico, NULL);
		for(;;) pause();

	}	


		//codigo del epidemiologo
	sleep(2); 
	kill(farmaceutico,SIGUSR1);
	sleep(2);
}

void manejadoraFarmaceutico(int signal){

	if(calculaAleatorios(0, 1) == 0){ //calculo un aleatorio para ver si hay dosis
		printf("NO hay dosis\n");

	}else{
		printf("Hay dosis\n");
	}
}

void manejadoraMedico(int signal){

	printf("Hola y adios\n");
}

int calculaAleatorios(int min, int max){
	return rand() % (max-min+1) + min;
}
