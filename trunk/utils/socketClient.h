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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//Defines
#define MAX_TRAMA 115
#define MAX 64
#define ERROR -1

// Includes propios
#include "socketClient.h"
#include "log.h"
#include "facadeLL.h"

// Definicion de constantes

// Definicion de tipos propios



//Funciones
int clientConnect (int nPort, char sUser[7], char sPwd[32]);
int checkTrama (char sTrama[MAX_TRAMA],char sUser[7], int nType);
void petitionConection (char sTrama[MAX_TRAMA], char sUser[7], char sPwd[20], int nTipusTrama);

#endif