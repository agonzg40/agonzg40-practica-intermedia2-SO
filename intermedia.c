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
void manejadoraPacientes(int signal);

int main(int argc, char* argv[]){

	if(argc!=2){ //comprobacion de los argumentos
		perror("Numero de argumentos invalido\n");
		exit(1);
	}	
	

	struct sigaction mFarmaceutico;
	struct sigaction mMedico;
	struct sigaction mPacientes;

	srand (time(NULL)); //semilla para que funcione bien el aleatorio

	printf(": ...Arrancando...\nInicializando hijos\n");	

	int posicion = atoi(argv[1]);
	
	if(posicion<=0){ //Aquí compruebo que el numero de pacientes es mayor que 0

		perror("Número de argumentos inválido, tiene que ser mayor que 0\n");

		exit(1);

	}

	pid_t farmaceutico, medico, pacientes[posicion];
	
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
			pacientes[i] = fork();
			if(pacientes[i] == -1){ //comprobacion de que los pinches se creen bien
				perror("Error en la creacion del hijo\n");
			}else if(pacientes[i] == 0){ //codigo de los hijos
				mPacientes.sa_handler = manejadoraPacientes;
				sigaction(SIGUSR1, &mPacientes, NULL); //nos movemos a la manejadora
				for(;;) pause();
			}
		}

		//for(;;) pause();
		
		int j= 0, status, s, variable1;
		mMedico.sa_handler = manejadoraMedico;
		sigaction(SIGUSR2, &mMedico, NULL);
		//for(;;)pause();
		//printf("bb%d\n",getpid());
		sleep(5);
		printf("as\n");
		for(j = 0; j<posicion; j++){
			kill(pacientes[j], SIGUSR1);
			wait(&status);
		}
		for(;;) pause();
	}	


		//codigo del epidemiologo
	sleep(2);
	kill(farmaceutico,SIGUSR1);
	int stat, variable;
	wait(&stat);
	variable=WEXITSTATUS(stat);

	if(variable == 1){
		printf("El farmaceutico no tiene dosis\n-----Abortando ejecucion-----\n");
		exit(1);
	}else {
		printf("El farmaceutico tiene dosis\n-----Avisando al medico para que comience la vacunacion-----\n");
		kill(medico, SIGUSR2);
	}
	sleep(2);
}

void manejadoraFarmaceutico(int signal){

	if(calculaAleatorios(0, 1) == 0){ //calculo un aleatorio para ver si hay dosis
		printf("NO hay dosis\n-----Avisando al epidemiologo-----\n");
		exit(1);
	}else{
		printf("Hay dosis\n-----Avisando al epidemiologo-----\n");
		exit(2);
	}

}

void manejadoraMedico(int signal){
	printf("hola\n");
	

}

void manejadoraPacientes(int signal){
	printf("me ha llegado\n");
}

int calculaAleatorios(int min, int max){
	return rand() % (max-min+1) + min;
}
