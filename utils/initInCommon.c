/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "initInCommon.h"


/**
 * Llegeix un enter al fitxer
 * @param nFichero File descriptor del fitxer
 */
int readInt (int nFichero){
	char cAux = '0';
	int nInt = 0;
	
	//AtoI mientras no haya salto de linea
	while (cAux != '\n'){
		read (nFichero,&cAux,1);
		if (cAux != '\n'){
			nInt = nInt*10 + cAux - '0';
		}
	}

	return nInt;
}




/**
 * Llegeix un string al fitxer
 * @param nFdIn		File descriptor del fitxer
 * @param ppsString Variable on guardar el string
 */
void readString (int nFdIn, char **ppsString){
	int i = 0;
	char cAux = '0';
	char sAux[MAX];
	
	//Leemos caracter a caracter mientras no haya salto de linea
	while (cAux != '\n'){
		read (nFdIn,&cAux,1);
		if (cAux != '\n'){
			sAux[i] = cAux;
		}else{
			sAux[i] = '\0';
		}
		i++;
	}
	//Pedimos memoria para la cadena
	*ppsString = malloc ((strlen (sAux)+1)*sizeof (char));
	strcpy ((*ppsString),sAux);
}




/**
 * Carrega el fitxer config.dat
 */
int getConfigInfo (char **psServer, char sDirPath[MAX_LONG]) {
	char cAux = '0';
	int i = 0;
	int nBytes = 1;
	int nFdIn = 0;
	int nPort = 0;

	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn) {
		write(2,"[Error] Error al obrir el fitxer 'config.dat'.\n",47);
		exit(ERROR);
	} else {

		readString(nFdIn, psServer);
		nPort = readInt (nFdIn);

		while ((cAux != '\n') && (0 != nBytes)){
			nBytes = read (nFdIn,&cAux,1);
			if ((cAux != '\n') && (0 != nBytes)){
				sDirPath[i] = cAux;
			}else{
				sDirPath[i] = '\0';
			}
			i++;
		}

		close(nFdIn);
	}
	return nPort;
}


/**
 * Fp necessaria per llegir el directori
 * @param  arg {struct dirent} path al directori
 */
static int triar (const struct dirent *arg) {
	if ( (char)arg->d_name[0] == (char)'.' ) {
	 	return 0;
	} 
	if ( strcmp (arg->d_name, ".") == 0 || strcmp (arg->d_name, "..") == 0 ) return 0;
	return 1;
}




/**
 * [getDateReal description]
 */
void getDateReal (char sRealDate[30], char sDirPath[MAX], char sName[30]) {

	struct stat status;
	char *sDate;
	char sRealDirPath[MAX+30];

	memset (sRealDirPath, '\0', MAX+30);
	strcat (sRealDirPath, sDirPath);
	strcat (sRealDirPath, sName);

	//hora de modificacio del arxiu -> sDate
	if (stat(sRealDirPath, &status) == 0) {
    sDate = ((char *)ctime(&status.st_mtime));
  }
  strcpy (sRealDate, sDate);
}




/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
int ReadDir (int bIsNull, char sMyLog[20]) {

	if (bIsNull == 0) {
		writeLog (sMyLog, "client.c","[Error] scandir","Path incorrecte",0);
	} else {
		writeLog (sMyLog, "client.c","scandir","Em escanejat el directori correctament",1);
	}
	return 0;
}




/**
 * Comprova di existeix el root, si no existeix el crea
 * @param  sDirPath {String} Path a la carpeta root
 * @param  sMyLog {String} Log al que escrius
 */
int pathExists (char sDirPath[MAX], char sMyLog[40]) {

	int bError = 0;
  bError = mkdir(sDirPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if ( !bError ) {
		writeLog (sMyLog, "initInCommon.c","[Error] No existeix carpeta root","Creant carpeta root",0);
	}
	return bError;
}




/**
 * Inicialitza la LinkedList posant tos els elements del directori a la LL
 */
int initLinkedList (char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20]) {
	int bArxiusOk = 0;
	int i = 0;
	struct dirent **arxius;

	pathExists (sDirPath, sMyLog);

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	
	if (nTotalFiles == 0) return 0;

	if (arxius != NULL) {
		bArxiusOk = 1;
		ReadDir(bArxiusOk, sMyLog);

		i = nTotalFiles-1;

		while (i >= 0) {
			addToLL(sDirPath, arxius[i]->d_name, (int)arxius[i]->d_type, LinkedList, LinkedListToTx, sMyLog);
			free (arxius[i]);
			i--;
		}
		free (arxius);

	} else {
		perror("scandir");
		writeLog (sMyLog, "initInCommon.c","scandir","Error amb l'escanejat",0);
	}
	return nTotalFiles;
}



/**
 * Mira al directori si hi ha hagut alguna modificacio i ho gestiona la LL
 */
int checkRootFiles (char sDirPath[MAX], struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20], sem_t *semLL) {

	struct dirent **arxius;
	int i = 0;
	int bUpdate = 0;
	int nTotalFiles = 0;
	int bArxiusOk = 0;
	int bSincro = 0;
	int nLLTotalFiles = 0;
	char sLLDate[30];
	char sRealDate[30];

	pathExists (sDirPath, sMyLog);

	nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);

	if (arxius != NULL) bArxiusOk = 1;
	ReadDir(bArxiusOk, sMyLog);

	i = nTotalFiles;

	sem_wait(semLL);
	nLLTotalFiles = count (LinkedList);
	sem_post(semLL);

	if (nTotalFiles == nLLTotalFiles) {
		
		//cas específic
		if (nTotalFiles == 0 && nLLTotalFiles == 0) {
			free (arxius);
			return 0;
		}

		//update o res
		while (i--) {

			memset (sLLDate, '\0', 30);
			memset (sRealDate, '\0', 30);

		 	getDateByName(sLLDate, arxius[i]->d_name, LinkedList);
		 	getDateReal (sRealDate, sDirPath, arxius[i]->d_name);	
			
			if ( strcmp (sLLDate, sRealDate) != 0 ) {
				sem_wait(semLL);
				updateToLL(sDirPath, sLLDate, arxius[i]->d_name, LinkedList, sMyLog);
				updateToLL(sDirPath, sLLDate, arxius[i]->d_name, LinkedListToTx, sMyLog);
				sem_post(semLL);

				bSincro = 1;
			}
			free (arxius[i]);
		}
		free (arxius);

	} else if (nTotalFiles > nLLTotalFiles) {

	 	write(1, "cal afegir el nou arxiu\n", 25);

		while (i--) {
			bUpdate = getDateByName(sLLDate, arxius[i]->d_name, LinkedList);
			if( bUpdate != 1 ) {
				sem_wait(semLL);
				addToLL(sDirPath, arxius[i]->d_name, (int)arxius[i]->d_type, LinkedList, LinkedListToTx, sMyLog);
				setEstatByName (arxius[i]->d_name, 1, LinkedListToTx);
				sem_post(semLL);
				bSincro = 1;
			}
			free (arxius[i]);
		}
		free (arxius);

	} else if (nTotalFiles < nLLTotalFiles) {
		write(1, "cal borrar un arxiu\n", 25);

		//cas específic
		if (nTotalFiles == 0 && nLLTotalFiles == 1) {
			sem_wait(semLL);
			buidaLL(LinkedList);
			sem_post(semLL);
			//buidaLL(LinkedListToTx);
			free (arxius);
			return 1;
		}

		sem_wait(semLL);
		removeToLL(nTotalFiles, arxius, LinkedList, LinkedListToTx, sMyLog, sDirPath);
		sem_post(semLL);

		bSincro = 1;
	}

return bSincro;
}


