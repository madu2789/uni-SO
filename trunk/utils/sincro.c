/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "sincro.h"

/**
 * Comença la sincronitzacio de la LL
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @return bCorrect {0 wrong | 1 right}
 */
int startSincroServer(int nFdIn, char sTrama[MAX_TRAMA], char sLoginDesti[7]) {

	//Creem la Trama 'S'
	creaTrama(sTrama, "LsBox  ", sLoginDesti, 4);

	//Enviem la trama de peticio d'autentificacio
	write (nFdIn, sTrama, MAX_TRAMA);
	printf("trama enviada: %s\n", sTrama);
	//writeLog ("LSBox_svr.log.html","sincro.c","Trama Enviada", sTrama, 1);

	return 0;
}

/**
 * Comença la sincronitzacio de la LL
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @return bCorrect {0 wrong | 1 right}
 */
int receiveServerSincro(int nFdIn, char sTrama[MAX_TRAMA], char sLoginOrigen[7]) {

	read (nFdIn, sTrama, MAX_TRAMA);
	printf("trama rebuda:  %s\n", sTrama);

	creaTrama(sTrama, sLoginOrigen, "LsBox  ", 4);
	write (nFdIn, sTrama, MAX_TRAMA);
	printf("trama enviada: %s\n", sTrama);

	//Comencem a enviar tota la LL
	getSincroInfo(nFdIn);


	return 0;
}

/**
 * Comença la sincronitzacio de la LL
 * @param  nFd {Number}	nFile descriptor del socket obert
 * @return bCorrect {0 wrong | 1 right}
 */
void getSincroInfo(int nFdIn) {

	char sName[30];
	char sDate[64];
	int *nSize;
 	int nLocation = 0;
 	int nTotalFiles = 0;


 	for (nLocation = 0 ; nLocation < nTotalFiles ; nLocation++ ) {
 		showNode(sName, sDate, &nSize, nLocation);
 		printf("%s - %s - %d\n", sName, sDate, *(nSize));
 		//creaTrama a "manija"
 		//write (nFdIn, sTrama, MAX_TRAMA);
 	}


}