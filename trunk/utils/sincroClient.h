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
#include "sincroClient.h"
#include "socketServer.h"
#include "socketClient.h"
#include "facadeLL.h"
#include "transfer.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones

int receiveServerSincro (int nFdIn, char sLoginOrigen[7], char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx);
void setSincroInfo (int nFdIn, char sLoginOrigen[7], struct node *LinkedList);
void pleaseSincro (int nFdIn, char sLoginOrigen[7]);

#endif
