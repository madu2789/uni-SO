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
int startSincro (int nFdIn, char sLoginDesti[7]) {

	char sTrama[MAX_TRAMA];

	printf("volem sincroo\n");

	//Creem la Trama 'S'
	creaTrama(sTrama, "LsBox  ", sLoginDesti, 4);

printf("trama creada\n");

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
void getSincroInfo (int nFdIn, struct node *LinkedList) {
	char sTrama[MAX_TRAMA];
	char sLoginOrigen[8];
	char sLoginDesti[8];
	char sPwd[33];

	int bFinalSincro = 0;
	int i = 0;
	int j = 0;
	int z = 0;
	char ArrayInfo[50][50];
	int nNumberOfSincroElemets = -1;

	char sName[24];
	char sData[24];
	char sDataLL[24];
	char sSize[8];

	struct node *LinkedListToTx;

	//Llinked list que contindra els elements a Tx
	LinkedListToTx = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedListToTx->sName, "fantasma");
	LinkedListToTx->nSize = 0;
	LinkedListToTx->next = NULL;

	memset(sName, '\0', 24);
	memset(sData, '\0', 24);
	memset(sDataLL, '\0', 24);

	while (!bFinalSincro)	{

		//Rebem Trames de Sincro amb les dades del client
		read (nFdIn, sTrama, MAX_TRAMA);
		printf ("trama rebuda: %s\n", sTrama);
		bFinalSincro = checkTrama (sTrama, sLoginOrigen, sLoginDesti, sPwd, 4);

	  strncpy (ArrayInfo[nNumberOfSincroElemets], sTrama+15, 100);
	  ArrayInfo[nNumberOfSincroElemets][strlen(ArrayInfo[nNumberOfSincroElemets])] = '\0';

	  nNumberOfSincroElemets++;
	}

  //aqui toca parsejar la data ens els tres camps

	for (i = 1; i < nNumberOfSincroElemets; i++) {
		j = 0;
		z = 0;
		memset(sName, '\0', 24);
		memset(sData, '\0', 24);

		printf("sPwd: %s - %d \n\n", ArrayInfo[i], i);

		while (ArrayInfo[i][j+1] != '>') {
			sName[j] = ArrayInfo[i][j+1];
			j++;
		}

		j += 3;

		while (ArrayInfo[i][j+1] != '>') {
			sData[z] = ArrayInfo[i][j+1];
			j++;
			z++;
		}

		sName[strlen(sName)-1] = '\0';
		sData[strlen(sData)-1] = '\0';
		printf("sName %s\n", sName);
		printf("sData %s\n", sData);

 	//comprobar de laltre LL si cal actualitzar, si cal inserir a la LLTx

		getDateByName (sDataLL, sName, LinkedList);
		printf("data de la LL %s\n", sDataLL);

	}


  //comprobar de laltre LL si cal actualitzar, si cal inserir a la LLTx


	//Omplir LLTx amb els elements a transmetre

	//addToLLTx (sTrama, LinkedListToTx);
}