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
#include "../utils/log.h"
#include "../utils/facadeLL.h"
#include "../utils/socketServer.h"

// Definicion de constantes
#define ERROR -1
#define OK 0
#define MAX 64


// Definicion de tipos propios


//Funciones
int main();
int getConfigInfo (char sServer[11], char sDirPath[MAX]);
int ReadDir (char sDirPath[MAX]);
int checkUser (char sSckUser[32], char sSckPswd[32]);
static int triar (const struct dirent *arg);
int initLinkedList (char sDirPath[MAX]) ;
void checkRootFiles (char sDirPath[MAX]);


#endif