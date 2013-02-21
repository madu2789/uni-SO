/* **********************************************
*
* Fitxer de capçalera de Servidor
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __SERVIDOR_H__
#define __SERVIDOR_H__


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
#include <signal.h>

// Includes propios
#include "../utils/facadeLL.h"

// Definicion de constantes
#define ERROR -1
#define OK 0
#define MAX 64


// Definicion de tipos propios


//Funciones
int main();
void getConfigInfo ();
static int triar (const struct dirent *arg);
int initLinkedList ();
void checkRootFiles ();


#endif