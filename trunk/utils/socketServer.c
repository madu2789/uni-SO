#include "socketServer.h"

//Variables globales
int gnSocketFD;

int nPort = 54024;


int ServerConection () {


	int nAux, nBytesLeidos, nFinal;
	int nSocketCliente;
	char sFrase[MAX], sTrama[MAX_TRAMA];
	uint16_t wPuerto;
	struct sockaddr_in stDireccionSocket, stDireccionCliente;

	if ( nPort < 1024 || nPort > 65535){
		sprintf (sFrase,"Puerto invalido!\n");
		write (1,sFrase,strlen (sFrase));
		return ERROR;
	}
	wPuerto = nPort;


//Creamos el socket
	gnSocketFD = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gnSocketFD < 0){
		sprintf (sFrase,"Error al crear el socket!\n");
		write (1,sFrase,strlen (sFrase));
		return ERROR;
	}

	//Especificamos la direccion del socket
	bzero (&stDireccionSocket, sizeof (stDireccionSocket));
	stDireccionSocket.sin_family = AF_INET;
	stDireccionSocket.sin_port = htons (wPuerto);
	stDireccionSocket.sin_addr.s_addr = INADDR_ANY;

	//Vinculamos el socket con la direccion
	if (bind (gnSocketFD, (void *) &stDireccionSocket, sizeof (stDireccionSocket)) < 0){
		sprintf (sFrase,"Error al vincular el socket y la direccion!\n");
		write (1,sFrase,strlen (sFrase));
		//Cerramos el socket
		close (gnSocketFD);
		return ERROR;
	}

	//Escuchamos peticiones (1 a la vez)
	listen (gnSocketFD, 15);

	sprintf (sFrase,"Servidor connectat!\n");
	write (1,sFrase,strlen (sFrase));

	while (1){

		printf("esperant client...\n");
		//Obtenemos un socket al cliente que se conecte
		socklen_t c_len = sizeof (stDireccionCliente);
		nSocketCliente = accept (gnSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nSocketCliente < 0){
			sprintf (sFrase,"Error al aceptar la peticion del cliente!\n");
			write (1,sFrase,strlen (sFrase));
			//Cerramos el socket
			close (gnSocketFD);
			return ERROR;
		}
		sprintf (sFrase,"\nClient conectat\n");
		write (1,sFrase,strlen (sFrase));

		nBytesLeidos = read (nSocketCliente, sTrama, MAX_TRAMA);
		printf("trama rebuda: %s...\n", sTrama);
	}


}
