/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"

struct dirent **arxius;



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
int initLinkedList (char sDirPath[MAX], struct node *LinkedList) {

	int nTotalFiles = ReadDir(sDirPath);

	while (nTotalFiles--) {
		addToLL(arxius[nTotalFiles]->d_name, (int)arxius[nTotalFiles]->d_type, LinkedList);
		free (arxius[nTotalFiles]);
	}
	free (arxius);
	return 0;
}


/**
 * Mira al directori si hi ha hagut alguna modificacio i ho gestiona la LL
 */
void checkRootFiles (char sDirPath[MAX], int nLLTotalFiles, struct node *LinkedList) {
	int i = 0;
	int bUpdate = 0;
	int nTotalFiles;
	char sLLDate[30];

	nTotalFiles = ReadDir(sDirPath);
	i = nTotalFiles;

	printf("%d -- %d\n",nTotalFiles, nLLTotalFiles);

	if (nTotalFiles == nLLTotalFiles) {
		//update o res
		while (i--) {
		 	bUpdate = getDateByName(sLLDate, arxius[i]->d_name, LinkedList);
			if( bUpdate == 1 ) {
				updateToLL(sLLDate, arxius[i]->d_name, LinkedList);
			}
			free (arxius[i]);
		}
		free (arxius);

	} else if (nTotalFiles > nLLTotalFiles) {

	 	write(2, "cal afegir el nou arxiu\n", 25);

		while (i--) {
			bUpdate = getDateByName(sLLDate, arxius[i]->d_name, LinkedList);
			if( bUpdate != 1 ) {
				addToLL(arxius[i]->d_name, (int)arxius[i]->d_type, LinkedList);
			}
			free (arxius[i]);
		}
		free (arxius);

		} else if (nTotalFiles < nLLTotalFiles) {
			removeToLL(nTotalFiles, nLLTotalFiles, arxius, LinkedList);
		}
	return;
	}


/**
 * main general
 */
int main () {

	int nPort = 0;
	int nLLTotalFiles = 0;
	char sServer[11];
	char sDirPath[MAX];

	struct node *LinkedList;

	//Crear/Obrir fitxer de Log
	createLog ("LSBox_svr.log.html");

	//Llegir "config.dat"
	nPort = getConfigInfo (sServer, sDirPath);

	//Socket peticio connexio
	ServerConection (nPort);

	//Init LL posant tots els ele. trobats al directori root
	LinkedList = (struct node *) malloc (sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->nSize = 0;
	LinkedList->next = NULL;

	//Init LL posant tots els ele. trobats al directori root
	initLinkedList (sDirPath, LinkedList);

	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {
		nLLTotalFiles = display(LinkedList);
		checkRootFiles (sDirPath, nLLTotalFiles, LinkedList);
		sleep (5);
	}

	//display(p);

	return 0;
}