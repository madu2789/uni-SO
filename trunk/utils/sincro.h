/* **********************************************
*
* Fitxer de capçalera de SocketsClient
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __SOCKETCLI_H__
#define __SOCKETCLI_H__

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
#include "sincro.h"
#include "socketServer.h"
#include "socketClient.h"
#include "facadeLL.h"
#include <stdlib.h>

// Definicion de constantes

// Definicion de tipos propios

//Funciones
int startSincro (int nFdIn, char sLoginDesti[7]);
int receiveServerSincro (int nFdIn, char sTrama[MAX_TRAMA], char sLoginOrigen[7], struct node *LinkedList);
void setSincroInfo (int nFdIn, char sLoginOrigen[7], struct node *LinkedList);
void getSincroInfo (int nFdIn, struct node *LinkedList, struct node *LinkedListToTx);
int ParserBucles (char Frase[50], char sName[24],	char sData[24]);

#endif
