/**
  * @brief Implementa el sistema de radares con varias simulaciones y calculo de medias y desviaciones 
  *        tipicas, con incremento del tiempo variable.
  */

#include <iostream>
#include <time.h>
#include <list>
#include <math.h>
#include <stdlib.h>

using namespace std;

#define suceso_llegada 0
#define suceso_fallo 1
#define suceso_finsimulacion 2
#define graficar 1

/**
  * @brief Estructura que representara un suceso en el sistema y en la que se almacenan el tipo de 
  *        suceso y el instante en que ocurrira.
  */
typedef struct {
	int suceso;
	float tiempo;
} Nodo;

void inicializacion();
int temporizacion();
void suceso(int suc_sig);
void llegada();
void fallo();
void fin();
bool compare(const Nodo &s1, const Nodo &s2);
void insertar_lsuc(Nodo n);
float generafallo();
float generareparacion();

bool parar;
int disponibles, totalReserva, totalRadares, radares, simulaciones, i;
float reloj, tfallo, tTotal, vmin, vmax, inicio, veces, mediaVeces = 0.0, mediaVeces2 = 0.0, desvVeces = 0.0, tDesprotegido;
float tDesprotegidoMedio = 0.0, tDesprotegidoMedio2 = 0.0, tDesprotegidoDesv = 0.0, porcentajeDesproteccion;
float porcentajeDesproteccionMedio = 0.0, porcentajeDesproteccionMedio2 = 0.0, porcentajeDesproteccionDesv = 0.0;
list<Nodo> lsuc;

/**
  * @brief Funcion principal.
  * @param argc Indica el numero de parametros que se le pasan al programa en la linea de ordenes.
  * @param argv Estructura que almacena los distintos parametros que se le pasan al programa en la linea de ordenes.
  * @return 0 en el caso de que su ejecucion sea correcta.
  */
int main(int argc, char *argv[]){
	if((argc != 8) && (argc != 3)){
		cout << "Error: Numero Incorrecto de Parametros.\n";
		cout << "El Uso Correcto del Programa es:\n";
		cout << "<Numero Radares> <Numero Repuestos> <Vmin> <Vmax> <Tiempo Fallo> <Tiempo Fin> <Numero Simulaciones>\n" << endl;
		cout << " o tambien <Numero de Repuestos> <Numero de Simulaciones>\n" << endl;
		exit(1);
	}else if(argc == 8){
		totalRadares = atoi(argv[1]);
		totalReserva = atoi(argv[2]);
		vmin = atof(argv[3]);
		vmax = atof(argv[4]);
		tfallo = atof(argv[5]);
		tTotal = atof(argv[6]);
		simulaciones = atoi(argv[7]);
	}else{
		totalRadares = 5;
		totalReserva = atoi(argv[1]);
		vmin = 15.0;
		vmax = 30.0;
		tfallo = 20.0;
		tTotal = 365.0;
		simulaciones = atoi(argv[2]);
	}
	clock_t j,fin;
	double tiempo;
	j = clock();
	int suc_sig;
	if(!graficar){
		cout << "\nSIMULANDO...\n" << endl;
	}
	srand(time(NULL));
	for(i = 0; i < simulaciones; i++){
		inicializacion();
		while(!parar){
			suc_sig = temporizacion();
			suceso(suc_sig);
		}
		while(!lsuc.empty()){
			lsuc.pop_front();
		}
	}
	mediaVeces = mediaVeces / simulaciones;
	porcentajeDesproteccionMedio = porcentajeDesproteccionMedio / simulaciones;
	tDesprotegidoMedio = tDesprotegidoMedio / simulaciones;
	porcentajeDesproteccionDesv = sqrt((porcentajeDesproteccionMedio2-simulaciones*porcentajeDesproteccionMedio*porcentajeDesproteccionMedio)/(simulaciones-1));
	tDesprotegidoDesv = sqrt((tDesprotegidoMedio2-simulaciones*tDesprotegidoMedio*tDesprotegidoMedio)/(simulaciones-1));
	desvVeces = sqrt((mediaVeces2-simulaciones*mediaVeces*mediaVeces)/(simulaciones-1));
	if(!graficar){
		cout << "\n\tMEDIAS:\n" << endl;
		cout << "La Media del Numero de Fallos en los Radares es: " << mediaVeces;
		cout << "\nLa Media del Tiempo de Desproteccion es: " << tDesprotegidoMedio;
		cout << "\nLa Media del Porcentaje de Tiempo de Desproteccion es: " << porcentajeDesproteccionMedio;
		cout << "\n\n\tDESVIACIONES TIPICAS:\n" << endl;
		cout << "La Desviacion Tipica del Numero de Fallos en los Radares es: " << desvVeces;
		cout << "\nLa Desviacion Tipica del Tiempo de Desproteccion es: " << tDesprotegidoDesv;
		cout << "\nLa Desviacion Tipica del Porcentaje de Tiempo de Desproteccion es: " << porcentajeDesproteccionDesv;
	}else{
		//printf("\n", totalRadares, totalReserva, mediaVeces, tDesprotegidoMedio, porcentajeDesproteccionMedio);
	}
	fin = clock();
	fin = fin - j;
	tiempo = (double)fin/CLOCKS_PER_SEC;
	if(!graficar){
		cout << "\n\nEl Tiempo en Realizar las " << simulaciones << " Simulaciones ha sido de: " << tiempo;
		cout << " Segundos, con " << totalRadares << " Radares y " << totalReserva << " Piezas, para un Tiempo de Parada de: " ;
		cout << tTotal << " y Periodo de Envio a Reparacion entre " << vmin << " y " << vmax;
		cout << ", y Tiempos entre Fallos del Radar: " <<  tfallo << "\n" << endl;
	}
}

/**
  * @brief Crea sucesos iniciales de fallo para cada radar, asi como el suceso fin de simulacion.
  *        Ademas establece los valores necesarios de las variables de estado a los valores iniciales.
  */
void inicializacion(){
	reloj = 0.0;
	disponibles = totalReserva;
	radares = totalRadares;
	veces = 0.0;
	tDesprotegido = 0.0;
	porcentajeDesproteccion = 0.0;
	inicio = reloj;
	Nodo aux;
	for(int k = 0; k < radares; k++){
		aux.suceso = suceso_fallo;
		aux.tiempo = reloj + generafallo();
		insertar_lsuc(aux);
	}
	aux.suceso = suceso_finsimulacion;
	aux.tiempo = tTotal;
	insertar_lsuc(aux);
	parar = false;
}

/**
  * @brief Extrae de la lista de sucesos, el suceso que se producira mas proximo en el tiempo 
  *        (es decir el que esta en el tope de la cola). Ademas hace que el reloj que se actualice al 
  *        instante en que ocurre el suceso.
  * @return Devuelve el tipo de suceso que se extrae de la lista.
  */
int temporizacion(){
	Nodo aux;
	int suc_sig;
	aux = lsuc.front();
	lsuc.pop_front();
	suc_sig = aux.suceso;
	reloj = aux.tiempo;
	return (suc_sig);
}

/**
  * @brief Evalua el suceso que se le pasa como parametro y llama a la funcion correspondiente.
  * @param suc_sig Representa al suceso que va a ocurrir a continuacion. Se evaluara y se llamara a 
  *        la funcion que lo procesa.
  */
void suceso(int suc_sig){
	if(suc_sig == suceso_llegada){
		llegada();
	}else{
		if(suc_sig == suceso_fallo){
			fallo();
		}else{
			if(suc_sig == suceso_finsimulacion){
				fin();
			}
		}
	}
}

/**
  * @brief Representa al suceso de llegada de un componente reparado. Este procedimiento comprueba si 
  *        habia algun radar no operativo por falta de componentes, en ese caso al llegar la pieza pasa 
  *        a estar operativo y se fija el instante de tiempo en el que sucedera el siguiente fallo y se 
  *        actualiza el tiempo que ha estado desprotegido el sistema. Si todos los radares estaban operativos 
  *        y se produce una llegada, se dispondra de una nueva pieza de respuesto.
  */
void llegada(){
	Nodo aux;
	if(radares < totalRadares){
		radares ++;
		aux.suceso = suceso_fallo;
		aux.tiempo = reloj + generafallo();
		insertar_lsuc(aux);
		if(radares == totalRadares){
			tDesprotegido += reloj - inicio;
		}
	}else{
		disponibles++;
	}
}

/**
  * @brief Representa el procedimiento de fallo uno de los componentes de un radar. Si hay piezas de repuesto 
  *        disponibles se sustituira de inmediato sin que el sistema este desprotegido, y se fijara el siguiente 
  *        fallo de ese componente, ademas de disponer de un repuesto menos. En caso de no haber piezas disponibles 
  *        comienza el tiempo de desproteccion del sistema porque se dispone de un radar menos. En cualquiera 
  *        de los casos el componente averiado debe ser reparado.
  */
void fallo(){
	Nodo aux;
	if(disponibles > 0){
		disponibles--;
		aux.suceso = suceso_fallo;
		aux.tiempo = reloj+generafallo();
		insertar_lsuc(aux);
	}else{
		if(radares == totalRadares){
			inicio = reloj;
		}
		radares --;
		veces += 1;
	}
	aux.suceso = suceso_llegada;
	aux.tiempo = reloj + generareparacion();
	insertar_lsuc(aux);
}

/**
  * @brief Procedimiento que finaliza la simulacion, dando la orden de parar y calculando los valores de 
  *        salida de la simulacion
  */
void fin(){
	parar = true;
	if(radares < totalRadares){
		tDesprotegido += reloj-inicio;
	}
	mediaVeces += veces;
	mediaVeces2 += veces*veces;
	tDesprotegidoMedio += tDesprotegido;
	tDesprotegidoMedio2 += tDesprotegido*tDesprotegido;
	porcentajeDesproteccion = 100.0*tDesprotegido/tTotal;
	porcentajeDesproteccionMedio += porcentajeDesproteccion;
	porcentajeDesproteccionMedio2 += porcentajeDesproteccion*porcentajeDesproteccion;
}

/**
  * @brief Compara sucesos por tiempo de ocurrencia
  */
bool compare(const Nodo &s1, const Nodo &s2){
	return s1.tiempo < s2.tiempo;
}

/**
  * @brief  Inserta de forma ordenada un elemento en la lista de sucesos
  */
void insertar_lsuc(Nodo n){
	lsuc.push_back(n);
	lsuc.sort(compare);
}

/**
  * @brief Generador de datos exponencial de tiempos entre fallos. El valor que le sirve de base 
  *        para calcular el nuevo tiempo de fallo es tfallo.
  * @return El valor real que representa el nuevo tiempo en el que se produce un fallo.
  */
float generafallo(){
	float u;
	u = (float) random();
	u = u/(float)(RAND_MAX + 1.0);
	u = (-tfallo*log(1-u));
	return u;
}

/**
  * @brief Generador de datos uniforme de tiempos de realizacion de una reparacion. Los valores que 
  *        le sirven de base para calcular el tiempo de reparacion son vmin y vmax.
  * @return El valor real que representa el tiempo que se tarda en reparar y devolver una pieza.
  */
float generareparacion(){
	float u;
	u = (float) random();
	u = u/(float)(RAND_MAX+1.0); //RAND_MAX es una constante del sistema
	return (vmin+(vmax-vmin)*u);
}

