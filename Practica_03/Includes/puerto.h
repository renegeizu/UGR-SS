#include <list>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define SUCESO_LLEGADA_BARCO 0
#define SUCESO_FIN_ATRAQUE 1
#define SUCESO_FIN_CARGA 2
#define SUCESO_FIN_DESATRAQUE 3
#define SUCESO_FIN_VIAJE_AT_BO 4
#define SUCESO_FIN_VIAJE_BO_AT 5
#define SUCESO_COMIENZO_TORMENTA 6
#define SUCESO_FIN_TORMENTA 7
#define SUCESO_FIN_SIMULACION 8

#define LIBRE 9
#define OCUPADO 10

typedef struct {
	int tipo;
	float tiempo;
} registro;

typedef struct {
	int suceso;
	float tiempo;
	registro reg_cola;
} suc;

bool parar, tormenta;
float reloj, tdus_at, tdus_lleg, tdus_sal, tdus_rem;
int atraques_libres, encola_lleg, encola_sal, remolcador;
list<registro> cola_llegadas, cola_salidas;
list<suc> lsuc;
registro reg_cola, reg_cola_null;
suc nodo;

int cont_simu, num_barcos[3];
float acum_estancia[3], acum_lleg, acum_at_desocupado, acum_at_cargando, acum_rem_amarrado, acum_sal;
float acum_at_yacargado, acum_rem_remolcando, acum_rem_viajando;
float **informe;

int num_atraques = 3, num_tiposbarco = 3, remolcadores = 1;
float tllegmax = 18.0, tllegmin = 4.0, tviajellenomax = 1.25, tviajellenomin = 0.75, tviajevacio = 0.25;
float dur_tormentamax = 6.0, dur_tormentamin = 2.0, tentre_tormentas = 48.0;
float frec1 = 0.25, frec2 = 0.25, frec3 = 0.50;
float tiempo_carga1min = 16.0, tiempo_carga2min = 21.0, tiempo_carga3min = 32.0;
float tiempo_carga1max = 20.0, tiempo_carga2max = 27.0, tiempo_carga3max = 40.0;
float tparada = 8760.0;

bool busca_suceso(int tipo);
bool compare(const suc &s1, const suc &s2);

float genera_barco(float tllegmin, float tllegmax);
float genera_durtormenta(float dur_tormentamin, float dur_tormentamax);
float genera_tiempocarga(int tipo);
float genera_tormenta(float tentre_tormentas);
float genera_viajelleno(float tviajellenomin, float tviajellenomax);
float generador_exponencial(float media);
float generador_uniforme(float min, float max);

int generador_discreto();
int genera_tipobarco();

void comienzo_tormenta();
void fin_atraque();
void fin_carga();
void fin_desatraque();
void fin_simulacion();
void fin_tormenta();
void fin_viaje_at_bo();
void fin_viaje_bo_at();
void generador_informes(int simulaciones);
void inicializacion();
void insertar_lsuc(suc n);
void llegada_barco();
void suceso();
void temporizacion();
