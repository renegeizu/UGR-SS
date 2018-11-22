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
	g++ -std=c++11 $1 $codigos/modSimul_incFijo.cpp -o $ejecutables/modSimul_incFijo
	g++ -std=c++11 $1 $codigos/modSimul_incVariable.cpp -o $ejecutables/modSimul_incVariable
	g++ -std=c++11 $1 $codigos/puerto.cpp -o $ejecutables/puerto

	echo -e "${orange}Fin de la compilacion${nocolor}"

	# Le pasamos valores al modelo y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos de los modelos fijo y variable...${nocolor}"

	tlleg=(0.15 9 540)
	tserv=(0.1 6 360)
	for ((A=0;A<3;A=A+1))
	do
		$ejecutables/modSimul_incFijo ${tlleg[$A]} ${tserv[$A]} 10000 10000 >> $datos/modSimul_incFijo_${tlleg[$A]}-${tserv[$A]}.dat
		$ejecutables/modSimul_incVariable ${tlleg[$A]} ${tserv[$A]} 10000 10000 >> $datos/modSimul_incVariable_${tlleg[$A]}-${tserv[$A]}.dat
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

	# Creacion de las Graficas	

	echo -e "${gray}Finalizado${nocolor}"
fi
