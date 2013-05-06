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
int createLog (char sIdLog[20]) {

	int nFdIn;
	char sAux[150];

	//Obrim el fitxer per llegir
	nFdIn = open(sIdLog, O_WRONLY);
	if (-1 == nFdIn) {
		printf("creant log!\n");
		// Creem els fitxers de log i backup
		nFdIn = open (sIdLog, O_CREAT, 0666);
		close (nFdIn);
		nFdIn = open ("backup", O_CREAT, 0666);
		close (nFdIn);

		//Obrim el fitxer per llegir i escriure
		nFdIn = open(sIdLog, O_WRONLY);

		//escric capçelera
		memset (sAux, '\0', 150);
		sprintf (sAux,"<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='utf−8'/>\n<title>[SO ] Logs</title>\n</head>\n<body id='index' class='home'>\n");
		write (nFdIn, sAux, strlen(sAux));

		//escric peu
		memset (sAux, '\0', 150);
		sprintf (sAux,"</body>\n</html>");
		write (nFdIn, sAux, strlen(sAux));
	}

	close (nFdIn);

}



/**
 * Afegeix els logs que van vinguent
 * @param  sIdLog {String}	Cadena amb l'Id de l'app
 * @param  sFunction {String}	Accio duta a terme
 * @param  bCorrect {Integer}	Resultat de l'acció descrita
 * @return
 */
int writeLog (char sIdLog[20], char sOrigen[32], char sFunction[32], char sExplanation[120], int bCorrect){

	int nFdInLog, nFdInBackUp;
	char sAux[450], sAuxInit[250], sAuxFi[250];

	//printf("sIdLog al writeLog: %s\n", sIdLog);

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
			memset(sAux, '\0', 350);
			memset(sAuxInit, '\0', 150);
			memset(sAuxFi, '\0', 50);
			sprintf (sAux,"<p> <fontcolor='blue'> [ </font> <fontcolor='teal'>");
			strcat(sAux, sOrigen);
			sprintf (sAuxFi," </font><fontcolor='blue'> ] </font>");
			strcat(sAux, sAuxFi);


			memset(sAuxInit, '\0', 150);
			memset(sAuxFi, '\0', 50);
			sprintf (sAuxInit,"<fontcolor='blue'> [ </font> <fontcolor='red'>");
			strcat(sAuxInit, sFunction);
			sprintf (sAuxFi," </font><fontcolor='blue'> ] </font>");
			strcat(sAux, sAuxInit);
			strcat(sAux, sAuxFi);


			memset(sAuxInit, '\0', 150);
			memset(sAuxFi, '\0', 50);
			sprintf (sAuxInit,"<fontcolor='black'><font>");
			strcat(sAuxInit, sExplanation);
			sprintf (sAuxFi," </font><fontcolor='blue'> ] </font>");
			strcat(sAux, sAuxInit);
			strcat(sAux, sAuxFi);


			memset(sAuxInit, '\0', 150);
			memset(sAuxFi, '\0', 50);

			if (bCorrect) {
				sprintf (sAuxInit,"<fontcolor='blue'> [ </font><fontcolor='green'>");
				strcat(sAuxInit, "OK");
			}else {
				sprintf (sAuxInit,"<fontcolor='blue'> [ </font><fontcolor='red'>");
				strcat(sAuxInit, "KO");
			}
			sprintf (sAuxFi,"</font><fontcolor='blue'>]</font></p> \0 ");
			strcat(sAux, sAuxInit);
			strcat(sAux, sAuxFi);


			write (nFdInBackUp, sAux, strlen(sAux));
			write (nFdInBackUp, 26, 1);

			//Tornem a formar el arxiu sencer de log

			//escric capçelera
			memset (sAux, '\0', 350);
			sprintf (sAux,"<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='utf−8'/>\n<title>[SO ] Logs</title>\n</head>\n<body id='index' class='home'>\n");
			write (nFdInLog, sAux, strlen(sAux));

			//aqui copiare els logs de backup -> log
			close(nFdInBackUp);

			nFdInBackUp = open("backup", O_RDWR);
			if (-1 == nFdInBackUp) {
				printf("error al obrir backup!\n");
				exit(-1);
			} else {

				int nBytes = -1;
				char cAux = '\0';
				// Llegim char a char fins a EOF
				while (nBytes != 0) {
					nBytes = read (nFdInBackUp, &cAux, 1);
					if (nBytes == 0) {
						cAux = '\0';
					}
					write (nFdInLog, &cAux, 1);
				}

				//escric peu
				memset (sAux, '\0', 350);
				sprintf (sAux,"</body>\n</html>");
				write (nFdInLog, sAux, strlen(sAux));
			}
		}
	}

	close(nFdInLog);
	close(nFdInBackUp);
	return 0;
}
