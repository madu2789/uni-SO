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
void addToLL (char sName[30], int nTipus, struct node *LinkedList) {
	struct stat status;
	char sAdaptedTipus[30];
	char *sDate;
	int nSize = 0;


	//hora de modificacio del arxiu -> sDate
	if (stat(sName, &status) == 0) {
    sDate = ((char *)ctime(&status.st_mtime));
    nSize = status.st_size;
  }

  conversorTipus(sAdaptedTipus, nTipus);
	//afegir a la cua el nou element: ->LinkedList
	append(sName, sAdaptedTipus, sDate, nSize, LinkedList);
	//writeLog ("LSBox_svr.log.html","facadeLL.c","Nou element afegit", sName, 1);

}




/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void addToLLTx (char sTrama[MAX_TRAMA], char sName[30], char sDate[64], int nSize, struct node *LinkedList) {

	char sAdaptedTipus[30];




	//afegir a la cua el nou element: ->LinkedList
	//append(sName, sAdaptedTipus, sDate, nSize, LinkedList);
	//writeLog ("LSBox_cli.log.html","facadeLL.c","Nou element afegit", sName, 1);

}




/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void updateToLL (char sLLDate[30], char sName[30], struct node *LinkedList) {
	struct stat status;
	char *sDate;
	int nSize = 0;

	if (stat(sName, &status) == 0) {
		sDate = ((char *)ctime(&status.st_mtime));
		nSize = status.st_size;
	}
	if (strcmp(sLLDate, sDate) != 0 ){
		setDateByName(sName, sDate, nSize, LinkedList);
	 	printf("updated\n");
	}
	//writeLog ("LSBox_cli.log.html","facadeLL.c","Element modificat", sName, 1);
}


/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void removeToLL (int nTotalFiles, int nLLTotalFiles, struct dirent **arxius, struct node *LinkedList) {
	int i = 0;
	int j= 0;
	int nSize = 0;
	int bToRemove= 1;
	char sNameToRemove[30];
	char sDateBasura[64];

	for (i = 1; i < nLLTotalFiles+1; i++) {
		nSize = showNode(sNameToRemove, sDateBasura, i, LinkedList);
		printf("mirem: %s si es: %s \n",sNameToRemove, arxius[j]->d_name );

		for (j = 1; j < nTotalFiles; j++){
			if (strcmp (sNameToRemove, arxius[j]->d_name ) == 0 ) {
				printf("NO hem de borrar : %s\n", arxius[j]->d_name);
				bToRemove = 0;
			}
		}

		if (bToRemove == 1) {
			printf("hem de borrar : %s\n", sNameToRemove);
			delnode(sNameToRemove, LinkedList);
			printf("BORRAT : %s\n", sNameToRemove);
		}
		bToRemove = 1;
	}

	for (j = 1; j < nTotalFiles; j++){
		//free(arxius[j]);
	}
	//free(arxius);
	//writeLog ("LSBox_cli.log.html","facadeLL.c","Element eliminat", sNameToRemove, 1);
}