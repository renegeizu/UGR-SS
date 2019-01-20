//Examen- Modelizacion por Monte Carlo

#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;

#define graficar 1

float desviacion = 0.0, ganancia = 0.0, ganancia_esperada = 0.0, mejor_desviacion = 0.0, mejor_ganancia = 0.0, 
	precio_ganado = 0.0, precio_ganado = 0.0, sum = 0.0, sum_total = 0.0;
int a = 2000, b = 3000, c = 2600, demanda = 0, mejor_s = 0, num_veces = 100;

/**
  * @brief Genera un numero uniformemente distribuido en el intervalo [0,1)
  */
float uniforme(){
	float u;
	u = (float) random();
	u = u/(float)(RAND_MAX+1.0);
	return u;
}

/**
  * @brief Generador triangular
  */
int genera_demanda() {
	float u, x;
	u = uniforme();
	if(u < (c-a)/(b-a))
		x = a + sqrt((b-a)*(c-a)*u);
	else
		x = b - sqrt((b-a)*(b-c)*(1-u));
	return (int) x;
}

/**
  * @brief Funcion Principal
  */
int main(int argc, char *argv[]){
	if(argc == 1){
		num_veces = 100;
	}else if(argc == 2){
		sscanf(argv[1], "%ld", &num_veces);
	}else{
		printf("\nFormato de Argumentos: <Numero Repeticiones>\n");
		exit(1);
	}
	srand(time(NULL));
	for(int s = a; a <= b; s++){
		sum = 0.0;
		sum_total = 0.0;
		for(i = 0; i < num_veces; i++){
			demanda = genera_demanda();
			if(s >= demanda){
				precio_pagado = (float)s*0.25;
				precio_ganado = (float)demanda*0.6;
				ganancia =  precio_ganado - precio_pagado;
			}else{
				precio_pagado = (float)s*0.25 + (float)(demanda-s)*0.375
				precio_ganado = (float)demanda*0.6
				ganancia = precio_ganado - precio_pagado;
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
