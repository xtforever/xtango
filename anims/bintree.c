/*****************************************************************************/
/*                                bintree.c                                  */
/*                                                                           */
/*                  Binary Tree insertion & deletion demo                    */
/*           Written for XTANGO at RPI by Aaron Candib 7/14/92               */
/*****************************************************************************/

#include <stdio.h>
#include "xtango.h"
#include "bintree.h"

void 
AnimInitTree(), AnimNewData(), AnimComparison(), AnimPutInTree(), 
AnimFoundDelNode(), AnimNoDelNode(), AnimBinaryDelete(),
AnimFlashLink(), AnimFlashNode();

static NAME_FUNCT fcn[] = { {"InitTree",1,{VOID, (FPTR)AnimInitTree}},
			    {"NewData",1,{VOID, (FPTR)AnimNewData}},
			    {"Comparison",1,{VOID, (FPTR)AnimComparison}},
			    {"PutInTree",1,{VOID, (FPTR)AnimPutInTree}},
			    {"BinaryDelete",1,{VOID, (FPTR)AnimBinaryDelete}},
			    {"FoundDelNode",1,{VOID, (FPTR)AnimFoundDelNode}},
			    {"NoDelNode",1,{VOID, (FPTR)AnimNoDelNode}},
			    {"FlashLink",1,{VOID, (FPTR)AnimFlashLink}},
			    {"FlashNode",1,{VOID, (FPTR)AnimFlashNode}},
			    {NULL,0,NULL,NULL}
			  };

bptr root=NULL;                 /* Global root for binary tree */
char comparisons;                /* Used to determine type of animation */

/*****************************************************************************/
bptr addtotree(dat) /* insert variable or const into binary tree */
int dat;
{
  bptr x,y,bk,location;
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
    x=(bptr)malloc(sizeof(bnode));
    location=x;
    if (bk==NULL) root=x;
    else if (dat>bk->val) bk->right=x;
	 else bk->left=x;
    x->val=dat;
    x->left=x->right=NULL;
    x->parent=bk;
    TANGOalgoOp(fcn,"PutInTree",x);
  }
  else location=bk; /* perfect match; no insertion needed, just mark pointer */
  return location;
}

/*****************************************************************************/
char deletefromtree(dat) /* returns TRUE if successful, FALSE if not */
int dat;
{
  bptr z=root,x,y,w;
  int lvl=0;

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
    else x=NULL;

    TANGOalgoOp(fcn,"BinaryDelete",z,x,y); /* no 'n'    Animate the delete */

    if (x) x->parent=y->parent;
    if (y->parent==NULL) { if (x!=NULL) root=x; else root=NULL; }
    else if (y==y->parent->left) y->parent->left=x;
         else y->parent->right=x;
    if (y!=z) { z->val=y->val; }

    free(y);
  }
}

/*****************************************************************************/
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

  printf("Binary Tree Insertion & Deletion Animation\n");
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
