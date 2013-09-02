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
#include <pthread.h>

// Includes propios
#include "../utils/sincroServer.h"
#include "../utils/initInCommon.h"
#include "../utils/log.h"
#include "../utils/facadeLL.h"
#include "../utils/socketServer.h"
#include "../utils/transfer.h"


// Definicion de constantes
#define ERROR -1
#define OK 0
#define MAX 64
#define MAX_LONG 128


// Definicion de tipos propios


//Funciones
int main();



#endif
