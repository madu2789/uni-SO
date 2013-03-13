/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "socketClient.h"


void TramaConnection (char sTrama[MAX_TRAMA], char sUser[7], char sPwd[20]) {
	char sLoginDesti[7];
	char sLoginOrigen[7];
	char sTipus[3];
	char sData[100];

	//Netejant variables
	bzero(sLoginOrigen, 7);
	bzero(sLoginDesti, 7);
	bzero(sTipus, 3);
	bzero(sData, 100);

	//creant Camps de la trama separadament
	strcpy(sLoginOrigen, sUser);
	strcpy(sLoginDesti, "LSBox");
	strcpy(sTipus, "P");

	//creant camp data
	strcpy(sData, sUser);
	strcat(sData, ":");
	strcat(sData, sPwd);

	//creant Trama final que enviarem
	strcat(sTrama, sLoginOrigen);
	strcat(sTrama, sLoginDesti);
	strcat(sTrama, sTipus);
	strcat(sTrama, sData);
}

int clientConnect (int nPort) {

	int nBytesLeidos, nSalir;
	int nSocketFD;
	char sFrase[MAX], sTrama[MAX_TRAMA];
	uint16_t wPuerto;
	struct hostent *stHost;
	struct sockaddr_in stDireccionServidor;

	char sUser[7];
	char sPwd[20];

	//Comprobem port valid
	if ( nPort < 1024 || nPort > 65535){
		sprintf (sFrase,"Port invalid!\n");
		write (1,sFrase,strlen(sFrase));
		return ERROR;
	}
	wPuerto = nPort;

	//Creem el socket
	nSocketFD = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(nSocketFD < 0){
		sprintf (sFrase,"Error al crear el socket!\n");
		write (1, sFrase, strlen(sFrase));
		return ERROR;
	}

	printf("socket creat\n");

	//Creamos la estructura para conectarnos
	memset (&stDireccionServidor, 0, sizeof(stDireccionServidor));
	stDireccionServidor.sin_family = AF_INET;
	stDireccionServidor.sin_port = htons (wPuerto);

	printf("estructura x connect\n");

	//Extraemos el host
	stHost = gethostbyname ("cygnus.salle.url.edu");
	if (NULL == stHost){
		sprintf (sFrase,"No se ha podido resolver la direccion de cygnus!\n");
		write (1,sFrase,strlen (sFrase));
		return ERROR;
	}

	printf("extreiem host\n");

	bcopy (stHost->h_addr, &stDireccionServidor.sin_addr.s_addr, stHost->h_length);

	printf("bcopy i ara intentem connectar\n");

	//Ens conectamem al servidor
	if (connect (nSocketFD, &stDireccionServidor, sizeof (stDireccionServidor)) < 0){
		sprintf (sFrase,"Error al intentarnos conectar al servidor!\n");
		write (1,sFrase,strlen (sFrase));
		return ERROR;
	} else {
		printf("connexio correcta \n");

		//---------------------------------------------------------------------------
		//proves denviament de trames

		nBytesLeidos = read (nSocketFD, sTrama, MAX_TRAMA);
		printf("trama rebuda: %s...\n", sTrama);


		//Formamos la trama para la petición
		bzero (sTrama, MAX_TRAMA);
		//en fase de proves!!!!
		TramaConnection(sTrama, sUser, sPwd);
		write (nSocketFD, sTrama, MAX_TRAMA);
	}

}