/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"

//VARS GLOBALS (PROVISIONALS)
	struct node *LinkedList;
	struct node *LinkedListToTx;
	char sDirPath[MAX];
	char sMyLog[20];

//lio amb Logins de la PARRA
	char sLoginUser[8];
	char sLoginDesti[8];
	char sLoginOrigen[8];

	int nPort = 0;
	int bSincro = 0;



void * ThreadTx (void *arg){
	
	int nSocketFD = 0;
	char sTrama[MAX_TRAMA];
	memset (sTrama, '\0', MAX_TRAMA);
	int nSocketCliente = 0 ;
	char sFrase[MAX];
	struct sockaddr_in stDireccionCliente;
	int *nPortTx = (int *) arg;

	//Creem el socket
	nSocketFD = socketConnectionServidor(nPortTx);	

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
	
 	 //Transmissio de dades
   transferContent (nSocketCliente, sDirPath, sLoginDesti, LinkedListToTx, sMyLog);
	 receiveContent(nSocketCliente, sDirPath, LinkedList, LinkedListToTx, sMyLog);
	 
	 //Tancar socket
	 close (nSocketFD);
	 printf("Mort thread TX!\n");
	}
	return NULL;
}




void * ServerDedicat (void *arg){
	
	int nPortTx = 0;
	int bSincroPetition = 0;
	pthread_t thread_id;
	int nEstatThread;
	int *nFdSocketClient = (int *) arg;


	printf("sLogin del meu client: %s\n", sLoginDesti);


	while (1) {
		bSincro = 0;
		bSincroPetition = 0;
		bSincroPetition = receiveClientSincro (nFdSocketClient);

		printf("server dedicat de: %s\n", sLoginDesti);

			if ( bSincro || bSincroPetition) {
				//Sincronitzacio
				startSincro (nFdSocketClient, sLoginDesti);
				//Agafa la info procedent de Client
				getSincroInfo (nFdSocketClient, sLoginDesti, LinkedList, LinkedListToTx);
				
				//Enviar el Port al client	
				nPortTx = nPort + rand() % 400;
				enviaPort (nFdSocketClient, nPortTx, sLoginDesti, "LSBox  ");
				
				alarm(0);
				//Crear Thread enviament
				nEstatThread = pthread_create (&thread_id, NULL, ThreadTx, nPortTx);
				if (nEstatThread != 0) printf("fail al fill!\n");
				nEstatThread = pthread_join(thread_id, NULL);
				if (nEstatThread != 0) 	printf("fail al fill!\n");

				alarm(15);

		} else {
			write (nFdSocketClient, "init", 4);
		}
	}

	return NULL;
}




void RSIInt (void){
	char sAux[MAX];
	
	sprintf (sAux,"Tancant LSBox server!\n");
	write (1,sAux,strlen (sAux));

	exit(0);
	//TODO
	//Cerramos la memoria compartida
	// shmdt (gpnPIDS);
	// shmctl (gnMemoria, IPC_RMID, NULL);
}




void RSIAlarm(void) {
	bSincro = checkRootFiles (sDirPath, LinkedList, LinkedListToTx, sMyLog);
	alarm(15);
}




/**
 * main general
 */
int main () {

	int nSocketCliente = 0 ;
	int bAuth = 0;
	struct sockaddr_in stDireccionCliente;
	char sServer[11];

	pthread_t thread_id;
	int nEstatThread = 0;
	int nSocketFD = 0;
	int gnSocketFD = 0;

	//INITS
	//Demanem memoria per la LL
	LinkedList = (struct node *) malloc (sizeof(struct node));
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

	//Socket peticio connexio
	gnSocketFD = socketConnectionServidor (nPort);
	nSocketFD = ServerConection (nPort, gnSocketFD, sLoginDesti);

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog);

	//Crear Thread enviament
	if ( nSocketFD != -1 ) {
		nEstatThread = pthread_create (&thread_id, NULL, ServerDedicat, nSocketFD);
		if (nEstatThread != 0) printf("fail al fill dedicat!\n");
	}

	socklen_t c_len = sizeof (stDireccionCliente);
	
	//bSincro = checkRootFiles (sDirPath, LinkedList, LinkedListToTx, sMyLog);
	alarm(15);
	while (1) {

		//Detecta si al directori si hi ha hagut algun canvi
		display (LinkedList);
		printf("canvii??? %d\n", bSincro);

		

		//Detecta si algun client nou es vol connectar
		nSocketCliente = 0;
		nSocketCliente = accept (gnSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nSocketCliente > 0){
			printf("client nou!!!!\n");
	  	bAuth = autentificacioClient (nSocketCliente, sLoginDesti, sLoginOrigen);	
	  	if ( bAuth ){
		  	nEstatThread = pthread_create (&thread_id, NULL, ServerDedicat, nSocketCliente);
				if (nEstatThread != 0) printf("fail al fill dedicat!\n");
				bAuth = 0;
	  	}

		}

	}

	return 0;
}
