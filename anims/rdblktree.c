/*****************************************************************************/
/*                               rdblktree.c                                 */
/*                                                                           */
/*                Red-Black Tree insertion & deletion demo                   */
/*           Written for XTANGO at RPI by Aaron Candib 6/23/92               */
/*****************************************************************************/

#include <stdio.h>
#include "xtango.h"
#include "rdblktree.h"
#define TRUE 1
#define FALSE 0

void 
AnimInitTree(), AnimColorNode(), AnimNewData(), AnimComparison(), 
AnimPutInTree(), AnimLeftRotate(), AnimRightRotate(), AnimFoundDelNode(),
AnimNoDelNode(), AnimBinaryDelete(), AnimFlashLink(), AnimFlashNode();

static NAME_FUNCT fcn[] = { {"InitTree",1,{VOID, (FPTR)AnimInitTree}},
			    {"ColorNode",1,{VOID, (FPTR)AnimColorNode}},
			    {"NewData",1,{VOID, (FPTR)AnimNewData}},
			    {"Comparison",1,{VOID, (FPTR)AnimComparison}},
			    {"PutInTree",1,{VOID, (FPTR)AnimPutInTree}},
			    {"LeftRotate",1,{VOID, (FPTR)AnimLeftRotate}},
			    {"RightRotate",1,{VOID, (FPTR)AnimRightRotate}},
			    {"BinaryDelete",1,{VOID, (FPTR)AnimBinaryDelete}},
			    {"FoundDelNode",1,{VOID, (FPTR)AnimFoundDelNode}},
			    {"NoDelNode",1,{VOID, (FPTR)AnimNoDelNode}},
			    {"FlashLink",1,{VOID, (FPTR)AnimFlashLink}},
			    {"FlashNode",1,{VOID, (FPTR)AnimFlashNode}},
			    {NULL,0,NULL,NULL}
			  };

typedef char vartype[9];     /* used to store tokens */

rbptr root=NULL;                 /* Global root for RB-tree */
char comparisons;                /* Used to determine type of animation */

/*****************************************************************************/

char black(p)  /* Used if a pointer may be null.  Null nodes are black */
rbptr p;
{
  if (p==NULL) return TRUE;
  else return (p->color=='B');
}

/*****************************************************************************/

leftrotate(x) /* Balancing routine used in 'addtotree' */
rbptr x;
{
  rbptr y;

  TANGOalgoOp(fcn,"LeftRotate",x);
  y=x->right;
  x->right=y->left; /* t11 */
  if (y->left!=NULL) y->left->parent=x;
  y->parent=x->parent; /* t13 */
  if (x->parent==NULL) root=y;
  else if (x==x->parent->left) x->parent->left=y;
       else x->parent->right=y;
  y->left=x;  /* t12 */
  x->parent=y;
}

rightrotate(x) /* Balancing routine used in 'addtotree' */
rbptr x;
{
  rbptr y;

  TANGOalgoOp(fcn,"RightRotate",x);
  y=x->left;
  x->left=y->right;
  if (y->right!=NULL) y->right->parent=x;
  y->parent=x->parent;
  if (x->parent==NULL) root=y;
  else if (x==x->parent->right) x->parent->right=y;
       else x->parent->left=y;
  y->right=x;
  x->parent=y;
}

/*****************************************************************************/

rbptr addtotree(dat) /* insert variable or const into RB-tree */
int dat;
{
  rbptr x,y,bk,location;
  int cmp,ct=0;
  char done=(root==NULL);

		      /* normal tree insert */
  x=root;bk=NULL;cmp=1;
  TANGOalgoOp(fcn,"NewData",'i',dat);
  while (!done)                 /* make 'bk' point to place to add node */
  {
    bk=x;
    cmp=x->val-dat;
    TANGOalgoOp(fcn,"Comparison",dat,x,comparisons);
    if (cmp<0)
      x=x->right;
    else if (cmp>0) x=x->left;
    if (cmp!=0) ++ct;
    done=((x==NULL)||(cmp==0));  /* check for perfect match or out of tree */
  }

  if ((cmp!=0)&&(ct<=maxdepth))
  {
    x=(rbptr)malloc(sizeof(rbnode));
    location=x;
    if (bk==NULL) root=x;
    else if (dat>bk->val) bk->right=x;
	 else bk->left=x;
    x->val=dat;
    x->left=x->right=NULL;
    x->parent=bk;
    TANGOalgoOp(fcn,"PutInTree",x);
			   /* Red-Black adjustment part */
    x->color='R';
    TANGOalgoOp(fcn,"ColorNode",x);
    while ((x!=root)&&(x->parent->color=='R'))
    {
      if (x->parent->parent->left==x->parent) /* x's parent is a left child */
      {
	y=x->parent->parent->right;
	if (!black(y))                    /* case Ia */
	{
	  x->parent->color='B';
          TANGOalgoOp(fcn,"ColorNode",x->parent);
	  y->color='B';
          TANGOalgoOp(fcn,"ColorNode",y);
	  x->parent->parent->color='R';
          TANGOalgoOp(fcn,"ColorNode",x->parent->parent);
	  x=x->parent->parent;
	}
	else /* y is black */
	{
	  if (x->parent->right==x)        /* case IIa */
	  {
	    x=x->parent;
	    leftrotate(x);
	  }
	  x->parent->color='B';           /* case IIIa */
          TANGOalgoOp(fcn,"ColorNode",x->parent);
	  x->parent->parent->color='R';
          TANGOalgoOp(fcn,"ColorNode",x->parent->parent);
	  rightrotate(x->parent->parent);
	}
      }
      else /* x's parent is a right child */
      {
	y=x->parent->parent->left;
	if (!black(y))                    /* case Ib */
	{
	  x->parent->color='B';
          TANGOalgoOp(fcn,"ColorNode",x->parent);
	  y->color='B';
          TANGOalgoOp(fcn,"ColorNode",y);
	  x->parent->parent->color='R';
          TANGOalgoOp(fcn,"ColorNode",x->parent->parent);
	  x=x->parent->parent;
	}
	else /* y is black */
	{
	  if (x->parent->left==x)         /* case IIb */
	  {
	    x=x->parent;
	    rightrotate(x);
	  }
	  x->parent->color='B';           /* case IIIb */
          TANGOalgoOp(fcn,"ColorNode",x->parent);
	  x->parent->parent->color='R';
          TANGOalgoOp(fcn,"ColorNode",x->parent->parent);
	  leftrotate(x->parent->parent);
	}
      }  /* end else (x's parent is a right child */
    }    /* end while */
    root->color='B';         /* root is always made black */
    TANGOalgoOp(fcn,"ColorNode",root);
  }
  else location=bk; /* perfect match; no insertion needed, just mark pointer */
  return location;
}

/*****************************************************************************/
char deletefromtree(dat) /* returns TRUE if successful, FALSE if not */
int dat;
{
  rbptr z=root,x,y,w,n;
  int lvl=0;

  n=(rbptr)malloc(sizeof(rbnode));     /* create 'nil' node */
  n->color='B';
  n->val= -1;
  n->left=n->right=n->parent=NULL;

  TANGOalgoOp(fcn,"NewData",'d',dat);
  while ((z)&&(dat!=z->val)&&(lvl<maxdepth))
  {
    TANGOalgoOp(fcn,"Comparison",dat,z,comparisons);
    if (dat<z->val) z=z->left; else z=z->right;
    ++lvl;
  }

  if ((!z)||(lvl>maxdepth)||(z->val!=dat))
  {
    printf("%d not found in tree, hence cannot be deleted.\n",dat);
TANGOalgoOp(fcn,"NoDelNode");
    return FALSE;
  }
  else                                          /* Normal binary tree delete */
  {
    TANGOalgoOp(fcn,"FoundDelNode",z);
    if ((z->left==NULL)||(z->right==NULL)) y=z;
    else   /* y=successor(z) */
    {
      if (z->right)
      {
        y=z->right;
        while (y->left) y=y->left;
      }
      else
      {
        y=z->parent;w=z;
        while ((y)&&(w==y->right)) { w=y; y=y->parent; }
      }
    }
    if (y->left!=NULL) x=y->left;
    else if (y->right!=NULL) x=y->right;
    else x=n;             /* null node inserted here so that x is never NULL */

    TANGOalgoOp(fcn,"BinaryDelete",z,x,y,n);           /* Animate the delete */

    x->parent=y->parent;
    if (y->parent==NULL) { if (x!=n) root=x; else root=NULL; }
    else if (y==y->parent->left) y->parent->left=x;
         else y->parent->right=x;
    if (y!=z) { z->val=y->val; TANGOalgoOp(fcn,"ColorNode",z); }

    if ((root)&&(y->color=='B')) /* The biggie:  RB-delete fixup on 'x' */
    {
      while ((x!=root)&&(x->color=='B'))
      {
        if (x==x->parent->left)
	{
          w=x->parent->right;
          if (w->color=='R') /* Case I left */
	  {
            w->color='B';
            TANGOalgoOp(fcn,"ColorNode",w);
            x->parent->color='R';
            TANGOalgoOp(fcn,"ColorNode",x->parent);
            leftrotate(x->parent);
            w=x->parent->right;
	  }
          if ((black(w->left))&&(black(w->right)))
	  {
            w->color='R'; /* Case II left */
            TANGOalgoOp(fcn,"ColorNode",w);
            x=x->parent;
	  }
          else
	  {
            if (black(w->right))
	    {
              w->left->color='B'; /* Case III left */
              TANGOalgoOp(fcn,"ColorNode",w->left);
              w->color='R';
              TANGOalgoOp(fcn,"ColorNode",w);
              rightrotate(w);
              w=x->parent->right;
	    }
            w->color=x->parent->color; /* Case IV left */
            TANGOalgoOp(fcn,"ColorNode",w);
            x->parent->color='B';
            TANGOalgoOp(fcn,"ColorNode",x->parent);
            w->right->color='B';
            TANGOalgoOp(fcn,"ColorNode",w->right);
            leftrotate(x->parent);
            x=root;
	  }
	}
        else /* x is a right child... l-r reverse of above */
	{
          w=x->parent->left;
          if (w->color=='R') /* Case I right */
	  {
            w->color='B';
            TANGOalgoOp(fcn,"ColorNode",w);
            x->parent->color='R';
            TANGOalgoOp(fcn,"ColorNode",x->parent);
            rightrotate(x->parent);
            w=x->parent->left;
	  }
          if ((black(w->right))&&(black(w->left)))
	  {
            w->color='R'; /* Case II right */
            TANGOalgoOp(fcn,"ColorNode",w);
            x=x->parent;
	  }
          else
	  {
            if (black(w->left))
	    {
              w->right->color='B'; /* Case III right */
              TANGOalgoOp(fcn,"ColorNode",w->right);
              w->color='R';
              TANGOalgoOp(fcn,"ColorNode",w);
              leftrotate(w);
              w=x->parent->left;
	    }
            w->color=x->parent->color; /* Case IV right */
            TANGOalgoOp(fcn,"ColorNode",w);
            x->parent->color='B';
            TANGOalgoOp(fcn,"ColorNode",x->parent);
            w->left->color='B';
            TANGOalgoOp(fcn,"ColorNode",w->left);
            rightrotate(x->parent);
            x=root;
	  }
	}
      }
      x->color='B';
      TANGOalgoOp(fcn,"ColorNode",x);
    }
    if (n->parent) /* unhook the nil node from the tree */
    {
      if (n->parent->left==n) n->parent->left=NULL;
      else if (n->parent->right==n) n->parent->right=NULL;
    }
    free(n);
    free(y);
  }
}

PrintCommands()
{
  printf("Commands: \n");
  printf("    i <n>     Type 'i' followed by a positive integer to insert\n");
  printf("    d <n>     Type 'd' followed by a positive integer to delete\n");
  printf("    h         Type 'h' to reprint these instructions\n");
  printf("    q         Type 'q' to end\n");
}

/*****************************************************************************/

main()
{
  int i;
  char ch;

  TANGOalgoOp(fcn,"BEGIN");
  TANGOalgoOp(fcn,"InitTree");

  printf("Red-Black Tree Insertion & Deletion Animation\n");
  printf("Written by Aaron Candib using XTANGO\n\n");
  printf("Would you like to see comparisons? (y or n)\n");
  scanf("%c",&comparisons);
  PrintCommands();
  do
  {
    scanf("%c",&ch);
    if ((ch!='q')&&(ch!='a')&&(ch!='h')) scanf("%d",&i);
    if (ch=='i') addtotree(i);
    else if (ch=='d') deletefromtree(i);
    else if (ch=='h') PrintCommands();
    else if (ch=='n') TANGOalgoOp(fcn,"FlashNode",i);
    else if (ch=='l') TANGOalgoOp(fcn,"FlashLink",i);
    else if (ch=='a') assoc_dump();
  } while (ch!='q');
  printf("Push the quit button to exit program.\n");
  TANGOalgoOp(fcn,"END");
}
