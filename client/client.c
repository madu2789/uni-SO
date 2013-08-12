/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "client.h"

	struct node *LinkedList;
	struct node *LinkedListToTx;
	char sLogin[8];
	char sMyLog[20];
	char sDirPath[MAX];
	int nSocketFD = 0;



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
	
	char sTrama[MAX_TRAMA];
	memset (sTrama, '\0', MAX_TRAMA);
	
	int *nPortTx = (int *) arg;
	int nSocketFDTx = 0;

	//Creem el socket
	nSocketFDTx = socketConnectionClient (nPortTx);

	//Primer rebem info, despres enviem
	receiveContent (nSocketFDTx, sDirPath, LinkedList, LinkedListToTx, sMyLog);
	transferContent (nSocketFDTx, sDirPath, sLogin, LinkedListToTx, sMyLog);
	
	//Tancar socket
	close (nSocketFDTx);
	printf("Mort thread!\n");

	return NULL;
}




void RSIInt (void){
	char sAux[MAX];
	
	sprintf (sAux,"Tancant LSBox client!\n");
	write (1,sAux,strlen (sAux));

	exit(0);
	//TODO
	//Cerramos la memoria compartida
	// shmdt (gpnPIDS);
	// shmctl (gnMemoria, IPC_RMID, NULL);
}




void RSIAlarm(void) {
	int bSincro = 0;

	bSincro = checkRootFiles (sDirPath, LinkedList, LinkedListToTx, sMyLog);

	if ( bSincro ) {
		pleaseSincro (nSocketFD, sLogin);
	} else {
		write (nSocketFD, "init", 4); 
	}
	alarm(5);
}




/**
 * main general
 */
int main () {

	int nPort = 0, nPortTx = 0;
	int bTransfer = 0;
	int bSincro = 0;

	char sServer[11];
	char sPswd[32];
  pthread_attr_t attr;
	pthread_t thread_id;
	int nEstatThread = 0;

	//INITS
	//Demanem memoria per la LL
	nSocketFD = 0;
	LinkedList = (struct node *)malloc(sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->next = NULL;

	//Memoria per Linked list que contindra els elements a Tx
	LinkedListToTx = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedListToTx->sName, "fantasma");
	LinkedListToTx->next = NULL;

	//Assignem la RSI al signal de Ctrl+C
	signal (SIGINT, (void*) RSIInt);
	//Assignem la RSI al signal Alarm
	signal(SIGALRM, (void*)RSIAlarm);

	//Netegem strings
	memset(sDirPath, '\0', MAX);
	memset(sServer, '\0', 11);
	memset(sLogin, '\0', 8);
	memset(sPswd, '\0', 32);
	memset(sMyLog, '\0', 20);

	//Crear/Obrir fitxer de Log
	strcpy (sMyLog, "LSBox_cli.log.html");
	sMyLog[strlen(sMyLog)] = '\0';
	createLog (sMyLog);

	//Guardem -> sLogin, sPswd
	loginUser (sLogin, sPswd);

	//Llegir "config.dat"
	nPort = getConfigInfo(sServer, sDirPath);

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog);

	//Socket peticio connexio
	nSocketFD = clientConnect (nPort, sLogin, sPswd, LinkedList);

	bSincro = checkRootFiles (sDirPath, LinkedList, LinkedListToTx, sMyLog);

	alarm(5);
	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		display (LinkedList);

		bTransfer = receiveServerSincro (nSocketFD, sLogin, sDirPath, LinkedList, LinkedListToTx);

		if ( bTransfer ) {
			//rebem el port on conectarem el thread
			nPortTx = rebPort(nSocketFD);

			alarm(0);

			//creem el Thread enviament
			nEstatThread = pthread_create (&thread_id, NULL, ThreadTx, nPortTx);
			if (nEstatThread != 0) 	printf("fail al fill!\n");
			nEstatThread = pthread_join(thread_id, NULL);
			if (nEstatThread != 0) 	printf("fail al fill!\n");
			
			alarm(5);

			printf("em acabat de tx!! \n");

			bTransfer = 0;
		}	
		printf("2 \n");
		sleep(1);
	}

	return 0;
}
