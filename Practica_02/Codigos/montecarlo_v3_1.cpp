//Ejercicio 2.1 - Mejorando los Generadores - Parte 01

#include <cmath>
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

double tTotalCS = 0.0, tTotalCO = 0.0;
float* tablaS;
int* posicion;
long mediciones = 1000000, tama = 100;

/**
  * @brief Genera un numero uniformemente distribuido en el intervalo [0,1)
  */
double uniforme(){
	return (double) (random()/((double) RAND_MAX+1.0));
}

/**
  * @brief Intercambia dos datos, de posicion, de un vector
  */
void intercambiarPos(int* B, int i, int j){
	float tmp = B[i];
	B[i] = B[j];
	B[j] = tmp;
}

/**
  * @brief Intercambia dos datos, de posicion, de un vector
  */
void intercambiarDato(float* A, int i, int j){
	float tmp = A[i];
	A[i] = A[j];
	A[j] = tmp;
}

/**
  * @brief Rellena el vector
  */
void rellenarVector(int* B, int N){
	int i;
	for(i = 0; i < N; i++){
		B[i] = i;
	}
}

/**
  * @brief Suma incremental para la tabla
  */
void sumaIncremental(float* A, int N){
	int i;
	for(i = 1; i < N; i++){
		A[i] = A[i]+A[i-1];
		
	}
}

/**
  * @brief Metodo de ordenacion por Seleccion
  */
void ordenacionSeleccion(float* A, int* B, int N){
	int i, j, k;
	for(i = 0; i < N - 1; i++){
		for(k = i, j = i + 1; j < N; j++){
			if(A[j] > A[k]){
				k = j;
			}
		}
		if(k != i){
			intercambiarDato(A, i, k);
			intercambiarPos(B, i, k);
		}
	}
	sumaIncremental(A, N);
}

/**
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado a)
  */
float* construye_prop_a(int n){
	int i;
	float *temp;
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
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado c)
  */
float* construye_prop_c_orden(int n, int* pos){
	int i, max;
	float *temp;
	if((temp = (float*) malloc(n*sizeof(float))) == NULL){
		fputs("Error reservando memoria para generador triangular\n",stderr);
		exit(1);
	}
	max = n*n/4;
	temp[0] = 0.0;
	for(i = 1; i < (n/2); i++){
		temp[i] = (float)i/max;
	}
	for(i = (n/2); i < n; i++){
		temp[i] = (float)(n-i)/max;
	}
	ordenacionSeleccion(temp, pos, n);
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
  * @brief Genera un valor de la distribucion de la demanda codificada en tabla, por el metodo de
  *		 tablas de busqueda. Tama es el tamaño de la tabla, 100 en nuestro caso particular
  */
int genera_demanda_orden(float* tabla, int* pos, int tama){
	int i;
	double u = uniforme();
	i = 0;
	while((i<tama) && (u>=tabla[i])){
		i++;
	}
	return pos[i];
}

/**
  * @brief Funcion Principal
  */
int main(int argc, char *argv[]){;
	if(argc == 1){
		mediciones = 1000000;
		tama = 100;
	}else if(argc == 2){
		sscanf(argv[1], "%ld", &mediciones);
	}else if(argc == 3){
		sscanf(argv[1], "%ld", &mediciones);
		sscanf(argv[2], "%ld", &tama);
	}else{
		printf("\nFormato de 1 Argumento: <Numero Mediciones>\n");
		printf("\nFormato de 2 Argumento: <Numero Mediciones> <Tamaño>\n");
		exit(1);
	}
	if((posicion = (int*) malloc(tama*sizeof(int))) == NULL){
		fputs("Error reservando memoria para vector\n",stderr);
		exit(1);
	}
	srand(time(NULL));
	high_resolution_clock::time_point tIni, tFin;	
	tablaS = construye_prop_c(tama);
	for(int i = 0; i < mediciones; i++){
		tIni = high_resolution_clock::now();
		genera_demanda(tablaS, tama);
		tFin = high_resolution_clock::now();
		tTotalCS += (duration_cast<duration<double>>(tFin-tIni)).count();
	}
	tablaS = construye_prop_c_orden(tama, posicion);
	for(int i = 0; i < mediciones; i++){
		tIni = high_resolution_clock::now();
		genera_demanda_orden(tablaS, posicion, tama);
		tFin = high_resolution_clock::now();
		tTotalCO += (duration_cast<duration<double>>(tFin-tIni)).count();
	}
	cout << "C-NoOrden\t" << (double)tTotalCS/mediciones << endl;
	cout << "C-Orden\t" << (double)tTotalCO/mediciones << endl;
	return 0;
}
