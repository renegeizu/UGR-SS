//Ejercicio 2.2 - Generadores Congruenciales

#include <cmath>
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

/**
  * @brief Genera un numero uniformemente distribuido en el intervalo [0,1)
  */
double uniforme(){
	return (double) (random()/((double) RAND_MAX+1.0));
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Entera - Long
  */
long* gen_cong_lin_entera(int a, int b, int m, int n){
	int i;
	long *temp;
	if((temp = (long*) malloc(n*sizeof(long))) == NULL){
		fputs("Error reservando memoria para generador uniforme\n", stderr);
		exit(1);
	}
	temp[0] = (long)0;
	for(i = 1; i < n; i++){
		temp[i] = (a*temp[i-1]+4321)%m;
	}
	return temp;
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real Artesanal - Float
  */
float* gen_cong_lin_real_art_float(int a, int b, int m, int n){
	int i;
	float *temp;
	if((temp = (float*) malloc(n*sizeof(float))) == NULL){
		fputs("Error reservando memoria para generador uniforme\n", stderr);
		exit(1);
	}
	temp[0] = (float)0;
	for(i = 1; i < n; i++){
		temp[i] = (float)(a*temp[i-1]+b)/m;
		temp[i] = (float)(temp[i]-(int)temp[i])*m;
	}
	return temp;
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real Artesanal - Double
  */
double* gen_cong_lin_real_art_double(int a, int b, int m, int n){
	int i;
	double *temp;
	if((temp = (double*) malloc(n*sizeof(double))) == NULL){
		fputs("Error reservando memoria para generador uniforme\n", stderr);
		exit(1);
	}
	temp[0] = (double)0;
	for(i = 1; i < n; i++){
		temp[i] = (double)(a*temp[i-1]+b)/m;
		temp[i] = (double)(temp[i]-(int)temp[i])*m;
	}
	return temp;
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real Artesanal Corregido - Int
  */
float* gen_cong_lin_real_art_int(int a, int b, int m, int n){
	int i;
	float *temp;
	float aux;
	if((temp = (int*) malloc(n*sizeof(int))) == NULL){
		fputs("Error reservando memoria para generador uniforme\n", stderr);
		exit(1);
	}
	temp[0] = (int)0;
	for(i = 1; i < n; i++){
		aux = (float)(a*temp[i-1]+b)/m;
		aux = (float)(aux-(int)aux)*m;
		temp[0] = (int)(aux+0.5);
	}
	return temp;
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real fmod - Double
  */
double* gen_cong_lin_real_art_double(int a, int b, int m, int n){
	int i;
	double *temp;
	if((temp = (double*) malloc(n*sizeof(double))) == NULL){
		fputs("Error reservando memoria para generador uniforme\n", stderr);
		exit(1);
	}
	temp[0] = (double)0;
	for(i = 1; i < n; i++){
		temp[i] = (double)fmod((a*temp[i-1]+b),m);
	}
	return temp;
}

/**
  * @brief Funcion Principal
  */
int main(int argc, char *argv[]){;
	
	return 0;
}
