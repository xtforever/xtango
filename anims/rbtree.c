/* Red-Black Trees by Rammohan Kordale */

#include <stdio.h>
#include "xtango.h"
#define RED 1
#define BLACK 0
void ANIMInit(), ANIMNewVal(), ANIMAddNode(), ANIMCompAndMove(), ANIMDrawscenes(),ANIMClearscenes(), ANIMCleardiag();


static NAME_FUNCT fcn[] = { {"Init",      1, {VOID, (FPTR)ANIMInit}},
                {"NewVal", 1, {VOID, (FPTR)ANIMNewVal}},
                {"AddNode",   1, {VOID, (FPTR)ANIMAddNode}},
                {"CompAndMove",   1, {VOID, (FPTR)ANIMCompAndMove}},
                {"Drawscenes",    1, {VOID, (FPTR)ANIMDrawscenes}},
                {"Clearscenes",    1, {VOID, (FPTR)ANIMClearscenes}},
                {"Cleardiag",    1, {VOID, (FPTR)ANIMCleardiag}},
                {NULL,      0,  NULL, NULL} };


typedef struct _TREE *    TREE;

typedef struct _TREE {
   int value;
   TREE par;
   TREE lson;
   TREE rson;
   int color;
  /* int size;*//*number of internal nodes in the subtree
                rooted here including this one*/
} TREE_B;

static    TREE    root;
static    TREE    new_node;

static    TREE    insert_tree();
static    void    print_tree();
static void rb_insert();
static void l_rotate();
static void Drawtree();
static void Cleartree();
static void r_rotate();
static void set_rtsize();
static int inorder_fnc();
int grank;



main()
{
   int i,j;

   TANGOalgoOp(fcn, "BEGIN");
   root = NULL;
   printf("Red-black trees:\n");
   printf("1. Every node is either red or black.\n");
   printf("2. Every leaf (NIL) is black.\n");
   printf("3. If a node is red, then both its children are black.\n");
   printf("4. Every simple path from a node to a descendant leaf\n");
   printf("   contains the same number of black nodes.\n\n");
   printf("Note the following things while watching insertion into a red black tree on a black/white  monitor\n");
   printf("Change defines in rbtreescenes.c - COLOR to 0\n");
   printf("A red node is denoted by a circle and a black one by only the value\n");    
   printf("To continue press return\n");
   getchar();
   TANGOalgoOp(fcn, "Init",5);
   for (;;) 
   {
      printf("Input a value (-1 to quit)\n");
      scanf("%d",&j);
      if (j==-1) break;
      TANGOalgoOp(fcn, "NewVal",j);
      rb_insert(j);
    };

#ifdef DEBUG
   print_tree(root);
#endif
   TANGOalgoOp(fcn, "END");
};




static TREE
insert_tree(t,v,parent)
   TREE t,parent;
   int v;
{
   if (t == NULL) {
      t = (TREE) malloc(sizeof(TREE_B));
		new_node = t;
/*Following 2 stmts not needed*/
          if (root == NULL)
            root = t;
      t->par = parent;
      t->lson = NULL;
      t->rson = NULL;
      t->value = v;
/*      t->size = 1;
      set_rtsize(t);
*/
      TANGOalgoOp(fcn, "AddNode");
    }
	else if (v <= t->value) 
	{
    	TANGOalgoOp(fcn, "CompAndMove",1);
      	t->lson = insert_tree(t->lson,v,t);
	}
	else 
	{
    	TANGOalgoOp(fcn, "CompAndMove",0);
     	t->rson = insert_tree(t->rson,v,t);
    };
   return t;
};


static void
rb_insert(v)
     int v;
{
     TREE y,x;
	 root = insert_tree(root,v,NULL);
                 /* Once in this routine the structure
                     or atleast the coloring was going
                     to change*/
	 x = new_node;
     x->color = RED;/*TAKE CARE to see that this appropriately modified when insertion is done*/
     while((x->par != NULL) && ((x->par)->color == RED))
     {    if (x->par == ((x->par)->par)->lson)
            /*if par of x is the left son of grandpa of x*/
        {    y = ((x->par)->par)->rson;
            if ((y != NULL) && (y->color == RED))
            {    
				printf("Before processing - Press RETURN\n");
				getchar();
                Cleartree((x->par)->par);
                (x->par)->color = BLACK;
                y->color = BLACK;
                ((x->par)->par)->color = RED;
                x = (x->par)->par;
                Drawtree(x);
				printf("Observe change - PRESS RETURN\n");
				getchar();
            }
            else
            {    if (x == (x->par)->rson)
                {    Cleartree(x->par);
                    x = x->par;
#ifdef DEBUG
                print_tree(root);
#endif
                    l_rotate(x);
                    Drawtree(x->par);
				printf("Observe change - PRESS RETURN\n");
				getchar();
#ifdef DEBUG
                print_tree(root);
#endif
                }
				printf("Before processing - Press RETURN\n");
				getchar();
                Cleartree((x->par)->par);




                (x->par)->color = BLACK;
                ((x->par)->par)->color = RED;
#ifdef DEBUG
                print_tree(root);
#endif
                r_rotate((x->par)->par);
#ifdef DEBUG
                print_tree(root);
#endif
                Drawtree(x->par);
				printf("Observe change - PRESS RETURN\n");
				getchar();
            }
        }
        else/*Symmetric case of the if part*/
        {    y = ((x->par)->par)->lson;
            if ((y != NULL) && (y->color == RED))
            {    
				printf("Before processing - Press RETURN\n");
				getchar();
                Cleartree((x->par)->par);
                (x->par)->color = BLACK;
                y->color = BLACK;
                ((x->par)->par)->color = RED;
                x = (x->par)->par;
                Drawtree(x);
				printf("Observe change - PRESS RETURN\n");
				getchar();
            }
            else
            {    if (x == (x->par)->lson)
                {    Cleartree(x->par);
                    x = x->par;
                    r_rotate(x);
#ifdef DEBUG
                	print_tree(root);
#endif
                    Drawtree(x->par);
				printf("Observe change - PRESS RETURN\n");
				getchar();
                }
				printf("Before processing - Press RETURN\n");
				getchar();
                Cleartree((x->par)->par);
                (x->par)->color = BLACK;
                ((x->par)->par)->color = RED;
                l_rotate((x->par)->par);
                Drawtree(x->par);
				printf("Observe change - PRESS RETURN\n");
				getchar();
            }
        }
     }
     root->color = BLACK;
	TANGOalgoOp(fcn, "Clearscenes",0);
	TANGOalgoOp(fcn, "Drawscenes",root->value,0,BLACK);
	printf("The BALANCED tree - Press RETURN\n");
	getchar();
}

static void
l_rotate(x)
    TREE x;
{
    TREE y;
    int rsize,lsize;
        y = x->rson;
        x->rson = y->lson;
        if ((y->lson) != NULL)
            (y->lson)->par = x;
        y->par = x->par;
        if (x->par == NULL)/*x was the root*/
            root = y;
        else
        {    if ((x->par)->lson == x)
                (x->par)->lson = y;
            else
                (x->par)->rson = y;
        }
        y->lson = x;
        x->par = y;
/*
        y->size = x->size;
        rsize = 0;
        lsize = 0;
        if ((x->rson) != NULL)
            rsize = (x->rson)->size;
        if ((x->lson) != NULL)
            lsize = (x->lson)->size;
        x->size = lsize + rsize + 1;
*/
}

static void
r_rotate(x)
    TREE x;
{
    TREE y;
    int rsize,lsize;
        y = x->lson;
        x->lson = y->rson;
        if ((y->rson) != NULL)
            (y->rson)->par = x;
        y->par = x->par;
        if (x->par == NULL)/*x was the root*/
            root = y;
        else
        {    if ((x->par)->rson == x)
                (x->par)->rson = y;
            else
                (x->par)->lson = y;
        }
        y->rson = x;
        x->par = y;
/*
        y->size = x->size;
        rsize = 0;
        lsize = 0;
        if ((x->rson) != NULL)
            rsize = (x->rson)->size;
        if ((x->lson) != NULL)
            lsize = (x->lson)->size;
        x->size = lsize + rsize + 1;
*/
}

static void
print_tree(t)
   TREE t;
{
   if (t == NULL) return;

   printf("TREE %d\n",t->value);
	printf("going left\n");
   print_tree(t->lson);
	printf("going right\n");
   print_tree(t->rson);
};

/*
static void
set_rtsize(x)
    TREE x;
{    
    TREE y;
    printf("in set size1\n");
    y = x;
    while (y->par != NULL)
    {
        (y->par)->size += 1;
        y = y->par;
    }
    printf("in set size2\n");
}
*/

static int
inorder_fnc(x,val)
   TREE x;
    int val;
{
   TREE y;
    y = x;
    if (y == NULL)
    {    printf("Impossible condition\n");
        return;
    }
    if ((y->value) == val)
        return(grank);
	else
	{	if (val < y->value)
		{	grank = 2*grank+1;
        	inorder_fnc(y->lson,val);
		}
		else
    	{   grank = 2*grank +2;
       		inorder_fnc(y->rson,val);
		}
    }
}

/* end of tree.c */

static void
Drawtree(ptr)
    TREE ptr;
{
    grank = 0;/*to facilitate correct evaluation of inorder number in the next stmt*/
    inorder_fnc(root,ptr->value);
    TANGOalgoOp(fcn, "Drawscenes",ptr->value,grank,ptr->color);
    if ((ptr->lson) != NULL)
        Drawtree(ptr->lson);
    if ((ptr->rson) != NULL)
        Drawtree(ptr->rson);
    return;
}    

static void
Cleartree(ptr)
    TREE ptr;
{
    grank = 0;
    inorder_fnc(root,ptr->value);
    TANGOalgoOp(fcn, "Clearscenes",grank);
    if ((ptr->lson) != NULL)
	{
        Cleartree(ptr->lson);
	}
    if ((ptr->rson) != NULL)
        Cleartree(ptr->rson);
    return;
}
