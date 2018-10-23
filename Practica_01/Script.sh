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

	for ((A=0;A<100;A=A+1))
	do
		$ejecutables/MonteCarlo 100000 100 $A 0.9 0 >> $datos/MonteCarlo_Vista.dat
		$ejecutables/MonteCarlo 100000 100 2 $A 1 >> $datos/MonteCarlo_Prob.dat
		$ejecutables/MonteCarlo 100000 100 $A $A 1 >> $datos/MonteCarlo_VistaProb.dat
	done

	echo -e "${purple}Finalizado${nocolor}"

	# Le pasamos valores al modelo Discreto y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo Discreto...${nocolor}"

	numSimulaciones=(1 5 10 50 100 500 1000)
	numRadares=5
	numRepuestos=$(( $numRadares * 3 ))
	for B in "${numSimulaciones[@]}"
	do
		for ((C=0;C<numRepuestos;C=C+1))
		do
			$ejecutables/Discreto $numRadares $C 15 30 20 365 $B >> $datos/Discreto_RepFallos$B.dat
		done
		for ((D=0;D<366;D=D+1))
		do
			$ejecutables/Discreto $numRadares 1 15 30 $D 365 $B >> $datos/Discreto_RobFallos$B.dat
		done
	done

	echo -e "${purple}Finalizado${nocolor}"

	# Le pasamos valores al modelo Continuo y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo Continuo...${nocolor}"

	numDiasPesca=50
	diasSimulacion=3650
	numPecesGrandes=(1 10 100 1000 10000 100000 1000000)
	porcentajePesca=(10 30 50 70 90)
	for E in "${numPecesGrandes[@]}"
	do
		numPecesPeq=$(( $E * 100 ))
		$ejecutables/Continuo $diasSimulacion $numPecesPeq $E -1 0 >> $datos/Continuo_$E.dat
		for F in "${porcentajePesca[@]}"
		do
			$ejecutables/Continuo $diasSimulacion $numPecesPeq $E $numDiasPesca $F >> $datos/Continuo_$E$F.dat
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

	for Z in "${numSimulaciones[@]}"
	do
		gnuplot -e "plot '$datos/Discreto_RepFallos$Z.dat' using 1:2 title 'Discreto - Parametros: Repuestos y Media Fallos' with lines; set terminal png; set output '$graficas/Discreto_ResFallos$Z.dat.png'; replot"
		gnuplot -e "plot '$datos/Discreto_RobFallos$Z.dat' using 3:2 title 'Discreto - Parametros: Robustez y Media Fallos' with lines; set terminal png; set output '$graficas/Discreto_RobFallos$Z.dat.png'; replot"
	done

	for Y in "${numPecesGrandes[@]}"
	do
		gnuplot -e "set logscale x; plot '$datos/Continuo_$Y.dat' using 1:2 title 'Continuo - Dias y Peces Pequeños' with lines; set terminal png; set output '$graficas/Continuo_P$Y.dat.png'; replot"
		gnuplot -e "set logscale x; plot '$datos/Continuo_$Y.dat' using 1:3 title 'Continuo - Dias y Peces Grandes' with lines; set terminal png; set output '$graficas/Continuo_G$Y.dat.png'; replot"
		for X in "${porcentajePesca[@]}"
		do
			gnuplot -e "set logscale x; plot '$datos/Continuo_$Y$X.dat' using 1:2 title 'Continuo - Dias y Peces Pequeños (Pesca)' with lines; set terminal png; set output '$graficas/Continuo_P$Y$X.dat.png'; replot"
			gnuplot -e "set logscale x; plot '$datos/Continuo_$Y$X.dat' using 1:3 title 'Continuo - Dias y Peces Grandes (Pesca)' with lines; set terminal png; set output '$graficas/Continuo_G$Y$X.dat.png'; replot"
		done
	done

	echo -e "${gray}Finalizado${nocolor}"
fi
