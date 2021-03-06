/* **********************************************
*
* Fitxer de capçalera de transfer
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __TRANSFER_H__
#define __TRANSFER_H__

// Includes propios
#define _GNU_SOURCE

#include <fcntl.h>	//pels o_create ...
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>



//Defines
#define MAX_TRAMA 115
#define MAX 64
#define ERROR -1

// Includes propios

#include "facadeLL.h"


// Definicion de constantes

// Definicion de tipos propios

//Funciones
int transferContent (int nFdIn, char sDirPath[MAX], char sUser[8], struct node *LinkedListToTx, char sMyLog[40], sem_t *semLL);
void receiveContent (int nFdIn, char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20], sem_t *semLL);
int openFile (char sDirPath[MAX], char sName[30], char sMyLog[40]);
int removeFile (char sDirPath[MAX], char sName[30]);
int createFile (char sDirPath[MAX], char sName[30]); 

#endif
