// Modelo de Simulacion - Metodo de Incremento Variable de Tiempo

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace std::chrono;

#define graficar 1

bool servidor;
double acum_cola, inicio_ocio, ocio, reloj, tiempo_llegada, tLlegada, tiempo_salida, tServicio, tultsuc;
long atendidos, encola, infinito, numSimul, total_a_atender;

float generaLlegada(float tLlegada){
	float u = (float) ((float) random()/(RAND_MAX+1.0));
	return (-tLlegada*log(1-u));
}

float generaServicio(float tServicio){
	float u = (float) ((float) random()/(RAND_MAX+1.0));
	return (-tServicio*log(1-u));
}

void iniciarValores(){
	srand(time(NULL));
	infinito = 10e17;
	atendidos = 0;
	inicio_ocio = 0.0;
	acum_cola = 0.0;
	reloj = 0.0;
	tultsuc = reloj;
	servidor = true;
	encola = 0;
	tiempo_llegada = reloj + generaLlegada(tLlegada);
	tiempo_salida = infinito;
	ocio = 0.0;
}

int main(int argc, char *argv[]){
	if(argc == 1){
		tLlegada = 9;
		tServicio = 6;
		total_a_atender = 10000;
		numSimul = 100;
	}else if(argc == 4){
		sscanf(argv[1], "%lf", &tLlegada);
		sscanf(argv[2], "%lf", &tServicio);
		sscanf(argv[3], "%ld", &total_a_atender);
		numSimul = 100;
	}else if(argc == 5){
		sscanf(argv[1], "%lf", &tLlegada);
		sscanf(argv[2], "%lf", &tServicio);
		sscanf(argv[3], "%ld", &total_a_atender);
		sscanf(argv[4], "%ld", &numSimul);
	}else{
		printf("\nFormato 3 Parametros: <Tiempo Llegada> <Tiempo Servicio> <Numero Clientes>\n");
		printf("\nFormato 4 Parametros: <Tiempo Llegada> <Tiempo Servicio> <Numero Clientes> <Numero Simulaciones>\n");
		exit(1);
	}
	for(int i = 0; i < numSimul; i++){
		iniciarValores();
		high_resolution_clock::time_point tIni, tFin;
		tIni = high_resolution_clock::now();
		while(atendidos < total_a_atender){
			reloj = min(tiempo_llegada, tiempo_salida);
			if(reloj == tiempo_llegada){
				tiempo_llegada = reloj + generaLlegada(tLlegada);
				if(servidor){
					servidor = false;
					tiempo_salida = reloj + generaServicio(tServicio);
					ocio += reloj - inicio_ocio;
				}else{
					acum_cola += (reloj-tultsuc)*encola;
					tultsuc = reloj;
					encola++;
				}
			}
			if(reloj == tiempo_salida){
				atendidos++;
				if(encola > 0){
					acum_cola += (reloj-tultsuc)*encola;
					tultsuc = reloj;
					encola--;
					tiempo_salida = reloj + generaServicio(tServicio);
				}else{
					servidor = true;
					inicio_ocio = reloj;
					tiempo_salida = infinito;
				}
			}
			reloj++;
		}
		tFin = high_resolution_clock::now();
		if(graficar){
				printf("%lf\t%lf\t%f\t%ld\n", (double) (ocio*100/reloj), (double) (acum_cola/reloj), 
					(duration_cast<duration<double>>(tFin-tIni)).count(), total_a_atender);
		}else{
				printf("Tiempo de Ocio del Servidor: %lf por Ciento", (double) (ocio*100/reloj));
				printf("Numero Medio de Clientes en Cola: %lf por Ciento", (double) (acum_cola/reloj));
				printf("Numero Total de Clientes: %ld", total_a_atender);
				printf("Tiempo Total de Ejecucion: %f", (duration_cast<duration<double>>(tFin-tIni)).count());
		}
	}
	return 0;
}
