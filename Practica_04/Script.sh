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
	includes=$(pwd)/Includes

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
	g++ -std=c++11 $1 $codigos/modelDinamic.cpp -o $ejecutables/modelDinamic

	echo -e "${orange}Fin de la compilacion${nocolor}"

	# Le pasamos valores al modelo y recogemos la informacion en .dat
	echo -e "${purple}Obteniendo datos del modelo...${nocolor}"
		# Condiciones Iniciales
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 0 100 500 100 0 >> $datos/modelDinamic_RungeKutta_01.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 1 100 500 100 1 >> $datos/modelDinamic_Euler_01.dat
		# Incrementos
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 0 100 600 200 0 >> $datos/modelDinamic_RungeKutta_02.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 1 100 600 200 1 >> $datos/modelDinamic_Euler_02.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 0 100 600 100 0 >> $datos/modelDinamic_RungeKutta_03.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 1 100 600 100 1 >> $datos/modelDinamic_Euler_03.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 0 100 500 200 0 >> $datos/modelDinamic_RungeKutta_04.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 1 100 500 200 1 >> $datos/modelDinamic_Euler_04.dat
		# Decrementos
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 0 100 400 50 0 >> $datos/modelDinamic_RungeKutta_05.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 1 100 400 50 1 >> $datos/modelDinamic_Euler_05.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 0 100 400 100 0 >> $datos/modelDinamic_RungeKutta_06.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 1 100 400 100 1 >> $datos/modelDinamic_Euler_06.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 0 100 500 50 0 >> $datos/modelDinamic_RungeKutta_07.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.1 1 100 500 50 1 >> $datos/modelDinamic_Euler_07.dat
		#Modificaciones
		$ejecutables/modelDinamic 5 0.5 0.0004 0.2 0.1 0 100 500 100 0 >> $datos/modelDinamic_RungeKutta_08.dat
		$ejecutables/modelDinamic 5 0.5 0.0004 0.2 0.1 1 100 500 100 0 >> $datos/modelDinamic_Euler_08.dat
		$ejecutables/modelDinamic 10 0.05 0.0004 0.2 0.1 0 100 500 100 1 >> $datos/modelDinamic_RungeKutta_09.dat
		$ejecutables/modelDinamic 10 0.05 0.0004 0.2 0.1 1 100 500 100 1 >> $datos/modelDinamic_Euler_09.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.1 0.1 0 100 500 100 0 >> $datos/modelDinamic_RungeKutta_10.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.1 0.1 1 100 500 100 0 >> $datos/modelDinamic_Euler_10.dat
		$ejecutables/modelDinamic 5 0.05 0.004 0.2 0.1 0 100 500 100 1 >> $datos/modelDinamic_RungeKutta_11.dat
		$ejecutables/modelDinamic 5 0.05 0.004 0.2 0.1 1 100 500 100 1 >> $datos/modelDinamic_Euler_11.dat
		# Cambios en H
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.5 0 100 600 200 0 >> $datos/modelDinamic_RungeKutta_12.dat
		$ejecutables/modelDinamic 5 0.05 0.0004 0.2 0.01 1 100 600 200 1 >> $datos/modelDinamic_Euler_12.dat


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

		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_01.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_01.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_01.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_01.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_02.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_02.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_02.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_02.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_03.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_03.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_03.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_03.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_04.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_04.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_04.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_04.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_05.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_05.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_05.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_05.png'; replot"
	
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_06.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_06.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_06.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_06.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_07.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_07.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_07.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_07.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_08.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_08.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_08.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_08.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_09.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_09.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_09.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_09.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_10.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_10.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_10.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_10.png'; replot"

		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_11.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_11.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_11.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_11.png'; replot"
		
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_RungeKutta_12.dat' using 1:2:3 title 'M. Dinamic (RK) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_RungeKutta_12.png'; replot"
		gnuplot -e "set dgrid3d 30,30; set hidden3d; splot '$datos/modelDinamic_Euler_12.dat' using 1:2:3 title 'M. Dinamic (Euler) - Parametros: X Y Tiempo' with lines; set terminal png; set output '$graficas/modelDinamic_Euler_12.png'; replot"
	
	echo -e "${gray}Finalizado${nocolor}"
fi
