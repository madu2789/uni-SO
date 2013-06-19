/* **********************************************
*
* Fitxer de capçalera de LinkedList
*
* Daniel Madurell Blanes (is19445)
* Albert Hilazo Aguilera (se19467)
*
* ********************************************** */
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__


// Includes del Sistema
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Includes propios
#include "log.h"

// Definicion de constantes


// Definicion de tipos propios
/* STRUCTURE : DATA PART AND A LINK PART */
struct node {
  char sName[30];
  char sTipus[30];
  char sDate[64];
  int nSize;
  int nEstat;
  struct node *next;
} *p;


//Funciones

void delnode (char sName[30], struct node *LinkedList);
void append (char sName[30], char sTipus[30], char sDate[64], int nSize, int nEstat, struct node *LinkedList);
int display (struct node *r);
int showNode (char sName[30], char sDate[64], int nLocation, struct node *LinkedList) ;
int getDateByName (char sDate[30], char sName[30], struct node *LinkedList);
int setDateByName (char sName[30], char sDate[64], int nSize, struct node *LinkedList);
int getEstatByName (char sName[30], struct node *LinkedList);
int setEstatByName (char sName[30], int nEstat, struct node *LinkedList);
void buidaLL (struct node *LinkedList);
int count (struct node *LinkedList);


#endif
