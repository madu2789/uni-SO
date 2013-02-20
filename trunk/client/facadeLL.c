/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */

/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void addToLL (int nTotalFiles) {
	struct stat status;
	char sTipus[30];
	char *sDate;

	//hora de modificacio del arxiu -> sDate
	if (stat(arxius[nTotalFiles]->d_name, &status) == 0) {
    sDate = ((char *)ctime(&status.st_mtime));
  }
  conversorTipus(sTipus, arxius[nTotalFiles]->d_type);
	//afegir a la cua el nou element: ->LinkedList
	addbeg(arxius[nTotalFiles]->d_name, sTipus, sDate);
	free (arxius[nTotalFiles]);
}


/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void updateToLL (int i, char sLLDate[30]) {
	struct stat status;
	char *sDate;

	if (stat(arxius[i]->d_name, &status) == 0) {
		sDate = ((char *)ctime(&status.st_mtime));
	}
	if (strcmp(sLLDate, sDate) != 0 ){
		setDateByName(arxius[i]->d_name, sDate);
	 	printf("updated\n");
	}
	free (arxius[i]);
}


/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void removeToLL ( int nTotalFiles, int nLLTotalFiles) {
	int i,j, bToRemove= 1;
	char sNameToRemove[30];

	for (i = 1; i < nLLTotalFiles; i++) {
		showNode(sNameToRemove, i);
		printf("mirem: %s\n",sNameToRemove );
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
			display(p); //test
		}
		bToRemove = 1;
	}
	for (j = 1; j < nTotalFiles; j++){
		free(arxius[j]);
	}
	free(arxius);
}