/* AVL Trees by Nick Duncan */

#include <stdio.h>
#include "xtango.h"


void ANIMInit(), ANIMNewVal(), ANIMAddNode(), ANIMCompAndMove(), 
     ANIMMovePivot(), ANIMErasePivot(), ANIMRightRight(), ANIMRightLeft(),
     ANIMLeftRight(), ANIMLeftLeft();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"NewVal", 1, {VOID, (FPTR)ANIMNewVal}},
			    {"AddNode",   1, {VOID, (FPTR)ANIMAddNode}},
			    {"MovePivot",   1, {VOID, (FPTR)ANIMMovePivot}},
			    {"ErasePivot",   1, {VOID, (FPTR)ANIMErasePivot}},
			    {"CheckNext",   1, {VOID, (FPTR)ANIMCompAndMove}},
			    {"RRrotate",   1, {VOID, (FPTR)ANIMRightRight}},
			    {"RLrotate",   1, {VOID, (FPTR)ANIMRightLeft}},
			    {"LRrotate",   1, {VOID, (FPTR)ANIMLeftRight}},
			    {"LLrotate",   1, {VOID, (FPTR)ANIMLeftLeft}},
			    {NULL,	  0,  NULL, NULL} };


typedef struct _TREE *	TREE;

typedef struct _TREE {
   int value;
   int balance;
   TREE lson;
   TREE rson;
} TREE_B;

static	TREE	root, pivot, pivot_parent;


/* rotate the left child of the pivot to be in the pivot position,
   move the pivot to the position of its right child */
static TREE
left_left(piv)
   TREE piv;
{
   TREE  p,q;

   TANGOalgoOp(fcn, "LLrotate");
   p = piv->lson;
   q = p->rson;
   p->rson = piv;
   piv->lson = q;
   piv = p;

   piv->balance = 0;
   piv->rson->balance = 0;

   return piv;
};

/* put the right child of the left child of the pivot at the pivot position
   move the pivot to the position of its right child */
static TREE
right_left(piv)
   TREE piv;
{
   TREE  p,q;

   TANGOalgoOp(fcn, "RLrotate");
   p = piv->lson;
   q = p->rson;
   piv->lson = q->rson;
   p->rson = q->lson;
   q->lson = p;
   q->rson = piv;
   piv = q;

   /* there are 3 subcases for resetting the balance factors */
   if (piv->balance == 0) {
      piv->lson->balance = 0;
      piv->rson->balance = 0;
    }
   else if (piv->balance == 1) {
      piv->balance = 0;
      piv->lson->balance = 0;
      piv->rson->balance = -1;
    }
   else if (piv->balance == -1) {
      piv->balance = 0;
      piv->lson->balance = 1;
      piv->rson->balance = 0;
    }

   return piv;
};

/* a mirror image of the left-left rotation */
static TREE
right_right(piv)
   TREE piv;
{
   TREE  p,q;

   TANGOalgoOp(fcn, "RRrotate");
   p = piv->rson;
   q = p->lson;
   p->lson = piv;
   piv->rson = q;
   piv = p;

   piv->balance = 0;
   piv->lson->balance = 0;

   return piv;
};

/* a mirror image of the right-left rotation */
static TREE
left_right(piv)
   TREE piv;
{
   TREE  p,q;

   TANGOalgoOp(fcn, "LRrotate");
   p = piv->rson;
   q = p->lson;
   piv->rson = q->lson;
   p->lson = q->rson;
   q->rson = p;
   q->lson = piv;
   piv = q;

   if (piv->balance == 0) {
      piv->lson->balance = 0;
      piv->rson->balance = 0;
    }
   else if (piv->balance == 1) {
      piv->balance = 0;
      piv->lson->balance = 0;
      piv->rson->balance = -1;
    }
   else if (piv->balance == -1) {
      piv->balance = 0;
      piv->lson->balance = 1;
      piv->rson->balance = 0;
    }

   return piv;
};

/* call the appropriate rotation function depending on where the unbalancing
   node was added, note that there are 3 different cases which are determined
   by the parent of the pivot */
static TREE
rotate(parent,piv,v)
   TREE parent;
   TREE piv;
   int v;
{
   if (piv == root) {
      if (v <= piv->value) {
         if (v <= piv->lson->value)
            root = left_left(root);
         else
            root = right_left(root);
       }
      else if (v > piv->value) {
         if (v <= piv->rson->value)
            root = left_right(root);
         else
            root = right_right(root);
       }
    }
   else if (piv == parent->lson) {
      if (v <= piv->value) {
         if (v <= piv->lson->value)
            parent->lson = left_left(pivot);
         else
            parent->lson = right_left(pivot);
       }
      else if (v > piv->value) {
         if (v <= piv->rson->value)
            parent->lson = left_right(pivot);
         else
            parent->lson = right_right(pivot);
       }
    }
   else if (piv == parent->rson) {
      if (v <= piv->value) {
         if (v <= piv->lson->value)
            parent->rson = left_left(pivot);
         else
            parent->rson = right_left(pivot);
       }
      else if (v > piv->value) {
         if (v <= piv->rson->value)
            parent->rson = left_right(pivot);
         else
            parent->rson = right_right(pivot);
       }
    }
   return parent;
};

/* same as the insert_tree in tree.c, except that the balance factors
   must be set and the insertions happen one level removed to allow
   for pivot selection */
static TREE
insert_tree(t,v)
   TREE t;
   int v;
{
   if (t->lson == NULL && v <= t->value) {
      t->lson = (TREE) malloc(sizeof(TREE_B));
      t->lson->lson = NULL;
      t->lson->rson = NULL;
      t->lson->value = v;
      t->lson->balance = 0;
      TANGOalgoOp(fcn, "CheckNext",1);
      TANGOalgoOp(fcn, "AddNode");
    }
   else if (t->rson == NULL && v > t->value) {
      t->rson = (TREE) malloc(sizeof(TREE_B));
      t->rson->lson = NULL;
      t->rson->rson = NULL;
      t->rson->value = v;
      t->rson->balance = 0;
      TANGOalgoOp(fcn, "CheckNext",0);
      TANGOalgoOp(fcn, "AddNode");
    }
   else if (v <= t->value) {
      TANGOalgoOp(fcn, "CheckNext",1);
      if (t->lson->balance != 0) {
         pivot = t->lson;
         pivot_parent = t;
         TANGOalgoOp(fcn, "MovePivot");
       }
      t->lson = insert_tree(t->lson,v);
    }
   else {
      TANGOalgoOp(fcn, "CheckNext",0);
      if (t->rson->balance != 0) {
         pivot = t->rson;
         pivot_parent = t;
         TANGOalgoOp(fcn, "MovePivot");
       }
      t->rson = insert_tree(t->rson,v);
    };

   return t;
};

/* reset balances to account for insertion, only need to worry about
   balances from pivot down */
static TREE
reset_balances(pivot,v)
   TREE pivot;
   int v;
{
   TREE temp;

   for(temp = pivot; temp->lson != NULL || temp->rson != NULL; ) {
      if (v <= temp->value) {
         temp->balance++;
         temp = temp->lson;
       }
      else {
         temp->balance--;
         temp = temp->rson;
       }
    }
};


/* tree printing function, used for debugging before animation was running */
void
walk_tree(t, level)
   TREE t;
   int  level;
{
   int i;

   if (t == NULL) return;

   level++;
   walk_tree(t->lson, level);
   level--;
   for(i=level;i>0;i--) printf("   ");
   if (t == pivot) printf("*");
   if (t == pivot_parent) printf("&");
   printf("%d(%d)\n",t->value,t->balance);
   level++;
   walk_tree(t->rson, level);
   level--;
};

main()
{
   int i,value;

   printf("AVL Trees\n");
   printf("   (this animation needs to have the nodes & edges drawn in)\n");
   TANGOalgoOp(fcn, "BEGIN");
   root = NULL;
   TANGOalgoOp(fcn, "Init",5);

   root = (TREE) malloc(sizeof(TREE_B));
   root->lson = NULL;
   root->rson = NULL;
   printf("Enter root node key value (1-1000)\n");
   scanf("%d",&value);
   root->value = value;
   TANGOalgoOp(fcn, "NewVal",root->value);
   root->balance = 0;
   TANGOalgoOp(fcn, "AddNode");

   for (i = 0; i < 30; ++i) {
      printf("Enter node key value (1-1000) (0 to quit)\n");
      scanf("%d",&value);
      if (value == 0) break;
      TANGOalgoOp(fcn, "NewVal",value);

      pivot = root;
      pivot_parent = NULL;
      root = insert_tree(root,value);
      pivot = reset_balances(pivot,value);
      if (pivot->balance == 2 || pivot->balance == -2)
         pivot_parent = rotate(pivot_parent, pivot, value);
      TANGOalgoOp(fcn, "ErasePivot");
    };

   walk_tree(root, 0);
   TANGOalgoOp(fcn, "END");

};



/* end of avl.c */

