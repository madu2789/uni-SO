/*Single linked list inplementation
1.INSERT A NUMBER AT THE BEGINNING;
2.INSERT A NUMBER AT LAST
3.INSERT A NUMBER AT A PARTICULAR LOCATION IN LIST
4.PRINT THE ELEMENTS IN THE LIST
5.PRINT THE TOTAL NUMBER OF ELEMENTS IN THE LIST
6.DELETE A NODE IN THE LINKED LIST:
8.Exit:*/
#include "LinkedList.h"


/*THIS FUNCTION DELETES A NODE */
void delnode (char sName[30]) {
  struct node *temp, *m;
	temp = p;

  while (temp != NULL) {
    if (strcmp(temp->sName, sName) == 0) {
      if (temp == p) {
        p = temp->next;
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

/* ADD A NEW NODE AT BEGINNING  */
void addbeg (char sName[30], char sTipus[30], char sDate[64], int nSize, struct node *LinkedList) {
  struct node *temp;
  temp =(struct node *)malloc(sizeof(struct node));

  strcpy(temp->sName, sName);
  strcpy(temp->sTipus, sTipus);
  strcpy(temp->sDate, sDate);
  temp->nSize = nSize;
  temp->next = NULL;

  display(temp);

  LinkedList->next = temp;

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

  printf("num total delements: %d ", nElements);
  r = inici;
  return nElements;
}

/*Show a A NODE (molt guarro utilitzant variables globals->REFACTORING)*/
int showNode (char sName[30], char sDate[64], int *nSize, int nLocation) {
  struct node *temp;
  int i = 0, bTrobat = 0;
  temp=p;

    while (temp != NULL && bTrobat == 0) {
        if (i == nLocation-1) {
          bTrobat = 1;
          printf(" ELEMENT %s FOUND!\n", temp->sName);
          strcpy(sName, temp->sName);
          strcpy(sDate, temp->sDate);
          *(nSize) = temp->nSize;
        } else {
          temp = temp->next;
        }
        i++;
    }
  return 0;
}

/*Show a A Date NODE By Name*/
int getDateByName (char sDate[30], char sName[30]) {
  struct node *temp;
  int bTrobat = 0;
  temp = p;

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
int setDateByName (char sName[30], char sDate[64], int nSize) {
  struct node *temp;
  int bTrobat = 0;
  temp = p;

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
  struct node *n;
  int c = 0;
  n = p;
  while (n != NULL) {
    n = n->next;
    c++;
  }
  return(c);
}
