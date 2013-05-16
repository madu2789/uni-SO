
#include "LinkedList.h"


/*THIS FUNCTION DELETES A NODE */
void delnode (char sName[30], struct node *LinkedList) {
  struct node *temp, *m;
  struct node *inici;
	temp = LinkedList;

  while (temp != NULL) {
    if (strcmp(temp->sName, sName) == 0) {
      if (temp == LinkedList) {
        LinkedList = temp->next;
        //free(temp); diria k no cal alliberar pk no arribes a demanar memoria (HIL?)
        return;
      } else {
        m->next = temp->next;
        //free(temp);
        return;
      }
    } else {
      m = temp;
      temp = temp->next;
    }
	}
}



/*THIS FUNCTION ADDS A NODE AT THE LAST OF LINKED LIST */
void append (char sName[30], char sTipus[30], char sDate[64], int nSize, struct node *LinkedList) {
  struct node *temp,*r;
  temp = (struct node *)malloc(sizeof(struct node));

  strcpy(temp->sName, sName);
  strcpy(temp->sTipus, sTipus);
  strcpy(temp->sDate, sDate);
  temp->nSize = nSize;
  r = (struct node *)LinkedList;

  if (LinkedList == NULL) {
    LinkedList = temp;
    LinkedList->next = NULL;
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
  while (r != NULL) {
    printf(" -> %s \t %s \t %s \t %d \n", r->sName, r->sTipus, r->sDate, r->nSize );
    r = r->next;
    nElements++;
  }

  //printf("num total delements: %d ", nElements-1);
  r = inici;
  return nElements-1;
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
  return 0;
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
  return nElements-1;
}
