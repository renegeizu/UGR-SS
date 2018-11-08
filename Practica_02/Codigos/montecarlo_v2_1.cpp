//Ejercicio 1.4 - Modificacion del Modelo (Segunda Modificacion)

#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;

#define graficar 1

double desviacion = 0.0, ganancia = 0.0, ganancia_esperada = 0.0, mejor_desviacion = 0.0, 
	mejor_ganancia = 0.0, sum = 0.0, sum_total = 0.0;
float* tabla_demanda;
int mejor_s = -1;
long demanda = 0, i = 0, num_apartado = 0, num_veces = 0, s = 0, tama = 100, valor_x = 0, valor_y = 0, valor_z = 0;

/**
  * @brief Genera un numero uniformemente distribuido en el intervalo [0,1)
  */
double uniforme(){
	return (double) (random()/((double) RAND_MAX+1.0));
}

/**
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado a)
  */
float* construye_prop_a(int n){
	int i;
	float* temp;
	if((temp = (float*) malloc(n*sizeof(float))) == NULL){
		fputs("Error reservando memoria para generador uniforme\n", stderr);
		exit(1);
	}
	temp[0] = 1.0/n;
	for(i = 1; i < n; i++){
		temp[i] = temp[i-1]+1.0/n;
	}
	return temp;
}

/**
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado b)
  */
float* construye_prop_b(int n){
	int i, max;
	float* temp;
	if((temp = (float*) malloc(n*sizeof(float))) == NULL){
		fputs("Error reservando memoria para generador proporcional\n", stderr);
		exit(1);
	}
	max = (n/2)*(n+1);
	temp[0] = n*1.0/max;
	for(i = 1; i < n; i++){
		temp[i] = temp[i-1]+(float)(n-i)/max;
	}
	return temp;
}

/**
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado c)
  */
float* construye_prop_c(int n){
	int i, max;
	float *temp;
	if((temp = (float*) malloc(n*sizeof(float))) == NULL){
		fputs("Error reservando memoria para generador triangular\n",stderr);
		exit(1);
	}
	max = n*n/4;
	temp[0] = 0.0;
	for(i = 1; i < (n/2); i++){
		temp[i] = temp[i-1]+(float)i/max;
	}
	for(i = (n/2); i < n; i++){
		temp[i] = temp[i-1]+(float)(n-i)/max;
	}
	return temp;
}

/**
  * @brief Genera un valor de la distribucion de la demanda codificada en tabla, por el metodo de
  *		 tablas de busqueda. Tama es el tamaño de la tabla, 100 en nuestro caso particular
  */
int genera_demanda(float *tabla, int tama){
	int i;
	double u = uniforme();
	i = 0;
	while((i<tama) && (u>=tabla[i])){
		i++;
	}
	return i;
}

/**
  * @brief Funcion Principal
  */
int main(int argc, char *argv[]){
	if(argc == 1){
		valor_x = 10;
		valor_y = 0;
		valor_z = 0;
		num_veces = 100;
		num_apartado = 1;
	}else if(argc == 6){
		sscanf(argv[1], "%ld", &valor_x);
		sscanf(argv[2], "%ld", &valor_y);
		sscanf(argv[3], "%ld", &valor_z);
		sscanf(argv[4], "%ld", &num_veces);
		sscanf(argv[5], "%ld", &num_apartado);
	}else if(argc == 7){
		sscanf(argv[1], "%ld", &valor_x);
		sscanf(argv[2], "%ld", &valor_y);
		sscanf(argv[3], "%ld", &valor_z);
		sscanf(argv[4], "%ld", &num_veces);
		sscanf(argv[5], "%ld", &num_apartado);
		sscanf(argv[6], "%ld", &tama);
	}else{
		printf("\nFormato de 5 Argumentos: <Valor de X> <Valor de Y> <Valor de Z> <Numero de Veces> <Numero de Apartado>\n");
		printf("\nFormato de 6 Argumentos: <Valor de X> <Valor de Y> <Valor de Z> <Numero de Veces> <Numero de Apartado> <Tamaño>\n");
		exit(1);
	}
	srand(time(NULL));
	switch(num_apartado){
		case 1:{
			tabla_demanda = construye_prop_a(tama);
			break;
		}
		case 2:{
			tabla_demanda = construye_prop_b(tama);
			break;
		}
		case 3:{
			tabla_demanda = construye_prop_c(tama);
			break;
		}
		default:{
			printf("\nError: Numeros validos de apartado -> 1, 2 o 3\n");
			exit(1);
			break;
		}
	}
	for(s = 0; s <= tama; s++){
		sum = 0.0;
		sum_total = 0.0;
		for(i = 0; i < num_veces; i++){
			demanda = genera_demanda(tabla_demanda, tama);
			if(s > demanda){
				ganancia = demanda*valor_x-min(valor_z, (s-demanda)*valor_y);
			}else{
				ganancia = s*valor_x;
			}
			sum += ganancia;
			sum_total += ganancia*ganancia;
		}
		ganancia_esperada = sum/num_veces;
		desviacion = sqrt((sum_total-num_veces*ganancia_esperada*ganancia_esperada)/(num_veces-1));
		if(graficar){
			printf("%ld\t%lf\n", s, ganancia_esperada);
		}
		if(mejor_ganancia < ganancia_esperada){
			mejor_ganancia = ganancia_esperada;
			mejor_desviacion = desviacion;
			mejor_s = s;
		}
	}
	if(!graficar){
		printf("Mayor Ganancia Esperada: %lf\tDesviacion: %lf\tMejor S: %d\n", mejor_ganancia, mejor_desviacion, mejor_s);
	}
	return 0;
}
