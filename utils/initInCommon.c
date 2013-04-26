/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "initInCommon.h"



/**
 * Carrega el fitxer config.dat
 */
int getConfigInfo (char sServer[11], char sDirPath[MAX]) {
	int nFdIn = 0;
	int nPort = 0;
	char sPort[5];

	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn) {
		write(2,"[Error] Error al obrir el fitxer 'config.dat'.\n",47);
		exit(ERROR);
	} else {
		bzero(sDirPath, MAX);
		read(nFdIn, sServer, 12);
		read(nFdIn, sPort, 5);
		read(nFdIn, sDirPath, MAX-1);
		close(nFdIn);

		sServer[strlen(sServer)] = '\0';
		sPort[strlen(sPort)] = '\0';
		sDirPath[strlen(sDirPath)] = '\0';

		//Convertim el nombre del port a Integer
		nPort = atoi(sPort);
	}
	return nPort;
}



