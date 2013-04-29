/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "sincro.h"




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
	char sSize[2];

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
			//nsize
			sprintf(sSize, "%d", nSize);
			strncat(sData, sSize, 3);

			sData[strlen(sData)-1] = '>';
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



/**
 * Servidor envia Trama al Client perque comenci a tx
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sLoginDesti {String}	Login d'usuari que inclourem
 * @return bCorrect {0 wrong | 1 right}
 */
int startSincro (int nFdIn, char sTrama[MAX_TRAMA], char sLoginDesti[7]) {

	//Creem la Trama 'S'
	creaTrama(sTrama, "LsBox  ", sLoginDesti, 4);

	//Enviem la trama de peticio d'autentificacio
	write (nFdIn, sTrama, MAX_TRAMA);
	printf("trama enviada: %s\n", sTrama);
	writeLog ("LSBox_svr.log.html","sincro.c","Trama Enviada", sTrama, 1);

	return 0;
}




/**
 * Client envia una Trama O|E  i crida a SetSincroInfo
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sLoginOrigen {String}	Login d'usuari que inclourem
 * @param  LinkedList {Struct}	Estructura On guardem tota la info
 * @return bCorrect {0 wrong | 1 right}
 */
int receiveServerSincro (int nFdIn, char sTrama[MAX_TRAMA], char sLoginOrigen[7], struct node *LinkedList) {

	read (nFdIn, sTrama, MAX_TRAMA);
	printf("trama rebuda:  %s\n", sTrama);
	writeLog ("LSBox_cli.log.html","sincro.c","Trama Rebuda", sTrama, 1);


	//Enviem la trama 'O' o 'E' sincronitzacio confirmada
	creaTrama(sTrama, sLoginOrigen, "LsBox  ", 4);
	write (nFdIn, sTrama, MAX_TRAMA);
	printf("trama enviada: %s\n", sTrama);
	writeLog ("LSBox_cli.log.html","sincro.c","Trama 'O' Enviada", sTrama, 1);

	//Comencem a enviar tota la LL -> trames 'N'
	setSincroInfo(nFdIn, sLoginOrigen, LinkedList);


	return 0;
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


 	for (nLocation = 1 ; nLocation < nTotalFiles ; nLocation++ ) {

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
 * Servidor rep la informacio del Client en trames N
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @return bCorrect {0 wrong | 1 right}
 */
void getSincroInfo (int nFdIn) {
	char sTrama[MAX_TRAMA];
	char sLoginOrigen[8];
	char sLoginDesti[8];
	char sPwd[33];
	int bFinalSincro = 0;

	while (!bFinalSincro)	{

		//Rebem Trames de Sincro amb les dades del client
		read (nFdIn, sTrama, MAX_TRAMA);
		printf ("trama rebuda: %s\n", sTrama);
		bFinalSincro = checkTrama (sTrama, sLoginOrigen, sLoginDesti, sPwd, 4);

		//crear una nova Linked list i posar tots els elements a transmetre

	}

}