/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"

	//VARS GLOBALS
	struct node *LinkedList;
	struct node *LinkedListToTx;
	sem_t semLL;
	char sDirPath[MAX_LONG];
	char sLoginOrigen[8];

	// Estructura Molongui (a la [0] guarda info del pare)
	int nIdClient = 1;
	int nFdSockClient[7];
	// A nPortTx[0] guarda el port del pare(avi)
	int nPortTx[7];
	int bSincro[7];
	char sLoginDesti[7][8];
	char sMyLog[7][20];


/**
 * Thread creat a partir del Server dedicat, només Tx i Rx el
 * que li diuen
 * @param arg [Id del client dedicat]
 */
void * ThreadTx (void *arg){
	
	int nSocketFD = 0;
	int nSocketCliente = 0 ;
	char sTrama[MAX_TRAMA];
	char sFrase[MAX];
	struct sockaddr_in stDireccionCliente;
	
	int nIdMyClient = (int ) arg;
	int nMyPortTx = nPortTx[nIdMyClient];

	memset (sTrama, '\0', MAX_TRAMA);
	memset (sFrase, '\0', MAX);

	//Creem el socket
	nSocketFD = socketConnectionServidor (nMyPortTx);	

	while (nSocketCliente == 0) {

		sprintf (sFrase,"esperant client...\n");
		write (1, sFrase, strlen (sFrase));

		//Obtenim un socket al client que es conecti
		socklen_t c_len = sizeof (stDireccionCliente);
		nSocketCliente = accept (nSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nSocketCliente < 0){
			writeLog ("LSBox_svr.log.html","servidor.c","[Error] Connexio","En acceptar la peticio del cliente!", 0);
			sprintf (sFrase,"Error al aceptar la peticion del cliente!\n");
			write (1, sFrase, strlen (sFrase));
			//Tanquem socket
			close (nSocketFD);
			return NULL;
		}

		sprintf (sFrase,"\nClient conectat\n");
		write (1, sFrase, strlen (sFrase));
	
 	 //Transmissio de dades
   transferContent (nSocketCliente, sDirPath, sLoginDesti[nIdMyClient], LinkedListToTx, sMyLog[nIdMyClient], &semLL);
	 receiveContent(nSocketCliente, sDirPath, LinkedList, LinkedListToTx, sMyLog[nIdMyClient], &semLL);
	
	  buidaLL(LinkedList);
		buidaLL(LinkedListToTx);
		initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog[nIdMyClient]);

	 //Tancar socket
	 close (nSocketFD);
	 sprintf (sFrase,"\nMort thread TX!\n");
	 write (1, sFrase, strlen (sFrase));
	}
	return NULL;
}



/**
 * Procés creat per thread del pare, dedicat a un sol client,
 *  i pendent de sincroClient o sincro del Pare
 * @param arg [Id del client dedicat]
 */
void * ServerDedicat (void *arg){
	
	int bSincroPetition = 0;
	pthread_t thread_id;
	int nEstatThread;
	int nBytesRead = 0;
	char sFrase[MAX];
	int nIdMyClient = (int ) arg;
	int nFdSocketClient = nFdSockClient[nIdMyClient];

	while (1) {
		
		bSincroPetition = 0;
		bSincroPetition = receiveClientSincro (nFdSocketClient);

		sprintf (sFrase,"server dedicat de: ");
	  write (1, sFrase, strlen (sFrase));
		write (1, sLoginDesti[nIdMyClient], strlen (sLoginDesti[nIdMyClient]));

			if ( 	bSincro[nIdMyClient] || bSincroPetition) {
				//Sincronitzacio
				startSincro (nFdSocketClient, sLoginDesti[nIdMyClient]);
				//Agafa la info procedent de Client
				getSincroInfo (nFdSocketClient, sLoginDesti[nIdMyClient], LinkedList, LinkedListToTx, nIdMyClient, sDirPath);
				
				//Enviar el Port al client	
				nPortTx[nIdMyClient] = nPortTx[0] + rand() % 400;
				enviaPort (nFdSocketClient, nPortTx[nIdMyClient], sLoginDesti[nIdMyClient], "LSBox  ");
				
				alarm(0);
				//Crear Thread enviament
				nEstatThread = pthread_create (&thread_id, NULL, ThreadTx, (void *)nIdMyClient);
				if (nEstatThread != 0)  write (1, "Error Thread", 12);
				nEstatThread = pthread_join(thread_id, NULL);
				if (nEstatThread != 0) 	write (1, "Error Thread", 12);
				writeLog (sMyLog[nIdMyClient], "servidor.c","Nou Thread","crea ThreadTx", 1);

				bSincro[nIdMyClient] = 0;
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

	writeLog ("LSBox_svr.log.html","servidor.c","Tancament aplicació","Hem rebut una INT", 1);

	exit(0);
	//TODO
	//Cerramos la memoria compartida
	// shmdt (gpnPIDS);
	// shmctl (gnMemoria, IPC_RMID, NULL);
}




void RSIAlarm(void) {
	int bMySincro = 0;
	int i = 0;

	bMySincro = checkRootFiles (sDirPath, LinkedList, LinkedListToTx, sMyLog[0], &semLL);

	if (bMySincro) {
		for ( i = 0; i <= nIdClient; i++) {
				bSincro[i] = 1;
		} 
	}

	alarm(15);
}


/**
 * [creaServidorDedicat crea el thread dedicat al client ja autenticat]
 * @param nIdClient [Id del client dedicat al Server]
 */
void creaServidorDedicat (int nIdClient) {

		pthread_t thread_id;
	  int nEstatThread = 0;

		nEstatThread = pthread_create (&thread_id, NULL, ServerDedicat, (void *)nIdClient);
		if (nEstatThread != 0) write (1, "Error Thread", 12);

		//Crear/Obrir fitxer de Log
		strcpy (sMyLog[nIdClient], sLoginDesti[nIdClient]);
		strcat (sMyLog[nIdClient], ".log.html");
		sMyLog[nIdClient][strlen(sMyLog[nIdClient])] = '\0';
		createLog (sMyLog[nIdClient]);

		writeLog ("LSBox_svr.log.html","servidor.c","Nou Thread","crea ServerDedicat", 1);

}




/**
 * main general
 */
int main () {

	int nSocketCliente = 0 ;
	int bAuth = 0;
	struct sockaddr_in stDireccionCliente;
	char *psServer;
	char sFrase[MAX];
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

	// Assignem la RSI al signal de Ctrl+C
	signal(SIGINT, (void*) RSIInt);
	// Assignem la RSI al signal Alarm
	signal(SIGALRM, (void*)RSIAlarm);

	// Creem el semafor
	sem_init(&semLL, 0, 1);

	//Inits strings
	memset(sDirPath, '\0', MAX_LONG);

	//Crear/Obrir fitxer de Log
	strcpy (sMyLog[0], "LSBox_svr.log.html");
	sMyLog[0][strlen(sMyLog[0])] = '\0';
	createLog (sMyLog[0]);

	//Llegir "config.dat"
	nPortTx[0] = getConfigInfo (&psServer, sDirPath);

	//Socket peticio connexio
	gnSocketFD = socketConnectionServidor (nPortTx[0]);
	nFdSockClient[nIdClient] = ServerConection (nPortTx[0], gnSocketFD, sLoginDesti[nIdClient]);

	//Posa tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog[0]);

	//Crear Thread de servidor dedicat
	if ( nFdSockClient[nIdClient] != -1 ) {
		
		creaServidorDedicat(nIdClient);

		//Incrementem Id pel proper client
		nIdClient++;
	}

	socklen_t c_len = sizeof (stDireccionCliente);
	alarm(15);

	while (1) {
		//Mostra LL
		display (LinkedList);

		//Detecta si algun client nou es vol connectar
		nFdSockClient[nIdClient] = 0;
		nFdSockClient[nIdClient] = accept (gnSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nFdSockClient[nIdClient] > 0){
			sprintf (sFrase,"\nClient conectat\n");
			write (1, sFrase, strlen (sFrase));
	  	bAuth = autentificacioClient (nFdSockClient[nIdClient], sLoginDesti[nIdClient], sLoginOrigen);	
	  	if ( bAuth ){
				creaServidorDedicat(nIdClient);
				
				//Incrementem Id pel proper client
				nIdClient++;
				bAuth = 0;
	  	}
		}
	}

	return 0;
}
