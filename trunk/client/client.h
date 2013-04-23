/* **********************************************
*
* Fitxer de capçalera de Client
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __CLIENT_H__
#define __CLIENT_H__


// Includes del Sistema
#include <unistd.h>			// write i read
#include <stdio.h>			// sprintf
#include <stdlib.h>
#include <string.h>			// strlen, bzero
#include <dirent.h>			// opendir, readdir...
#include <sys/stat.h>		//stat, obtenir data modificacio i nSize
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

// Includes propios
#include "../utils/log.h"
#include "../utils/socketClient.h"
#include "../utils/facadeLL.h"
#include "../utils/md5.h"

// Definicion de constantes
#define ERROR -1
#define OK 0
#define MAX 64


// Definicion de tipos propios


//Funciones
int main();
void loginUser (char sLogin[MAX], char sPswd[MAX]);
int getConfigInfo (char sServer[11], char sDirPath[MAX]);
int ReadDir (char sDirPath[MAX]);
static int triar (const struct dirent *arg);
void conversorTipus (char sTipus[30], int nToConvert);
int initLinkedList (char sDirPath[MAX], struct node *LinkedList);
void checkRootFiles (char sDirPath[MAX], int nLLTotalFiles, struct node *LinkedList);


#endif