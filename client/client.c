/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */

#include <unistd.h>			// write i read
#include <stdio.h>			// sprintf
#include <stdlib.h>
#include <string.h>			// strlen, bzero
#include <dirent.h>			// opendir, readdir...
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#define ERROR -1
#define OK 0
#define MAX 64

char sServer[11];
char nPort[3];
char sDirPath[MAX];
char sLogin[MAX];
char sPswd[MAX];
int nLastTotalFiles;

void loginUser () {
	char sAux[MAX];

	//Inicialitzacions
	bzero (sLogin, MAX);
	bzero (sPswd, MAX);
	//Demana nom d'usuari
	sprintf (sAux, "Username: ");
	write (1, sAux, strlen(sAux));
	read (0, sLogin, MAX);
	sLogin[strlen(sLogin)-1] = '\0';

	//Demana contrasenya
	sprintf (sAux, "Password: ");
	write (1, sAux, strlen(sAux));
	read (0, sPswd, MAX);
	sPswd[strlen(sPswd)-1] = '\0';
}

void getConfigInfo () {
	int nFdIn;
	char sServer1[MAX];

	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn){
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

static int triar (const struct dirent *arg) {
	if (strcmp (arg->d_name, ".") == 0 || strcmp (arg->d_name, "..") == 0) return 0;
	return 1;
}

int initLinkedList () {
	struct dirent **arxius;

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius == NULL) {
		perror ("scandir");
	}

	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);
	nLastTotalFiles = nTotalFiles;

	while (nTotalFiles--) {
		printf ("[%d] %s\n", nTotalFiles, arxius[nTotalFiles]->d_name);
		//append(arxius[i]->d_name)
		free (arxius[nTotalFiles]);
	}
	free (arxius);
}

int checkRootFiles () {
	int i = 0;
	struct dirent **arxius;

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius == NULL) {
		perror ("scandir");
	}
	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);

	i = nTotalFiles;
	if (nTotalFiles == nLastTotalFiles) {
		while (i--) {
			printf ("[%d] %s\n", i, arxius[i]->d_name);
			free (arxius[i]);
		}
	}else{
			//haure de recorrer els arxius i copiar el k em falti a la LL
	}

	free (arxius);
	return nTotalFiles;
}

int main() {

	loginUser();
	//Obtenim les dades contingudes a config.dat
	getConfigInfo();
	initLinkedList();

/*comprovacions
	printf("hey server: %s len %d \n", sServer, (int)strlen(sServer));
	printf("hey port: %s len %d \n", nPort, (int)strlen(nPort));
	printf("hey path: %s len %d \n", sDirPath, (int)strlen(sDirPath));
*/
	//checkRootFiles(); //cada dos segons??

	return 0;
}
