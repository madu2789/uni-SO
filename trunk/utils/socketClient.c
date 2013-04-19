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
	memset(sTrama, '\0', MAX_TRAMA);
	memset(sLoginOrigen, '\0', 7);
	memset(sLoginDesti, '\0', 7);
	sTipus= '\0';
	memset(sData, '\0', 100);


	//Detectem quin tipus es i podem dades necessaries
	switch (nTipusTrama) {
		case 1:
			sprintf (sData,"'peticio d'autentificacio'");
			sTipus = 'P';
		break;
		case 2:
			//creant Camps de la trama separadament
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox  ");
			sprintf (sData,"'Error en el procediment d'autentificació.'");
			sTipus = 'E';
		break;
		case 3:
			//creant Camps de la trama separadament
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox  ");
			sTipus = 'A';

			//creant camp data
			strcpy(sData, sUser);
			strcat(sData, ":");
			strcat(sData, sPwd);
		break;
	}


	//creant Trama final que enviarem
	strncat(sTrama, sLoginDesti, 7);
	strncat(sTrama, sLoginOrigen, 7);
	sTrama[strlen(sTrama)] = sTipus;
	strncat(sTrama, sData, 100);

}




/**
 * Revisa que la Trama rebuda sigui del fotmat i correcte en si
 * @param  sTrama {String}	Trama rebuda que analitzarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int checkTrama (char sTrama[MAX_TRAMA], char sUser[7], int nType) {

	int bTramaOk = 0;
	//Camps de la trama:
	char sLoginOrigen[8];
	char sLoginDTrama[8];
	char sPwd[33];
	char sTypeTrama;
	char sDataTrama[100];


	memset(sLoginOrigen, '\0', 7);
	memset(sLoginDTrama, '\0', 7);
	memset(sDataTrama, '\0', 100);


	memcpy( sLoginOrigen, &sTrama[0], 7 );
	sLoginOrigen[7] = '\0';

	memcpy( sLoginDTrama, &sTrama[7], 7 );
	sLoginDTrama[7] = '\0';

  sTypeTrama = sTrama[14];

  strncpy(sDataTrama, sTrama+15, 100);
  sDataTrama[strlen(sDataTrama)] = '\0';

  memcpy( sPwd, &sDataTrama[8], 32 );
	sPwd[32] = '\0';


/* // Comprovacio que parseja be la trama:
  printf("camp login origen parsejat:  %s\n", sLoginOrigen);
  printf("camp login desti parsejat:  %s\n", sLoginDTrama);
  printf("camp trama parsejat:  %c\n", sTypeTrama);
  printf("camp data parsejat:  %s\n", sDataTrama);
  printf("password parsejat:  %s\n", sPwd);
*/

	bTramaOk = 0;

	switch (nType) {
		//cas autentificacio
		case 1:

			if (sTypeTrama == 'O'){
				if(strcmp(sLoginOrigen, "LSBox  ") == 0) {
					if(strcmp(sLoginDTrama, sUser) == 0) {
						bTramaOk = 1;
					}
				}
			}

		break;
	}

	return bTramaOk;
}




/**
 * Connecta el socket inicial amb el servidor
 * @param  nPort {Integer}	Number of Port al que ens conectarem
 * @return nSocketFD {Integer} Id del socket associat
 */
int socketConnection (int nPort) {
	struct sockaddr_in stDireccionServidor;
	struct hostent *stHost;
	uint16_t wPuerto;
	char sFrase[MAX];
	int nSocketFD;

	//Comprobem port valid
	if ( nPort < 1024 || nPort > 65535){
		writeLog ("LSBox_cli.log.html","socketClient.c","[Error] socket","Port invalid!", 0);
		return ERROR;
	}
	wPuerto = nPort;

	//Creem el socket
	nSocketFD = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(nSocketFD < 0){
		writeLog ("LSBox_cli.log.html", "socketClient.c","[Error] socket","Error al crear el socket!", 0);
		return ERROR;
	}

	//Creamos la estructura para conectarnos
	memset (&stDireccionServidor, 0, sizeof(stDireccionServidor));
	stDireccionServidor.sin_family = AF_INET;
	stDireccionServidor.sin_port = htons (wPuerto);

	//Extraemos el host
	stHost = gethostbyname ("cygnus.salle.url.edu");
	if (NULL == stHost){
		writeLog ("LSBox_cli.log.html","socketClient.c","[Error] socket","No se ha podido resolver la direccion de cygnus!", 0);
		return ERROR;
	}

	//Copiem les dades a la estructura
	bcopy (stHost->h_addr, &stDireccionServidor.sin_addr.s_addr, stHost->h_length);

	if (connect (nSocketFD, &stDireccionServidor, sizeof (stDireccionServidor)) < 0){
		writeLog ("LSBox_cli.log.html","socketClient.c","[Error] socket","Error al intentarnos conectar al servidor!", 0);
		return ERROR;
	}

	return nSocketFD;
}




/**
 * Connecta el client amb el servidor i Autentica el Usuari mitjançant el protocol
 * @param  nPort {Integer}	Number of Port al que ens conectarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int clientConnect (int nPort, char sUser[7], char sPwd[32]) {

	int nSocketFD;
	char sFrase[MAX], sTrama[MAX_TRAMA];
	int bTramaOk = 0;
	int nTipusTrama = 0;

	//Ens conectamem al servidor
	nSocketFD = socketConnection (nPort);


	//Protocol d'establiment de connexio
	//Llegim la primera trama del servidor de P
	read (nSocketFD, sTrama, MAX_TRAMA);

	//Comprovem si la Trama es correcte
	if (checkTrama(sTrama, sUser, 1)) {
		writeLog ("LSBox_cli.log.html","socketClient.c","[Error] Trama rebuda incorrecte",sTrama, 0);
		printf("error trama incorrecte\n");
		nTipusTrama = 2;
		//peticio again??
	} else {
		writeLog ("LSBox_cli.log.html","socketClient.c","Trama rebuda",sTrama, 1);
		printf("trama rebuda: %s\n", sTrama);
		nTipusTrama = 3;
	}

	printf("%s\n", sPwd);
	//Formem trama per la petició
	petitionConection(sTrama, sUser, sPwd, nTipusTrama);

	//Enviem la trama de peticio
	write (nSocketFD, sTrama, MAX_TRAMA);
	writeLog ("LSBox_cli.log.html","socketClient.c","Trama enviada",sTrama, 1);
	printf("trama enviada: %s\n", sTrama);

	//Llegim la Trama d'autoritzacio del
	read (nSocketFD, sTrama, MAX_TRAMA);

	bTramaOk = checkTrama(sTrama, sUser, 1);
	printf("trama correcte? %d \n", bTramaOk);

	//Comprovem si la Trama es correcte
	if (bTramaOk) {
		printf("2a trama rebuda: %s\n", sTrama);
		writeLog ("LSBox_cli.log.html","socketClient.c","Trama rebuda", sTrama, 1);
	} else {
		printf("error trama incorrecte\n");
		writeLog ("LSBox_cli.log.html","socketClient.c","[Error] Trama rebuda incorrecte",sTrama, 0);
	}

}