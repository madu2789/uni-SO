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
#define MAX_TRAMA 11
#define MAX 64
#define ERROR -1

// Includes propios
#include "socketClient.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones
int clientConnect (int nPort);

#endif