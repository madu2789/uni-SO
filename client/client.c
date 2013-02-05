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

char sServer[8];
char sDirPath[5MAX];


void loginUser () {
	char sAux[MAX];
	char sLogin[MAX];
	char sPswd[MAX];
	
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
		
		//Comprovacio al servidor
		
		//sprintf (sFrase,"Error al fer login!\n");
		//write (1,sFrase,strlen (sFrase));		
}

int getConfigInfo() {
	int nFdIn;
	int nPort = 0;
	
	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn){
		write(2,"[Error] Error al obrir el fitxer 'config.dat'.\n",47);
		exit(ERROR);
	} else {
		
		read(nFdIn, sServer, 8);
		read(nFdIn, nPort, 8);
		read(nFdIn, sDirPath, 8);
		close(nFdIn);
		
		//Comprovacio de fitxer ben llegit
		printf("%s", &sServer);
		printf("%s", &nPort);
		printf("%s", &sDirPath);
		
	}
	return nPort;
}

int initLinkedList(){

	
}

int checkRootFiles() {
	
}

int main() {
	char sFrase[MAX];
	int nPort = 0;

	//Autenticacio d'usuari
	loginUser();
	//Obtenim les dades contingudes a config.dat
	nPort = getConfigInfo();
	//Inicialitzem Linked List
	initLinkedList();
	//Analitzem el directori root cada 2 segons 
	checkRootFiles();
	
	return 0;
}
