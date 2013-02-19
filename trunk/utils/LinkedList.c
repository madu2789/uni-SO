/*Single linked list inplementation
1.INSERT A NUMBER AT THE BEGINNING;
2.INSERT A NUMBER AT LAST
3.INSERT A NUMBER AT A PARTICULAR LOCATION IN LIST
4.PRINT THE ELEMENTS IN THE LIST
5.PRINT THE TOTAL NUMBER OF ELEMENTS IN THE LIST
6.DELETE A NODE IN THE LINKED LIST:
7.REVERSE A LINKED LIST :
8.Exit:*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
void append (char sName[30], char sTipus[30], char sDate[64]) {
  struct node *temp,*r;
  temp = (struct node *)malloc(sizeof(struct node));

  strcpy(temp->sName, sName);
  strcpy(temp->sTipus, sTipus);
  strcpy(temp->sDate, sDate);
  r = (struct node *)p;

  if (p == NULL) {
    p = temp;
    p->next = NULL;
  } else {
    while (r->next != NULL)
      r = r->next;
      r->next = temp;
      r = temp;
      r->next = NULL;
    }
  }

/* ADD A NEW NODE AT BEGINNING  */
void addbeg (char sName[30], char sTipus[30], char sDate[64]) {
  struct node *temp;
  temp =(struct node *)malloc(sizeof(struct node));

  strcpy(temp->sName, sName);
  strcpy(temp->sTipus, sTipus);
  strcpy(temp->sDate, sDate);

  if (p == NULL) {
    p = temp;
    p->next = NULL;
  } else {
    temp->next = p;
    p = temp;
  }
}

/* ADD A NEW NODE AFTER A SPECIFIED NO OF NODES */
void addafter (int loc , char sName[30], char sTipus[30], char sDate[64]) {
  int i;
  struct node *temp,*t,*r;
  //here r stores the first location
  r = p;

  if (loc > count()+1 || loc <= 0) {
    printf("insertion is not possible :\n");
    return;
  }
  if (loc == 1) {
    addbeg(sName, sTipus, sDate);
    return;
  } else {
    for (i=1;i<loc;i++) {
      t = r;   // t will be holding previous value
      r = r->next;
    }
    temp = (struct node *)malloc(sizeof(struct node));

    strcpy(temp->sName, sName);
    strcpy(temp->sTipus, sTipus);
    strcpy(temp->sDate, sDate);
    t->next = temp;
    t = temp;
    t->next = r;
    return;
  }
}

/* THIS FUNCTION DISPLAYS THE CONTENTS OF THE LINKED LIST */
void display (struct node *r) {
  r = p;
  if (r == NULL) {
    printf("NO ELEMENT IN THE LIST :\n");
    return;
  }
  while (r != NULL) {
    printf(" -> %s \t %s \t %s \n", r->sName, r->sTipus, r->sDate );
    r = r->next;
  }
    printf(" ");
}

/*Show a A NODE (molt guarro utilitzant variables globals->REFACTORING)*/
int showNode (char sName[30], int nLocation) {
  struct node *temp;
  int i = 0, bTrobat = 0;
  temp=p;

    while (temp != NULL && bTrobat == 0) {
        if (i == nLocation-1) {
          bTrobat = 1;
          printf(" ELEMENT %s FOUND!\n", temp->sName);
          strcpy(sName, temp->sName);
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
int setDateByName (char sName[30], char sDate[64]) {
  struct node *temp;
  int bTrobat = 0;
  temp = p;

  while (temp != NULL && bTrobat == 0) {
    if (strcmp(sName, temp->sName) == 0) {
      bTrobat = 1;
      strcpy(temp->sDate, sDate);
    } else {
      temp = temp->next;
    }
  }
  return 0;
}

//THIS FUNCTION COUNTS THE NUMBER OF ELEMENTS IN THE LIST
int count (void) {
  struct node *n;
  int c = 0;
  n = p;
  while (n != NULL) {
    n = n->next;
    c++;
  }
  return(c);
}

//THIS FUNCTION REVERSES A LINKED LIST
void reverse (struct node *q) {
  struct node *m,*n,*s;
  m = q;
  n = NULL;
  while (m != NULL) {
    s = n;
    n = m;
    m = m->next;
    n->next = s;
  }
  p = n;
}

/* THIS IS THE MAIN PROGRAM  */
int main () {
  int i;

	p = NULL;
	while(1) {
		printf("1.INSERT AT BEGINNING;\n");
		printf("2.INSERT AT LAST:\n");
		printf("3.INSERT AT A PARTICULAR LOCATION:\n");
		printf("4.PRINT THE ELEMENTS:\n");
		printf("5.STRLENGTH:\n");
		printf("6.DELETE A NODE:\n");
		printf("7.REVERSE:\n");
    printf("8.getDateByName:\n");
		printf("9.Exit:\n");
		printf("PLEASE, ENTER THE NUMBER:\n");

		scanf("%d",&i);
    switch (i) {
      case 1: {
        char sName[30], sTipus[30], sDate[64];
        printf(" PLEASE ENTER sName, sType, sDate :-");
        scanf("%s",&sName);
        scanf("%s",&sTipus);
        scanf("%s",&sDate);
        addbeg(sName, sTipus, sDate);
        break;
      }
      case 2: {
        int num;
        char sName[30], sTipus[30], sDate[64];
        printf(" PLEASE ENTER THE sName, sType, sDate :-");
        scanf("%s",&sName);
        scanf("%s",&sTipus);
        scanf("%s",&sDate);
        append(sName, sTipus, sDate);
        break;
       }
      case 3: {
        int loc;
        char sName[30], sTipus[30], sDate[64];
        printf(" PLEASE ENTER THE sName, sType, sDate :-");
        scanf("%s",&sName);
        scanf("%s",&sTipus);
        scanf("%s",&sDate);
        printf("PLEASE ENTER THE LOCATION NUMBER :-");
        scanf("%d",&loc);
        addafter(loc, sName, sTipus, sDate);
        break;
      }
      case 4: {
        printf("THE  ELEMENTS IN THE LIST ARE :");
        display(p);
        printf("\n");
        break;
      }
      case 5: {
        printf(" TOTAL NO OF ELEMENTS IN THE LIST ARE %d \n",count());
        break;
      }
      case 6: {
        char sName[30];
        printf("PLEASE ENTER A sName FROM THE LIST :");
        scanf("%s",&sName);
        delnode(sName);
      break;
      }
      case 7: {
        reverse(p);
        display(p);
        printf("\n");
        break;
      }
      case 8: {
        char sName[30], sDate[64];
        printf("PLEASE ENTER A sName FROM THE LIST :");
        scanf("%s",&sName);
        getDateByName(sDate, sName);
        printf("%s\n", sDate);
        break;
      }
      case 9: {
        return 0;
      }
    }
  }
}

