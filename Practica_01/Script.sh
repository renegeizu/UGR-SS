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
	g++ -std=c++11 $1 $codigos/MonteCarlo.cpp -o $ejecutables/MonteCarlo
	g++ -std=c++11 $1 $codigos/Discreto.cpp -o $ejecutables/Discreto
	g++ -std=c++11 $1 $codigos/Continuo.C -o $ejecutables/Continuo

	echo -e "${orange}Fin de la compilacion${nocolor}"

	# Le pasamos valores al modelo de MonteCarlo y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo de MonteCarlo...${nocolor}"

	for ((N=0;N<101;N=N+1))
	do
		$ejecutables/MonteCarlo 100000 100 $N 0.9 0 >> $datos/MonteCarlo_Vista.dat
		$ejecutables/MonteCarlo 100000 100 2 $N 1 >> $datos/MonteCarlo_Prob.dat
		$ejecutables/MonteCarlo 100000 100 $N $N 1 >> $datos/MonteCarlo_VistaProb.dat
	done

	echo -e "${purple}Finalizado${nocolor}"

	# Le pasamos valores al modelo Discreto y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo Discreto...${nocolor}"

	numSimulaciones=(1 5 10 50 100 500 1000)
	numRadares=5
	numRepuestos=15
	for N in "${numSimulaciones[@]}"
	do
		for ((M=0;M<numRepuestos;M=M+1))
		do
			$ejecutables/Discreto $numRadares $M 15 30 20 365 $N >> $datos/Discreto_RepFallos$N.dat
		done
		for ((M=0;M<366;M=M+1))
		do
			$ejecutables/Discreto $numRadares 1 15 30 $M 365 $N >> $datos/Discreto_RobFallos$N.dat
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

	gnuplot -e "plot '$datos/MonteCarlo_Vista.dat' using 3:6 title 'MonteCarlo - Parametro: Vista' with lines; set terminal png; set output '$graficas/MonteCarlo_Vista.png'; replot"
	gnuplot -e "plot '$datos/MonteCarlo_Prob.dat' using 4:6 title 'MonteCarlo - Parametro: Prob' with lines; set terminal png; set output '$graficas/MonteCarlo_Prob.png'; replot"
	gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/MonteCarlo_VistaProb.dat' using 3:4:6 title 'MonteCarlo - Parametro: Vista y Prob' with lines; set terminal png; set output '$graficas/MonteCarlo_VistaProb.png'; replot"

	for N in "${numSimulaciones[@]}"
	do
		gnuplot -e "plot '$datos/Discreto_RepFallos$N.dat' using 1:2 title 'Discreto - Parametros: Repuestos y Media Fallos' with lines; set terminal png; set output '$graficas/Discreto_ResFallos$N.dat.png'; replot"
		gnuplot -e "plot '$datos/Discreto_RobFallos$N.dat' using 3:2 title 'Discreto - Parametros: Robustez y Media Fallos' with lines; set terminal png; set output '$graficas/Discreto_RobFallos$N.dat.png'; replot"
	done

	echo -e "${gray}Finalizado${nocolor}"
fi
