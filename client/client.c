/**
 * Daniel Madurell Blanes (is19445)
 * Albert Hilazo Aguilera (se19467)
 */

#include <unistd.h>			// write i read
#include <stdio.h>			// sprintf
#include <stdlib.h>
#include <string.h>			// strlen, bzero
#include <dirent.h>			// opendir, readdir...
#include <sys/stat.h>		//stat, obtenir data modificacio
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#define ERROR -1
#define OK 0
#define MAX 64

char sServer[11];
char nPort[3];
char sDirPath[MAX];
char sLogin[MAX];
char sPswd[MAX];
int nLastTotalFiles;



//prova per incloure la LL
//BORRAR
//
/* STRUCTURE : DATA PART AND A LINK PART */
struct node {
  char sName[30];
  char sTipus[30];
  char sDate[64];
  struct node *next;
} *p;
/* P IS A GLOBAL POINTER CONTAINS THE ADRESS OF THE FIRST NODE*/


/*THIS FUNCTION DELETES A NODE */
void delnode (char sName[30]) {
  struct node *temp, *m;
	temp=p;

  while(temp != NULL){
    if(strcmp(temp->sName, sName) == 0){
      if(temp == p){
        p=temp->next;
        //free(temp); diria k no cal alliberar pk no arribes a demanar memoria (HIL?)
        return;
      }else{
        m->next=temp->next;
        //free(temp);
        return;
      }
    }else{
      m=temp;
      temp= temp->next;
    }
	}
    printf(" ELEMENT %s NOT FOUND!\n", sName);
}


/* ADD A NEW NODE AT BEGINNING  */
void addbeg (char sName[30], char sTipus[30], char sDate[64]) {
  struct node *temp;
  temp=(struct node *)malloc(sizeof(struct node));

  strcpy(temp->sName, sName);
  strcpy(temp->sTipus, sTipus);
  strcpy(temp->sDate, sDate);

  if (p == NULL) {
    p=temp;
    p->next=NULL;
  }else{
    temp->next=p;
    p=temp;
  }
}


/* THIS FUNCTION DISPLAYS THE CONTENTS OF THE LINKED LIST */
void display (struct node *r) {
  r=p;
  if(r == NULL) {
    printf("NO ELEMENT IN THE LIST :\n");
    return;
  }
  while(r != NULL) {
    printf(" -> %s \t %s \t %s \n", r->sName, r->sTipus, r->sDate );
    r=r->next;
  }
    printf(" ");
}

//FINS AQUI

/**
 * Demana al usuari sLogin i sPswd
 */
void loginUser () {
	char sAux[MAX];

	//Inicialitzacions
	bzero (sLogin, MAX);
	bzero (sPswd, MAX);
	//Demana nom d'usuari
	sprintf (sAux, "Username: ");
	write (1, sAux, strlen(sAux));
	read (0, sLogin, MAX);
	sLogin[strlen(sLogin)-1] = '\0';

	//Demana contrasenya
	sprintf (sAux, "Password: ");
	write (1, sAux, strlen(sAux));
	read (0, sPswd, MAX);
	sPswd[strlen(sPswd)-1] = '\0';
}
/**
 * Carrega el fitxer config.dat
 */
void getConfigInfo () {
	int nFdIn;

	nFdIn = open("config.dat", O_RDONLY);
	if (-1 == nFdIn){
		write(2,"[Error] Error al obrir el fitxer 'config.dat'.\n",47);
		exit(ERROR);
	} else {
		read(nFdIn, sServer, 12);
		read(nFdIn, nPort, 3);
		read(nFdIn, sDirPath, MAX);
		close(nFdIn);

		sServer[strlen(sServer)] = '\0';
		nPort[strlen(nPort)] = '\0';
		sDirPath[strlen(sDirPath)] = '\0';
	}
	return;
}
/**
 * Fp necessaria per llegir el directori
 * @param  arg ,path al directori
 */
static int triar (const struct dirent *arg) {
	if (strcmp (arg->d_name, ".") == 0 || strcmp (arg->d_name, "..") == 0) return 0;
	return 1;
}
/**
 * Inicialitza la LinkedList posant tos els elements del directori a la LL
 */
int initLinkedList () {
	struct dirent **arxius;
	struct stat status;
	char *sTipus;
	char *sDate;

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius == NULL) {
		printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);
		perror ("scandir");
		return -1;
	}

	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);
	nLastTotalFiles = nTotalFiles;

	while (nTotalFiles--) {
		//Agafem la hora de modificacio del arxiu -> sDate
		if (stat(arxius[nTotalFiles]->d_name,&status) == 0) {
            sDate = ((char *)ctime(&status.st_mtime));
        }
        //Mirem quin tipus d'arxiu és -> sTipus
		switch (arxius[nTotalFiles]->d_type) {
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

		//comprovació de dades llegides del directori
		//printf ("[%d] %s -- %s -- %s \n", nTotalFiles, arxius[nTotalFiles]->d_name, sTipus, sDate);
		//afegir a la cua el nou element: ->LinkedList
		addbeg(arxius[nTotalFiles]->d_name, sTipus, sDate);
		//Allibero aquest arxiu
		free (arxius[nTotalFiles]);
	}
	free (arxius);
}

/**
 * Mira al directori si hi ha hagut alguna modificacio i ho gestiona la LL
 */
int checkRootFiles () {
	int i = 0;
	struct dirent **arxius;
	struct stat status;
	char *sName;
	char *sTipus;
	char *sDate;

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius == NULL) {
		perror ("scandir");
	}
	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);

	i = nTotalFiles;
	if (nTotalFiles == nLastTotalFiles) {
		//update
		while (i--) {
			//nomes em cal mirar les dates
			//Agafem la hora de modificacio del arxiu -> sDate
			if (stat(arxius[nTotalFiles]->d_name,&status) == 0) {
	            sDate = ((char *)ctime(&status.st_mtime));
	        }
			printf ("[%d] %s\n", i, arxius[i]->d_name);
			free (arxius[i]);
		}
	} else {
		//add or remove

	}

	free (arxius);
	return nTotalFiles;
}

/**
 * main general
 */
int main() {
	//Guardem -> sLogin, sPswd
	loginUser();
	//Llegir "config.dat"
	getConfigInfo();
	//Init LL posant tots els ele. trobats al directori root
	initLinkedList();
	//Check al directori si hi ha hagut algun canvi cada 2''
	checkRootFiles();

	//display(p);

/*comprovacions fitxer "config.dat" ben llegit
	printf("hey server: %s len %d \n", sServer, (int)strlen(sServer));
	printf("hey port: %s len %d \n", nPort, (int)strlen(nPort));
	printf("hey path: %s len %d \n", sDirPath, (int)strlen(sDirPath));
*/

	return 0;
}