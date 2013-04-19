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
  struct node *next;
} *p;
/* P IS A GLOBAL POINTER CONTAINS THE ADRESS OF THE FIRST NODE*/




//Funciones
void delnode (char sName[30]);
void append (char sName[30], char sTipus[30], char sDate[64], int nSize);
void addbeg (char sName[30], char sTipus[30], char sDate[64], int nSize);
void addafter (int loc , char sName[30], char sTipus[30], char sDate[64], int nSize);
void display (struct node *r);
int showNode (char sName[30], int nLocation);
int getDateByName (char sDate[30], char sName[30]);
int setDateByName (char sName[30], char sDate[64], int nSize);
int count (void);
void reverse (struct node *q);
int main ();


#endif