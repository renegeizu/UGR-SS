//Ejercicio 2.2 - Generadores Congruenciales

#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <vector>

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
int gen_cong_lin_entera(int a, int b, int m){
	vector<long> temp;
	long aux = 0;
	do{
		temp.push_back(aux);
		aux = (a*temp[temp.size()-1]+4321)%m;
	}while(find(temp.begin(), temp.end(), aux) == temp.end());
	return temp.size();
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real Artesanal - Float
  */
int gen_cong_lin_real_art_float(int a, int b, int m){
	vector<float> temp;
	float aux = 0;
	do{
		temp.push_back(aux);
		aux = (float)(a*temp[temp.size()-1]+b)/m;
		aux = (float)(aux-(int)aux)*m;
	}while(find(temp.begin(), temp.end(), aux) == temp.end());
	return temp.size();
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real Artesanal - Double
  */
int gen_cong_lin_real_art_double(int a, int b, int m){
	vector<double> temp;
	double aux = 0;
	do{
		temp.push_back(aux);
		aux = (double)(a*temp[temp.size()-1]+b)/m;
		aux = (double)(aux-(int)aux)*m;
	}while(find(temp.begin(), temp.end(), aux) == temp.end());
	return temp.size();
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real Artesanal Corregido - Int
  */
int gen_cong_lin_real_art_int(int a, int b, int m){
	vector<int> temp;
	float aux2 = 0;
	int aux = 0;
	do{
		temp.push_back(aux);
		aux2 = (float)(a*temp[temp.size()-1]+b)/m;
		aux2 = (float)(aux2-(int)aux2)*m;
		aux = (int)(aux2+0.5);
	}while(find(temp.begin(), temp.end(), aux) == temp.end());
	return temp.size();
}

/**
  * @brief Generador Congruencial Lineal - Aritmetica Real fmod - Double
  */
int gen_cong_lin_real_art_fmod(int a, int b, int m){
	vector<double> temp;
	double aux = 0;
	do{
		temp.push_back(aux);
		aux = (double)fmod((a*temp[temp.size()-1]+b),m);
	}while(find(temp.begin(), temp.end(), aux) == temp.end());
	return temp.size();
}

/**
  * @brief Funcion Principal
  */
int main(int argc, char *argv[]){
	int a1 = 2016, a2 = 2060, b12 = 4321, m12 = 10000;
	cout << "x(n+1) = (2061x(n)+4321) mod m  ->  Gen. Cong. Lin. Entera:           " << gen_cong_lin_entera(a1, b12, m12) << endl;
	cout << "x(n+1) = (2061x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Float:  " << gen_cong_lin_real_art_float(a1, b12, m12) << endl;
	cout << "x(n+1) = (2061x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Double: " << gen_cong_lin_real_art_double(a1, b12, m12) << endl;
	cout << "x(n+1) = (2061x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Int:    " << gen_cong_lin_real_art_int(a1, b12, m12) << endl;
	cout << "x(n+1) = (2061x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Fmod:   " << gen_cong_lin_real_art_fmod(a1, b12, m12) << endl;
	cout << endl;
	cout << "x(n+1) = (2060x(n)+4321) mod m  ->  Gen. Cong. Lin. Entera:           " << gen_cong_lin_entera(a2, b12, m12) << endl;
	cout << "x(n+1) = (2060x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Float:  " << gen_cong_lin_real_art_float(a2, b12, m12) << endl;
	cout << "x(n+1) = (2060x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Double: " << gen_cong_lin_real_art_double(a2, b12, m12) << endl;
	cout << "x(n+1) = (2060x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Int:    " << gen_cong_lin_real_art_int(a2, b12, m12) << endl;
	cout << "x(n+1) = (2060x(n)+4321) mod m  ->  Gen. Cong. Lin. Real Art. Fmod:   " << gen_cong_lin_real_art_fmod(a2, b12, m12) << endl;
	return 0;
}
