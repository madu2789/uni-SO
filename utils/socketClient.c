/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "socketClient.h"

/**
 * Crea la Trama de peticio de coneccio per al servidor
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sUser {String}	Login d'usuari que inclourem
 * @param  sPwd {String}	Password amb md5 que inclourem
 * @return
 */
void petitionConection (char sTrama[MAX_TRAMA], char sUser[7], char sPwd[20], int nTipusTrama) {
	char sLoginDesti[7];
	char sLoginOrigen[7];
	char sTipus;
	char sData[100];

	//Netejant variables
	bzero(sLoginOrigen, 7);
	bzero(sLoginDesti, 7);
	sTipus= '\0';
	bzero(sData, 100);


	//Detectem quin tipus es i podem dades necessaries
	switch (nTipusTrama) {
		case 1:
			sprintf (sData,"'peticio d'autentificacio'");
			sTipus = 'P';
		break;
		case 2:
			//creant Camps de la trama separadament
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox");
			sprintf (sData,"'Error en el procediment d'autentificació.'");
			sTipus = 'E';
		break;
		case 3:
			//creant Camps de la trama separadament
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox");
			sTipus = 'A';

			//creant camp data
			strcpy(sData, sUser);
			strcat(sData, ":");
			strcat(sData, sPwd);
		break;
	}


	//creant Trama final que enviarem
	strcat(sTrama, sLoginOrigen);
	strcat(sTrama, sLoginDesti);
	sTrama[strlen(sTrama)] = sTipus;
	strcat(sTrama, sData);
}


/**
 * Revisa que la Trama rebuda sigui del fotmat i correcte en si
 * @param  sTrama {String}	Trama rebuda que analitzarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int checkTrama (char sTrama[MAX_TRAMA]) {
	int bTramaOk = 0;

	int i = 0;

//dos heuristikes : trama tipus E || trama incorrecte

	while (sTrama[i] != '\0') {

		if (sTrama[i] == 'O' || sTrama[i] == 'A' || sTrama[i] == 'P' ) {
			bTramaOk = 1;
			printf("Correcte\n");
		} else if (sTrama[i] == 'E') {
			bTramaOk = 0;
		}
		i++;
	}

	return bTramaOk;
}

/**
 * Connecta el socket Inicial amb el servidor i Autentica el Usuari
 * @param  nPort {Integer}	Number of Port al que ens conectarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int clientConnect (int nPort, char sUser[7], char sPwd[32]) {

	int nBytesLlegits, nSalir;
	int nSocketFD;
	char sFrase[MAX], sTrama[MAX_TRAMA];
	uint16_t wPuerto;
	struct hostent *stHost;
	struct sockaddr_in stDireccionServidor;

	int bTramaOk = 0;
	int nTipusTrama = 0;

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

	//Creamos la estructura para conectarnos
	memset (&stDireccionServidor, 0, sizeof(stDireccionServidor));
	stDireccionServidor.sin_family = AF_INET;
	stDireccionServidor.sin_port = htons (wPuerto);

	//Extraemos el host
	stHost = gethostbyname ("cygnus.salle.url.edu");
	if (NULL == stHost){
		sprintf (sFrase,"No se ha podido resolver la direccion de cygnus!\n");
		write (1,sFrase,strlen (sFrase));
		return ERROR;
	}

	//Copiem les dades a la estructura
	bcopy (stHost->h_addr, &stDireccionServidor.sin_addr.s_addr, stHost->h_length);

	//Ens conectamem al servidor
	if (connect (nSocketFD, &stDireccionServidor, sizeof (stDireccionServidor)) < 0){
		sprintf (sFrase, "Error al intentarnos conectar al servidor!\n");
		write (1, sFrase, strlen(sFrase));
		return ERROR;
	} else {

		//---------------------------------------------------------------------------
		//Protocol de trames d'establiment de connexio
		//---------------------------------------------------------------------------

		//Llegim la primera trama del servidor de P
		nBytesLlegits = read (nSocketFD, sTrama, MAX_TRAMA);

		//Comprovem si la Trama es correcte
		if (nBytesLlegits < MAX_TRAMA && checkTrama(sTrama)) {
			printf("error trama incorrecte\n");
			nTipusTrama = 2;
			//peticio again??
		} else {
			printf("trama rebuda: %s...\n", sTrama);
			nTipusTrama = 3;
		}


		//Formem trama per la petició
		bzero (sTrama, MAX_TRAMA);
		petitionConection(sTrama, sUser, sPwd, nTipusTrama);

		//Enviem la trama de peticio
		write (nSocketFD, sTrama, MAX_TRAMA);

		//Llegim la Trama d'autoritzacio del
		nBytesLlegits = read (nSocketFD, sTrama, MAX_TRAMA);

		//Comprovem si la Trama es correcte
		if (nBytesLlegits < MAX_TRAMA && checkTrama(sTrama)) {
			printf("error trama incorrecte\n");
		} else {
			printf("2a trama rebuda: %s...\n", sTrama);
		}
	}

}