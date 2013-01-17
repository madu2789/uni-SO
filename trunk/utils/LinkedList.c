  /*Single linked list inplementation using different functions

1.INSERT A NUMBER AT THE BEGINNING;

2.INSERT A NUMBER AT LAST

3.INSERT A NUMBER AT A PARTICULAR LOCATION IN LIST

4.PRINT THE ELEMENTS IN THE LIST

5.PRINT THE TOTAL NUMBER OF ELEMENTS IN THE LIST

6.DELETE A NODE IN THE LINKED LIST:

7.REVERSE A LINKED LIST :

8.GET OUT OF LINKED LIST (BYEE BYEE):*/
  
  
  /* PROGRAM IMPLEMENTATION OF SINGLE LINKED LIST */

  #include"stdio.h"
  //#define NULL 0
  /* STRUCTURE CONTANING A DATA PART AND A LINK PART */

   struct node
 {
   int data;
   struct node *next;
 }*p;

  /* P IS A GLOBAL POINTER CONTAINS THE ADRESS OF THE FIRST NODE IN LIST*/

 /*THIS FUNCTION DELETES A NODE */

     void delnode(int num)
 {
     struct node *temp, *m;
		temp=p;
		
      while(temp!=NULL)
    {
       if(temp->data==num)
       {
           if(temp==p)
           {
              p=temp->next;
              free(temp);
              return;
           }
           else
         {
           m->next=temp->next;
           free(temp);
           return;
         }
      }else
        {
           m=temp;
          temp= temp->next;
        }

	}
    printf(" ELEMENT %d NOT FOUND ", num);
    
}

/*THIS FUNCTION ADDS A NODE AT THE LAST OF LINKED LIST */

   void append( int num )
 {
     struct node *temp,*r;
     /* CREATING A NODE AND ASSIGNING A VALUE TO IT */

       temp= (struct node *)malloc(sizeof(struct node));
       temp->data=num;
       r=(struct node *)p;

      if (p == NULL) /* IF LIST IS EMPTY CREATE FIRST NODE */
     {
    p=temp;
         p->next =NULL;
     }
  else
     {        /* GO TO LAST AND ADD*/

             while( r->next != NULL)
       r=r->next;
       r->next =temp;
       r=temp;
       r->next=NULL;
     }
  }
  
  /* ADD A NEW NODE AT BEGINNING  */

   void addbeg( int num )
   {
        /*  CREATING A NODE AND INSERTING VALUE TO IT  */

   struct node *temp;
        temp=(struct node *)malloc(sizeof(struct node));
        temp->data=num;

       /* IF LIST IS NULL ADD AT BEGINNING  */
        if ( p== NULL)
       {
          p=temp;
          p->next=NULL;
       }

   else
      {
          temp->next=p;
          p=temp;
      }
   }

 /*  ADD A NEW NODE AFTER A SPECIFIED NO OF NODES */

 void addafter(int num, int loc)
  {
     int i;
     struct node *temp,*t,*r;
     r=p;       /* here r stores the first location */
      if(loc > count()+1 || loc <= 0)
   {
         printf("insertion is not possible :");
            return;
   }
        if (loc == 1)/* if list is null then add at beginning */
         {
           addbeg(num);
           return;
         }
      else
 {
       for(i=1;i<loc;i++)
          {
             t=r;   /* t will be holding previous value */
             r=r->next;
          }
         temp=(struct node *)malloc(sizeof(struct node));
         temp->data=num;
         t->next=temp;
         t=temp;
         t->next=r;
        return;
       }
}/* THIS FUNCTION DISPLAYS THE CONTENTS OF THE LINKED LIST */

  void display(struct node *r)
  {
      r=p;
      if(r==NULL)
     {
       printf("NO ELEMENT IN THE LIST :");
       return;
     }
       /* traverse the entire linked list */
       while(r!=NULL)
    {
      printf(" -> %d ",r->data);
      r=r->next;
    }
     printf("");
  }
//THIS FUNCTION COUNTS THE NUMBER OF ELEMENTS IN THE LIST
int count(void)
 {
   struct node *n;
   int c=0;
   n=p;
    while(n!=NULL)
   {
     n=n->next;
     c++;
   }
  return(c);
 }
 
//THIS FUNCTION REVERSES A LINKED LIST
void reverse(struct node *q)
{
   struct node *m, *n,*l,*s;
   m=q;
   n=NULL;
  while(m!=NULL)
 {
   s=n;
   n=m;
  m=m->next;
  n->next=s;
 }
   p=n;
}


/* THIS IS THE MAIN PROGRAM  */

  void main()
 {
        int i;
   p=NULL;
   while(1) /* this is an indefinite loop */
 {
    printf("1.INSERT A NUMBER AT BEGINNING;\n");
    printf("2.INSERT A NUMBER AT LAST:\n");
    printf("3.INSERT A NUMBER AT A PARTICULAR LOCATION INlIST:\n");
    printf("4.PRINT THE ELEMENTS IN THE LIST :\n");
    printf("5.PRINT THE NUMBER OF ELEMENTS IN THE LIST:\n");
    printf("6.DELETE A NODE IN THE LINKED LIST:\n");
    printf("7.REVERSE A LINKED LIST :\n");
    printf("8.GET OUT OF LINKED LIST (BYEE BYEE):\n");
    printf("PLEASE, ENTER THE NUMBER:\n");

    scanf("%d",&i); /* ENTER A VALUE FOR SWITCH  */

      switch(i)
    {
         case 1:
      {
        int num;
        printf(" PLEASE ENTER THE NUMBER :-");
        scanf("%d",&num);
        addbeg(num);
        break;
      }
          case 2:
       {
         int num;
         printf(" PLEASE ENTER THE NUMBER :-");
         scanf("%d",&num);
         append(num);
         break;
       }

   case 3:
     {
      int num, loc;
      printf(" PLEASE ENTER THE NUMBER :-");
      scanf("%d",&num);
      printf("PLEASE ENTER THE LOCATION NUMBER :-");
      scanf("%d",&loc);
      addafter(num,loc);
      break;
    }  case 4:
      {
         struct node *n;
         printf("THE  ELEMENTS IN THE LIST ARE :");
         display(n);
         break;
      }

      case 5:
   {
      struct node *n;
      display(n);
      printf(" TOTAL NO OF ELEMENTS IN THE LSIT ARE %d",count());
      break;
   } case 6:
    {
            int    num;
      printf("PLEASE ENTER A NUMBER FROM THE LIST :");
      scanf("%d",&num);
      delnode(num);
     break;
    }
   case 7:
    {
      reverse(p);
        display(p);
        break;
    }
  case 8:
 {
  exit(1);
 }
    }/* end if switch */
 }/* end of while */
}/* end of main */

