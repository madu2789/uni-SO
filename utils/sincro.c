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
int startSincro() {
	char sTrama[MAX];

	printf("heyy sincro!\n");
	//enviar la Trama 'S'
	creaTrama(sTrama, "LsBox  ", "madu123", 3);
	printf("%s\n", sTrama);
	return;
}