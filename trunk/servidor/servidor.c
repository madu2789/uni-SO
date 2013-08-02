/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"

//VARS GLOBALS (PROVISIONALS)
	struct node *LinkedList;
	struct node *LinkedListToTx;
	char sDirPath[MAX];
	char sLoginUser[8];
	char sMyLog[20];



void * ThreadTx (void *arg){
	
	int nSocketFD = 0;
	char sTrama[MAX_TRAMA];
	memset (sTrama, '\0', MAX_TRAMA);

	int nSocketCliente = 0 ;
	char sFrase[MAX];
	struct sockaddr_in stDireccionCliente;

	int *nPortTx = (int *) arg;

	//Creem el socket
	nSocketFD = socketConnnection(nPortTx);	

	while (nSocketCliente == 0) {

		printf("esperant client...\n");

		//Obtenim un socket al client que es conecti
		socklen_t c_len = sizeof (stDireccionCliente);
		nSocketCliente = accept (nSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nSocketCliente < 0){
			writeLog ("LSBox_svr.log.html","socketServer.c","[Error] Connexio","En acceptar la peticio del cliente!", 0);
			sprintf (sFrase,"Error al aceptar la peticion del cliente!\n");
			write (1,sFrase,strlen (sFrase));
			//Tanquem socket
			close (nSocketFD);
			return NULL;
		}

		sprintf (sFrase,"\nClient conectat\n");
		write (1, sFrase, strlen (sFrase));

		strcpy (sTrama, "hola client fill!!!!!!!!");
	  sTrama[strlen(sTrama)] = '\0';

		write (nSocketCliente, sTrama, MAX_TRAMA);

		read (nSocketCliente, sTrama, MAX_TRAMA);
	  printf("strama rebuda: %s\n", sTrama);
	
 	 //Transmissio de dades
   transferContent (nSocketCliente, sDirPath, sLoginUser, LinkedListToTx, sMyLog);
	 receiveContent(nSocketCliente, sDirPath, LinkedList, LinkedListToTx, sMyLog);
	 
	 //Tancar socket
	 close (nSocketFD);
	 printf("Mort thread!\n");
	}
	return NULL;
}



/**
 * main general
 */
int main () {

	int nPort = 0;
	int nSocketFD = 0;
	int bSincro = 0;
	int bSincroPetition = 0;

	char sServer[11];

	pthread_t thread_id;
	int nEstatThread;

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
			getSincroInfo (nSocketFD, sLoginUser, LinkedList, LinkedListToTx);
			
			//Enviar el Port al client	
			enviaPort(nSocketFD, nPort+2, sLoginUser, "LSBox  ");
			
			//Crear Thread enviament
			nEstatThread = pthread_create (&thread_id, NULL, ThreadTx, nPort+2);
			if (nEstatThread != 0) printf("fail al fill!\n");

		} else {
			write (nSocketFD, "init", 4);
		}

		sleep (5);
	}
	return 0;
}
