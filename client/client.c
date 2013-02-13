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
char sProvaName[30];
char sProvaDate[64];
//
/* STRUCTURE : DATA PART AND A LINK PART */
struct node {
  char sName[30];
  char sTipus[30];
  char sDate[64];
  struct node *next;
} *p;
/* P IS A GLOBAL POINTER CONTAINS THE ADRESS OF THE FIRST NODE*/

//THIS FUNCTION COUNTS THE NUMBER OF ELEMENTS IN THE LIST
int count (void) {
  struct node *n;
  int c=0;
  n=p;
  while(n!=NULL){
    n=n->next;
    c++;
  }
  return(c);
}
/*THIS FUNCTION DELETES A NODE */
void delnode (char sName[30]) {
  struct node *temp, *m;
	temp=p;

  while(temp != NULL){
    if(strcmp(temp->sName, sName) == 0){
      if(temp == p){
        p=temp->next;
        return;
      }else{
        m->next=temp->next;
        return;
      }
    }else{
      m=temp;
      temp= temp->next;
   }
}
    printf(" ELEMENT %s NOT FOUND!\n", sName);
}

/*Show a A NODE (molt guarro utilitzant variables globals->REFACTORING)*/
int showNode (int nLocation) {
  struct node *temp;
  int i = 0, bTrobat = 0;
  temp=p;

  if (nLocation-1 < count() ) {
    while(temp != NULL && bTrobat == 0){
        if(i == nLocation-1){
          bTrobat = 1;
        } else {
          temp= temp->next;
        }
        i++;
        printf(" ELEMENT %s FOUND!\n", temp->sName);
    }
  } else {
    printf(" ELEMENT DOESN'T EXISTS!\n");
    return -1;
  }
  return 0;
}

/*Show a A NODE (molt guarro utilitzant variables globals->REFACTORING)*/
int getDateByName (char sName[30]) {
  struct node *temp;
  int bTrobat = 0;
  temp=p;

    while(temp != NULL && bTrobat == 0){
        if(strcmp(sName, temp->sName) == 0){
          bTrobat = 1;
        } else {
          temp= temp->next;
        }
        strcpy(sProvaDate, temp->sDate);
        //printf(" ELEMENT %s FOUND!\n", temp->sName);
    }
    if(bTrobat == 0){
      printf(" ELEMENT DOESN'T EXISTS!\n");
      return -1;
    }
  return 0;
}

/*Canvia la sDate segons el sName*/
int setDateByName (char sName[30], char sDate[64]) {
  struct node *temp;
  int bTrobat = 0;
  temp=p;

    while(temp != NULL && bTrobat == 0){
        if(strcmp(sName, temp->sName) == 0){
          bTrobat = 1;
          strcpy(temp->sDate, sDate);
        } else {
          temp= temp->next;
        }
        //printf(" ELEMENT %s FOUND!\n", temp->sName);
    }
    if(bTrobat == 0){
      printf(" ELEMENT DOESN'T EXISTS!\n");
      return -1;
    }
  return 0;
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
 * @param  arg {struct dirent} path al directori
 */
static int triar (const struct dirent *arg) {
	if (strcmp (arg->d_name, ".") == 0 || strcmp (arg->d_name, "..") == 0) return 0;
	return 1;
}

/**
 * Fp que passa de codi a string el tipus de fitxer
 * @param  sTipus {String} on es guardara el resultat,(ref)
 * @param  nToConvert {Integer} Codi
 */
void conversorTipus (char sTipus[30], int nToConvert) {
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
 * Inicialitza la LinkedList posant tos els elements del directori a la LL
 */
int initLinkedList () {
	struct dirent **arxius;
	struct stat status;
	char sTipus[30];
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
    conversorTipus(sTipus, arxius[nTotalFiles]->d_type);

		//afegir a la cua el nou element: ->LinkedList
		addbeg(arxius[nTotalFiles]->d_name, sTipus, sDate);
		free (arxius[nTotalFiles]);
	}
	free (arxius);
}

/**
 * Mira al directori si hi ha hagut alguna modificacio i ho gestiona la LL
 */
void checkRootFiles () {
	int i = 0, bUpdate = 0;
	struct dirent **arxius;
	struct stat status;
	char *sName, *sDate;
	char sTipus[30];

	int nTotalFiles = scandir (sDirPath, &arxius, triar, alphasort);
	if (arxius == NULL) {
		perror ("scandir");
	}
	printf ("Hi ha %d entrades de directori: %s \n", nTotalFiles, sDirPath);
	i = nTotalFiles;

	while (i--) {
		//date de LL -> sProvaDate
		bUpdate = getDateByName(arxius[i]->d_name);
		if(bUpdate == 0){
			//date del arxiu -> sDate
			if (stat(arxius[i]->d_name, &status) == 0) {
			  sDate = ((char *)ctime(&status.st_mtime));
		  }
	    if(strcmp(sProvaDate, sDate) == 0){
	    	//igual
				printf("iguals no cal fer res\n");
	    }else{
	    	//update
		   	printf("diferents hi ha que actualitzar\n");
		   	setDateByName(arxius[nTotalFiles]->d_name, sDate);
	    }
		}else{
			//afegir
			conversorTipus(sTipus, arxius[nTotalFiles]->d_type);
			addbeg(arxius[nTotalFiles]->d_name, sTipus, sDate);
		}
		free (arxius[i]);
	}
	free (arxius);

	//remove
	if (nTotalFiles < nLastTotalFiles) {
		int i, bExists= 0, nTotal = count();
		//la idea es fero amb getbyname
		i = nTotalFiles;
	}

	return;
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

	return 0;
}