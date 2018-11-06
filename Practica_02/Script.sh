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
		mkdir $datos/V1
		echo -e "${blue}Directorio 'Datos/V1' listo${nocolor}"
		mkdir $datos/V2
		echo -e "${blue}Directorio 'Datos/V2' listo${nocolor}"
		mkdir $datos/V3
		echo -e "${blue}Directorio 'Datos/V3' listo${nocolor}"
		mkdir $datos/V4
		echo -e "${blue}Directorio 'Datos/V4' listo${nocolor}"
		mkdir $datos/V5
		echo -e "${blue}Directorio 'Datos/V5' listo${nocolor}"
	else
		mkdir $datos
		echo -e "${blue}Directorio 'Datos' creado${nocolor}"
		mkdir $datos/V1
		echo -e "${blue}Directorio 'Datos/V1' creado${nocolor}"
		mkdir $datos/V2
		echo -e "${blue}Directorio 'Datos/V2' creado${nocolor}"
		mkdir $datos/V3
		echo -e "${blue}Directorio 'Datos/V3' listo${nocolor}"
		mkdir $datos/V4
		echo -e "${blue}Directorio 'Datos/V4' listo${nocolor}"
		mkdir $datos/V5
		echo -e "${blue}Directorio 'Datos/V5' listo${nocolor}"
	fi

	echo -e "${orange}Compilando codigos...${nocolor}"

	# Compilamos todos los .cpp y .C
	g++ -std=c++11 $1 $codigos/montecarlo_v1.cpp -o $ejecutables/MonteCarlo_V1
	g++ -std=c++11 $1 $codigos/montecarlo_v2.cpp -o $ejecutables/MonteCarlo_V2
	g++ -std=c++11 $1 $codigos/montecarlo_v3.cpp -o $ejecutables/MonteCarlo_V3
	g++ -std=c++11 $1 $codigos/montecarlo_v3_1.cpp -o $ejecutables/MonteCarlo_V3_1
	g++ -std=c++11 $1 $codigos/montecarlo_v4.cpp -o $ejecutables/MonteCarlo_V4
	g++ -std=c++11 $1 $codigos/montecarlo_v5.cpp -o $ejecutables/MonteCarlo_V5

	echo -e "${orange}Fin de la compilacion${nocolor}"

	# Le pasamos valores al modelo de MonteCarlo y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo de MonteCarlo V1 y V2...${nocolor}"

	numSimulaciones=(100 1000 5000 10000 100000)
	for A in "${numSimulaciones[@]}"
	do
		for ((B=1;B<4;B=B+1))
		do
			$ejecutables/MonteCarlo_V1 10 1 $A $B >> $datos/V1/MonteCarlo_V1_X10Y1_$A-$B.dat
			$ejecutables/MonteCarlo_V1 10 5 $A $B >> $datos/V1/MonteCarlo_V1_X10Y5_$A-$B.dat
			$ejecutables/MonteCarlo_V1 10 10 $A $B >> $datos/V1/MonteCarlo_V1_X10Y10_$A-$B.dat

			$ejecutables/MonteCarlo_V2 10 1 $A $B >> $datos/V2/MonteCarlo_V2_X10Y1_$A-$B.dat
			$ejecutables/MonteCarlo_V2 10 5 $A $B >> $datos/V2/MonteCarlo_V2_X10Y5_$A-$B.dat
			$ejecutables/MonteCarlo_V2 10 10 $A $B >> $datos/V2/MonteCarlo_V2_X10Y10_$A-$B.dat
		done
	done

	echo -e "${purple}Finalizado${nocolor}"

	# Le pasamos valores al modelo de MonteCarlo y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo de MonteCarlo V3...${nocolor}"

	$ejecutables/MonteCarlo_V3 100 10000 >> $datos/V3/ComparacionTiempos_V3.dat
	$ejecutables/MonteCarlo_V3_1 100 10000 >> $datos/V3/ComparacionTiempos_V3_1.dat
	$ejecutables/MonteCarlo_V4 100 100000 >> $datos/V4/ComparacionTiempos_V4.dat
	$ejecutables/MonteCarlo_V5 100 100000 >> $datos/V5/ComparacionTiempos_V5.dat

	echo -e "${purple}Finalizado${nocolor}"

	# Crear directorios necesarios
	if [ -d $graficas ];
	then
		echo -e "${blue}Directorio 'Graficas' listo${nocolor}"
		rm -Rf $graficas/*
		mkdir $graficas/V1
		echo -e "${blue}Directorio 'Graficas/V1' listo${nocolor}"
		mkdir $graficas/V2
		echo -e "${blue}Directorio 'Graficas/V2' listo${nocolor}"
	else
		mkdir $graficas
		echo -e "${blue}Directorio 'Graficas' creado${nocolor}"
		mkdir $graficas/V1
		echo -e "${blue}Directorio 'Graficas/V1' creado${nocolor}"
		mkdir $graficas/V2
		echo -e "${blue}Directorio 'Graficas/V2' creado${nocolor}"
	fi

	# Lanzamos gnuplot para crear las graficas
	echo -e "${gray}Creando graficas...${nocolor}"

	for C in "${numSimulaciones[@]}"
	do
		for ((D=1;D<4;D=D+1))
		do
			gnuplot -e "plot '$datos/V1/MonteCarlo_V1_X10Y1_$C-$D.dat' using 1:2 title 'MonteCarlo_V1 - X(10) Y(1) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/V1/MonteCarlo_V1_X10Y1_$C-$D.png'; replot"
			gnuplot -e "plot '$datos/V1/MonteCarlo_V1_X10Y5_$C-$D.dat' using 1:2 title 'MonteCarlo_V1 - X(10) Y(5) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/V1/MonteCarlo_V1_X10Y5_$C-$D.png'; replot"
			gnuplot -e "plot '$datos/V1/MonteCarlo_V1_X10Y10_$C-$D.dat' using 1:2 title 'MonteCarlo_V1 - X(10) Y(10) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/V1/MonteCarlo_V1_X10Y10_$C-$D.png'; replot"

			gnuplot -e "plot '$datos/V2/MonteCarlo_V2_X10Y1_$C-$D.dat' using 1:2 title 'MonteCarlo_V2 - X(10) Y(1) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/V2/MonteCarlo_V2_X10Y1_$C-$D.png'; replot"
			gnuplot -e "plot '$datos/V2/MonteCarlo_V2_X10Y5_$C-$D.dat' using 1:2 title 'MonteCarlo_V2 - X(10) Y(5) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/V2/MonteCarlo_V2_X10Y5_$C-$D.png'; replot"
			gnuplot -e "plot '$datos/V2/MonteCarlo_V2_X10Y10_$C-$D.dat' using 1:2 title 'MonteCarlo_V2 - X(10) Y(10) - Veces: $C - Apartado $D' with lines; set terminal png; set output '$graficas/V2/MonteCarlo_V2_X10Y10_$C-$D.png'; replot"
		done
	done

	echo -e "${gray}Finalizado${nocolor}"
fi
