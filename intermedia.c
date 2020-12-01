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
		//printf("[FARMACEUTICO]: %d\n",getpid());
		mFarmaceutico.sa_handler = manejadoraFarmaceutico;
		
		sigaction(SIGUSR1, &mFarmaceutico, NULL); //mandamos la senyal a la manejadoraFarmaceutico	

		for(;;) pause();
		
	}

	medico = fork(); //creacion del medico

	if(medico == -1){ //comprobacion de que el medico se cree bien
		perror("Erros en la ceracion del medico\n");
	}else if(medico == 0){ //codigo del medico
		int i = 0;	
		int j= 0, status, reaccion = 0, variable1;
		mMedico.sa_handler = manejadoraMedico;
		sigaction(SIGUSR2, &mMedico, NULL);

		pause();
		for(i=0; i<posicion; i++){
			pacientes[i] = fork();
			if(pacientes[i] == -1){ //comprobacion de que los pacientes se creen bien
				perror("Error en la creacion del hijo\n");
			}else if(pacientes[i] == 0){ //codigo de los hijos
				mPacientes.sa_handler = manejadoraPacientes;
				sigaction(SIGUSR1, &mPacientes, NULL); //nos movemos a la manejadora
				for(;;) pause();
			}
		}

		for(j = 0; j<posicion; j++){
			kill(pacientes[j], SIGUSR1);
			wait(&status);
			variable1 = WEXITSTATUS(status);

			if(variable1 == 1){
				reaccion++;
				printf("Soy el paciente[%d], y tengo reaccion\n",j+1);
			}else if(variable1 == 2){
				printf("Soy el paciente[%d], y no tengo reaccion\n", j+1);
			}
		}
		printf("%d\n",reaccion);
		exit(reaccion);

	}	


		//codigo del epidemiologo
	sleep(2);
	kill(farmaceutico,SIGUSR1);
	int stat, variable;
	wait(&stat);
	variable=WEXITSTATUS(stat);

	if(variable == 1){
		printf("El farmaceutico no tiene dosis\n-----Abortando ejecucion-----\n");
		kill(medico, SIGUSR1);
		exit(1);
	}else {
		printf("El farmaceutico tiene dosis\n-----Avisando al medico para que comience la vacunacion-----\n");
		kill(medico, SIGUSR2);
	}
	
	int sta, variable3;
	wait(&sta);
	variable3 = WEXITSTATUS(sta);

	printf("Se han vacunado a %d pacientes\n",variable3);
	//sleep(2);
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
	if(signal == SIGUSR1){
		exit(0);
	}
	printf("Empezando a vacunar a los pacientes\n");
	

}

void manejadoraPacientes(int signal){

	srand (time(NULL));
	sleep(2);
	
	if(calculaAleatorios(1,10)%2==0){
		exit(1);
	}else{
		exit(2);

	}
	
}

int calculaAleatorios(int min, int max){
	return rand() % (max-min+1) + min;
}
