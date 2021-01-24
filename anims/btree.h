/* (written by Paul Rowland)  Project #1 Animating Computer Algorithms

This header contains all the definitions and declarations needed
to run btree.c and btreescenes.c, for animating a btree insertion with
XTANGO */

#include <stdio.h>
#include "xtango.h"

#define TRUE 1
#define FALSE 0
#define NIL 0L
#define t 3               /* btree variables */
#define maxkey 2*t - 1    /* maximum number of keys in a node */
#define maxc 2*t          /* maximum number of children for a node */
#define maxlevels 9       /* maximum # of levels keep track of */

extern int level, lastlevel;      /* level information */
extern int children[maxlevels];   /* children information for each level */

typedef struct bnode {
       int n;  /* # of keys used in node */ 
       int fromleft;           /* number of children to the left */
       int leaf;               /* whether a node is a leaf */
       int keys[maxkey];       /* keys for the node */
       struct bnode *c[maxc];    /* the children for the node */
      } btree;
 
