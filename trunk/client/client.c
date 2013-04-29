/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "client.h"



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