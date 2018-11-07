//Ejercicio 2.1 - Mejorando los Generadores - Parte 01

#include <cmath>
#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

float* tablaS;
double tTotalCS = 0.0, tTotalCO = 0.0;
long mediciones = 1000000, tama = 100;
multimap<float,int,greater<float>> tablaO;

/**
  * @brief Genera un numero uniformemente distribuido en el intervalo [0,1)
  */
double uniforme(){
	return (double) (random()/((double) RAND_MAX+1.0));
}

/**
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado a)
  */
float *construye_prop_a(int n){
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
multimap<float,int,greater<float>> construye_prop_c_orden(int n){
	int i, max;
	multimap<float,int,greater<float>> temp, aux;
	max = n*n/4;
	temp.insert(pair<float,int>((float)0.0,0));
	for(i = 1; i < (n/2); i++){
		temp.insert(pair<float,int>((float)i/max,i));
	}
	for(i = (n/2); i < n; i++){
		temp.insert(pair<float,int>((float)(n-i)/max,i));
	}
	pair<float,int> par;
	for (multimap<float,int>::iterator it = temp.begin(); it != temp.end(); ++it){
		if(it != temp.begin()){
			par = make_pair(par.first+it->first,it->second);
			aux.insert(par);
		}else{
			par = make_pair(it->first,it->second);
			aux.insert(par);
		}
	}
	return aux;
}

/**
  * @brief Genera un valor de la distribucion de la demanda codificada en tabla, por el metodo de
  *		 tablas de busqueda. Tama es el tamaño de la tabla, 100 en nuestro caso particular
  */
int genera_demanda(float *tabla, int tama){
	int i;
	double u = uniforme();
	i = 0;
	while((i>=tama) && (u>=tabla[i])){
		i++;
	}
	return i;
}

/**
  * @brief Genera un valor de la distribucion de la demanda codificada en tabla, por el metodo de
  *		 tablas de busqueda. Tama es el tamaño de la tabla, 100 en nuestro caso particular
  */
int genera_demanda_orden(multimap<float,int,greater<float>> tabla, int tama){
	int i;
	double u = uniforme();
	i = 0;
	for (multimap<float,int>::iterator it = tabla.begin(); it != tabla.end(); ++it){
		if(u >= it->first){
			it = --(tabla.end());
		}
		i++;
	}
	return i;
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
	tablaS = construye_prop_c(tama);
	for(int i = 0; i < mediciones; i++){
		tIni = high_resolution_clock::now();
		genera_demanda(tablaS, tama);
		tFin = high_resolution_clock::now();
		tTotalCS += (duration_cast<duration<double>>(tFin-tIni)).count();
	}
	tablaO = construye_prop_c_orden(tama);
	for(int i = 0; i < mediciones; i++){
		tIni = high_resolution_clock::now();
		genera_demanda_orden(tablaO, tama);
		tFin = high_resolution_clock::now();
		tTotalCO += (duration_cast<duration<double>>(tFin-tIni)).count();
	}
	cout << "Tabla Busqueda C - Sin Ordenar\t" << (double)tTotalCS/mediciones << endl;
	cout << "Tabla Busqueda C - Ordenada\t" << (double)tTotalCO/mediciones << endl;
	return 0;
}
