/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"

struct dirent **arxius;

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
int ReadDir (char sDirPath[MAX]) {

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius == NULL) {
		writeLog ("LSBox_svr.log.html", "servidor.c","[Error] scandir","Path incorrecte",0);
		write(2,"[Error] Prova amb un path correcte el proxim cop!\n",51);
		perror ("scandir");
		exit(0);
		return -1;
	} else {
			writeLog ("LSBox_svr.log.html", "servidor.c","scandir","Em escanejat el directori correctament",1);
	}
	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);
	return nTotalFiles;
}


/**
 * Inicialitza la LinkedList posant tos els elements del directori a la LL
 */
int initLinkedList (char sDirPath[MAX]) {

	int nTotalFiles = ReadDir(sDirPath);
	while (nTotalFiles--) {
		addToLL(arxius[nTotalFiles]->d_name, (int)arxius[nTotalFiles]->d_type);
		free (arxius[nTotalFiles]);
	}
	free (arxius);
	return 0;
}


/**
 * Mira al directori si hi ha hagut alguna modificacio i ho gestiona la LL
 */
void checkRootFiles (char sDirPath[MAX]) {
	int i = 0;
	int bUpdate = 0;
	int nTotalFiles;
	int nLLTotalFiles;
	char sLLDate[30];

	nTotalFiles = ReadDir(sDirPath);
	nLLTotalFiles = count();
	i = nTotalFiles;

	printf("%d -- %d\n",nTotalFiles, nLLTotalFiles);

	if (nTotalFiles == nLLTotalFiles) {
		//update o res
		while (i--) {
		 	bUpdate = getDateByName(sLLDate, arxius[i]->d_name);
			if( bUpdate == 1 ) {
				updateToLL(sLLDate, arxius[i]->d_name);
			}
			free (arxius[i]);
		}
		free (arxius);

	} else if (nTotalFiles > nLLTotalFiles) {

	 	write(2, "cal afegir el nou arxiu\n", 25);

		while (i--) {
			bUpdate = getDateByName(sLLDate, arxius[i]->d_name);
			if( bUpdate != 1 ) {
				addToLL(arxius[i]->d_name, (int)arxius[i]->d_type);
			}
			free (arxius[i]);
		}
		free (arxius);

		} else if (nTotalFiles < nLLTotalFiles) {
			removeToLL(nTotalFiles, nLLTotalFiles, arxius);
		}
	return;
	}


/**
 * main general
 */
int main () {

	int bError = 0;
	int nPort = 0;
	char sServer[11];
	char sDirPath[MAX];

	//Crear/Obrir fitxer de Log
	bError = createLog("LSBox_svr.log.html");

	//Llegir "config.dat"
	nPort = getConfigInfo( sServer, sDirPath);

	//Socket peticio connexio
	ServerConection(nPort);

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList(sDirPath);

	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		checkRootFiles(sDirPath);
		sleep(5);
	}

	//display(p);

	return 0;
}