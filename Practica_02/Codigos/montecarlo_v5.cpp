//Ejercicio 2.1 - Mejorando los Generadores - Parte 03

#include <cmath>
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

double tTotalAS = 0.0, tTotalAO = 0.0, tTotalBS = 0.0, tTotalBO = 0.0, tTotalCS = 0.0, tTotalCO = 0.0;
float* tablaS;
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
  * @brief Genera un valor de la distribucion de la demanda codificada en tabla, por el metodo de
  *		 tablas de busqueda. Tama es el tamaño de la tabla, 100 en nuestro caso particular. Como
  *		 el incremento es costante, podemos multiplicar el dato por el tamaño y redondear al alza
  *		 para obtener la posicion
  */
int genera_demanda(float *tabla, int tama){
	return ceil((double)uniforme()*tama);
}

/**
  * @brief Genera un valor de la distribucion de la demanda codificada en tabla, por el metodo de
  *		 tablas de busqueda. Tama es el tamaño de la tabla, 100 en nuestro caso particular
  */
int genera_demanda_BS(float* tabla, int tama){
	double u = uniforme(), comparador = 1.0/tama, valor;
	int derecha = tama-1, izquierda = 0, centro;
	if(tama == 1){
		if(abs(tabla[0]-u) <  comparador){
			return 0;
		}else{
			return -1;
		}
	}else{
		while(izquierda <= derecha){
			centro = (derecha+izquierda)/2;
			valor = abs(tabla[centro]-u);
			if(valor <  comparador){
				return centro;			
			}else if(u >= tabla[centro]){
				izquierda = centro+1;
			}else{
				derecha = centro-1;
			}
		}
	}
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
	srand(time(NULL));
	high_resolution_clock::time_point tIni, tFin;
	tablaS = construye_prop_a(tama);
	for(int i = 0; i < mediciones; i++){
		tIni = high_resolution_clock::now();
		genera_demanda_BS(tablaS, tama);
		tFin = high_resolution_clock::now();
		tTotalAS += (duration_cast<duration<double>>(tFin-tIni)).count();
	}
	tablaS = construye_prop_a(tama);
	for(int i = 0; i < mediciones; i++){		
		tIni = high_resolution_clock::now();
		genera_demanda(tablaS, tama);
		tFin = high_resolution_clock::now();
		tTotalAO += (duration_cast<duration<double>>(tFin-tIni)).count();
	}
	cout << "Tabla Busqueda A - BS\t" << (double)tTotalAS/mediciones << endl;
	cout << "Tabla Busqueda A - Mejorada\t" << (double)tTotalAO/mediciones << endl;
	return 0;
}
