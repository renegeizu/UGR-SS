#!/bin/bash

# uso: ./Script.sh -eficiencia
# ejemplo: ./Script.sh -O2

# Colores para los mensajes por terminal
nocolor='\033[0m'
red='\033[0;31m'
green='\033[0;32m'
orange='\033[0;33m'
blue='\033[0;34m'
purple='\033[0;35m'
gray='\033[1;30m'
yellow='\033[1;33m'

# Pedimos por parametro la eficiencia de compilacion
if [ -z "$1" ]
then
	echo -e "${red}Faltan el parametro de eficiencia en la compilacion${nocolor}"
else
	# Guardamos los directorios necesarios
	codigos=$(pwd)/Codigos
	ejecutables=$(pwd)/Ejecutables
	datos=$(pwd)/Datos
	graficas=$(pwd)/Graficas

	# Crear directorios necesarios
	if [ -d $ejecutables ];
	then
		echo -e "${blue}Directorio 'Ejecutables' listo${nocolor}"
		rm -Rf $ejecutables/*
	else
		mkdir $ejecutables
		echo -e "${blue}Directorio 'Ejecutables' creado${nocolor}"
	fi

	if [ -d $datos ];
	then
		echo -e "${blue}Directorio 'Datos' listo${nocolor}"
		rm -Rf $datos/*
	else
		mkdir $datos
		echo -e "${blue}Directorio 'Datos' creado${nocolor}"
	fi

	echo -e "${orange}Compilando codigos...${nocolor}"

	# Compilamos todos los .cpp y .C
	g++ -std=c++11 $1 $codigos/montecarlo_v1.cpp -o $ejecutables/MonteCarlo_V1

	echo -e "${orange}Fin de la compilacion${nocolor}"

	# Le pasamos valores al modelo de MonteCarlo y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo de MonteCarlo...${nocolor}"

	numSimulaciones=(100 1000 5000 10000 100000)
	for A in "${numSimulaciones[@]}"
	do
		for ((B=1;B<4;B=B+1))
		do
			$ejecutables/MonteCarlo_V1 10 1 $A $B >> $datos/MonteCarlo_V1_X10Y1_$A.$B.dat
			$ejecutables/MonteCarlo_V1 10 5 $A $B >> $datos/MonteCarlo_V1_X10Y5_$A.$B.dat
			$ejecutables/MonteCarlo_V1 10 10 $A $B >> $datos/MonteCarlo_V1_X10Y10_$A.$B.dat
		done
	done

	echo -e "${purple}Finalizado${nocolor}"

	# Crear directorios necesarios
	if [ -d $graficas ];
	then
		echo -e "${blue}Directorio 'Graficas' listo${nocolor}"
		rm -Rf $graficas/*
	else
		mkdir $graficas
		echo -e "${blue}Directorio 'Graficas' creado${nocolor}"
	fi

	# Lanzamos gnuplot para crear las graficas
	echo -e "${gray}Creando graficas...${nocolor}"

	for C in "${numSimulaciones[@]}"
	do
		for ((D=1;D<4;D=D+1))
		do
			gnuplot -e "plot '$datos/MonteCarlo_V1_X10Y1_$C.$D.dat' using 1:2 title 'MonteCarlo_V1 - X(10) Y(1) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/MonteCarlo_V1_X10Y1_$C.$D.png'; replot"
			gnuplot -e "plot '$datos/MonteCarlo_V1_X10Y1_$C.$D.dat' using 1:2 title 'MonteCarlo_V1 - X(10) Y(5) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/MonteCarlo_V1_X10Y5_$C.$D.png'; replot"
			gnuplot -e "plot '$datos/MonteCarlo_V1_X10Y1_$C.$D.dat' using 1:2 title 'MonteCarlo_V1 - X(10) Y(10) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/MonteCarlo_V1_X10Y10_$C.$D.png'; replot"
		done
	done

	echo -e "${gray}Finalizado${nocolor}"
fi
