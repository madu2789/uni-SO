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
#include "log.h"
#include "facadeLL.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones
void creaTrama (char sTrama[MAX_TRAMA], char sLoginOrigen[7], char sLoginDesti[7], int nType);
int getNumUsers(int nFdIn);
int checkUserInfo (int nFdIn, char sUser[7], char sPswd[32]);
int checkAuthentication (char sUser[32], char sPswd[32]);
int checkTrama (char sTrama[MAX_TRAMA], char sLoginOrigen[8], char sLoginDesti[8], char sPwd[33], int nType);
int ServerConection (int nPort, char sLoginDesti[8]);

#endif
