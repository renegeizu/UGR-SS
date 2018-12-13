// Modelo de Simulacion Dinamico y Discreto

#include <iostream>
#include <list>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

#define suceso_llegada 0
#define suceso_salida 1
#define suceso_finsimulacion 3
#define suceso_monitor 4
#define graficar 1

typedef struct {
	int suceso;
	float tiempo;
	int retraso;
} suc;

bool parar;
float acum_cola, acum_ocio, acum_retraso, acum_sistema, reloj, tlleg, tparada, tserv, valor;
float acum_sincola, init_sincola, tultsuc_cola, tultsuc_ocio, tultsuc_sistema;
int atendidos, encola, ensistema, libres, nada = 0, numSimul, m, maximacola;
list<float> cola;
list<suc> lsuc;
suc nodo;

float totalMedidasMed[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
int totalMaxColaMed = 0;
vector<vector<float>> totalMedidasDesv;
vector<vector<int>> totalMaxColaDesv;

bool compare(const suc &s1, const suc &s2){
	return s1.tiempo < s2.tiempo;
}

void insertar_lsuc(suc n){
	lsuc.push_back(n);
	lsuc.sort(compare);
}

float generador_exponencial(float media){
	float u;
	u = (float) random();
	u = u/(float)(RAND_MAX+1.0);
	return(-media*log(1-u));
}

float generallegada(float media){
	return generador_exponencial(media);
}

float generaservicio(float media){
	return generador_exponencial(media);
}

void inicializacion(){
	reloj = 0.0;
	libres = m;
	encola = 0;
	ensistema = 0;
	atendidos = 0;
	acum_cola = 0.0;
	acum_sistema = 0.0;
	acum_ocio = 0.0;
	acum_retraso = 0.0;
	tultsuc_cola = reloj;
	tultsuc_ocio = reloj;
	tultsuc_sistema = reloj;
	acum_sincola = 0.0;
	init_sincola = reloj;
	maximacola = 0;
	nodo.suceso = suceso_llegada;
	nodo.tiempo = reloj+generallegada(tlleg);
	nodo.retraso = nada;
	insertar_lsuc(nodo);
	nodo.suceso = suceso_finsimulacion;
	nodo.tiempo = reloj+tparada;
	nodo.retraso = nada;
	insertar_lsuc(nodo);
	parar = false;
}

void temporizacion(){
	nodo = lsuc.front();
	lsuc.pop_front();
	reloj = nodo.tiempo;
}

void llegada(){
	acum_sistema += (reloj - tultsuc_sistema) * ensistema;
	tultsuc_sistema = reloj;
	ensistema++;
	nodo.suceso = suceso_llegada;
	nodo.tiempo = reloj+generallegada(tlleg);
	nodo.retraso = nada;
	insertar_lsuc(nodo);
	if(libres > 0){
		acum_ocio += (reloj - tultsuc_ocio) * libres;
		tultsuc_ocio = reloj;
		libres--;
		nodo.suceso = suceso_salida;
		nodo.tiempo = reloj+generaservicio(tserv);
		nodo.retraso = nada;
		insertar_lsuc(nodo);
	}else{
		if(encola == 0)
			acum_sincola += reloj - init_sincola;
		acum_cola += (reloj - tultsuc_cola) * encola;
		tultsuc_cola = reloj;
		encola++;
		if(encola > maximacola)
			maximacola = encola;
		cola.push_back(reloj);
	}
}

void salida(){
	acum_sistema += (reloj - tultsuc_sistema) * ensistema;
	tultsuc_sistema = reloj;
	ensistema--;
	atendidos++;
	acum_retraso += nodo.retraso;
	if(encola > 0){
		acum_cola += (reloj - tultsuc_cola) * encola;
		tultsuc_cola = reloj;
		encola--;
		if(encola == 0)
			init_sincola = reloj;
      	valor = cola.front();
	     cola.pop_front();
     	nodo.suceso = suceso_salida;
     	nodo.tiempo = reloj+generaservicio(tserv);
     	nodo.retraso = reloj-valor;
     	insertar_lsuc(nodo);
     }else{
		acum_ocio += (reloj - tultsuc_ocio) * libres;
		tultsuc_ocio = reloj;
		libres++;
     }
}

void fin(){
	parar = true;
	float retrasomedio = acum_retraso/atendidos;
	float estanciamedia = retrasomedio + tserv;
	acum_cola += (reloj - tultsuc_cola) * encola;
	float encolamedio = acum_cola/reloj;
	acum_sistema += (reloj - tultsuc_sistema) * ensistema;
	float ensistemamedio = acum_sistema/reloj;
	if(encola == 0)
		acum_sincola += reloj - init_sincola;
	float colasnovaciasmedio = acum_cola/(reloj - acum_sincola);
	acum_ocio += (reloj - tultsuc_ocio) * libres;
	float porcentajemedioocio = 100*acum_ocio/(m*reloj);
	if(!graficar){
		printf("\nTiempo Medio de Espera en Cola = %.3f", retrasomedio);
		printf("\nTiempo Medio de Estancia en el Sistema = %.3f", estanciamedia);	
		printf("\nNumero Medio de Personas en Cola = %.3f", encolamedio);
		printf("\nNumero Medio de Personas en el Sistema = %.3f", ensistemamedio);
		printf("\nLongitud Media de Colas no Vacias = %.3f", colasnovaciasmedio);
		printf("\nPorcentaje Medio de Tiempo de Ocio por Servidor = %.3f", porcentajemedioocio);
		printf("\nLongitud Maxima de la Cola = %d", maximacola);
		printf("\n");
	}else{
		float totalMedidasMed[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		int totalMaxColaMed = 0;
		vector<vector<float>> totalMedidasDesv;
		vector<vector<int>> totalMaxColaDesv;
		//Media
		totalMedidasMed[0] += retrasomedio;
		totalMedidasMed[1] += estanciamedia;
		totalMedidasMed[2] += encolamedio;
		totalMedidasMed[3] += ensistemamedio;
		totalMedidasMed[4] += colasnovaciasmedio;
		totalMedidasMed[5] += porcentajemedioocio;
		totalMaxColaMed += maximacola;
		//Desviacion
		//Inicializar Vector de Vectores y agregar
	}
}

void monitor(){
	nodo.suceso = suceso_monitor;
	nodo.tiempo = reloj+100.0;
	nodo.retraso = nada;
	insertar_lsuc(nodo);
	if(!graficar){
		cout << "Tiempo y Media en Sistema: " << reloj << " -- " << (acum_sistema+(reloj - tultsuc_sistema) * ensistema)/reloj << endl;
	}
}

void suceso(){
	switch(nodo.suceso){
		case suceso_llegada:
			llegada();
			break;
		case suceso_salida:
			salida();
			break;
		case suceso_finsimulacion:
			fin();
			break;
		case suceso_monitor:
			monitor();
			break;
	}
}

int main(int argc, char *argv[]){
	int i;
	if(argc == 5){
		sscanf(argv[1], "%d", &m);
		sscanf(argv[2], "%f", &tparada);
		sscanf(argv[3], "%f", &tlleg);
		sscanf(argv[4], "%f", &tserv);
		numSimul = 100;
	}else if(argc == 6){
		sscanf(argv[1], "%d", &m);
		sscanf(argv[2], "%f", &tparada);
		sscanf(argv[3], "%f", &tlleg);
		sscanf(argv[4], "%f", &tserv);
		sscanf(argv[5], "%d", &numSimul);
	}else{
		printf("\nFormato 4 Parametros: <Numero Servidores> <Tiempo Parada> <tlleg> <tserv>\n");
		printf("\nFormato 5 Parametros: <Numero Servidores> <Tiempo Parada> <tlleg> <tserv> <Numero Simulaciones>\n");
		exit(1);
	}
	srandom(time(NULL));	
	for(int i = 0; i < numSimul; i++){
		if(!graficar){
			printf("\nResultados Interacion %d:\n", i);
		}
		inicializacion();
		while(!parar){
			temporizacion();
			suceso();
		}
	}
	if(graficar){
		//Media

		//Desviacion
	}
}