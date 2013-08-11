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
#include "log.h"
#include "facadeLL.h"
#include "../servidor/servidor.h"

// Definicion de constantes

// Definicion de tipos propios

//Funciones
void creaTramaServidor (char sTrama[MAX_TRAMA], char sLoginOrigen[7], char sLoginDesti[7], int nType);
int getNumUsers(int nFdIn);
int checkUserInfo (int nFdIn, char sUser[7], char sPswd[32]);
int checkAuthentication (char sUser[32], char sPswd[32]);
int checkTramaServidor (char sTrama[MAX_TRAMA], char sLoginOrigen[8], char sLoginDesti[8], char sPwd[33], int nType);
int socketConnectionServidor (int nPort);
int ServerConection (int nPort, int gnSocketFD, char sLoginDesti[8]);
int enviaPort (int nFdIn, int nPort, char sLoginDesti[7], char sLoginOrigen[7]);

#endif
