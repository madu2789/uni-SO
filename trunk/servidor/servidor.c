/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"




/**
 * main general
 */
int main () {

	int nPort = 0;
	int nLLTotalFiles = 0;
	char sServer[11];
	char sDirPath[MAX];

	struct node *LinkedList;

	//Crear/Obrir fitxer de Log
	createLog ("LSBox_svr.log.html");

	//Llegir "config.dat"
	nPort = getConfigInfo (sServer, sDirPath);

	//Socket peticio connexio
	ServerConection (nPort);

	//Init LL posant tots els ele. trobats al directori root
	LinkedList = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->nSize = 0;
	LinkedList->next = NULL;

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList);

	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		nLLTotalFiles = display(LinkedList);
		checkRootFiles (sDirPath, nLLTotalFiles, LinkedList);
		sleep (5);
	}

	//display(p);

	return 0;
}