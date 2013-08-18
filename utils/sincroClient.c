/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "sincroClient.h"




/**
 * Crea la Trama de peticio de coneccio per al servidor
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sUser {String}	Login d'usuari que inclourem
 * @param  sPwd {String}	Password amb md5 que inclourem
 * @param  bEmissor {Integer}	numero que diu si l'emissor es client o servidor
 * @return
 */
void creaTramaSincro (char sTrama[MAX_TRAMA], char sUser[7], char sName[30], char sDate[64], int nSize, int bEmissor) {
	char sLoginDesti[7];
	char sLoginOrigen[7];
	char sTipus = '0';
	char sData[100];
	char sSize[10];

	//Netejant variables
	memset(sTrama, '\0', MAX_TRAMA);
	memset(sLoginOrigen, '\0', 7);
	memset(sLoginDesti, '\0', 7);
	sTipus= '\0';
	memset(sData, '\0', 100);

	//Detectem quin tipus es i podem dades necessaries
	switch (bEmissor) {
		case 1:
			//creant Camps de la trama separadament
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox  ");
			sData[0] = '<';
			strncat(sData, sName, 20);
			strncat(sData, ">:<", 3);
			strncat(sData, sDate, 24);
			strncat(sData, ">:<", 3);
			//nSize
			sprintf(sSize, "%d", nSize);
			strcat(sData, sSize);

			sData[strlen(sData)] = '>';
			sData[strlen(sData)] = '\0';
			sTipus = 'N';
		break;
		case 2:
		/*	//creant Camps de la trama separadament
			strcpy(sLoginOrigen, "LSBox  ");
			strcpy(sLoginDesti, sUser);
			sprintf (sData,"'Error en el procediment d'autentificació.'");
			sTipus = 'E';
			*/
		break;
		case 3:
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox  ");
			sTipus = 'X';
			strcpy(sData, "'Sincronitzacio finalitzada'");
		break;
	}

	//creant Trama final que enviarem
	strncat(sTrama, sLoginDesti, 7);
	strncat(sTrama, sLoginOrigen, 7);
	sTrama[strlen(sTrama)] = sTipus;
	strncat(sTrama, sData, 100);

}


void getTramesG (int nFdIn, char sLoginOrigen[7], struct node *LinkedListToTx) {
	char sTrama[MAX_TRAMA];
	char sName[24];
	char sDataTrama[100];
	int bFinalSincro = 0, nBytesRead = 0;

	memset (sDataTrama, '\0', 100);
	memset (sName, '\0', 24);

	while (!bFinalSincro)	{
		nBytesRead = read (nFdIn, sTrama, MAX_TRAMA);
		bFinalSincro = checkTramaClient (sTrama, sLoginOrigen, 4);
		if (!bFinalSincro) {
				//Parsejo la part de DataTrama -> sName
		  	strncpy (sDataTrama, sTrama+15, 100);
		  	sDataTrama[strlen(sDataTrama)] = '\0';
		  	ParserNameTx(sDataTrama, sName);

				printf("sTrama G: %s\n", sTrama);
				writeLog ("LSBox_cli.log.html","sincroClient.c","Trama G Enviada", sTrama, 1);

		  	//canviem el estat de la LlinkedListToTx per enviar-li al servidor
		  	setEstatByName (sName, 4, LinkedListToTx);
		  	memset (sName, '\0', 24);
		  	memset (sDataTrama, '\0', 100);
		}
	}
}




/**
 * Client envia una Trama O|E  i crida a SetSincroInfo
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sLoginOrigen {String}	Login d'usuari que inclourem
 * @param  LinkedList {Struct}	Estructura On guardem tota la info
 * @return bCorrect {0 wrong | 1 right}
 */
int receiveServerSincro (int nFdIn, char sLoginOrigen[7], char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx) {
	char sTrama[MAX_TRAMA];
	int nBytesRead = 0;

	//Comprovem si realment el server ens ha confirmat sincro
	nBytesRead = read (nFdIn, sTrama, MAX_TRAMA);
	if (nBytesRead <= 4) return 0;

	printf("trama rebuda:  %s\n", sTrama);
	writeLog ("LSBox_cli.log.html","sincroClient.c","Trama Rebuda", sTrama, 1);

	//Enviem la trama 'O' o 'E' sincronitzacio confirmada
	creaTramaClient(sTrama, sLoginOrigen, "LsBox  ", 4);
	write (nFdIn, sTrama, MAX_TRAMA);
	printf("trama enviada: %s\n", sTrama);
	writeLog ("LSBox_cli.log.html","sincroClient.c","Trama 'O' Enviada", sTrama, 1);

	//Comencem a enviar tota la LL -> trames 'N'
	setSincroInfo(nFdIn, sLoginOrigen, LinkedList);

	//Llegim les trames G per saber que enviar
	getTramesG(nFdIn, sLoginOrigen, LinkedListToTx);

	return 1;
}



/**
 * Client envia les trames amb la info N, i la trama de final X
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @param  sLoginOrigen {String}	Login d'usuari que inclourem
 * @param  LinkedList {Struct}	Estructura On guardem tota la info
 * @return bCorrect {0 wrong | 1 right}
 */
void setSincroInfo (int nFdIn, char sLoginOrigen[7], struct node *LinkedList) {
	char sTrama[MAX_TRAMA];
	char sName[30];
	char sDate[64];
	int nSize = 0;
 	int nLocation = 0;
 	int nTotalFiles = count(LinkedList);

 	//	REVISAR!!!
 	//		 nLocation es 1 pk la primera casella de la llista
 	//		 nTotalFiles+1 per el primer punt i pk el count ens dona -1 casella k la k ens interesa
 	for (nLocation = 1 ; nLocation < nTotalFiles+1 ; nLocation++ ) {

 		nSize = showNode(sName, sDate, nLocation, LinkedList);

 		//creaTramaSincro
 		creaTramaSincro(sTrama, sLoginOrigen, sName, sDate, nSize, 1);
 		printf("trama N enviades: %s\n", sTrama);
 		write (nFdIn, sTrama, MAX_TRAMA);
 		writeLog ("LSBox_cli.log.html","sincro.c","Trama 'N' Enviada", sTrama, 1);
 	}

	//creaTrama a "manija"
 	creaTramaSincro(sTrama, sLoginOrigen, sName, sDate, nSize, 3);
 	printf("trama X enviada: %s\n", sTrama);
 	write (nFdIn, sTrama, MAX_TRAMA);
 	writeLog ("LSBox_cli.log.html","sincro.c","Trama 'X' Enviada", sTrama, 1);

}



/**
 * Client solicita al Servidor Sincro
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @param  sLoginOrigen {String}	Login d'usuari que inclourem
 */
void pleaseSincro (int nFdIn, char sLoginOrigen[7]) {

	char sTrama[MAX_TRAMA];
	char sLoginDesti[7];
	char sTipus = '0';
	char sData[100];

	memset (sTrama, '\0', MAX_TRAMA);

	//Posant dades adients
	strcpy(sLoginDesti, "LSBox  ");
	strcpy(sData, "'Peticio de Sincronitzacio'");
	sTipus = 'L';

	//creant Trama final que enviarem
	strncat(sTrama, sLoginDesti, 7);
	strncat(sTrama, sLoginOrigen, 7);
	sTrama[strlen(sTrama)] = sTipus;
	strncat(sTrama, sData, 100);

	//Enviem Trama
	write (nFdIn, sTrama, MAX_TRAMA);
	printf("sTrama : %s\n", sTrama);
	writeLog ("LSBox_cli.log.html", "sincroClient.c", "Peticio de Sincronitzacio", sTrama, 1);
}
