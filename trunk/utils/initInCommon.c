/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "initInCommon.h"



/**
 * Carrega el fitxer config.dat
 */
int getConfigInfo (char sServer[11], char sDirPath[MAX]) {
	int nFdIn = 0;
	int nPort = 0;
	char sPort[5];

	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn) {
		write(2,"[Error] Error al obrir el fitxer 'config.dat'.\n",47);
		exit(ERROR);
	} else {
		bzero(sDirPath, MAX);
		read(nFdIn, sServer, 12);
		read(nFdIn, sPort, 5);
		read(nFdIn, sDirPath, MAX-1);
		close(nFdIn);

		sServer[strlen(sServer)] = '\0';
		sPort[strlen(sPort)] = '\0';
		sDirPath[strlen(sDirPath)] = '\0';

		//Convertim el nombre del port a Integer
		nPort = atoi(sPort);
	}
	return nPort;
}


/**
 * Fp necessaria per llegir el directori
 * @param  arg {struct dirent} path al directori
 */
static int triar (const struct dirent *arg) {
	if (strcmp (arg->d_name, ".") == 0 || strcmp (arg->d_name, "..") == 0 ) return 0;
	return 1;
}

/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
int ReadDir (int bIsNull, char sMyLog[20]) {

	if (bIsNull == 0) {
		writeLog (sMyLog, "client.c","[Error] scandir","Path incorrecte",0);
		exit(ERROR);
		return 0;
	} else {
		writeLog (sMyLog, "client.c","scandir","Em escanejat el directori correctament",1);
	}
	return 0;
}


/**
 * Inicialitza la LinkedList posant tos els elements del directori a la LL
 */
int initLinkedList (char sDirPath[MAX], struct node *LinkedList, char sMyLog[20]) {
	int bArxiusOk = 0;
	struct dirent **arxius;

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius != NULL) bArxiusOk = 1;
	ReadDir(bArxiusOk, sMyLog);
	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);

	while (nTotalFiles--) {
		addToLL(arxius[nTotalFiles]->d_name, (int)arxius[nTotalFiles]->d_type, LinkedList, sMyLog);
		free (arxius[nTotalFiles]);
	}
	free (arxius);
	return nTotalFiles;
}


/**
 * Mira al directori si hi ha hagut alguna modificacio i ho gestiona la LL
 */
void checkRootFiles (char sDirPath[MAX], int nLLTotalFiles, struct node *LinkedList, char sMyLog[20]) {

	struct dirent **arxius;
	int i = 0;
	int bUpdate = 0;
	int nTotalFiles = 0;
	int bArxiusOk = 0;
	char sLLDate[30];

	nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius != NULL) bArxiusOk = 1;
	ReadDir(bArxiusOk, sMyLog);
	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);

	i = nTotalFiles;

	printf("%d -- %d\n",nTotalFiles, nLLTotalFiles);

	if (nTotalFiles == nLLTotalFiles) {
		//update o res
		while (i--) {
		 	bUpdate = getDateByName(sLLDate, arxius[i]->d_name, LinkedList);
			if( bUpdate == 1 ) {
				updateToLL(sLLDate, arxius[i]->d_name, LinkedList, sMyLog);
			}
			free (arxius[i]);
		}
		free (arxius);

	} else if (nTotalFiles > nLLTotalFiles) {

	 	write(2, "cal afegir el nou arxiu\n", 25);

		while (i--) {
			bUpdate = getDateByName(sLLDate, arxius[i]->d_name, LinkedList);
			if( bUpdate != 1 ) {
				addToLL(arxius[i]->d_name, (int)arxius[i]->d_type, LinkedList, sMyLog);
			}
			free (arxius[i]);
		}
		free (arxius);

		} else if (nTotalFiles < nLLTotalFiles) {
			removeToLL(nTotalFiles, nLLTotalFiles, arxius, LinkedList, sMyLog);
		}
	return;
	}


