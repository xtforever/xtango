/***************************************************************************
***
***   Title  : Animation of Fibonacci Heaps
***
***   File   : ~jitaek/xtango/fiboheap.c
***
***   Author : Ji-Taek Hong
***
***   Date   : Dec 1 1993
***
***   References :
***     TANGO: A Framework and System for Algorithm Animation by John Stasko
***
***     The Path-transition Paradigm: A Practical Methodology for Adding
***     Animation to Program Interfaces by John Stasko
***
***     XTANGO Algorithm Animation Designer's Package by John Stasko
***
***     Introduction to Algorithms by Thomas Cormen
***
***     "binoheap.c" and "binoheapscenes.c" by Terry Kane
***
***************************************************************************/

#define   TRUE   1
#define   FALSE  0

#define   DH     10   /* assume the maximum degree is 10 */


#include <stdio.h>

#include "fiboheap.h"


void ANIMInit (), 
     ANIMInsert (),
     ANIMConsolidate (),
     ANIMLink (), 
     ANIMDelete (),
     ANIMExtract ();

static NAME_FUNCT fcn[] =
{
  {"Init", 1, {VOID, (FPTR) ANIMInit}},
  {"Insert", 1, {VOID, (FPTR) ANIMInsert}},
  {"Consolidate", 1, {VOID, (FPTR) ANIMConsolidate}},
  {"Link", 1, {VOID, (FPTR) ANIMLink}},
  {"Delete", 1, {VOID, (FPTR) ANIMDelete}},
  {"Extract", 1, {VOID, (FPTR) ANIMExtract}},
  {NULL, 0, NULL, NULL}
};


pHeap H;   /* Heap pointer */
pHeap N;   /* temporary heap for a new node */

pNode A[DH+1];     /* to be used in Consolidation */


/* initial values for a new heap */

pNode minH = NULL;
int nH=0;  /* the number of nodes in the heap */
int rmH=0; /* the number of root nodes */


/***************************************************************************
***   
***   Function newNode creates a new node and assigns some initial vlaues
***   and returns the node pointer.
***
***************************************************************************/

pNode newNode ()
{
   pNode ptr = (pNode) malloc (sizeof (struct node));

   ptr -> p = NULL;   /* parent */
   ptr -> child = NULL;
   ptr -> right = NULL;
   ptr -> mark = FALSE;
   ptr -> sibling = NULL;
   ptr -> key = 0;
   ptr -> degree = 0;

   ptr -> loc = NULL;
   ptr -> tree_loc = NULL;
   ptr -> tree_im = NULL;

   return ptr;
}


/***************************************************************************
***   
***   Function newHeap creates a new Heap and assigns some initial vlaues
***   and returns the heap pointer.
***
***************************************************************************/

pHeap newHeap ()
{
   pHeap p = (pHeap) malloc (sizeof (struct heap));

   p -> loc = NULL;
   p -> head = NULL;

   return p;
}


/***************************************************************************
***   
***   Procedure addMember adds a node into the root list.
***
***************************************************************************/

void addMember(x)
     pNode x;
{
  ++rmH;   /* increment the number of root nodes */

  if ( minH == NULL )   /* if empty heap */
     minH = x;
  else   /* insert x right after minH */
    {
      x->right = minH->right;
      minH->right = x;
    }
}


/***************************************************************************
***   
***   Procedure Insert inserts node x into the Fibonacci heap.
***
***************************************************************************/

void Insert (x)
     pNode x;
{
   x -> right = x;   /* The root list is circular. */

   addMember(x);
 
   if ( (minH == NULL) || (x->key < minH->key) ) 
     minH = x;

   ++nH;   /* increment the number of nodes in the Fibonacci heap */

   H -> head = minH;
   TANGOalgoOp (fcn, "Insert", x);
}


/***************************************************************************
***   
***   Function getLeft returns the left node of x which is given as
***   parameter.
***
***************************************************************************/

pNode getLeft(x)
     pNode x;
{
  pNode y = x;

  while (y->right != x)
    y = y->right;

  return y;
}


/***************************************************************************
***   
***   Procedure remove removes x from the root list of the Fibonacci heap.
***
***************************************************************************/

void remove(x)
     pNode x;
{
  pNode p;

  p = getLeft(x);

  if ( p == x )
    minH = NULL;
  else
    {
      printf("\nget-left : %d\n", p->key);
      p->right = x->right;
    }

  --rmH;   /* decrement the number of root nodes */
}


/***************************************************************************
***   
***   Procedure FibHeapLink removes y from the root list and makes y 
***   a child of x and sets the "mark" of y as False.
***
***************************************************************************/

void FibHeapLink(y, x)
     pNode y, x;
{
  remove(y);   /* remove y from the root list of H */

  /* make y a child of x */
  y -> p = x;
  y->sibling = x->child;
  x->child = y;
  ++(x->degree);

  y->mark = FALSE;

  TANGOalgoOp (fcn, "Link", x);
}


/***************************************************************************
***   
***   Procedure Consolidate reduce the number of trees in the Fibonacci
***   heap by calling FibHeapLink repeatedly until every node in the
***   root list has a distinct degree valsue.
***
***************************************************************************/

void Consolidate()
{
  int i;
  int nR = rmH;   /* the number of root nodes */
  int d;   /* degree */

  pNode x,y,w,temp;

  TANGOalgoOp (fcn, "Consolidate");

  for (i=0; i<=DH; i++)
    A[i] = NULL;

  /* for each node w in the root list of H */
  
  A[minH->degree] = minH;

  if ( minH != minH->right )
    {
      w = minH;
      for (i=0; i<(nR-1); i++)
	{
	  w = w->right;
	  x = w;
	  d = x->degree;
	  while ( A[d] != NULL )
	    {
	      y = A[d];
	      if ( x->key > y->key ) 
		{
		  temp = x;
		  x = y;
		  y = temp;
		}
	      FibHeapLink(y, x);
	      A[d] = NULL;
	      ++d;
	    }
	  A[d] = x;
	}
    }
  minH = NULL;

  for (i=0; i<=DH; i++) 
    if ( A[i] != NULL )
      if ( (minH == NULL) || ((A[i])->key < minH->key) )
	minH = A[i];
}


/***************************************************************************
***   
***   Procedure childMember add the children of a removed node to the
***   root list.
***
***************************************************************************/

void childMember(x)
     pNode x;
{
   if ( x != NULL )
     {
       addMember(x);
       x->p = NULL;
       childMember(x->sibling);   /* recursive step */
     }
}


/***************************************************************************
***   
***   Function ExtractMin returns the pointer to the minimum node.
***   In addition, it does the delayed work of consolidating trees 
***   in the root list
***
***************************************************************************/

pNode ExtractMin()
{
   pNode z = minH;

   if (z != NULL) 
     {
       /* add x to the root list of H */
       /* x -> p = NULL */
     
       childMember(z->child);

       /* remove z from the root list of H */
       remove(z);

       TANGOalgoOp (fcn, "Delete", z);

       if ( z == z->right ) 
	 minH = NULL;
       else
	 {
	   minH = z->right;
	   H->head = minH;
           Consolidate();
	 }

       --nH;
     }

   H -> head = minH;
   TANGOalgoOp (fcn, "Extract");

   return z;
}


/***************************************************************************
***
***   main
***
***************************************************************************/

main ()
{
   pNode x;

   char cmd[32];
   char cmdbuf[64];
   char buf[32];
   int i, n;

   int tty = isatty (stdin);

   TANGOalgoOp (fcn, "BEGIN");

   printf("\n==============================\n");
   printf(" Animation of Fibonacci Heaps\n");
   printf("==============================\n");
   printf("Commands are:\n");
   printf("   i <num>        insert a value\n");
   printf("   e              extract minimum\n");
   printf("   q              quit\n");

   H = newHeap();
   N = newHeap(); 

   TANGOalgoOp (fcn, "Init");

   do {
      *cmd = '\0';
      printf ("\nEnter a command: ");
      gets (buf);
      sprintf (cmdbuf, "%s 0", buf);

      sscanf (cmdbuf, "%s %d", cmd, &i);

      if (*cmd) {
	 switch (*cmd) {
	 case 'I':
	 case 'i':
	    if (!tty)
	       printf ("Insert %d\n", i);
	    x = newNode ();
	    x -> key = i;
	    Insert (x);
	    break;

	 case 'E':
	 case 'e':
	    if (!tty)
	       printf ("Extract Minimum\n");
            x = ExtractMin();
            if ( x != NULL )
	      {
		printf ("   removed node w/ value: %d\n", x -> key);
		free(x);
	      }
	    else
	      printf ("   empty heap!\n");
	    break;

	 case 'R':   /* Root nodes */
	 case 'r':
	    if (!tty)
	      {
		n=0;
		x = minH;

		if ( minH == NULL )
		  printf("\n empty root list!\n");
		else
		  {
		    printf("%d ", x->key);
		    x = x->right;
		    while ( x != minH ) 
		      {
			printf("%d ", x->key);
			x = x->right;
		      }
		    putchar('\n');
		  }
	      }   
	    break;

	 case 'Q':
	 case 'q':
	    printf (" ok - quit\n");
	    *cmd = '\0';
	    break;

	 default:
	    printf ("Command not recognized.\n");
	 }
      }
   } while (*cmd);

   TANGOalgoOp (fcn, "END");
}

