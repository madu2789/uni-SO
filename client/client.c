/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define ERROR -1
#define OK 0
#define MAX 64




void leerString (int nFichero, char **ppsString, int *pnError){
	int i = 0;
	char cAux = '0';
	char sAux[MAX];
	
	//Leemos caracter a caracter mientras no haya salto de linea
	while (cAux != '\n'){
		read (nFichero,&cAux,1);
		if (cAux != '\n'){
			sAux[i] = cAux;
		}else{
			sAux[i] = '\0';
		}
		i++;
	}
	//Pedimos memoria para la cadena
	*ppsString = malloc ((strlen (sAux)+1)*sizeof (char));
	//Si no se puede pedir memoria
	if (NULL == *ppsString){
		(*pnError) = ERROR; 
	}else{
		strcpy ((*ppsString),sAux);
		(*pnError) = OK;
	}
}




void loginUser (char sLogin[MAX], char sPswd[MAX]) {
	int nAuthOK = 0;
	char sAux[MAX];
	
	//Demana autenticació mentre no sigui correcta
	do{
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
		nAuthOK = 1;
		//Verifica dades
		//WIP: Crida a la verificació
		/*if (ERROR == flagAuth) {
			sprintf (sAux, "Authentication failed!\n");
			write (1, sAux, strlen (sAux));
		}else{
			//Autenticació correcta
			nAuthOK = 1;
		}*/
	}while (!nAuthOK);
}




int getConfigInfo(char sServer[MAX], char sDirPath[MAX]) {
	int nFdIn;
	int nPort = 0;
	
	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn){
		write(2,"[Error] Error al obrir el fitxer 'config.dat'.\n",47);
		exit(ERROR);
	} else {
		
		read(nFdIn, sServer, <numbytes>);
		read(nFdIn, nPort, <numbytes>);
		read(nFdIn, sDirPath, <numbytes>);
		close(nFdIn);
	}
	
	return nPort;
}




//Funcio per filtrar entrades a scandir
int filterScandir (const struct dirent *arg) {
	if (strcmp (arg->d_name, ".") == 0 || strcmp (arg->d_name, "..") == 0)
		return 0;
	return 1;
}




void checkRootDirFiles(char sDirPath[MAX]) {
	//DIR *dirp;
	struct dirent **files;
	int nNumFiles;
	
	//Potser no cal opendir
	/*if ((dirp = opendir(sDirPath)) == NULL)
	{
		perror ("opendir");
	}*/

	nNumFiles = scandir (sDirPath, &files, filterScandir, alphasort);
	if (files == NULL) perror ("scandir");
}




int main() {
	int nError = OK;
	char sFrase[MAX];
	char sLogin[MAX];
	char sPswd[MAX];
	char sServer[MAX];
	int nPort;
	char sDirPath[MAX];

	//Autenticacio d'usuari
	loginUser(sLogin, sPswd, &nError);
	if (ERROR == nError){
		//Terminamos el programa
		sprintf (sFrase,"Error al fer login!\n");
		write (1,sFrase,strlen (sFrase));
		liberarMemoria (datos);
		return ERROR;
	}
	
	//Obtenim les dades contingudes a config.dat
	nPort = getConfigInfo(sServer, sDirPath);
	
	//Analitzem el directori root
	checkRootDirFiles(sDirPath);
	
	return 0;
}
