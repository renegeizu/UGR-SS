#!/bin/bash

#uso: ./Script.sh -eficiencia
#ejemplo: ./Script.sh -O2

#Colores para los mensajes por terminal
nocolor='\033[0m'
color='\033[0;34m'

#Pedimos por parametro la eficiencia de compilacion
if [ -z "$1" ]
then
	echo -e "${color}Faltan el parametro de eficiencia en la compilacion${nocolor}"
else
	#Guardamos los directorios necesarios
	codigos=$(pwd)/Codigos
	ejecutables=$(pwd)/Ejecutables
	datos=$(pwd)/Datos
	graficas=$(pwd)/Graficas

	#Crear directorios necesarios
	if [ -d $ejecutables ];
	then
		echo -e "${color}Directorio 'Ejecutables' listo${nocolor}"
	else
		mkdir $ejecutables
		echo -e "${color}Directorio 'Ejecutables' creado${nocolor}"
	fi

	if [ -d $datos ];
	then
		echo -e "${color}Directorio 'Datos' listo${nocolor}"
	else
		mkdir $datos
		echo -e "${color}Directorio 'Datos' creado${nocolor}"
	fi

	echo -e "${color}Compilando codigos...${nocolor}"

	#Compilamos todos los .c
	g++ -std=c++11 $1 $codigos/MonteCarlo.cpp -o $ejecutables/MonteCarlo
	g++ -std=c++11 $1 $codigos/Discreto.cpp -o $ejecutables/Discreto
	g++ -std=c++11 $1 $codigos/Continuo.C -o $ejecutables/Continuo

	echo -e "${color}Fin de la compilacion${nocolor}"

	#Le pasamos valores al modelo de MonteCarlo y recogemos la informacion en .dat
	echo -e "${color}Obteniendo datos del modelo de MonteCarlo...${nocolor}"

	for ((N=0;N<100;N=N+1))
	do
		$ejecutables/MonteCarlo 100000 100 $N 0.9 0 >> $datos/MonteCarlo_Vista.dat
		$ejecutables/MonteCarlo 100000 100 2 $N 1 >> $datos/MonteCarlo_Prob.dat
		$ejecutables/MonteCarlo 100000 100 $N $N 1 >> $datos/MonteCarlo_VistaProb.dat
	done

	echo -e "${color}Finalizado${nocolor}"

	#Le pasamos valores al modelo Discreto y recogemos la informacion en .dat
	#echo -e "${color}Obteniendo datos del modelo Discreto...${nocolor}"

	#$ejecutables/Discreto 5 3 15 30 20 365 1 >> $datos/Discreto_RepSim.dat
	#for ((N=1;N<201;N=N+1))
	#do
	#	$ejecutables/Discreto 5 3 15 30 20 365 $(($N * 5)) >> $datos/Discreto_RepSim.dat
	#done

	#echo -e "${color}Finalizado${nocolor}"

	#Crear directorios necesarios
	if [ -d $graficas ];
	then
		echo -e "${color}Directorio 'Graficas' listo${nocolor}"
	else
		mkdir $graficas
		echo -e "${color}Directorio 'Graficas' creado${nocolor}"
	fi

	#Lanzamos gnuplot para crear las graficas
	echo -e "${color}Creando graficas...${nocolor}"

	gnuplot -e "plot '$datos/MonteCarlo_Vista.dat' using 3:6 title 'MonteCarlo - Parametro: Vista' with lines; set terminal png; set output '$graficas/MonteCarlo_Vista.png'; replot"
	gnuplot -e "plot '$datos/MonteCarlo_Prob.dat' using 4:6 title 'MonteCarlo - Parametro: Prob' with lines; set terminal png; set output '$graficas/MonteCarlo_Prob.png'; replot"
	#gnuplot -e "splot '$datos/MonteCarlo_VistaProb.dat' using 3:4:6 title 'MonteCarlo - Parametro: Vista y Prob' with lines; set terminal png; set output '$graficas/MonteCarlo_VistaProb.png'; replot"

	#gnuplot -e "plot '$datos/Discreto_RepSim.dat' title 'Discreto - Parametro: Reparaciones y Simulaciones' using 1: with points; set terminal png; set output '$graficas/Discreto_RepSim.png'; replot"

	echo -e "${color}Finalizado${nocolor}"
fi
