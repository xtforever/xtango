#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMNewVal(), ANIMAddNode(), ANIMCompAndMove();


static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"NewVal", 1, {VOID, (FPTR)ANIMNewVal}},
			    {"AddNode",   1, {VOID, (FPTR)ANIMAddNode}},
			    {"CheckNext",   1, {VOID, (FPTR)ANIMCompAndMove}},
			    {NULL,	  0,  NULL, NULL} };


typedef struct _TREE *	TREE;

typedef struct _TREE {
   int value;
   TREE lson;
   TREE rson;
} TREE_B;

static	TREE	root;

static	TREE	insert_tree();
static	void	tree_walk();



main()
{
   int i,j;

  printf("Binary tree\n");
  printf("This'll core dump eventually when it goes outside the allocated tree\n");
   TANGOalgoOp(fcn, "BEGIN");
   root = NULL;

   TANGOalgoOp(fcn, "Init",5);
   for (i = 0; i < 30; ++i) {
      j = random() % 1024;
      TANGOalgoOp(fcn, "NewVal",j);
      root = insert_tree(root,j);
    };

   tree_walk(root);
   TANGOalgoOp(fcn, "END");
};




static TREE
insert_tree(t,v)
   TREE t;
   int v;
{
   if (t == NULL) {
      t = (TREE) malloc(sizeof(TREE_B));
      t->lson = NULL;
      t->rson = NULL;
      t->value = v;
      TANGOalgoOp(fcn, "AddNode");
    }
   else if (v <= t->value) {
      TANGOalgoOp(fcn, "CheckNext",1);
      t->lson = insert_tree(t->lson,v);
    }
   else {
      TANGOalgoOp(fcn, "CheckNext",0);
      t->rson = insert_tree(t->rson,v);
    };

   return t;
};




static void
tree_walk(t)
   TREE t;
{
   if (t == NULL) return;

   tree_walk(t->lson);
   printf("TREE %d\n",t->value);
   tree_walk(t->rson);
};


/* end of tree.c */
