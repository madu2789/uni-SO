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



/**
 * Servidor envia Trama al Client perque comenci a tx
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sLoginDesti {String}	Login d'usuari que inclourem
 * @return bCorrect {0 wrong | 1 right}
 */
int startSincro (int nFdIn, char sLoginDesti[7]) {

	char sTrama[MAX_TRAMA];

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

 	//	REVISAR!!!
 	//Aixo es raro i sa de ajustar:
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


int ParserBucles (char Frase[50], char sName[24],	char sData[24]) {
	int nSize = 0;
	char sSize[10];
	int j, z;

	j = z = 0;
	memset(sSize, '\0', 10);

	while (Frase[j+1] != '>') {
		sName[j] = Frase[j+1];
		j++;
	}

	j += 3;
	while (Frase[j+1] != '>') {
		sData[z] = Frase[j+1];
		j++;
		z++;
	}

	j += 3;
	z = 0;
	while (Frase[j+1] != '>') {
		sSize[z] = Frase[j+1];
		j++;
		z++;
	}

	sName[strlen (sName)] = '\0';
	sData[strlen (sData)] = '\0';
	sSize[strlen (sSize)] = '\0';
	nSize = atoi (sSize);

	return nSize;
}



int decideWhoUpdate (char sDataTrama[24], char sDataLL[24]) {

	char sDataTramaShort[24];
	char sDataLLShort[24];
	int nResult = 0;

	memset(sDataTramaShort, '\0', 24);
	memset(sDataLLShort, '\0', 24);

	strncpy (sDataTramaShort, sDataTrama+3, 21);
	strncpy (sDataLLShort, sDataLL+3, 21);
	sDataTramaShort [strlen (sDataTramaShort)] = '\0';
	sDataLLShort [strlen (sDataLLShort)] = '\0';

	nResult = strcmp (sDataTramaShort, sDataLLShort);

	printf("sData: %s - sDataLL %s\n", sDataTramaShort, sDataLLShort);

	return nResult;
}



/**
 * Servidor rep la informacio del Client en trames N
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @return bCorrect {0 wrong | 1 right}
 */
void getSincroInfo (int nFdIn, struct node *LinkedList, struct node *LinkedListToTx) {
	
	int bFinalSincro, bTrobat, i, nNumberOfSincroElemets, nSize, nWhoUpdate;
	char sTrama[MAX_TRAMA];
	//chars que realment no em caldrien... ¬¬
	char sLoginOrigen[8];
	char sLoginDesti[8];
	char sPwd[33];
	//Vars
	char ArrayInfo[50][50];
	char sName[24];
	char sDataTrama[24];
	char sDataLL[24];


	bFinalSincro = bTrobat = i = nNumberOfSincroElemets = nSize = nWhoUpdate = 0;
	
	while (!bFinalSincro)	{

		memset(sTrama, '\0', MAX_TRAMA);

		//Rebem Trames de Sincro amb les dades del client
		read (nFdIn, sTrama, MAX_TRAMA);
		printf ("trama rebuda: %s\n", sTrama);
		bFinalSincro = checkTrama (sTrama, sLoginOrigen, sLoginDesti, sPwd, 4);

	  strncpy (ArrayInfo[nNumberOfSincroElemets], sTrama+15, 100);
	  ArrayInfo[nNumberOfSincroElemets][strlen(ArrayInfo[nNumberOfSincroElemets])] = '\0';

	  nNumberOfSincroElemets++;
	}
  

  //var i comença a 1 per saltarnos la trama de inici sincro
  //nNumberOfSincroElements -1 per saltarnos la trama final sincro
	for (i = 1; i < nNumberOfSincroElemets-1; i++) {
		memset(sName, '\0', 24);
		memset(sDataTrama, '\0', 24);
		memset(sDataLL, '\0', 24);

		//Parseja la info de les trames N (sincro)
		nSize = ParserBucles (ArrayInfo[i], sName, sDataTrama);

 		//Buscar la nostra Data del element Rebut
		bTrobat = getDateByName (sDataLL, sName, LinkedList);
		printf("info: sName %s , sDataTrama %s, sDataLL %s\n", sName, sDataTrama, sDataLL);

		if ( bTrobat == 0) {
			//no trobat, client ma d'enviar el fitxer
			printf("SERVER_ADD: que me lenvii!\n");
			addToLLTx (sName, sDataTrama, nSize, 4, LinkedListToTx);

		} else {

			//distingir si REMOVE | UPDATE mirant el camp estat pero ala LLTx

			nWhoUpdate = decideWhoUpdate (sDataTrama, sDataLL);
			if ( nWhoUpdate > 0 ) {
				printf("SER_UPDATE:  Client envia a Servidor\n");
				addToLLTx (sName, sDataTrama, nSize, 5, LinkedListToTx);

			} else if ( nWhoUpdate < 0 ) {
				printf("CLI_UPDATE: Servidor envia a client\n");
				addToLLTx (sName, sDataTrama, nSize, 2, LinkedListToTx);

			} else if (nWhoUpdate == 0)  {
				printf("ningu envia res! les dates son iguals\n");
			}
		}
	
		printf("llistaTx:\n");
	  display (LinkedListToTx);

	  printf("llista:\n");
	  display (LinkedList);
	}

	//aqui s'hauria de mirar ADD_CLI comparar llistes LL i LLTx (for)
}
