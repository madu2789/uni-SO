/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "sincroServer.h"



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
		case 4:
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox  ");
			sTipus = 'G';
			sData[0] = '<';
			strncat(sData, sName, 20);
			sData[strlen(sData)] = '>';
			sData[strlen(sData)] = '\0';
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
	creaTramaServidor(sTrama, "LsBox  ", sLoginDesti, 4);

	//Enviem la trama de peticio d'autentificacio
	write (nFdIn, sTrama, MAX_TRAMA);
	printf("trama enviada: %s\n", sTrama);
	writeLog ("LSBox_svr.log.html","sincro.c","Trama Enviada", sTrama, 1);

	return 0;
}





/**
 * Client envia les trames amb la info N, i la trama de final X
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @param  sLoginOrigen {String}	Login d'usuari que inclourem
 * @param  LinkedList {Struct}	Estructura On guardem tota la info
 * @return bCorrect {0 wrong | 1 right}
 */
int receiveClientSincro (int nFdIn) {
	char sTrama[MAX_TRAMA];
	int nBytesRead = 0;

	//Comprovem si realment el server ens ha confirmat sincro
	nBytesRead = read (nFdIn, sTrama, MAX_TRAMA);
	if (nBytesRead <= 4) return 0;

	return 1;
}





void sendTramesG (int nFdIn, char sLoginOrigen[8], struct node *LinkedListToTx, int nIdMyClient) {

	int nNumberOfElements = count(LinkedListToTx);
	int i, bTrobat = 0;
	char sName[24];
	char sDataLL[24];
	char sUser[8];
	int nEstat = 0;
	char sTrama[MAX_TRAMA];

	strcpy(sUser, sLoginOrigen);

	for (i = 1; i < nNumberOfElements+1; i++) {
		showNode(sName, sDataLL, i, LinkedListToTx);

		nEstat = getEstatByName(sName, LinkedListToTx);
		printf("sName: %s, estat: %d\n",sName, nEstat);
		if (nEstat == 4 || nEstat == 5) {
			//crea i envia trama G -> 4
			creaTramaSincro (sTrama, sUser, sName, "  ", 0, 4);
			printf("sTrama: %s\n", sTrama); 
			write (nFdIn, sTrama, MAX_TRAMA);
			writeLog ("LSBox_svr.log.html", "sincroServer.c","Trama enviada", sTrama, 1);
		}
	}
	//trama X
	creaTramaSincro (sTrama, sUser, " ", " ", 0, 3);
	printf("sTrama: %s\n", sTrama); 
	write (nFdIn, sTrama, MAX_TRAMA);
	writeLog ("LSBox_svr.log.html", "sincroServer.c","Trama enviada", sTrama, 1);

}




/**
 * Server parseja les trames per Sincro
 * @param  Frase {String}	cadena que enviarem
 * @param  sLoginOrigen {String}	Login d'usuari que inclourem
 * @param  sData {String}	data que adjuntarem
 * @return nSize {Integer} tamany del arxiu
 */
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


/**
 * Server Mira les dates i decideix quin es mes recent
 * @param  sDataTrama {String}	Data client
 * @param  sDataLL {String}		Data servidor
 * @return {-1 servidor | 0 iguals | 1 client}
 */
int decideWhoUpdate (char sDataTrama[24], char sDataLL[24]) {
	struct tm tmDataTrama, tmDataLL;
	time_t tDataTrama, tDataLL;

	// Passem de string a time_t per comparar les dates
	strptime(sDataTrama, "%A %b %d %H:%M:%S %Y", &tmDataTrama);
	tDataTrama = mktime(&tmDataTrama);

	strptime(sDataLL, "%A %b %d %H:%M:%S %Y", &tmDataLL);
	tDataLL = mktime(&tmDataLL);

	if(  abs(tDataTrama-tDataLL) <= 35 )	return 0;
	if( tDataTrama > tDataLL )						return 1;
	else if( tDataTrama < tDataLL )				return -1;
	else																	return 0;
}



/**
 * Servidor rep la informacio del Client en trames N
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @return bCorrect {0 wrong | 1 right}
 */
void getSincroInfo (int nFdIn, char sLoginUser[8], struct node *LinkedList, struct node *LinkedListToTx, int nIdMyClient) {
	
	int bFinalSincro, bTrobat, i, nNumberOfSincroElemets, nNumberOfElements, nSize, nWhoUpdate, nEstat;
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


	bFinalSincro = bTrobat = i = nNumberOfSincroElemets = nSize = nWhoUpdate = nEstat = nNumberOfElements = 0;
	
	while (!bFinalSincro)	{

		memset(sTrama, '\0', MAX_TRAMA);

		//Rebem Trames de Sincro amb les dades del client
		read (nFdIn, sTrama, MAX_TRAMA);
		printf ("trama rebuda: %s\n", sTrama);
		writeLog ("LSBox_svr.log.html", "sincroServer.c","Trama rebuda", sTrama, 1);
		bFinalSincro = checkTramaServidor (sTrama, sLoginOrigen, sLoginDesti, sPwd, 4);

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

		if ( bTrobat == 0 ) {
			if ( !getDateByName(sDataLL, sName, LinkedListToTx)) {
				//no trobat, client ma d'enviar el fitxer
				printf("SERVER_ADD: que me lenvii!\n");
				addToLLTx (sName, sDataTrama, nSize, 4, LinkedListToTx);
			}
		} else {
			
			//UPDATE
			nWhoUpdate = decideWhoUpdate (sDataTrama, sDataLL);
			if ( !getDateByName(sDataLL, sName, LinkedListToTx)) { //sino esta l'afegeixo
				if ( nWhoUpdate == 0 ) 	setEstatByName (sName, 8, LinkedListToTx);
				if ( nWhoUpdate > 0 ) {
					printf("SER_UPDATE: %s Client envia a Servidor\n", sName);
					addToLLTx (sName, sDataTrama, nSize, 5, LinkedListToTx);
				} else if ( nWhoUpdate < 0 ) {
					printf("CLI_UPDATE: %s Servidor envia a client\n", sName);
					addToLLTx (sName, sDataTrama, nSize, 2, LinkedListToTx);
				}
			} else {	
				if ( nWhoUpdate == 0 ) 	setEstatByName (sName, 8, LinkedListToTx);																						//si esta actualitzo nEstat
				if ( nWhoUpdate > 0 ) {
					printf("SER_UPDATE: %s Client envia a Servidor\n", sName);
					setEstatByName (sName, 5, LinkedListToTx);
				} else if ( nWhoUpdate < 0 ) {
					printf("CLI_UPDATE: %s Servidor envia a client\n", sName);
					setEstatByName (sName, 2, LinkedListToTx);
				}
			}
		}
	}

	//Inicialitzem vars per la segona comprovacio
	//que NO rebut pero SI que el tinc: SER_RM 	pendeeeent
	nNumberOfElements = count (LinkedList);
	bTrobat = 0;

	for (i = 1; i < nNumberOfElements+1; i++) {
		showNode(sName, sDataLL, i, LinkedList);
		bTrobat = getDateByName (sDataLL, sName, LinkedListToTx);
		if ( !bTrobat ) {
			printf("SER_RM: %s Servidor envia a Client\n", sName);
			setEstatByName (sName, 6, LinkedListToTx);
		}	
	}

	//Trames G que envia el servidor al client pk sapigui quins fitxers enviar
	sendTramesG(nFdIn, sLoginUser, LinkedListToTx, nIdMyClient);

  printf("llistaTx:\n");
	display (LinkedListToTx);

	printf("llista:\n");
	display (LinkedList);
}
