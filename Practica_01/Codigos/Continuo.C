#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define sq(X) { int aux = X; (X)*(X); }
#define dec(X) pow(10, X)
#define ctg(X) (1.0/tan(X))
#define actg(X) atan(1.0/(X))
#define sqn(X,Y) pow(X, 1.0/(Y))
#define lgn(X,Y) (log(Y)/log(X))
#define graficar 1

double _t = 0.0, _tinicio = 0.0, _tfin = 0.0, _inc_t = 0.0;
int _tcom = 1;
double __A = 0.0667, __B = 1.0E7, __C = 0.0222, __D = 1.0E7, __E = 36000.0, __F = 10.0;
double *__x = new double[1], *__y = new double[1], *x_aux = new double[1], *y_aux = new double[1];

double f1(double *__x, double *__y, double _t ){
	return ( ( ( __A ) * ( __x[0] ) ) * ( ( 1.0 ) - ( ( __x[0] ) / ( __B ) ) ) ) - ( ( __F ) * ( __y[0] ) );
}

double f2( double *__x, double *__y, double _t ){
	return ( ( __C ) * ( __y[0] ) ) * ( ( ( __x[0] ) / ( __D ) ) - ( ( __y[0] ) / ( __E ) ) ) ;
}

void resolver(double _t0, double _tf, double * _res ){
	double k1[2][1];
	double k2[2][1];
	double k3[2][1];
	double k4[2][1];
	double h = _inc_t ;
	for(_t = _t0; _t < (_tf-10e-8); _t += _inc_t){
		x_aux[0] = __x[0];
		y_aux[0] = __y[0];
		k1[0][0] = h * f1( __x, __y,_t );
		k1[1][0] = h * f2( __x, __y,_t );
		__x[0] = (x_aux[0]+k1[0][0] / 2);
		__y[0] = (y_aux[0]+k1[1][0] / 2);
		k2[0][0] = h * f1( __x, __y,_t + (h/2) );
		k2[1][0] = h * f2( __x, __y,_t + (h/2) );
		__x[0] = (x_aux[0]+k2[0][0] / 2);
		__y[0] = (y_aux[0]+k2[1][0] / 2);
		k3[0][0] = h * f1( __x, __y,_t + (h/2) );
		k3[1][0] = h * f2( __x, __y,_t + (h/2) );
		__x[0] = (x_aux[0]+k3[0][0]);
		__y[0] = (y_aux[0]+k3[1][0]);
		k4[0][0] = h * f1( __x, __y,_t + h);
		k4[1][0] = h * f2( __x, __y,_t + h);
		__x[0] = x_aux[0];
		__x[0] += (k1[0][0]+2*k2[0][0]+2*k3[0][0]+k4[0][0])/6;
		__y[0] = y_aux[0];
		__y[0] += (k1[1][0]+2*k2[1][0]+2*k3[1][0]+k4[1][0])/6;
		if(__x[0] < 0.0){ 
			__x[0] = 0.0;
		}
		if(__y[0] < 0.0){
			__y[0] = 0.0;
		}
	}
	_res[0] = _t;
	_res[1] = __x[0];
	_res[2] = __y[0];
}

int main(int argc, char *argv[]){
	_tinicio = 0.0;
	_inc_t = 0.1;
	_tcom = 10;
	if(argc == 1){
		cerr << " Duracion de la simulacion => ";
		cin >> _tfin;
		cerr << " Numero inicial de peces pequeños, x(0) => ";
		cin >> __x[0];
		cerr << " Numero inicial de peces grandes, y(0) => ";
		cin >> __y[0];
	}else if(argc != 4){
		printf("\nFormato Argumentos: <Duracion> <Numero Inicial Peces Pequeños> <Numero Inicial Peces Grandes>\n");
		exit(1);
	}else{
		sscanf(argv[1], "%lf", &_tfin);
		sscanf(argv[2], "%lf", &__x[0]);
		sscanf(argv[3], "%lf", &__y[0]);
	}
	double inc = _inc_t*_tcom;
	double *aux = new double[2];
	if(!graficar){	
		cout << "\tt\tx\ty" << endl;
		cout << "\t" << _tinicio << "\t" << __x[0]<< "\t" << __y[0]<< endl;
	}
	for(double t = _tinicio ; t < (_tfin-10e-8) ; t += inc ){
		resolver(t, t + inc, aux);
		if(!graficar){
			cout << "\t" << aux[0] << "\t" << aux[1] << "\t" << aux[2] << endl;
		}
	}
}
