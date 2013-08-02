
#include "LinkedList.h"


/*THIS FUNCTION DELETES A NODE */
int delnode (char sName[30], struct node *LinkedList) {
  struct node *temp, *anterior;
  int i = 0;

  temp = (struct node *)malloc(sizeof(struct node));
  anterior = (struct node *)malloc(sizeof(struct node));

	temp = LinkedList;
  anterior = LinkedList;

  while (temp != NULL) {
    if (strcmp(temp->sName, sName) == 0) {
      printf("borrant: %s\n", temp->sName);
      if ( i == 0 ) {
        printf("primer cabron!\n");
        LinkedList = temp->next;
      } else {
        anterior->next = temp->next;
        temp->next = NULL;
        temp = NULL;
        free(temp);
      }    
      return 1;
    } else {
      anterior = temp;
      temp = temp->next;
    }
    i++;
	}
  return 1;
}

/*THIS FUNCTION Buida La LL */
void buidaLL (struct node *LinkedList) {

  struct node *temp, *r;

  r = LinkedList;

  while (r != NULL) {
    temp = r;
    r = r->next;
    free(temp);
  }

  LinkedList = (struct node *)malloc(sizeof(struct node));
  strcpy(LinkedList->sName,"fantasma");
  LinkedList->next = NULL;

}



/*THIS FUNCTION ADDS A NODE AT THE LAST OF LINKED LIST */
void append (char sName[30], char sTipus[30], char sDate[64], int nSize, int nEstat, struct node *LinkedList) {
  struct node *temp,*r;
  temp = (struct node *)malloc(sizeof(struct node));

  strcpy(temp->sName, sName);
  strcpy(temp->sTipus, sTipus);
  strcpy(temp->sDate, sDate);
  temp->nSize = nSize;
  temp->nEstat = nEstat;
  r = (struct node *)LinkedList;

  if (strcmp(LinkedList->sName, "fantasma") == 0) {
    strcpy(LinkedList->sName, sName);
    strcpy(LinkedList->sTipus, sTipus);
    strcpy(LinkedList->sDate, sDate);
    LinkedList->nSize = nSize;
    LinkedList->nEstat = nEstat;
  } else {
    while (r->next != NULL)
      r = r->next;
      r->next = temp;
      r = temp;
      r->next = NULL;
  }
}



/* THIS FUNCTION DISPLAYS THE CONTENTS OF THE LINKED LIST */
int display (struct node *r) {
  struct node *inici;
  inici = r;
  //r = p;
  int nElements = 0;
  if (r == NULL) {
    printf("NO ELEMENT IN THE LIST :\n");
    return;
  }
  printf("\t sName \t sTipus \t sDate \t nSize \t nEstat \n");
  while (r != NULL) {
    printf(" -> %s \t %s \t %s \t %d \t %d \n", r->sName, r->sTipus, r->sDate, r->nSize, r->nEstat );
    r = r->next;
    nElements++;
  }

  //printf("num total delements: %d ", nElements-1);
  r = inici;
  return nElements;
}


/*Show a A NODE (molt guarro utilitzant variables globals->REFACTORING)*/
int showNode (char sName[30], char sDate[64], int nLocation, struct node *LinkedList) {
  struct node *temp;
  struct node *inici;
  int i = 0, bTrobat = 0;
  int nSize = 0;
  temp = LinkedList;

    while (temp != NULL && bTrobat == 0) {
        if (i == nLocation-1) {
          bTrobat = 1;
          //printf(" ELEMENT %s FOUND!\n", temp->sName);
          strcpy(sName, temp->sName);
          strcpy(sDate, temp->sDate);
          nSize = temp->nSize;
        } else {
          temp = temp->next;
        }
        i++;
    }
  return nSize;
}



/*Show a A Date NODE By Name*/
int getDateByName (char sDate[30], char sName[30], struct node *LinkedList) {
  struct node *temp;
  struct node *inici;
  int bTrobat = 0;
  temp = LinkedList;

  while(temp != NULL && bTrobat == 0){
    if(strcmp(sName, temp->sName) == 0){
      bTrobat = 1;
      strcpy(sDate, temp->sDate);
    } else {
      temp= temp->next;
    }
    //printf(" ELEMENT %s FOUND!\n", temp->sName);
  }
  return bTrobat;
}


/*Canvia la sDate segons el sName*/
int setDateByName (char sName[30], char sDate[64], int nSize, struct node *LinkedList) {
  struct node *temp;
  struct node *inici;
  int bTrobat = 0;
  temp = LinkedList;

  while (temp != NULL && bTrobat == 0) {
    if (strcmp(sName, temp->sName) == 0) {
      bTrobat = 1;
      strcpy(temp->sDate, sDate);
      temp->nSize = nSize;
    } else {
      temp = temp->next;
    }
  }
  return bTrobat;
}



/*RETURN nEstat of a Node*/
int getEstatByName (char sName[30], struct node *LinkedList) {
  struct node *temp;
  struct node *inici;
  int bTrobat = 0;
  temp = LinkedList;

  while(temp != NULL && bTrobat == 0){
    if(strcmp(sName, temp->sName) == 0){
      bTrobat = 1;
    } else {
      temp= temp->next;
    }

  }
  return temp->nEstat;
}


/*Canvia la sDate segons el sName*/
int setEstatByName (char sName[30], int nEstat, struct node *LinkedList) {
  struct node *temp;
  struct node *inici;
  int bTrobat = 0;
  temp = LinkedList;

  while (temp != NULL && bTrobat == 0) {
    if (strcmp(sName, temp->sName) == 0) {
      bTrobat = 1;
      temp->nEstat = nEstat;
    } else {
      temp = temp->next;
    }
  }
  return bTrobat;
}

/*Canvia la nSize segons el sName*/
int setSizeByName (char sName[30], int nSize, struct node *LinkedList) {
  struct node *temp;
  struct node *inici;
  int bTrobat = 0;
  temp = LinkedList;

  while (temp != NULL && bTrobat == 0) {
    if (strcmp(sName, temp->sName) == 0) {
      bTrobat = 1;
      temp->nSize = nSize;
    } else {
      temp = temp->next;
    }
  }
  return bTrobat;
}



//THIS FUNCTION COUNTS THE NUMBER OF ELEMENTS IN THE LIST
int count (struct node *LinkedList) {
  struct node *inici;
  inici = LinkedList;
  //r = p;
  int nElements = 0;
  if (LinkedList == NULL) {
    printf("NO ELEMENT IN THE LIST :\n");
    return;
  }
  while (LinkedList != NULL) {
    LinkedList = LinkedList->next;
    nElements++;
  }

  LinkedList = inici;
  
  return nElements;
}
