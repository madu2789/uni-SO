/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */

#include "facadeLL.h"


/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void conversorTipus (char sTipus[30], int nToConvert) {
		bzero(sTipus, 30);
		switch (nToConvert) {
			case DT_FIFO:
				sTipus = "Fifo";
			break;
			case DT_CHR:
				sTipus = "Character device";
			break;
			case DT_BLK:
				sTipus = "Block device";
			break;
			case DT_REG:
				sTipus = "Regular file";
			break;
			case DT_LNK:
				sTipus = "Link";
			break;
			case DT_SOCK:
				sTipus = "Socket";
			break;
			case DT_DIR:
				sTipus = "Directory";
			break;
			case DT_UNKNOWN:
				sTipus = "UNKNOWN";
			break;
		}
}




/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void addToLL (char sDirPath[MAX], char sName[30], int nTipus, struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20]) {
	struct stat status;
	char sAdaptedTipus[30];
	char *sDate;
	char sRealDirPath[MAX+30];
	int nSize = 0;

	memset (sRealDirPath, '\0', MAX+30);
	strcat (sRealDirPath, sDirPath);
	strcat (sRealDirPath, sName);

	//hora de modificacio del arxiu -> sDate
	if (stat(sRealDirPath, &status) == 0) {
	  sDate = ((char *)ctime(&status.st_mtime));
	  nSize = status.st_size;
  }

  conversorTipus (sAdaptedTipus, nTipus);

	//afegir a la cua el nou element: ->LinkedList
	append (sName, sAdaptedTipus, sDate, nSize, 0, LinkedList);
	addToLLTx (sName, sDate, nSize, 8, LinkedListToTx);
	writeLog (sMyLog, "facadeLL.c", "Nou element afegit", sName, 1);

}




/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void addToLLTx (char sName[30], char sDate[24], int nSize, int nEstat, struct node *LinkedList) {

	char sAdaptedTipus[30];

	//afegir a la cua el nou element: ->LinkedList
	append(sName, sAdaptedTipus, sDate, nSize, nEstat, LinkedList);
}




/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void updateToLL (char sDirPath[MAX], char sLLDate[30], char sName[30], struct node *LinkedList, char sMyLog[20]) {
	struct stat status;
	char *sDate;
	int nSize = 0;
	char sRealDirPath[MAX+30];

	memset (sRealDirPath, '\0', MAX+30);
	strcat (sRealDirPath, sDirPath);
	strcat (sRealDirPath, sName);

	if (stat(sRealDirPath, &status) == 0) {
		sDate = ((char *)ctime(&status.st_mtime));
		nSize = status.st_size;
	}
	if (strcmp(sLLDate, sDate) != 0 ) {
		setDateByName(sName, sDate, nSize, LinkedList);
	 	writeLog (sMyLog, "facadeLL.c", "Element modificat", sName, 1);
	}

}


/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void removeToLL (int nTotalFiles, struct dirent **arxius, struct node *LinkedList, struct node *LinkedListToTx, char sMyLog[20], char sDirPath[MAX]) {
	int i = 0;
	int j= 0;
	int nSize = 0;
	int bToRemove= 1;
	int nLLTotalFiles = 0;
	char sNameToRemove[30];
	char sDateLL[64];

	nLLTotalFiles = count(LinkedList);

	for (i = 1; i < nLLTotalFiles; i++) {
		memset(sNameToRemove, '\0', 30);
		nSize = showNode(sNameToRemove, sDateLL, i, LinkedList);

		for (j = 0; j < nTotalFiles; j++){

			if (strcmp (sNameToRemove, arxius[j]->d_name ) == 0 ) {
				bToRemove = 0;
			}
		}

		if (bToRemove == 1) {

			buidaLL(LinkedList);
			buidaLL(LinkedListToTx);
			initLinkedList (sDirPath, LinkedList, LinkedListToTx, sMyLog);
			
			//Codi nomes pel servidor, no borrem de la LL, canviem el nEstat per saber qui TX o RX
			if ( strcmp(sMyLog, "LSBox_svr.log.html") == 0) {
				printf("CLI_RM: %s", sNameToRemove);
				addToLLTx(sNameToRemove, "fake", 1, 3, LinkedListToTx);
			}
		}
		bToRemove = 1;
	}

	for (j = 1; j < nTotalFiles; j++){
		free(arxius[j]);
	}
	free(arxius);
	writeLog (sMyLog, "facadeLL.c", "Element eliminat", sNameToRemove, 1);
}
