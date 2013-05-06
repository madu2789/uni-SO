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
	int nSocketFD = 0;
	char sServer[11];
	char sDirPath[MAX];
	char sMyLog[20];

	struct node *LinkedList;
	struct node *LinkedListToTx;

	//Init LL posant tots els ele. trobats al directori root
	LinkedList = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->nSize = 0;
	LinkedList->next = NULL;

	//Linked list que contindra els elements a Tx
	LinkedListToTx = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedListToTx->sName, "fantasma");
	LinkedListToTx->nSize = 0;
	LinkedListToTx->next = NULL;

	//Crear/Obrir fitxer de Log
	strcpy (sMyLog, "LSBox_svr.log.html");
	sMyLog[strlen(sMyLog)] = '\0';
	createLog (sMyLog);

	//Llegir "config.dat"
	nPort = getConfigInfo (sServer, sDirPath);

	//Socket peticio connexio
	nSocketFD = ServerConection (nPort);

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList, sMyLog);

	//Sincronitzacio - a manija
	startSincro (nSocketFD, "madu123");

	//Agafa la info procedent de Client
	getSincroInfo(nSocketFD, LinkedList, LinkedListToTx);

	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		nLLTotalFiles = display(LinkedList);
		checkRootFiles (sDirPath, nLLTotalFiles, LinkedList, sMyLog);
		sleep (5);
	}


	return 0;
}
