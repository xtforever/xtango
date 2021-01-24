#include <stdio.h>
#include <stdlib.h>

#include "xtango.h"

/* Binomail Heaps by Terry Kane */



void ANIMInit (), ANIMLink (), ANIMMerge (), ANIMUnion (), ANIMInsert (),
     ANIMDelete (), ANIMExtract ();

static NAME_FUNCT fcn[]=
{
   {"Init", 1,
    {VOID, (FPTR) ANIMInit}},
   {"Link", 1,
    {VOID, (FPTR) ANIMLink}},
   {"Merge", 1,
    {VOID, (FPTR) ANIMMerge}},
   {"Union", 1,
    {VOID, (FPTR) ANIMUnion}},
   {"Insert", 1,
    {VOID, (FPTR) ANIMInsert}},
   {"Delete", 1,
    {VOID, (FPTR) ANIMDelete}},
   {"Extract", 1,
    {VOID, (FPTR) ANIMExtract}},
   {NULL, 0, NULL, NULL}
};

/* ---------------------------------------------------------------- */

typedef struct heap * pHeap;
typedef struct node * pNode;

struct node {
   pNode p;
   pNode child;
   pNode sibling;
   int key;
   int degree;

   TANGO_LOC loc;
   TANGO_LOC tree_loc;
   TANGO_IMAGE tree_im;
};

struct heap {
   pNode	head;
   TANGO_LOC	loc;
};

pHeap H;
pHeap M;
pHeap N;
pNode linknode;

/* ---------------------------------------------------------------- */

#define MinInfty	(1 << ((8 * (int)sizeof(int)) - 1))
#define Infty		(~MinInfty)

#define Null	((pNode)NULL)

/* ---------------------------------------------------------------- */

int debugFlag = 0;

/* ---------------------------------------------------------------- */

pNode
newNode ()
{
   pNode tmp;

   tmp = (pNode) malloc (sizeof (struct node));

   tmp -> p = Null;
   tmp -> child = Null;
   tmp -> sibling = Null;
   tmp -> key = 0;
   tmp -> degree = 0;

   tmp->loc = NULL;
   tmp->tree_loc = NULL;
   tmp->tree_im = NULL;

   return tmp;
}

void
freeNode (x)
pNode x;
{
   free (x);
}


pHeap
newHeap ()
{
   pHeap tmp;

   tmp = (pHeap) malloc (sizeof (struct heap));
   tmp->loc = NULL;

   tmp -> head = Null;
   return tmp;
}

void
freeHeap (H)
pHeap H;
{
   free (H);
}

/* ---------------------------------------------------------------- */

void
printTree (x, level)
pNode x;
int level;
{
   int i = level;

   if (!x)
      return;

   while (--i)
      putchar ('.');
   printf ("%d\n", x -> key);

/*
   printf("[%2d] key = %-6d   ~(%d)\n", level, x->key, x->degree);
 */

   if (x -> child) {
      printf (" Child   ");
      printTree (x -> child, level + 1);
   }

   if (x -> sibling) {
      if (level == 1)
	 printf ("\n Root    ");
      else
	 printf (" Sibling ");

      printTree (x -> sibling, level);
   }
}

/* ---------------------------------------------------------------- */

void
printHeap (H)
pHeap H;
{
   if (!H)
      return;

   printf (" Root    ");
   printTree (H -> head, 1);
}

/* ---------------------------------------------------------------- */

void
Link (y, z)
pNode y;
pNode z;
{
   y -> p = z;
   y -> sibling = z -> child;
   z -> child = y;
   z -> degree++;

   linknode = z;
   TANGOalgoOp (fcn, "Link");
}

/* ---------------------------------------------------------------- */

void
MMerge()
{
   pNode cur1 = H -> head;
   pNode cur2 = N -> head;
   pNode next1;
   pNode next2;

   pNode prev = Null;

   pNode returnVal = Null;

   if (!cur1 && !cur2)
      return;

   if (cur1 && !cur2) {
      M -> head = cur1;
   }

   else if (cur2 && !cur1) {
      M -> head = cur2;
   }

   else {
      if (cur1 -> degree <= cur2 -> degree) {
	 M -> head = prev = cur1;
	 if (cur1 -> sibling)
	   next1 = cur1 -> sibling;
	 cur1 = cur1 -> sibling;
      }
      else {
	 M -> head = prev = cur2;
	 if (cur2 -> sibling)
	   next2 = cur2 -> sibling;
	 cur2 = cur2 -> sibling;
      }

      while (cur1 && cur2) {
	 if (cur1 -> degree <= cur2 -> degree) {
	    prev -> sibling = cur1;
	    prev = cur1;
	    cur1 = cur1 -> sibling;
	    if (cur1)
	      next1 = cur1 -> sibling;
	 }
	 else {
	    prev -> sibling = cur2;
	    prev = cur2;
	    cur2 = cur2 -> sibling;
	    if (cur2)
	      next2 = cur2 -> sibling;
	 }
      }

      if (cur1 && !cur2) {
	 prev -> sibling = cur1;
      }

      if (cur2 && !cur1) {
	 prev -> sibling = cur2;
      }
   }

   TANGOalgoOp (fcn, "Merge");

   H -> head = Null;
   N -> head = Null;
   return;
}

/* ---------------------------------------------------------------- */

void
Union ()
{
   pNode prev_x, x, next_x;

   if (debugFlag) {
      printf ("Debugging Union (before Merge)\n\n");
      printf("Heap H\n");
      printHeap(H);
      printf("\nHeap N\n");
      printHeap(N);
   }

   MMerge ();

   if (debugFlag) {
      printf ("\n                (after Merge)\n\n");
      printf("\nHeap M\n");
      printHeap(M);
   }

   if (!M->head)
      return;

   prev_x = Null;
   x = H -> head = M -> head;
   next_x = x -> sibling;

   while (next_x) {
      if (x -> degree != next_x -> degree ||
	  (next_x -> sibling &&
	   next_x -> sibling -> degree == x -> degree)) {
	 prev_x = x;
	 x = next_x;
      }
      else {
	 if (x -> key <= next_x -> key) {
	    x -> sibling = next_x -> sibling;
	    Link (next_x, x);
	 }
	 else {
	    if (prev_x == Null)
	       H -> head = next_x;
	    else
	       prev_x -> sibling = next_x;
	    Link (x, next_x);
	    x = next_x;
	 }
      }
      next_x = x -> sibling;
   }

   if (debugFlag) {
      printf("\n                (after Union)\n\n");
      printf("\nHeap H\n");
      printHeap(H);
   }

   TANGOalgoOp (fcn, "Union");

   M -> head = NULL;
}

/* ---------------------------------------------------------------- */

void
Insert (x)
pNode x;
{
   x -> p = Null;
   x -> child = Null;
   x -> sibling = Null;
   x -> degree = 0;

   N -> head = x;

   TANGOalgoOp (fcn, "Insert", x);

   Union ();
}

/* ---------------------------------------------------------------- */

pNode
minHeap (H)
pHeap H;
{
   pNode y = Null;
   pNode x = H -> head;
   int least = Infty;

   while (x != Null) {
      if (x -> key < least) {
	 least = x -> key;
	 y = x;
      }
      x = x -> sibling;
   }
   return y;
}

/* ---------------------------------------------------------------- */

void
Extract (returnX)
pNode *returnX;
{
   pNode x = minHeap (H);
   pNode a = H -> head;
   pNode b = a -> sibling;
   pNode c = Null;

   if (a == x) {
      H -> head = x -> sibling;
   }
   else {
      while (b != x) {
	 a = b;
	 b = a -> sibling;
      }
      a -> sibling = x -> sibling;
   }

   TANGOalgoOp (fcn, "Delete", x);

   a = x -> child;

   if (a) {
      a -> p = Null;
      b = a -> sibling;
      if (b) {
	 c = b -> sibling;
	 a -> sibling = Null;
	 do {
	    b -> p = Null;
	    b -> sibling = a;
	    a = b;
	    b = c;
	    if (c)
	       c = c -> sibling;
	 } while (b);
      }
   }

   N -> head = a;

   TANGOalgoOp (fcn, "Extract");

   Union ();

   *returnX = x;
}

/* ---------------------------------------------------------------- */

int
main (argc, argv)
int argc;
char *argv[];
{
   pNode x;

   char cmd[32];
   char cmdbuf[64];
   char buf[32];
   int i;

   int tty = isatty (stdin);

   printf ("\nEntering Binomial Heaps Demo\n");
   printf("Commands are:\n");
   printf("   i <num>        insert a value\n");
   printf("   e              extract minimum\n");
   printf("   q              quit\n");

   TANGOalgoOp (fcn, "BEGIN");

   H = newHeap();
   M = newHeap();
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
	    Extract (&x);
	    printf ("   removed node w/ value: %d\n", x -> key);
	    break;

	 case 'P':
	 case 'p':
	    if (!tty)
	       printf ("Print\n");
	    printHeap (H);
	    break;

	 case 'D':
	 case 'd':
	    if (!tty)
	       printf ("Debugging at level: %d\n", i);
	    debugFlag = i;
	    break;

	 case 'Q':
	 case 'q':
	    printf (" ok - quit\n");
	    *cmd = '\0';
	    break;

	 default:
	    printf ("Invalid command!\n");
	 }
      }
   } while (*cmd);

   TANGOalgoOp (fcn, "END");
   printf ("\nDone.\n");
}

