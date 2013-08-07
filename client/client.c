/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "client.h"

	char sDirPath[MAX];
	char sLogin[8];
	char sMyLog[20];

	struct node *LinkedList;
	struct node *LinkedListToTx;

/**
 * Demana al usuari sLogin i sPswd
 */
void loginUser (char sLogin[7], char sPswd[32]) {
	char sAux[MAX];
	char sPswdMd5[MAX];
	int nValid = 0;

	//Inicialitzacions
	bzero (sLogin, MAX);
	bzero (sPswd, MAX);

	//Demana nom d'usuari
	while (!nValid) {
		sprintf (sAux, "\n Username: ");
		write (1, sAux, strlen(sAux));
		read (0, sLogin, 7);
		if (strlen(sLogin) == 7) {
			nValid = 1;
		} else {
			sprintf (sAux, "\n Username  ha de ser de 7 caracters: ");
			write (1, sAux, strlen(sAux));
		}
	}
	sLogin[strlen(sLogin)] = '\0';
	nValid = 0;

	//Demana contrasenya
	while (!nValid) {
		sprintf (sAux, "\n Password: ");
		write (1, sAux, strlen(sAux));
		getchar();
		read (0, sPswd, 7);
		if (strlen(sPswd) == 7) {
			nValid = 1;
		} else {
			sprintf (sAux, "\n Password ha de ser de 7 caracters: ");
			write (1, sAux, strlen(sAux));
		}
	}

	//Md5
	stringToMd5 (sPswd, sPswdMd5);
	memset(sPswd, '\0', 32);
	strcpy (sPswd, sPswdMd5);
	sPswd[strlen(sPswd)] = '\0';

}



void * ThreadTx (void *arg){
	
	int nSocketFD = 0;
	char sTrama[MAX_TRAMA];
	memset (sTrama, '\0', MAX_TRAMA);

	//strcpy(sDirPath, "/home/madu/Repo_SO/trunk/client/root/");
	strcpy(sDirPath, "/users/home/alumnes/IS/is19445/SO/trunk/servidor/root/");
	
	int *nPortTx = (int *) arg;
	//Creem el socket
	nSocketFD = socketConnection (nPortTx);

	//Primer rebem info, despres enviem
	receiveContent (nSocketFD, sDirPath, LinkedList, LinkedListToTx, sMyLog);
	transferContent (nSocketFD, sDirPath, sLogin, LinkedListToTx, sMyLog);
	
	return NULL;
}




/**
 * main general
 */
int main () {

	int nPort = 0, nPortTx = 0;
	int bSincro = 0, bTransfer = 0;
	int nSocketFD = 0;

	char sServer[11];
	char sPswd[32];
	char sDirPath[MAX];

	pthread_t thread_id;
	int nEstatThread;

	//INITS
	//Demanem memoria per la LL
	LinkedList = (struct node *)malloc(sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->next = NULL;

	//Memoria per Linked list que contindra els elements a Tx
	LinkedListToTx = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedListToTx->sName, "fantasma");
	LinkedListToTx->next = NULL;

	//Netegem strings
	memset(sDirPath, '\0', MAX);
	memset(sServer, '\0', 11);
	memset(sLogin, '\0', 8);
	memset(sPswd, '\0', 32);
	memset(sMyLog, '\0', 20);

	//Llegir "config.dat"
	nPort = getConfigInfo(sServer, sDirPath);

	//Guardem -> sLogin, sPswd
	loginUser (sLogin, sPswd);

	//Crear/Obrir fitxer de Log
	strcpy (sMyLog, "LSBox_cli.log.html");
	sMyLog[strlen(sMyLog)] = '\0';
	createLog (sMyLog);

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog);

	//Socket peticio connexio
	nSocketFD = clientConnect (nPort, sLogin, sPswd, LinkedList);

	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		bSincro = 0;
		display (LinkedList);

		bSincro = checkRootFiles (sDirPath, LinkedList, LinkedListToTx, sMyLog);

		if ( bSincro ) {
			pleaseSincro (nSocketFD, sLogin);
		} else {
			write (nSocketFD, "init", 4); 
		}

		bTransfer = receiveServerSincro (nSocketFD, sLogin, sDirPath, LinkedList, LinkedListToTx);
		if ( bTransfer ) {
			//rebem el port on conectarem el thread
			nPortTx = rebPort(nSocketFD);

			//creem el Thread enviament
			nEstatThread = pthread_create (&thread_id, NULL, ThreadTx, nPortTx);
			if (nEstatThread != 0) 	printf("fail al fill!\n");

			bTransfer = 0;
		}	
	
		sleep(5);
	}

	return 0;
}
