/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "socketServer.h"

/**
 * Crea les diferents possibles Trames segons nType
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sLoginDesti {String}	Login desti de la trama
 * @param  sLoginOrigen {String}	Login origen de la trama
 * @param  nType {Integer}	Tipus de trama que enviarem
 * @return sTrama Ja formada
 */
void creaTrama (char sTrama[MAX_TRAMA], char sLoginOrigen[7], char sLoginDesti[7], int nType){

	char sTipus = '0';
	char sData[100];

	//Netejant variables
	memset (sTrama, '\0', MAX_TRAMA);
	memset (sData, '\0', 100);

	//Detectem quin tipus es i podem el sTipus i Data necessaries
	switch (nType) {
		case 1:
			sprintf (sData,"'peticio d'autentificacio'");
			sTipus = 'P';
		break;
		case 2:
			sprintf (sData,"'Error en el procediment d'autentificació. Procedim a desconnexio'");
			sTipus = 'E';
		break;
		case 3:
			sprintf (sData,"'Autentificacio realitzada satisfactoriament'");
			sTipus = 'O';
		break;
		case 4:
			sprintf (sData,"'Inici sincronitzacio'");
			sTipus = 'S';
		break;
	}

	//creant Trama final que enviarem
	strcat(sTrama, sLoginOrigen);
	strcat(sTrama, sLoginDesti);
	sTrama[strlen(sTrama)] = sTipus;
	strcat(sTrama, sData);
}


int enviaPort (int nFdIn, int nPort, char sLoginDesti[7], char sLoginOrigen[7]) {

	char sTrama[MAX_TRAMA];
	char sTipus = '\0';
	char sData[100];
	char sPort[6];

	//Netejant variables
	memset (sTrama, '\0', MAX_TRAMA);
	memset (sData, '\0', 100);
	memset (sPort, '\0', 6);

	//Preparo dades trama
	sData[0] = '<';
	sprintf (sPort, "%d", nPort);
	strcat(sData, sPort);
	sData[strlen(sData)] = '>';
	sTipus = 'P';

	//creant Trama final que enviarem
	strcat(sTrama, sLoginOrigen);
	strcat(sTrama, sLoginDesti);
	sTrama[strlen(sTrama)] = sTipus;
	strcat(sTrama, sData);

	printf("\nenviem port: sTrama: %s \n", sTrama);
	//Enviar-la Socket
	write (nFdIn, sTrama, MAX_TRAMA);
}



/**
 * Retorna el nombre d'entrades al fitxer
 * @param  nFdIn {Integer}	Fitxer a llegir
 * @return nNum {Integer} Nombre d'entrades
 */
int getNumUsers(int nFdIn) {
	int nNum = 0;
	char cAux = '0';

	//AtoI fins final de linia
	while (cAux != '\n') {
		read (nFdIn, &cAux, 1);
		if (cAux != '\n') {
			nNum = nNum*10 + cAux - '0';
		}
	}

	return nNum;
}


/**
 * Comprova si les dades del usuari es corresponen amb la línia actual del fitxer (shadows.dat)
 * @param  nFdIn {Integer}	Fitxer a llegir
 * @param  sUser {String}	Login del usuari
 * @param  sPswd {String}	Password del usuari
 * @return {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int checkUserInfo (int nFdIn, char sUser[7], char sPswd[32]) {
	int i = 0;
	char cAux = '0';
	char sFileUser[32];
	char sAux[MAX];
	char sFilePswd[32];

	memset(sFileUser, '\0', 32);
	memset(sFilePswd, '\0', 32);
	memset(sAux, '\0', MAX);

	// Llegim char a char fins a ':'
	while (cAux != ':') {
		read (nFdIn, &cAux, 1);
		if (cAux != ':') {
			sAux[i] = cAux;
		} else {
			sAux[i] = '\0';
		}
		i++;
	}
	sAux[strlen(sAux)] = '\0';

	strcpy (sFileUser, sAux);
	sFileUser[strlen(sFileUser)] = '\0';

	//netegem variables
	memset(sFilePswd, '\0', 32);
	memset(sAux, '\0', MAX);
	cAux = '\0';
	i = 0;

	// Llegim contrasenya
	// Llegim char a char fins a '\n'
	while (cAux != '\n') {
		read (nFdIn, &cAux, 1);
		if (cAux != ':') {
			sAux[i] = cAux;
		} else {
			sAux[i] = '\0';
		}
		i++;
	}

	strcpy (sFilePswd, sAux);
	sFilePswd[strlen(sFilePswd)] = '\0';
	sFilePswd[strlen(sFilePswd)-1] = '\0';
	sPswd[strlen(sPswd)] = '\0';

	if (strcmp(sUser, sFileUser) == 0 ) {
			if (strcmp(sPswd, sFilePswd) == 0 ){
				return 1;
			}
	}

	return 0;
}


/**
 * Comprova que el usuari que ha enviat la trama estigui registrat al sistema (shadows.dat)
 * @param  sUser {String}	Login de la trama
 * @param  sPswd {String}	Password de la trama
 * @return bValid {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int checkAuthentication (char sUser[7], char sPswd[32]) {
	int nFdIn = 0;
	int i = 0;
	int nNumUsers = 0;
	char cAux = '0';

	nFdIn = open("shadows.dat", O_RDONLY);
	if (-1 == nFdIn) {
		write(2,"[Error] Error al obrir el fitxer 'shadows.dat'.\n",47);
		exit(ERROR);
	} else {
		nNumUsers = getNumUsers(nFdIn);

		for (i=0; i<nNumUsers; i++) {
			if ( checkUserInfo(nFdIn, sUser, sPswd) == 1 ) {
				return 1;
			}
		}

		close(nFdIn);
		return 0;
	}
}

/**
 * Revisa que la Trama rebuda sigui del fotmat i correcte en si
 * @param  sTrama {String}	Trama rebuda que analitzarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int checkTrama (char sTrama[MAX_TRAMA], char sLoginOrigen[8], char sLoginDesti[8], char sPwd[33], int nType) {

	int bTramaOk = 0;
	//Camps de la trama:
	char sLoginDTrama[8];
	char sTypeTrama;
	char sDataTrama[100];

	//Netejant strings
	memset(sLoginOrigen, '\0', 8);
	memset(sLoginDTrama, '\0', 8);
	memset(sPwd, '\0', 33);
	memset(sDataTrama, '\0', 100);

	//Parseig de la trama
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

			if (sTypeTrama == 'A'){
				if(strcmp(sLoginOrigen, "LSBox  ") == 0) {
					bTramaOk = 1;
					strcpy(sLoginDesti, sLoginDTrama);
				}
			}

		break;
		case 4:
			if (sTypeTrama == 'X'){
				if(strcmp(sLoginOrigen, "LSBox  ") == 0) {
					bTramaOk = 1;
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
int socketConnnection (int nPort) {

	char sFrase[MAX];
	uint16_t wPuerto;
	struct sockaddr_in stDireccionSocket;
	int gnSocketFD = 0;

	//Comprovem que el port estigui en el marge correcte
	if ( nPort < 1024 || nPort > 65535){
		writeLog ("LSBox_svr.log.html","socketServer.c","[Error] socket","Port invalid!", 0);
		return ERROR;
	}
	wPuerto = nPort;

	//Creamos el socket
	gnSocketFD = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gnSocketFD < 0){
		writeLog ("LSBox_svr.log.html", "socketServer.c","[Error] socket","Error al crear el socket!", 0);
		return ERROR;
	}

	//Especificamos la direccion del socket
	bzero (&stDireccionSocket, sizeof (stDireccionSocket));
	stDireccionSocket.sin_family = AF_INET;
	stDireccionSocket.sin_port = htons (wPuerto);
	stDireccionSocket.sin_addr.s_addr = INADDR_ANY;

	//Vinculamos el socket con la direccion
	if (bind (gnSocketFD, (void *) &stDireccionSocket, sizeof (stDireccionSocket)) < 0){
		writeLog ("LSBox_svr.log.html", "socketServer.c","[Error] socket","Error al vincular el socket y la direccion!", 0);
		//Cerramos el socket
		close (gnSocketFD);
		return ERROR;
	}

	//Escuchamos peticiones (6 a la vez)
	listen (gnSocketFD, 6);

	writeLog ("LSBox_svr.log.html", "socketServer.c","Socket Connectat","Socket esperant entrades de clients", 0);
	sprintf (sFrase, "Servidor connectat!\n");
	write (1, sFrase, strlen (sFrase));


	return gnSocketFD;

}



/**
 * Connecta el socket Inicial amb el client i Autentica el Usuari
 * @param  nPort {Integer}	Number of Port al que ens conectarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int ServerConection (int nPort, char sLoginDesti[8]) {

	int gnSocketFD = 0;
	int nSocketCliente = 0 ;
	char sFrase[MAX], sTrama[MAX_TRAMA];
	struct sockaddr_in stDireccionCliente;

	char sLoginOrigen[8];
	char sPwd[33];
	int bValidTrama = 0, bValidAuth = 0;


	gnSocketFD = socketConnnection(nPort);

	while (nSocketCliente == 0) {

		printf("esperant client...\n");

		//Obtenim un socket al client que es conecti
		socklen_t c_len = sizeof (stDireccionCliente);
		nSocketCliente = accept (gnSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nSocketCliente < 0){
			writeLog ("LSBox_svr.log.html","socketServer.c","[Error] Connexio","En acceptar la peticio del cliente!", 0);
			sprintf (sFrase,"Error al aceptar la peticion del cliente!\n");
			write (1,sFrase,strlen (sFrase));
			//Tanquem socket
			close (gnSocketFD);
			return ERROR;
		}

		sprintf (sFrase,"\nClient conectat\n");
		write (1, sFrase, strlen (sFrase));

		//Protocol de trames d'establiment de connexio

		//Creem la primera trama de peticio d'autentificacio
 		creaTrama(sTrama, "LSBox  ", "client ", 1);

		//Enviem la trama de peticio d'autentificacio
		write (nSocketCliente, sTrama, MAX_TRAMA);
		printf("trama enviada: %s\n", sTrama);
		writeLog ("LSBox_svr.log.html","socketServer.c","Trama Enviada", sTrama, 1);

		//Rebem Trama amb les dades del client
		read(nSocketCliente, sTrama, MAX_TRAMA);
		printf("trama rebuda: %s\n", sTrama);


		bValidTrama = checkTrama(sTrama, sLoginOrigen, sLoginDesti, sPwd, 1);
		bValidAuth = checkAuthentication (sLoginDesti, sPwd);
		printf("trama correcte? %d - %d\n", bValidTrama, bValidAuth);

		//Comprovem que la trama rebuda es correcte
		if ( bValidAuth && bValidTrama ) {

			writeLog ("LSBox_svr.log.html","socketServer.c","Trama Rebuda", sTrama, 1);

			//Creem la trama de resposta OK peticio d'autentificacio
 			creaTrama (sTrama, "LSBox  ", sLoginDesti, 3);

 			//Enviem la trama de de connexio correcta
			write (nSocketCliente, sTrama, MAX_TRAMA);
			printf ("trama enviada: %s\n", sTrama);
			//Escribim al Log
			writeLog ("LSBox_svr.log.html","socketServer.c","Trama Enviada", sTrama, 1);

		} else {

			writeLog ("LSBox_svr.log.html", "socketServer.c", "[Error]Trama Rebuda", sTrama, 0);
			//Creem la trama de resposta KO peticio d'autentificacio
 			creaTrama (sTrama, "LSBox  ", sLoginOrigen, 2);

 			//Enviem la trama de de connexio correcta
			write (nSocketCliente, sTrama, MAX_TRAMA);
			printf ("[desconnexio] trama enviada: %s\n", sTrama);
			writeLog ("LSBox_svr.log.html", "socketServer.c", "Trama Enviada", sTrama, 1);

 			//Cerramos el socket
			//close (gnSocketFD);
		}
	}
	return nSocketCliente;
}

