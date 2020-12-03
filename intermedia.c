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
		perror("[Sistema]\nNumero de argumentos invalido\n");
		exit(1);
	}	
	

	struct sigaction mFarmaceutico;
	struct sigaction mMedico;
	struct sigaction mPacientes;

	srand (time(NULL)); //semilla para que funcione bien el aleatorio

	printf("-------------Arrancando-------------\n[Sistema]: Inicializando hijos\n");
	printf("\n");	

	int posicion = atoi(argv[1]);
	
	if(posicion<=0){ //Aquí compruebo que el numero de pacientes es mayor que 0

		perror("[Sistema]: Número de argumentos inválido, tiene que ser mayor que 0\n");

		exit(1);

	}

	pid_t farmaceutico, medico, pacientes[posicion];
	
	farmaceutico = fork(); //Creacion del farmaceutico

	if(farmaceutico == -1){
		perror("[Sistema]: Error en la creacion del farmaceutico\n");
	}else if(farmaceutico == 0){ //codigo del farmaceutico;	
		mFarmaceutico.sa_handler = manejadoraFarmaceutico;
		sigaction(SIGUSR1, &mFarmaceutico, NULL); //mandamos la senyal a la manejadoraFarmaceutico	

		for(;;) pause();
		
	}

	medico = fork(); //creacion del medico

	if(medico == -1){ //comprobacion de que el medico se cree bien
		perror("[Sistema]: Error en la ceracion del medico\n");
	}else if(medico == 0){ //codigo del medico
		int i = 0;	
		int j= 0, status, reaccion = 0, variable1;
		mMedico.sa_handler = manejadoraMedico;
		sigaction(SIGUSR2, &mMedico, NULL);

		pause();
		for(i=0; i<posicion; i++){
			pacientes[i] = fork();
			if(pacientes[i] == -1){ //comprobacion de que los pacientes se creen bien
				perror("[Sistema]\nError en la creacion del hijo\n");
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
			}
		}
		printf("\n-------------Acabada vacunacion, avisando al epidemiologo-------------\n");
		exit(reaccion);

	}	


					//codigo del epidemiologo
	sleep(2);
	printf("[Epidemiologo] Soy el epidemiologo con pid [%d] hijos inicializados\n-------------Avisando al farmaceutico-------------\n", getpid());
	printf("\n");
	kill(farmaceutico,SIGUSR1);
	int stat, variable;
	wait(&stat);
	variable=WEXITSTATUS(stat);

	if(variable == 1){
		printf("[Epidemiologo] Soy el epidemiologo con pid [%d], no hay dosis\n-------------Abortando ejecucion-------------\n", getpid());
		kill(medico, SIGUSR1);
		exit(1);
	}else {
		printf("[Epidemiologo] Soy el epidemiologo con pid [%d], hay dosis\n-----Avisando al medico para que comience la vacunacion-----\n", getpid());
		printf("\n");
		kill(medico, SIGUSR2);
	}
	
	int sta, variable3;
	wait(&sta);
	variable3 = WEXITSTATUS(sta);

	printf("\n[Epidemiologo] Soy el epidemiologo con pid [%d], Se han vacunado a %d pacientes, y %d no han sido vacunados\n",getpid(), variable3, posicion-variable3);
	printf("\n-------------Acabada la ejecucion-------------\n");
}

void manejadoraFarmaceutico(int signal){

	if(calculaAleatorios(0, 1) == 0){ //calculo un aleatorio para ver si hay dosis
		printf("[Farmaceutico] Soy el farmaceutico con pid [%d], NO hay dosis\n-------------Avisando al epidemiologo-------------\n", getpid());
		printf("\n");
		exit(1);
	}else{
		printf("[Farmaceutico] Soy el farmaceutico con pid [%d], Hay dosis\n-----Avisando al epidemiologo-----\n", getpid());
		printf("\n");
		exit(2);
	}

}

void manejadoraMedico(int signal){
	if(signal == SIGUSR1){
		exit(0);
	}
	printf("[Medico] Soy el medico con pid [%d], Empezando a vacunar a los pacientes\n", getpid());
	printf("\n");
	

}

void manejadoraPacientes(int signal){

	srand (time(NULL));
	sleep(2);
	
	if(calculaAleatorios(1,10)%2==0){
		printf("[Paciente] Soy el paciente con pid[%d], y tengo reaccion\n", getpid());
		exit(1);
	}else{
		printf("[Paciente] Soy el paciente con pid[%d], y no tengo reaccion\n", getpid());
		exit(2);

	}
	
}

int calculaAleatorios(int min, int max){
	return rand() % (max-min+1) + min;
}
