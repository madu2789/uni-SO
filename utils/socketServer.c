/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */
#include "socketServer.h"

/**
 * Crea les diferents possibles Trames segons nType
 * @param  sTrama {String}	Cadena on posarem la trama que enviarem
 * @param  sLoginDesti {String}	Login desti de la trama
 * @param  sLoginOrigen {String}	Login origen de la trama
 * @param  nType {Integer}	Tipus de trama que enviarem
 * @return sTrama Ja formada
 */
void creaTrama (char sTrama[MAX_TRAMA], char sLoginOrigen[7], char sLoginDesti[7], int nType){

	char sTipus;
	char sData[100];

	//Netejant variables
	bzero (sTrama, MAX_TRAMA);
	bzero (sData, 100);

	//Detectem quin tipus es i podem el sTipus i Data necessaries
	switch (nType) {
		case 1:
			sprintf (sData,"'peticio d'autentificacio'");
			sTipus = 'P';
		break;
		case 2:
			sprintf (sData,"'Error en el procediment d'autentificació. Procedim a desconnexio'");
			sTipus = 'E';
		break;
		case 3:
			sprintf (sData,"'Autentificacio realitzada satisfactoriament'");
			sTipus = 'O';
		break;
	}

	//creant Trama final que enviarem
	strcat(sTrama, sLoginOrigen);
	strcat(sTrama, sLoginDesti);
	sTrama[strlen(sTrama)] = sTipus;
	strcat(sTrama, sData);
}


/**FUNCIO QUE HA DE TOCAR L'HILAZO, EII ! #AMBILUSIO! #NOTENIMPRESSA
 * Comprova que el usuari que ha enviat la trama estigui registrat al sistema (shadows.dat)
 * @param  sTrama {String}	Trama rebuda que analitzarem
 * @param  sUser {String}	Login de la trama
 * @param  sPswd {String}	Password de la trama
 * @return bValid {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int checkAuthentication (char sTrama[MAX_TRAMA], char sUser[7], char sPswd[32]) {
	int nFdIn = 0;
	int i = 0;
	int bValid = 0;
	char sFileUser[32];
	char sFilePswd[32];
	char sAux[MAX];
	char cAux = '0';

	nFdIn = open("shadows.dat", O_RDONLY);
	if (-1 == nFdIn) {
		write(2,"[Error] Error al obrir el fitxer 'shadows.dat'.\n",47);
		exit(ERROR);
	} else {

		//Llegim char a char fins a ':'
		while (cAux != ':'){
			read (nFdIn, &cAux, 1);
			if (cAux != ':'){
				sAux[i] = cAux;
			}else{
				sAux[i] = '\0';
			}
			i++;
		}
		strcpy (sFileUser, sAux);

		read (nFdIn, &sFilePswd, 32);
		sFilePswd[strlen(sFilePswd)] = '\0';

		//Test
		//printf("sUser: %s\n", sFileUser);
		//printf("sPswd: %s -- %s \n", sFilePswd, sPswd);

		close(nFdIn);

		//s'ha de fer per tothom for(){...}
		if (strcmp(sUser, sFileUser) == 0) {
			if (strcmp(sPswd, sFilePswd) == 0) {
				bValid = 1;
			}
		}
	}
	return bValid;
}

/**
 * Revisa que la Trama rebuda sigui del fotmat i correcte en si
 * @param  sTrama {String}	Trama rebuda que analitzarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int checkTrama (char sTrama[MAX_TRAMA]) {
	int bTramaOk = 0;

//tres heuristikes : trama tipus E || trama incorrecte

	while (sTrama != '\0') {

	}


	return bTramaOk;
}

/**
 * Connecta el socket Inicial amb el client i Autentica el Usuari
 * @param  nPort {Integer}	Number of Port al que ens conectarem
 * @return bTramaOk {Boolean} Rebrem: [correcte = 1 | incorrecte = 0]
 */
int ServerConection (int nPort, char sUser[7], char sPswd[32]) {

	int gnSocketFD;
	int nAux, nBytesLeidos, nFinal;
	int nSocketCliente;
	char sFrase[MAX], sTrama[MAX_TRAMA];
	uint16_t wPuerto;
	struct sockaddr_in stDireccionSocket, stDireccionCliente;

	char sLoginDesti[7];
	char sLoginOrigen[7];
	int nBytesLlegits = 0;

	//Comprovem que el port estigui en el marge correcte
	if ( nPort < 1024 || nPort > 65535){
		sprintf (sFrase,"Port invalid!\n");
		write (1,sFrase,strlen (sFrase));
		return ERROR;
	}
	wPuerto = nPort;

	//Creamos el socket
	gnSocketFD = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gnSocketFD < 0){
		sprintf (sFrase,"Error al crear el socket!\n");
		write (1,sFrase,strlen (sFrase));
		return ERROR;
	}

	//Especificamos la direccion del socket
	bzero (&stDireccionSocket, sizeof (stDireccionSocket));
	stDireccionSocket.sin_family = AF_INET;
	stDireccionSocket.sin_port = htons (wPuerto);
	stDireccionSocket.sin_addr.s_addr = INADDR_ANY;

	//Vinculamos el socket con la direccion
	if (bind (gnSocketFD, (void *) &stDireccionSocket, sizeof (stDireccionSocket)) < 0){
		sprintf (sFrase,"Error al vincular el socket y la direccion!\n");
		write (1,sFrase,strlen (sFrase));
		//Cerramos el socket
		close (gnSocketFD);
		return ERROR;
	}

	//Escuchamos peticiones (1 a la vez)
	listen (gnSocketFD, 1);

	sprintf (sFrase, "Servidor connectat!\n");
	write (1, sFrase, strlen (sFrase));


	while (1) {

		printf("esperant client...\n");
		//Obtenim un socket al client que es conecti
		socklen_t c_len = sizeof (stDireccionCliente);
		nSocketCliente = accept (gnSocketFD, (void *) &stDireccionCliente, &c_len);
		if (nSocketCliente < 0){
			sprintf (sFrase,"Error al aceptar la peticion del cliente!\n");
			write (1,sFrase,strlen (sFrase));
			//Tanquem socket
			close (gnSocketFD);
			return ERROR;
		}

		sprintf (sFrase,"\nClient conectat\n");
		write (1,sFrase,strlen (sFrase));


		//---------------------------------------------------------------------------
		//Protocol de trames d'establiment de connexio
		//---------------------------------------------------------------------------

		//Creem la primera trama de peticio d'autentificacio
 		creaTrama(sTrama, "LSBox", "client", 1);

		//Enviem la trama de peticio d'autentificacio
		write (nSocketCliente, sTrama, MAX_TRAMA);
		printf("trama enviada: %s...\n", sTrama);

		//Rebem Trama amb les dades del client
		nBytesLlegits = read(nSocketCliente, sTrama, MAX_TRAMA);
		printf("trama rebuda: %s...\n", sTrama);

		//Comprovem que la trama rebuda es correcte
		if ( nBytesLlegits <= MAX_TRAMA && checkAuthentication (sTrama, sUser, sPswd) && checkTrama(sTrama) ) {
			//Creem la trama de resposta OK peticio d'autentificacio
 			creaTrama(sTrama, sLoginOrigen, sLoginDesti, 2);
		} else {
			//Creem la trama de resposta KO peticio d'autentificacio
 			creaTrama(sTrama, sLoginOrigen, sLoginDesti, 3);
 			//hem de desconectar...
		}
		write (nSocketCliente, sTrama, MAX_TRAMA);

	}


}