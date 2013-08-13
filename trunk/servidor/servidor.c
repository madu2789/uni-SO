/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"

	//VARS GLOBALS
	struct node *LinkedList;
	struct node *LinkedListToTx;
	int bSincro = 0;
	char sDirPath[MAX];

	//Hi ha que fer un maxambrat d'aquests dos
	char sLoginUser[8];
	char sLoginOrigen[8];

	// prova Estructura Molongui
	int nIdClient = 1;
	int nFdSockClient[7];
	// A la casella nPortTx[0] guarda el port del pare(avi)
	int nPortTx[7];
	char sLoginDesti[7][8];
	char sMyLog[7][20];



void * ThreadTx (void *arg){
	
	int nSocketFD = 0;
	char sTrama[MAX_TRAMA];
	int nSocketCliente = 0 ;
	char sFrase[MAX];
	struct sockaddr_in stDireccionCliente;
	
	int nIdMyClient = (int ) arg;
	int nMyPortTx = nPortTx[nIdMyClient];

	memset (sTrama, '\0', MAX_TRAMA);
	memset (sFrase, '\0', MAX);

	//Creem el socket
	nSocketFD = socketConnectionServidor (nMyPortTx);	

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
   transferContent (nSocketCliente, sDirPath, sLoginDesti[nIdMyClient], LinkedListToTx, sMyLog[nIdMyClient]);
	 receiveContent(nSocketCliente, sDirPath, LinkedList, LinkedListToTx, sMyLog[nIdMyClient]);
	 
	 //Tancar socket
	 close (nSocketFD);
	 printf("Mort thread TX!\n");
	}
	return NULL;
}




void * ServerDedicat (void *arg){
	
	int bSincroPetition = 0;
	pthread_t thread_id;
	int nEstatThread;
	
	int nIdMyClient = (int ) arg;
	int nFdSocketClient = nFdSockClient[nIdMyClient];

	while (1) {
		bSincro = 0;
		bSincroPetition = 0;
		bSincroPetition = receiveClientSincro (nFdSocketClient);

		printf("server dedicat de: %s\n", sLoginDesti[nIdMyClient]);

			if ( bSincro || bSincroPetition) {
				//Sincronitzacio
				startSincro (nFdSocketClient, sLoginDesti[nIdMyClient]);
				//Agafa la info procedent de Client
				getSincroInfo (nFdSocketClient, sLoginDesti[nIdMyClient], LinkedList, LinkedListToTx);
				
				//Enviar el Port al client	
				nPortTx[nIdMyClient] = nPortTx[0] + rand() % 400;
				enviaPort (nFdSocketClient, nPortTx[nIdMyClient], sLoginDesti[nIdMyClient], "LSBox  ");
				
				alarm(0);
				//Crear Thread enviament
				nEstatThread = pthread_create (&thread_id, NULL, ThreadTx, nIdMyClient);
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

	//Crear/Obrir fitxer de Log
	strcpy (sMyLog[0], "LSBox_svr.log.html");
	sMyLog[0][strlen(sMyLog[0])] = '\0';
	createLog (sMyLog[0]);

	//Llegir "config.dat"
	nPortTx[0] = getConfigInfo (sServer, sDirPath);

	//Socket peticio connexio
	gnSocketFD = socketConnectionServidor (nPortTx[0]);

	nFdSockClient[nIdClient] = ServerConection (nPortTx[0], gnSocketFD, sLoginDesti[nIdClient]);

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog[0]);

	//Crear Thread enviament
	if ( nFdSockClient[nIdClient] != -1 ) {
		nEstatThread = pthread_create (&thread_id, NULL, ServerDedicat, nIdClient);
		if (nEstatThread != 0) printf("fail al fill dedicat!\n");

		//Crear/Obrir fitxer de Log
		strcpy (sMyLog[nIdClient], sLoginDesti[nIdClient]);
		strcat (sMyLog[nIdClient], ".log.html");
		sMyLog[nIdClient][strlen(sMyLog[nIdClient])] = '\0';
		createLog (sMyLog[nIdClient]);

		//Incrementem el Id pel proper client
		nIdClient++;
	}

	socklen_t c_len = sizeof (stDireccionCliente);
	
	alarm(15);
	while (1) {

		//Detecta si al directori si hi ha hagut algun canvi
		display (LinkedList);

		//Detecta si algun client nou es vol connectar
		nFdSockClient[nIdClient] = 0;
		nFdSockClient[nIdClient] = accept (gnSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nFdSockClient[nIdClient] > 0){
			printf("client nou!!\n");
	  	bAuth = autentificacioClient (nFdSockClient[nIdClient], sLoginDesti[nIdClient], sLoginOrigen);	
	  	if ( bAuth ){
		  	nEstatThread = pthread_create (&thread_id, NULL, ServerDedicat, nIdClient);
				if (nEstatThread != 0) printf("fail al fill dedicat!\n");
				
				//Crear/Obrir fitxer de Log
				strcpy (sMyLog[nIdClient], sLoginDesti[nIdClient]);
				strcat (sMyLog[nIdClient], ".log.html");
				sMyLog[nIdClient][strlen(sMyLog[nIdClient])] = '\0';
				createLog (sMyLog[nIdClient]);
				
				//Incrementem el Id pel proper client
				nIdClient++;
				bAuth = 0;
	  	}
		}
	}

	return 0;
}
