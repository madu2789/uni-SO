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
#include <semaphore.h>

//Defines
#define MAX_TRAMA 115
#define MAX 64
#define MAX_LONG 128
#define ERROR -1

// Includes propios
#include "facadeLL.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones
static int triar (const struct dirent *arg);
int readInt (int nFichero);
void readString (int nFichero, char **ppsString);
int getConfigInfo (char **psServer, char sDirPath[MAX_LONG]);
int initLinkedList (char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20]);
int checkRootFiles (char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20], sem_t *semLL);
int ReadDir (int bIsNull, char sMyLog[20]);
void getDateReal (char sRealDate[30], char sDirPath[MAX], char sName[30]);
int pathExists (char sDirPath[MAX], char sMyLog[40]);

#endif
