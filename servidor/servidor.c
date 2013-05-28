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
	int nSocketFD = 0;
	int bSincro = 0;
	int bSincroPetition = 0;

	char sServer[11];
	char sDirPath[MAX];
	char sLoginUser[8];
	char sMyLog[20];

	struct node *LinkedList;
	struct node *LinkedListToTx;

	//INITS
	//Demanem memoria per la LL
	LinkedList = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->next = NULL;

	//Memoria per Linked list que contindra els elements a Tx
	LinkedListToTx = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedListToTx->sName, "fantasma");
	LinkedListToTx->next = NULL;

	memset(sServer, '\0', 11);
	memset(sDirPath, '\0', MAX);
	memset(sLoginUser, '\0', 7);
	memset(sMyLog, '\0', 20);

	//Crear/Obrir fitxer de Log
	strcpy (sMyLog, "LSBox_svr.log.html");
	sMyLog[strlen(sMyLog)] = '\0';
	createLog (sMyLog);

	//Llegir "config.dat"
	nPort = getConfigInfo (sServer, sDirPath);

	//Socket peticio connexio	//que aqui em retoni tambe el login del client per posarlo a la sincro
	nSocketFD = ServerConection (nPort, sLoginUser);

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog);


	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		bSincro = 0;
		bSincroPetition = 0;

		display (LinkedList);
		bSincro = checkRootFiles (sDirPath, LinkedList, LinkedListToTx, sMyLog);
		bSincroPetition = receiveClientSincro (nSocketFD);

		if ( bSincro || bSincroPetition) {
			//Sincronitzacio
			startSincro (nSocketFD, sLoginUser);
			//Agafa la info procedent de Client
			getSincroInfo (nSocketFD, LinkedList, LinkedListToTx);
			
			//Crearia el thread per la TX de Info
			transferContent (nSocketFD, sDirPath, sLoginUser, LinkedListToTx);

		} else {
			write (nSocketFD, "init", 4);
		}

		sleep (5);

	}


	return 0;
}
