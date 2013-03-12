/* **********************************************
*
* Fitxer de capçalera de SocketsClient
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __SOCKETSER_H__
#define __SOCKETSER_H__

// Includes propios
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Defines
#define MAX_TRAMA 115
#define MAX 64
#define ERROR -1

// Includes propios
#include "socketServer.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones
void sayHello (char sTrama[MAX_TRAMA]);
int ServerConection (int nPort);

#endif