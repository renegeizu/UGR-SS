//Ejercicio 2.1 - Mejorando los Generadores

#include <cmath>
#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <stdlib.h>

using namespace std;

long mediciones = 10;

/**
  * @brief Genera un numero uniformemente distribuido en el intervalo [0,1)
  */
double uniforme(){
	return (double) (random()/((double) RAND_MAX+1.0));
}

/**
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado a)
  */
float *construye_prop_a(int n){ //Ordenado Ascendente Automaticamente
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
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado a)
  */
multimap<float,int,greater<float>> construye_prop_a_orden(int n){ //Ordenado Usando Multimap (Decreciente)
	int i;
	multimap<float,int,greater<float>> temp, aux;
	for(i = 0; i < n; i++){
		temp.insert(pair<float,int>((float)1.0/n,i));
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
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado b)
  */
float* construye_prop_b(int n){ //Ordenado Ascendente Automaticamente
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
  * @brief Construye la tabla de busqueda de tamaño n para la distribucion de la demanda (Apartado b)
  */
multimap<float,int,greater<float>> construye_prop_b_orden(int n){ //Ordenado Usando Multimap (Decreciente)
	int i, max;
	multimap<float,int,greater<float>> temp, aux;
	max = (n/2)*(n+1);
	temp.insert(pair<float,int>(n*1.0/max,0));
	for(i = 1; i < n; i++){
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
multimap<float,int,greater<float>> construye_prop_c_orden(int n){ //Ordenado Usando Multimap (Decreciente)
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
	while((i<tama) && (u>=tabla[i])){
		i++;
	}
	return i;
}

/**
  * @brief Funcion Principal
  */
int main(int argc, char *argv[]){;
	if(argc == 1){
		mediciones = 10;
	}else if(argc == 2){
		sscanf(argv[1], "%ld", &mediciones);
	}else{
		printf("\nFormato de 1 Argumento: <Numero Mediciones>\n");
		exit(1);
	}
	srand(time(NULL));

	//Mediciones y Printar resultados

	return 0;
}
