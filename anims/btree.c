                  /* B-Trees */
/* (written by: Paul Rowland) Project #1 Animating Computer Algorithms */
#include "btree.h"

static btree *root = NIL;  /* root of the BTree */
int lastlevel = 0;         /* depth of the Btree */
int level = 0;             /* current level in the tree */
int children[maxlevels];   /* # of children at levels 0 - (maxlevels - 1) */

/* XTANGO functions used to animate the various operations in the Btree
                    insertion */

void ANIMInit(), ANIMNew(), ANIMInsPt(), ANIMSplit(), ANIMAddNew();

static NAME_FUNCT fcn[] = {{"Init", 1, {VOID, (FPTR) ANIMInit}},
                           {"Slide", 1, {VOID, (FPTR) ANIMInsPt}},
                           {"Split", 1, {VOID, (FPTR) ANIMSplit}},
                           {"Add", 1, {VOID, (FPTR) ANIMAddNew}},
                           {"New", 1, {VOID, (FPTR) ANIMNew}},
                           {NULL, 0, NULL, NULL}};
void MakeBTree()
/* MakeBTree initializes the node of the BTree that will get filled in later.*/
{
  int j;

  TANGOalgoOp(fcn, "Init");  /* initialize XTANGO window */
  root = (btree *) malloc( sizeof(btree));
  root->leaf = TRUE;
  root->n  = root->fromleft = 0;
  for (j = 0; j < maxkey; j++) root->keys[j] = 0; /* clear keys */
  for (j = 0; j < maxc; j++) root->c[j] = NIL;    /* clear children */

/* initialize the # of children to 0 for all levels but 0 which has 1 child
   (the root) */

  for (j = 1, children[0] = 1; j < maxlevels; j++) children[j] = 0;
}

void BTreeSplitChild(x,y,i)

btree * x, * y;
int i;

/* BTreeSplitChild takes y and splits it into two parts. Then y and
   z (the other half of y) become children of x, which is the parent of
   y and x also receives the middle entry of y and inserts it into position
   i of x. level will be set to the parents depth (i.e. x) */
{
  btree * z;
  int j;

/* Perform split operations in XTANGO first then fix tree */
   TANGOalgoOp(fcn, "Slide", x->fromleft, i, x->n, level);
   TANGOalgoOp(fcn, "Split", y->fromleft, x->fromleft, i, level + 1);

  children[level + 1]++;

/* make z (the right child) and initialize its fields, while clearing
those fields in y. */

  z = (btree *) malloc(sizeof(btree));
  z->leaf = y->leaf;
  z->n = y->n = t-1;
  for ( j = 0; j < t - 1; j++) {
       z->keys[j] = y->keys[j+t];
       y->keys[j+t] = NIL;
    }

  if (!y->leaf)
      for (j = 0; j < t; j++) {
          z->c[j] = y->c[j+t];
          y->keys[j+t] = 0;
       }

 /* move x's children, keys, and positional indicators over 1 to make room
    for z and y's middle key */
 
  for (j = x->n; j > i; j--) {
    x->c[j]->fromleft++;
    x->c[j+1] = x->c[j];
  }
  x->c[i + 1] = z;

  for (j = (x->n - 1); j > (i-1); j--)
     x->keys[j+1] = x->keys[j];
  x->keys[i] = y->keys[t - 1];
  y->keys[t - 1] = 0;
  x->n++;
}

void BTreeInsertNonFull(x, k)

btree *x;
int k;
/* BTreeInsertNonFull inserts k at x and it assumes x is not full. If 
   x is a leaf then the insertion is easy. If not then it tries to split 
   the appropriate child and then insert k in that new (non-empty) node. */ 
{
  int i = (x->n - 1);       /* last key in x */

  if (x->leaf) {
   /* find point of insertion in the node for k and shift keys down. Note
      that i will go past the insertion point so must add 1 to i to get 
      correct insertion point */

   for (; (i >= 0) && (k < x->keys[i]); x->keys[i+1] = x->keys[i], i--)
               ;
   i++;
   /* perform insertion into node in XTANGO */

   TANGOalgoOp(fcn, "Slide", x->fromleft, i, x->n, level);
   TANGOalgoOp(fcn, "Add", x->fromleft, i, level);

   x->keys[i] = k;    /* add k to x at i */
   x->n++;
  }
 else {
        /* find insertion point for k so can get correct child */
       for( ; (i>=0) && (k < x->keys[i]); i--)
             ;
       i++;
       if (x->c[i]->n == maxkey) {        /* if child full, */
          BTreeSplitChild(x, x->c[i], i); /* split child and check */
          if (k > x->keys[i]) i++;        /*k against new key */
         }
      level++;              /* going down a level in the tree to insert */ 
       BTreeInsertNonFull(x->c[i], k);    /* try inserting again */
      }
}  

void BTreeInsert(k)

int k;
/* BTreeInsert takes a integer and inserts it into the BTree. It attempts 
   to insert k as far down in the tree as possible until all remaining nodes
   are full. */
{
  btree * r = root, *s;
  int j;

  level = 0;                   /* initialize the level */
  TANGOalgoOp(fcn, "New", k);  /* Display new number in XTANGO */

  if (r->n == maxkey) {      /* if root full, make new root */

/* shift children down to account fro new root since all levels are moved down
    1 */
    for (j = maxlevels; j > 0; j--) children[j] = children[j-1];
    lastlevel++;
    s = (btree *) malloc(sizeof(btree));
    root = s;
    s->leaf = FALSE;
    s->n = s->fromleft = 0;
    s->c[0] = r;
    for (j = 1; j < maxc; j++) s->c[j] = NIL;
    BTreeSplitChild(s,r,0);      /* split root and then insert */
    BTreeInsertNonFull(s, k);
  }
 else BTreeInsertNonFull(r,k);  /* if root not full, just insert */
}


void main() {

  int i;

  printf("B-Tree    (this anim is a little rough yet)\n");
  printf("It'll core dump eventually when it goes outside the allocated tree\n");
  TANGOalgoOp(fcn, "BEGIN");
  MakeBTree();
  do { 
       printf("Enter a number to insert into the BTree (0 to stop): "); 
       scanf("%d", &i);
       printf("\n");
       if (i==0) break;
       BTreeInsert(i);
    } while (1);
  TANGOalgoOp(fcn, "END");
}
