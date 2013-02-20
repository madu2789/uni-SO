/* **********************************************
*
* Fitxer de capçalera de FaçadeLL
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __FACADELL_H__
#define __FACADELL_H__


// Includes del Sistema
#include <unistd.h>			// write i read
#include <stdio.h>			// sprintf
#include <stdlib.h>
#include <string.h>			// strlen, bzero
#include <dirent.h>			// opendir, readdir...
#include <sys/stat.h>		//stat, obtenir data modificacio
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>


// Includes propios

// Definicion de constantes

// Definicion de tipos propios

//Funciones
void addToLL (int nTotalFiles);
void updateToLL (int i, char sLLDate[30]);
void removeToLL ( int nTotalFiles, int nLLTotalFiles);

#endif