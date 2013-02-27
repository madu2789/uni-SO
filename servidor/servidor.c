/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "servidor.h"

struct dirent **arxius;

/**
 * Carrega el fitxer config.dat
 */
void getConfigInfo (char sServer[11], char nPort[3], char sDirPath[MAX]) {
	int nFdIn;

	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn) {
		write(2,"[Error] Error al obrir el fitxer 'config.dat'.\n",47);
		exit(ERROR);
	} else {
		read(nFdIn, sServer, 12);
		read(nFdIn, nPort, 3);
		read(nFdIn, sDirPath, MAX);
		close(nFdIn);

		sServer[strlen(sServer)] = '\0';
		nPort[strlen(nPort)] = '\0';
		sDirPath[strlen(sDirPath)] = '\0';
	}
	return;
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
		printf("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);
		printf("Prova amb un path correcte el proxim cop!\n");
		perror ("scandir");
		exit(0);
		return -1;
	}
	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);
	return nTotalFiles;
}



/**
 * Check al sPswd del sUser per veure si te permís per iniciar la app
 * FALTA QUE CARREGUI TOOOT EL FITXER, ARA PER ARA NOMES CARREGA LA PRIMERA FILA!!!!!!
 */
int checkUser (char sSckUser[32], char sSckPswd[32]) {

	int nFdIn;
	int i = 0;
	int bValid = 0;
	char sFileUser[32];
	char sFilePswd[32];
	char cAux = '0';
	char sAux[MAX];

	nFdIn = open("shadows.dat", O_RDONLY);
	if (-1 == nFdIn) {
		write(2,"[Error] Error al obrir el fitxer 'shadows.dat'.\n",47);
		exit(ERROR);
	} else {

		//Llegim char a char fins a ':'
		while (cAux != ':'){
			read (nFdIn, &cAux, 1);
			if (cAux != ':'){
				sAux[i] = cAux;
			}else{
				sAux[i] = '\0';
			}
			i++;
		}
		strcpy (sFileUser, sAux);

		read (nFdIn, &sFilePswd, 32);
		sFilePswd[strlen(sFilePswd)] = '\0';

		//Test
		//printf("sUser: %s\n", sFileUser);
		//printf("sPswd: %s -- %s \n", sFilePswd, sSckPswd);

		close(nFdIn);

		//s'ha de fer per tothom for(){...}
		if (strcmp(sSckUser, sFileUser) == 0) {
			if (strcmp(sSckPswd, sFilePswd) == 0) {
				bValid = 1;
			}
		}
	}
	return bValid;
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

	 	printf("cal afegir el nou arxiu\n");

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

	char sServer[11];
	char nPort[3];
	char sDirPath[MAX];
	char sSckUser[32];
	char sSckPswd[32];


	//Llegir "config.dat"
	getConfigInfo( sServer, nPort, sDirPath);

	//prova simulant llegit del socket
	strcpy(sSckUser, "boiras");
	strcpy(sSckPswd, "nooo"); 															//incorrecte
	strcpy(sSckPswd, "0cabc558f4d02a1bcaeaeb280d41a6ed");		//correcte

	//Comprobar sSckUser i sSckPwd
	if( !checkUser(sSckUser, sSckPswd) ) {
		write(2, "[Error] Autentificacio fallida.\n", 33);
		exit(ERROR);
	}

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