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
void addToLL (char sName[30], int nTipus) {
	struct stat status;
	char sAdaptedTipus[30];
	char *sDate;

	//hora de modificacio del arxiu -> sDate
	if (stat(sName, &status) == 0) {
    sDate = ((char *)ctime(&status.st_mtime));
  }
  conversorTipus(sAdaptedTipus, nTipus);
	//afegir a la cua el nou element: ->LinkedList
	addbeg(sName, sAdaptedTipus, sDate);
}


/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void updateToLL (char sLLDate[30], char sName[30]) {
	struct stat status;
	char *sDate;

	if (stat(sName, &status) == 0) {
		sDate = ((char *)ctime(&status.st_mtime));
	}
	if (strcmp(sLLDate, sDate) != 0 ){
		setDateByName(sName, sDate);
	 	printf("updated\n");
	}
}


/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void removeToLL (int nTotalFiles, int nLLTotalFiles, struct dirent **arxius) {
	int i,j, bToRemove= 1;
	char sNameToRemove[30];

	for (i = 1; i < nLLTotalFiles; i++) {
		showNode(sNameToRemove, i);
		printf("mirem: %s si %s \n",sNameToRemove, (*arxius[j]).d_name );
		for (j = 1; j < nTotalFiles; j++){
			if (strcmp (sNameToRemove, arxius[j]->d_name ) == 0 ) {
				printf("NO hem de borrar : %s\n", arxius[j]->d_name);
				bToRemove = 0;
			}
		}
		if (bToRemove == 1) {
			printf("hem de borrar : %s\n", sNameToRemove);
			delnode(sNameToRemove);
			printf("BORRAT : %s\n", sNameToRemove);
		}
		bToRemove = 1;
	}
	for (j = 1; j < nTotalFiles; j++){
		free(arxius[j]);
	}
	free(arxius);
}