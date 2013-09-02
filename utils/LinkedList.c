
#include "LinkedList.h"


/*THIS FUNCTION DELETES A NODE */
int delnode (char sName[30], struct node *LinkedList) {
  struct node *temp, *anterior, *inici, *r, *temp2;
  int i = 0;
	temp = LinkedList;
  anterior = LinkedList;

  while (temp != NULL) {
    if (strcmp(temp->sName, sName) == 0) {
      if ( i == 0 ) {
        temp2 = LinkedList;
        LinkedList = LinkedList->next;
        temp2->next = NULL;
        temp2 = NULL;
        free(temp2);
        return 1;
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
  char sFrase[MAX];
  inici = r;

  int nElements = 0;
  if (r == NULL) {
    return 0;
  }
  sprintf (sFrase,"\nLinkedList:\n\tsName:\n \t\t");
  write (1, sFrase, strlen (sFrase));
  while (r != NULL) {

    sprintf (sFrase, r->sName);
    write (1, sFrase, strlen (sFrase));
    sprintf (sFrase,"\n \t\t");
    write (1, sFrase, strlen (sFrase));
    r = r->next;
    nElements++;
  }

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

  int nElements = 0;
  if (LinkedList == NULL) {
    return 0;
  }
  if (strcmp(LinkedList->sName, "fantasma") == 0) {
    return 0;
  }
  while (LinkedList != NULL) {
    LinkedList = LinkedList->next;
    nElements++;
  }

  LinkedList = inici;
  
  return nElements;
}
