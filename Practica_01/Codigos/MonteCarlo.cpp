#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define graficar 1

float distAcumulada, dist2, distMedia, desvTipica, probOcupada, distMejor;
long vez, j, posInicial, posicion, destino, aparca, vision, distancia, veces, cuantoPasarme, distActual, distrib[2000], posMejor;
bool stop, plazaLibre;
int opcionProbabilidad;

bool generar_dato(){
	float u;
	u = (float) random();
	u = u/(float)(RAND_MAX+1.0);
	if(u < probOcupada){
		return false;
	}else {
		return true;
	}
}

int main(int argc, char *argv[]){
	if(argc == 1){
		veces = 100000;
		destino = 100;
		vision = 2;
		probOcupada = 0.9;
	}else if(argc != 6){
		printf("\nFormato Argumentos: <Numero de Iteraciones> <Posicion de Destino> <Numero de Posiciones a la Vista> <Probabilidad de Plaza Ocupada> <0 -> [0,1], 1 -> [0,100]>\n");
		exit(1);
	}else{
		sscanf(argv[1], "%ld", &veces);
		sscanf(argv[2], "%ld", &destino);
		sscanf(argv[3], "%ld",&vision);
		sscanf(argv[4], "%f", &probOcupada);
		sscanf(argv[5], "%d", &opcionProbabilidad);
		if(probOcupada > 100){
			probOcupada = 100;
		}else if(probOcupada < 0){
			probOcupada = 0;
		}
		if(opcionProbabilidad == 1){
			probOcupada /= 100;
		}
	}
	srandom(time(NULL));
	distMejor = 10e30;
	posMejor = -1;
	for(posInicial = 0; posInicial < (destino+1); posInicial++){
		distAcumulada = 0.0;
		dist2 = 0.0;
		for(j = posInicial; j < (2*destino); j++){
			distrib[j] = 0;
		}
		for(vez = 0; vez < veces; vez++){
			stop = false;
			for(posicion = posInicial; !stop; posicion += (vision+1)){
				if(posicion <= (destino-vision)){
					for(j = 0; ((!stop) && (j < (vision+1))); j++){
						plazaLibre = generar_dato();
						if(plazaLibre){
							aparca = posicion+vision-j;
							distancia = destino-aparca;
							stop = true;
						}
					}
				}else if(posicion < destino){
					plazaLibre = generar_dato();
					if(plazaLibre){
						aparca = destino;
						distancia = 0;
						stop = true;
					}else{
						distActual = destino - posicion;
						cuantoPasarme = vision - distActual;
						if(cuantoPasarme < distActual){
							for(j = 1; ((!stop) && (j < (cuantoPasarme+1))); j++){
								plazaLibre = generar_dato();
								if(plazaLibre){
									aparca = destino - j;
									distancia = j;
									stop = true;
								}else{
									plazaLibre = generar_dato();
									if(plazaLibre){
										aparca = destino + j;
										distancia = j;
										stop = true;
									}
								}
							}
							if(!stop){
								for(j = (cuantoPasarme+1); ((!stop) && (j < (distActual+1))); j++){
									plazaLibre = generar_dato();
									if(plazaLibre){
										aparca = destino - j;
										distancia = j;
										stop = true;
									}
								}
							}
						}else{
							for(j = 1; ((!stop) && (j < (distActual+1))); j++){
								plazaLibre = generar_dato();
								if(plazaLibre){
									aparca = destino - j;
									distancia = j;
									stop = true;
								}else{
									plazaLibre = generar_dato();
									if(plazaLibre){
										aparca = destino + j;
										distancia = j;
										stop = true;
									}
								}
							}
							if(!stop){
								for(j = (distActual+1); ((!stop) && (j < (cuantoPasarme+1))); j++){
									plazaLibre = generar_dato();
									if(plazaLibre){
										aparca = destino + j;
										distancia = j;
										stop = true;
									}
								}
							}
						}
					}
				}else{
					for(j = 0; ((!stop) && (j < (vision+1))); j++){
						plazaLibre = generar_dato();
						if(plazaLibre){
							aparca = posicion+j;
							distancia = aparca - destino;
							stop = true;
						}
					}
				}
			}
			distAcumulada += distancia;
			dist2 += distancia * distancia;
			distrib[aparca]++;
		}
		distMedia = distAcumulada/veces;
		desvTipica = sqrt((dist2-veces*distMedia*distMedia)/(veces-1));
		if(distMedia < distMejor){
			distMejor = distMedia;
			posMejor = posInicial;
		}
		if(!graficar){
			printf("\nINFORME PARCIAL -> Posicion Inicial: (%ld)", posInicial);
			printf("\nDistancia al Punto de Destino: Media %f, Desv. Tipica %f", distMedia, desvTipica);
		}
	}
	if(!graficar){
		printf("\n\nINFORME ->");
		printf("\nNumero de Veces que se Repite: (%ld)", veces);
		printf("\nPosicion Destino: (%ld)", destino);
		printf("\nAlcance: (%ld)", vision);
		printf("\nProbabilidad de Plaza Ocupada: (%f) ", probOcupada);
		printf("\n\nMejor Posicion Inicial: %ld ", posMejor);
		printf("\nMejor Distancia: %f ", distMejor);
		printf("\n");
	}else{
		printf("%ld\t%ld\t%ld\t%f\t%ld\t%f\n", veces, destino, vision, probOcupada, posMejor, distMejor);
	}
}
