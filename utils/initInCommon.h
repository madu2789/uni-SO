/* **********************************************
*
* Fitxer de capçalera de SocketsClient
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __INITINCOMMON_H__
#define __INITINCOMMON_H__

// Includes propios
#define _GNU_SOURCE

#include <fcntl.h>	//pels o_create ...
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

//Defines
#define MAX_TRAMA 115
#define MAX 64
#define ERROR -1

// Includes propios
#include "initInCommon.h"
#include "../utils/facadeLL.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones
static int triar (const struct dirent *arg);
int getConfigInfo (char sServer[11], char sDirPath[MAX]);
int initLinkedList (char sDirPath[MAX], struct node *LinkedList);
void checkRootFiles (char sDirPath[MAX], int nLLTotalFiles, struct node *LinkedList);
int ReadDir (int bIsNull) ;

#endif