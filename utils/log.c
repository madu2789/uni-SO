/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "log.h"

/**
 * Crea si no esta creat el Log correponent
 * @param  sIdLog {String}	Cadena amb l'Id de l'app
 * @return bCorrect {0 wrong | 1 right}
 */
int createLog (char sIdLog[15]) {

	int nFdIn;
	char sAux[150];


	//Obrim el fitxer per llegir
	nFdIn = open(sIdLog, O_WRONLY);
	if (-1 == nFdIn) {
		printf("creant log!\n");
		// Creem els fitxers de log i backup
		nFdIn = open(sIdLog, O_CREAT, 0666);
		close(nFdIn);
		nFdIn = open("backup", O_CREAT, 0666);
		close(nFdIn);

		//Obrim el fitxer per llegir i escriure
		nFdIn = open(sIdLog, O_WRONLY);

		//escric capçelera
		memset(sAux, '\0', 150);
		sprintf (sAux,"<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='utf−8'/>\n<title>[SO ] Logs</title>\n</head>\n<body id='index' class='home'>\n");
		write (nFdIn, sAux, strlen(sAux));

		//escric peu
		memset(sAux, '\0', 150);
		sprintf (sAux,"</body>\n</html>");
		write (nFdIn, sAux, strlen(sAux));
	}

	close(nFdIn);

}

/**
 * Afegeix els logs que van vinguent
 * @param  sIdLog {String}	Cadena amb l'Id de l'app
 * @param  sFunction {String}	Accio duta a terme
 * @param  bCorrect {Integer}	Resultat de l'acció descrita
 * @return
 */
int writeLog (char sIdLog[15], char sFunction[32], int bCorrect){

	int nFdInLog, nFdInBackUp;
	char sAux[250], sAuxInit[50], sAuxFi[50];
	char sProvaGuarrada[1400];

	//Obrim el fitxer per llegir
	nFdInLog = open(sIdLog, O_WRONLY | O_TRUNC );
	if (-1 == nFdInLog) {
		printf("error al obrir log!\n");
		exit(-1);
	} else {
		nFdInBackUp = open("backup", O_RDWR | O_APPEND);
		if (-1 == nFdInBackUp) {
		printf("error al obrir backup!\n");
		exit(-1);
		} else {

			//Creem i Afegim el nou log al backup
			memset(sAux, '\0', 250);
			memset(sAuxInit, '\0', 50);
			memset(sAuxFi, '\0', 50);
			sprintf (sAux,"<p> <fontcolor='blue'>[</font> <fontcolor='teal'>");
			strcat(sAux, sIdLog);
			sprintf (sAuxFi," </font><fontcolor='blue'>]</font>");
			strcat(sAux, sAuxFi);

			memset(sAuxInit, '\0', 50);
			memset(sAuxFi, '\0', 50);
			sprintf (sAuxInit,"<fontcolor='blue'>[</font> <fontcolor='red'>");
			strcat(sAuxInit, sFunction);
			sprintf (sAuxFi," </font><fontcolor='blue'>]</font>");
			strcat(sAux, sAuxInit);
			strcat(sAux, sAuxFi);

			memset(sAuxInit, '\0', 50);
			memset(sAuxFi, '\0', 50);
			sprintf (sAuxInit,"<fontcolor='black'> explicacio </font>");
			//strcat(sAuxInit, sExplicacio);
			//sprintf (sAuxFi," </font><fontcolor='blue'>]</font>");
			strcat(sAux, sAuxInit);


			memset(sAuxInit, '\0', 50);
			memset(sAuxFi, '\0', 50);
			if (bCorrect) {
				sprintf (sAuxInit,"<fontcolor='blue'>[ </font><fontcolor='green'>");
				strcat(sAuxInit, "OK");
			}else {
				sprintf (sAuxInit,"<fontcolor='blue'>[ </font><fontcolor='red'>");
				strcat(sAuxInit, "KO");
			}
			sprintf (sAuxFi," </font><fontcolor='blue'>]</font></p>");
			strcat(sAux, sAuxInit);
			strcat(sAux, sAuxFi);


			write (nFdInBackUp, sAux, strlen(sAux));

			//Tornem a formar el arxiu sencer de log

			//escric capçelera
			memset (sAux, '\0', 150);
			sprintf (sAux,"<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='utf−8'/>\n<title>[SO ] Logs</title>\n</head>\n<body id='index' class='home'>\n");
			write (nFdInLog, sAux, strlen(sAux));

			//aqui copiare els logs de backup -> log
			close(nFdInBackUp);
			nFdInBackUp = open("backup", O_RDWR);
			if (-1 == nFdInBackUp) {
				printf("error al obrir backup!\n");
				exit(-1);
			} else {
				//guarrada maxima k sa de canviar a un bucle fins final de fitxer
				memset (sProvaGuarrada, '\0', 1400);
				read (nFdInBackUp, sProvaGuarrada, 1399);
				write (nFdInLog, sProvaGuarrada, strlen(sProvaGuarrada));

				//escric peu
				memset (sAux, '\0', 150);
				sprintf (sAux,"</body>\n</html>");
				write (nFdInLog, sAux, strlen(sAux));

			}
		}
	}
	close(nFdInLog);
	close(nFdInBackUp);
}