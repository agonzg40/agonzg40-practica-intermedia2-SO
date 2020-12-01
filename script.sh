#!/bin/bash 


while true $n
do

echo Menu:
echo 1-Ver el codigo del programa
echo 2-Compilar archivo
echo 3-Ejecutar el programa
echo 0-Salir

read n


case $n in

	1) cat intermedia.c;;

	2) gcc intermedia.c -o intermedia;;

	3) echo Introduce el numero de pinches

		read i

		./intermedia  $i;;

	0) exit 0;;

esac

done
