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

	struct node *LinkedList;

	//Crear/Obrir fitxer de Log
	createLog ("LSBox_svr.log.html");

	//Llegir "config.dat"
	nPort = getConfigInfo (sServer, sDirPath);

	//Socket peticio connexio
	nSocketFD = ServerConection (nPort);

	printf("conectaat\n");

	//Init LL posant tots els ele. trobats al directori root
	LinkedList = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->nSize = 0;
	LinkedList->next = NULL;

printf("creada LL i ara lomplirem\n");

	//Init LL posant tots els ele. trobats al directori root
	//initLinkedList (sDirPath, LinkedList);

	printf("anem a sincro a socket: %d \n", nSocketFD);

	//Sincronitzacio - a manija
	startSincro (nSocketFD, "madu123");

	//Agafa la info procedent de Client
	getSincroInfo(nSocketFD, LinkedList);

	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		nLLTotalFiles = display(LinkedList);
		//checkRootFiles (sDirPath, nLLTotalFiles, LinkedList);
		sleep (5);
	}

	//display(p);

	return 0;
}