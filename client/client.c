/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "client.h"

struct dirent **arxius;

/**
 * Demana al usuari sLogin i sPswd
 */
void loginUser (char sLogin[7], char sPswd[32]) {
	char sAux[MAX];
	char sPswdMd5[MAX];
	int nValid = 0;

	//Inicialitzacions
	bzero (sLogin, MAX);
	bzero (sPswd, MAX);

	//Demana nom d'usuari
	while (!nValid) {
		sprintf (sAux, "\n Username: ");
		write (1, sAux, strlen(sAux));
		read (0, sLogin, 7);
		if (strlen(sLogin) == 7) {
			nValid = 1;
		} else {
			sprintf (sAux, "\n Username  ha de ser de 7 caracters: ");
			write (1, sAux, strlen(sAux));
		}
	}
	//sLogin[strlen(sLogin)-1] = '\0';

	//Reiniciem variable
	nValid = 0;

	//Demana contrasenya
	while (!nValid) {
		sprintf (sAux, "\n Password: ");
		write (1, sAux, strlen(sAux));
		getchar();
		read (0, sPswd, 7);
		if (strlen(sPswd) == 7) {
			nValid = 1;
		} else {
			sprintf (sAux, "\n Password ha de ser de 7 caracters: ");
			write (1, sAux, strlen(sAux));
		}
	}

	//Md5
	stringToMd5 (sPswd, sPswdMd5);
	memset(sPswd, '\0', 32);
	strcpy (sPswd, sPswdMd5);
	sPswd[strlen(sPswd)] = '\0';
	printf("%s\n", sPswd);

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
		writeLog ("LSBox_cli.log.html", "client.c","[Error] scandir","Path incorrecte",0);
		exit(ERROR);
		return 0;
	} else {
		writeLog ("LSBox_cli.log.html", "client.c","scandir","Em escanejat el directori correctament",1);
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

	char sDirPath[MAX];
	char sServer[11];
	char sLogin[7];
	char sPswd[32];

	struct node *LinkedList;

	//Crear/Obrir fitxer de Log
	createLog("LSBox_cli.log.html");

	//Llegir "config.dat"
	nPort = getConfigInfo(sServer, sDirPath);

	//Guardem -> sLogin, sPswd
	loginUser(sLogin, sPswd);
	writeLog ("LSBox_cli.log.html", "client.c", "Login del usuari", "User introdueix Login i Password",1);

	//Init LL posant tots els ele. trobats al directori root
	LinkedList = (struct node *)malloc(sizeof(struct node));
	strcpy(LinkedList->sName,"fantasma");
	LinkedList->nSize = 0;
	LinkedList->next = NULL;

	initLinkedList(sDirPath, LinkedList);

	//Socket peticio connexio
	clientConnect(nPort, sLogin, sPswd, LinkedList);


	//Check al directori si hi ha hagut algun canvi cada 2''
	while (1) {

		nLLTotalFiles = display(LinkedList);

		checkRootFiles(sDirPath, nLLTotalFiles, LinkedList);
		sleep(5);
	}

	return 0;
}