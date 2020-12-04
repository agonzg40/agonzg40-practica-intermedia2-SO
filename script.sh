#!/bin/bash 


while true $n
do

echo Menu:
echo 1-Ver el codigo del programa
echo 2-Compilar archivo
echo 3-Ejecutar el programa
echo 4-Salir

read n


case $n in

	1) cat intermedia.c;;

	2) gcc intermedia.c -o intermedia;;

	3) 
	variable=true;
	if [ -f intermedia ];
	then
		echo Existe el archivo, procedemos a la ejecucion
	else
		echo No existe el archivo, seleccione la opcion 2
		variable=false;
	fi
	if [ "$variable" == true ];
	then
			echo Introduce el numero de pacientes

			read i

			./intermedia  $i


	
		
	fi;;
	
	4) exit 0;;

esac

done
