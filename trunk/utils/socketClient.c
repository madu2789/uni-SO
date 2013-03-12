#include "socketClient.h"

int nBytesLeidos, nSalir;
int nSocketFD;
char sFrase[MAX], sTrama[MAX_TRAMA];
uint16_t wPuerto;
struct hostent *stHost;
struct sockaddr_in stDireccionServidor;

int nPort = 54024;
char sUser[20];
char sPwd[20];
char sTrama[MAX_TRAMA];


int clientConnect (int nPort) {

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

		//Formamos la trama para la petición
		bzero (sTrama,MAX_TRAMA);

		strcpy(sUser,"boiras");
		strcpy(sPwd,"abcsdde");
		//Li copiem les dades a la trama
		strcat (sTrama, sUser);
		strcat (sTrama, sPwd);

		printf("%s\n", sTrama);

		//Enviamos la trama
		write (nSocketFD,sTrama,MAX_TRAMA);

		printf("trama enviada!!\n");
	}

}
/*
void main () {
	clientConnect(nPort);

}*/