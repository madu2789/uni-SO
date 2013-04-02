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
#include "log.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones
int createLog (char sIdLog[10]);
int writeLog (char sIdLog[10], char sFunction[32], int bCorrect);

#endif