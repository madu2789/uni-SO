/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "transfer.h"




/**
 * Crea la Trama de peticio de coneccio per al servidor
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sUser {String}	Login d'usuari que inclourem
 * @param  sPwd {String}	Password amb md5 que inclourem
 * @param  bEmissor {Integer}	numero que diu si l'emissor es client o servidor
 * @return
 */
void creaTramaTx (char sTrama[MAX_TRAMA], char sUser[7], char sName[30], char sDate[120], int nSize, int bEmissor) {
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
			//nSize
			sprintf(sSize, "%d", nSize);
			strcat(sData, sSize);

			sData[strlen(sData)] = '>';
			sData[strlen(sData)] = '\0';
			sTipus = 'M';
		break;
		case 2:
			//creant Camps de la trama separadament
			strcpy(sLoginOrigen, "LSBox  ");
			strcpy(sLoginDesti, sUser);
			strcpy(sData, sDate);
			sTipus = 'T';
			
		break;
		case 3:
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox  ");
			sTipus = 'X';
			strcpy(sData, "'Transferencia finalitzada'");
		break;
		case 4:	//Remove
			//creant Camps de la trama separadament
			strcpy(sLoginOrigen, sUser);
			strcpy(sLoginDesti, "LSBox  ");
			sData[0] = '<';
			strncat(sData, sName, 20);

			sData[strlen(sData)] = '>';
			sData[strlen(sData)] = '\0';
			sTipus = 'R';
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
int checkTramaTx (char sTrama[MAX_TRAMA], char sLoginOrigen[8], char sLoginDesti[8], char sDataTrama[101]) {

	int bTramaOk = 0;
	//Camps de la trama:
	char sLoginDTrama[8];
	char sTypeTrama;
	char sPwd[33];

	//Netejant strings
	memset(sLoginOrigen, '\0', 8);
	memset(sLoginDTrama, '\0', 8);
	memset(sPwd, '\0', 33);
	memset(sDataTrama, '\0', 101);

	//Parseig de la trama
	memcpy( sLoginOrigen, &sTrama[0], 7 );
	sLoginOrigen[7] = '\0';

	memcpy( sLoginDTrama, &sTrama[7], 7 );
	sLoginDTrama[7] = '\0';

  sTypeTrama = sTrama[14];

  memcpy( sDataTrama, &sTrama[15], 100);
  sDataTrama[strlen(sDataTrama)] = '\0';

  memcpy( sPwd, &sDataTrama[8], 32);
	sPwd[32] = '\0';

	bTramaOk = -2;
	switch (sTypeTrama) {
		//cas autentificacio
		case 'M':
				bTramaOk = 1;
		break;
		case 'T':
				bTramaOk = 0;
		break;
		case 'X':
				bTramaOk = -1;
		break;
		case 'R':
				bTramaOk = 2;
	}

	return bTramaOk;
}



void ParserNameTx (char Frase[100], char sName[24]) {

	int j = 0;

	while (Frase[j+1] != '>') {
		sName[j] = Frase[j+1];
		j++;
	}
}



/**
 * Server parseja les trames per Sincro
 * @param  Frase {String}	cadena que enviarem
 * @param  sLoginOrigen {String}	Login d'usuari que inclourem
 * @param  sData {String}	data que adjuntarem
 * @return nSize {Integer} tamany del arxiu
 */
int ParserBuclesTx (char Frase[100], char sName[24]) {
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
	z = 0;
	while (Frase[j+1] != '>') {
		sSize[z] = Frase[j+1];
		j++;
		z++;
	}

	sName[strlen (sName)] = '\0';
	sSize[strlen (sSize)] = '\0';
	nSize = atoi (sSize);

	return nSize;
}



int openFile (char sDirPath[MAX], char sName[30], char sMyLog[40]) {

	char sRealDirPath[MAX+30];
	memset (sRealDirPath, '\0', MAX+30);
	strcat (sRealDirPath, sDirPath);
	strcat (sRealDirPath, sName);

	pathExists (sDirPath, sMyLog);

	int nFdIn = open (sRealDirPath, O_RDWR);

	if (-1 == nFdIn){
		return 0;
	} else {
		return nFdIn;
	}
}



int createFile (char sDirPath[MAX], char sName[30]) {
	char sRealDirPath[MAX+30];
	memset (sRealDirPath, '\0', MAX+30);
	strcat (sRealDirPath, sDirPath);
	strcat (sRealDirPath, sName);

	int nFdIn = open (sRealDirPath, O_CREAT, 0666);
	close (nFdIn);
	return nFdIn;
}



int removeFile (char sDirPath[MAX], char sName[30]) {
	char sRealDirPath[MAX+100];

	memset (sRealDirPath, '\0', MAX+100);
	strcat (sRealDirPath, sDirPath);
  strcat (sRealDirPath, sName);

	return remove(sRealDirPath);
}




int transferContent (int nFdSocket, char sDirPath[MAX], char sUser[8], struct node *LinkedListToTx, char sMyLog[40], sem_t *semLL) {

	int nTotalFiles = 0;
	int i = 0;
	int nSize = 0;
	int nEstat = 0;
	int nFdFitxer = 0;
	int bFi = 0;
	int nEstatPerEnviar1 = 0, nEstatPerEnviar2 = 0;

	char sName[30];
	char sData[64];
	char sTrama[MAX_TRAMA];
	char sInfo[105];

	sem_wait(semLL);
	nTotalFiles = count (LinkedListToTx);
	display(LinkedListToTx);
	sem_post(semLL);
	
	for (i = 1; i < nTotalFiles+1; i++) {
		memset (sName, '\0', 30);
		memset (sData, '\0', 64);

		sem_wait(semLL);
		nSize = showNode (sName, sData, i, LinkedListToTx);
		nEstat = getEstatByName (sName, LinkedListToTx);
		sem_post(semLL);

		//Aixo es arriscat i TREPITXEROOOOO!!!!!
		if (!strcmp (sMyLog, "LSBox_cli.log.html")){
			nEstatPerEnviar1 = 4;
			nEstatPerEnviar2 = 5;

		} else {
			nEstatPerEnviar1 = 1;
			nEstatPerEnviar2 = 2;
		}

		//Si estat == a CLI_ADD o CLI_UPDATE
		if (nEstat == nEstatPerEnviar1 || nEstat == nEstatPerEnviar2) {
			bFi = 1;
			nFdFitxer = openFile (sDirPath, sName, sMyLog);
			if ( 0 ==  nFdFitxer ) {
				//creem i enviem trama 'X'
				memset(sTrama, '\0', MAX_TRAMA);
				creaTramaTx (sTrama, sUser, sName, sInfo, nSize, 3);
				write (nFdSocket, sTrama, MAX_TRAMA);
				printf ("Trama enviada: %s\n", sTrama);
				writeLog (sMyLog, "transfer.c", "Trama Enviada", sTrama, 1);		

				pthread_exit(NULL);
			}

			//Creo la 1a trama 'M' amb info basica del fitxer
			memset(sTrama, '\0', MAX_TRAMA);
			creaTramaTx (sTrama, sUser, sName, sData, nSize, 1);
			
			while ( bFi != 0 ) {
				write (1, sTrama, strlen (sTrama));
				write (1, "\n ", strlen ("\n "));
				write (nFdSocket, sTrama, MAX_TRAMA);
				writeLog (sMyLog, "transfer.c", "Trama Enviada", sTrama, 1);
				memset (sInfo, '\0', 104);
				bFi = read (nFdFitxer, sInfo, 100);
				memset(sTrama, '\0', MAX_TRAMA);
				creaTramaTx (sTrama, sUser, sName, sInfo, nSize, 2);
			}
		} else if (nEstat == 3) { //Si estat == CLI_RM
			//Creo la 1a trama 'R' amb info basica del fitxer
			memset(sTrama, '\0', MAX_TRAMA);
			creaTramaTx (sTrama, sUser, sName, sData, nSize, 4);
			write (1, sTrama, strlen (sTrama));
			write (1, "\n ", strlen ("\n "));
			write (nFdSocket, sTrama, MAX_TRAMA);
			writeLog (sMyLog, "transfer.c", "Trama Enviada", sTrama, 1);
		}
	}
	
	//creem i enviem trama 'X'
	memset(sTrama, '\0', MAX_TRAMA);
	creaTramaTx (sTrama, sUser, sName, sInfo, nSize, 3);
	write (nFdSocket, sTrama, MAX_TRAMA);
	write (1, sTrama, strlen (sTrama));
	write (1, "\n ", strlen ("\n "));
	writeLog (sMyLog, "transfer.c", "Trama Enviada", sTrama, 1);

	return 0;
}



//aqui creara un fitxer i l'obrira per escriure les trames de info
void receiveContent (int nFdIn, char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20], sem_t *semLL) {

	int nSize = 0;
	int nTipusTrama = 0;
	int bCopiant = 0;
	int nFileFd = 0;
	char sName[24];
	char sData[30];
	char sTrama[MAX_TRAMA];
	char sLoginOrigen[8]; char sLoginDesti[8]; char sDataTrama[101];
	char sRealDirPath[MAX+30];

	char sSpecialPath[MAX];

	memset(sTrama, '\0', MAX_TRAMA);
	read (nFdIn, sTrama, MAX_TRAMA);
	write (1, sTrama, strlen (sTrama));
	write (1, "\n ", strlen ("\n "));
	writeLog (sMyLog, "transfer.c", "Trama Rebuda", sTrama, 1);

	memset(sSpecialPath, '\0', MAX);
	strcpy(sSpecialPath, sDirPath);

	nTipusTrama = checkTramaTx (sTrama, sLoginOrigen, sLoginDesti, sDataTrama);

	while ( nTipusTrama != -1 )	{

		switch (nTipusTrama) {

			case 1:	//trama 'M'	
				memset(sName, '\0', 24);
				nSize = ParserBuclesTx (sDataTrama, sName);

				// Intenten Obrim el fitxer per llegir sino, el creem
				nFileFd = 0;
				nFileFd = openFile (sDirPath, sName, sMyLog);
				if ( !nFileFd ) {
					nFileFd = createFile(sDirPath, sName);
					//l'ageixo ala LL perque no noti canvi i demani sincro!

					sem_wait(semLL);
					addToLL(sDirPath, sName, 1, LinkedList, LinkedListToTx, sMyLog);
					sem_post(semLL);
					nFileFd = openFile (sDirPath, sName, sMyLog);
				}

				bCopiant = 0;
				memset(sTrama, '\0', MAX_TRAMA);
				memset(sDataTrama, '\0', 101);

				read (nFdIn, sTrama, MAX_TRAMA);
				write (1, sTrama, strlen (sTrama));
				write (1, "\n ", strlen ("\n "));
				writeLog (sMyLog, "transfer.c", "Trama Rebuda", sTrama, 1);
				bCopiant = checkTramaTx (sTrama, sLoginOrigen, sLoginDesti, sDataTrama);
				sDataTrama[strlen(sDataTrama)] = '\0';
	
				while ( bCopiant == 0) {
					write (nFileFd, sDataTrama, strlen(sDataTrama));

					memset(sDataTrama, '\0', 101);
					memset(sTrama, '\0', MAX_TRAMA);
					read (nFdIn, sTrama, MAX_TRAMA);
					write (1, sTrama, strlen (sTrama));
					write (1, "\n ", strlen ("\n "));
					writeLog (sMyLog, "transfer.c", "Trama Rebuda", sTrama, 1);
					bCopiant = checkTramaTx (sTrama, sLoginOrigen, sLoginDesti, sDataTrama);
					sDataTrama[strlen(sDataTrama)] = '\0';
				}

				nTipusTrama = bCopiant;
				close(nFileFd);

				//Actualitzo les Dates a les LL perque no noti el canvi i torni a demanar una Sincro!
				memset (sData, '\0', 30);

				sem_wait(semLL);
				getDateReal(sData, sDirPath, sName);
				setDateByName (sName, sData, 0, LinkedList);

				//Actualitzo tambe el actual tamany del arxiu
				setSizeByName(sName, nSize, LinkedList);
				sem_post(semLL);
			break;
			case 2: //trama	'R' remove
				memset(sName, '\0', 24);
			
				ParserNameTx(sDataTrama, sName);
				
				removeFile (sSpecialPath, sName);

				memset(sTrama, '\0', MAX_TRAMA);
				read (nFdIn, sTrama, MAX_TRAMA);
				write (1, sTrama, strlen (sTrama));
				write (1, "\n ", strlen ("\n "));
				writeLog (sMyLog, "transfer.c", "Trama Rebuda", sTrama, 1);
				nTipusTrama	= checkTramaTx (sTrama, sLoginOrigen, sLoginDesti, sDataTrama);

			break;
		}
	}
}

