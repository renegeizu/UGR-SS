// Modelo de Simulacion Dinamico Continuo

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

#define graficar 1

const int numeq = 2;
bool tipo;
float estado[numeq], oldestado[numeq], t, dt, tinic, tfin;
float a11, a12, a21, a22;

void salida(){
	printf("%f\t%f\t%f\n", estado[0], estado[1], t);
}

void derivacion(float est[numeq], float f[numeq], float tt){
	f[0] = a11 * est[0] - a12 * est[0] * est[1];
	f[1] = a21 * est[0] * est[1] - a22 * est[1];
}

void one_step_runge_kutta(float inp[numeq], float out[numeq], float tt, float hh){
	int i, j;
	float time, f[numeq], incr, k[numeq][4];
	for(i = 0; i < numeq; i++){
		out[i] = inp[i];
	}
	time = tt;
	for(j = 0; j < 4; j++){
		derivacion(out, f, time);
     	for(i = 0; i < numeq; i++){
			k[i][j] = f[i];
		}
		if(j < 2){
			incr = hh/2;
		}else{
			incr = hh;
		}
		time = tt + incr;
		for(i = 0; i < numeq; i++){
			out[i] = inp[i] + k[i][j] * incr;
		}
	}
	for(i = 0; i < numeq; i++){
		out[i] = inp[i] + hh/6 * (k[i][0] + 2 * k[i][1] + 2 * k[i][2] + k[i][3]);
	}
}

void one_step_euler(float inp[numeq], float out[numeq], float tt, float hh){
	float f[numeq];
	derivacion(inp, f, tt);
	for(int i = 0; i < numeq; i++){
		out[i] = inp[i] + hh * f[i];
	}
}

void integracion_runge_kutta(){
	do{
		salida();
		oldestado[0] = estado[0];
		oldestado[1] = estado[1];
		one_step_runge_kutta(oldestado, estado, t, dt);
		t += dt;
	}while(t < tfin);
}

void integracion_euler(){
	do{
		salida();
		oldestado[0] = estado[0];
		oldestado[1] = estado[1];
		one_step_euler(oldestado, estado, t, dt);
		t += dt;
	}while(t < tfin);
}

void integracion(bool flag){
	if(flag){
		integracion_runge_kutta();
	}else{
		integracion_euler();
	}
}

int main(int argc, char *argv[]){
	int aux = 0;
	if(argc == 11){
		sscanf(argv[1], "%f", &a11);
		sscanf(argv[2], "%f", &a12);
		sscanf(argv[3], "%f", &a21);
		sscanf(argv[4], "%f", &a22);
		sscanf(argv[5], "%f", &dt);
		sscanf(argv[6], "%f", &tinic);
		sscanf(argv[7], "%f", &tfin);
		sscanf(argv[8], "%f", &estado[0]);
		sscanf(argv[9], "%f", &estado[1]);
		sscanf(argv[10], "%d", &aux);
		if(aux){
			tipo = true;
		}else{
			tipo = false;
		}
	}else{
		printf("\nArgumentos: <a11> <a12> <a21> <a22> <Int_Calculo> <T_Inicio> <T_Fin> <Est_X> <Est_Y>\n");
		printf("<Tipo (0: runge kutta, 1: euler)>\n");
		exit(1);
	}
	t = tinic;
	integracion(tipo);
	return 0;
}
