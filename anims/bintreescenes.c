/*****************************************************************************/
/*                             bintreescenes.c                               */
/*                                                                           */
/*                 Binary Tree insertion & deletion demo                     */
/*           Written for XTANGO at RPI by Aaron Candib 7/15/92               */
/*****************************************************************************/

#include <stdio.h>
#include <math.h>
#include "xtango.h"
#include "bintree.h"
#define treewidth 0.9
#define treeheight 0.9


/*****************************************************************************/
int p(a,b)
int a,b;
{
  int i,x=1;
  for (i=1;i<=b;++i) x*=a;
  return x;
}

/*****************************************************************************/
void AnimInitTree()
{
  TANGO_IMAGE insert,delete;
  char ins[6],del[6];

  ASSOCinit();
  ASSOCmake("Link",1);
  ASSOCmake("TMPLink",1);
  ASSOCmake("Node",1);
  ASSOCmake("TMPNode",1);
  ASSOCmake("CurrentNode",0);
  TWISTcreate_bintree(NULL,(1-treewidth)/2.0,1-(1-treeheight)/2.0,
                       1-(1-treewidth)/2.0,(1-treeheight)/2.0,maxdepth);
  ASSOCmake("Insert",0);
  ASSOCmake("Delete",0);
  strcpy(ins,"Insert");
  strcpy(del,"Delete");
  insert=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.0,0.05,0,TANGO_COLOR_MAROON,
                           NULL,ins,0);
  delete=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.0,0.05,0,TANGO_COLOR_MAROON,
                           NULL,del,0);
  ASSOCstore("Insert",insert);
  ASSOCstore("Delete",delete);
}

/*****************************************************************************/
TANGO_TRANS DeleteImage(image)
TANGO_IMAGE image;
{
  return TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,image,TANGOpath_null(1));
}

/*****************************************************************************/
AnimFlashNode(i)
int i;
{
  TANGO_IMAGE node;
  if (ASSOCmember("Node",i))
  {
    node=(TANGO_IMAGE)ASSOCretrieve("Node",i);
    TANGOtrans_perform(TAPflash(node,5));
  }
  else printf("No assosciation for node %d\n",i);
}

/*****************************************************************************/
AnimFlashLink(i)
int i;
{
  TANGO_IMAGE link;
  if (ASSOCmember("Link",i))
  {
    link=(TANGO_IMAGE)ASSOCretrieve("Link",i);
    TANGOtrans_perform(TAPflash(link,5));
  }
  else printf("No assosciation for link %d\n",i);
}

/*****************************************************************************/
TANGO_TRANS Delay(n)
int n;
{
  return TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(n));
}

/*****************************************************************************/
char InTree(node)
int node;
{
  return ASSOCmember("Node",node);
}

/*****************************************************************************/
TANGO_PATH TreePath(from,to) /* Returns a path between two tree locations */
int from,to;
{
  return TANGOpath_motion((TANGO_LOC)ASSOCretrieve("NUMBER",NULL,from),
                          (TANGO_LOC)ASSOCretrieve("NUMBER",NULL,to),
                          TANGO_PATH_TYPE_STRAIGHT);
}

/*****************************************************************************/
double dx(node) /* arrow shift of link pointing to node */
int node;
{
  double x1,x2;
  x1=TANGOloc_X((TANGO_LOC)ASSOCretrieve("NUMBER",NULL,node));
  x2=TANGOloc_X((TANGO_LOC)ASSOCretrieve("NUMBER",NULL,(node+1)/2-1));
  return x2-x1;
}

double width(image)
TANGO_IMAGE image;
{
  return TANGOloc_X(TANGOimage_loc(image,TANGO_PART_TYPE_E))-
         TANGOloc_X(TANGOimage_loc(image,TANGO_PART_TYPE_W));
}

/*****************************************************************************/
AnimNewData(ch,dat) /* Creates new node and moves it to the root pos. */
char ch; int dat;
{
  TANGO_IMAGE node,msg;
  TANGO_LOC msgloc;
  char str[6];
  double x=0,y=0,w;

  if (ch=='i') msg=(TANGO_IMAGE)ASSOCretrieve("Insert");
  else msg=(TANGO_IMAGE)ASSOCretrieve("Delete");
  msgloc=TANGOimage_loc(msg,TANGO_PART_TYPE_E);
  TANGOloc_inquire(msgloc,&x,&y);
  sprintf(str,"%d",dat);
  node=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,0,
                         TANGO_COLOR_MAROON,NULL,str,1);
  w=width(node);
  x+=(w/2.0+0.01);
  node=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,
                         TANGO_COLOR_MAROON,NULL,str,1);
  TANGOtrans_perform(TAPvis_toggle(msg));
  TANGOtrans_perform(Delay(15));
  TANGOtrans_perform(TAPvis_toggle(msg));
  TANGOtrans_perform(TAPcolor(node,TANGO_COLOR_BLUE));
  TANGOtrans_perform(TAPmove(node,TANGO_PART_TYPE_C,
                             (TANGO_LOC)ASSOCretrieve("NUMBER",NULL,0)));
  ASSOCstore("CurrentNode",node);
}

/*****************************************************************************/
int loc(p) /* returns integer representation of node's position in tree */
bptr p;
{
  if (p==NULL) {printf("ERROR: Null pointer passed to loc.\n"); return -1; }
  else if (p->parent==NULL) return 0;
  else if (p==p->parent->left) return 2*(loc(p->parent)+1)-1;
  else return 2*(loc(p->parent)+1);
}

/*****************************************************************************/
AnimComparison(dat,x,flag) /* Animates comparison and moving down the tree */
int dat; bptr x; char flag;
{
  int n=loc(x);
  TANGO_IMAGE link,newnode,curnode,lessthan;
  TANGO_LOC loc,dest;
  TANGO_TRANS move,wait,compare;
  TANGO_PATH path=NULL;
  double dx=1.0,lx=0.0,ly=0.0;

  newnode=(TANGO_IMAGE)ASSOCretrieve("CurrentNode");
  loc=TANGOimage_loc(newnode,TANGO_PART_TYPE_C);
  if (dat!=x->val)
  {
    if (n>=p(2,maxdepth)-1)
    {
      printf("Not enough levels in tree to add node %d\n",dat);
      TANGOtrans_perform(DeleteImage(newnode));
    }
    else
    {
      if (dat<x->val)
        { dx*= -1; dest=(TANGO_LOC)ASSOCretrieve("NUMBER",NULL,(n+1)*2-1); }
      else dest=(TANGO_LOC)ASSOCretrieve("NUMBER",NULL,(n+1)*2);

      if (flag=='y')
      {
        curnode=(TANGO_IMAGE)ASSOCretrieve("Node",n);
        lx=TANGOloc_X(loc); ly=TANGOloc_Y(loc);
        lessthan=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,lx,ly,0,
                                   TANGO_COLOR_RED,NULL,"<",1);
        dx*=(width(curnode)/2.0+2*width(lessthan)+width(newnode)/2.0);
        if (dx<0) lx-=(width(lessthan)+width(curnode)/2.0);
        else lx+=(width(lessthan)+width(curnode)/2.0);
        lessthan=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,lx,ly,0,
                                   TANGO_COLOR_RED,NULL,"<",1);

        path=TANGOpath_example(loc,TANGOloc_create(TANGOloc_X(loc)+dx,
                               TANGOloc_Y(loc)),TANGOpath_null(5));
        path=TANGOpath_concatenate(3,path,TANGOpath_null(10),
                                     TANGOpath_reverse(path));
        move=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,newnode,path);
        compare=TANGOtrans_concatenate(4,Delay(5),TAPvis_toggle(lessthan),
                                         Delay(9),DeleteImage(lessthan));
/*        compare=TANGOtrans_concatenate(3,Delay(5),TAPflash(lessthan,5),
                                         DeleteImage(lessthan)); */
        TANGOtrans_perform(TANGOtrans_compose(2,move,compare));
        TANGOtrans_free(2,move,compare);
      }
                                          /* Now actually move down the tree */
      TANGOtrans_perform(TAPmove(newnode,TANGO_PART_TYPE_C,dest));
      if (path) TANGOpath_free(1,path);
    }
  }
  else            /* if it's already in the tree, throw it out of the tree */
  {
    move=TAPmove(newnode,TANGO_PART_TYPE_C,
                 TANGOloc_create(TANGOloc_X(loc),(1-treeheight)/2.0));
    move=TANGOtrans_concatenate(3,TAPflash(newnode,5),move,
                                  DeleteImage(newnode));
    TANGOtrans_perform(move);
    TANGOtrans_free(1,move);
  }
}
/*****************************************************************************/
AnimFoundDelNode(p)
bptr p;
{
  int n=loc(p);
  TANGO_IMAGE node=NULL,tmpnode=(TANGO_IMAGE)ASSOCretrieve("CurrentNode");

  ASSOCdelete("CurrentNode");
  TANGOtrans_perform(DeleteImage(tmpnode));
  node=(TANGO_IMAGE)ASSOCretrieve("Node",n);
  TANGOtrans_perform(TAPflash(node,5));
}

/*****************************************************************************/
AnimNoDelNode()
{
  TANGO_IMAGE tmpnode=(TANGO_IMAGE)ASSOCretrieve("CurrentNode");
  TANGO_TRANS move;
  TANGO_LOC loc=TANGOimage_loc(tmpnode,TANGO_PART_TYPE_C);

  move=TAPmove(tmpnode,TANGO_PART_TYPE_C,
                 TANGOloc_create(TANGOloc_X(loc),(1-treeheight)/2.0));
  move=TANGOtrans_concatenate(3,TAPflash(tmpnode,5),move,
                                  DeleteImage(tmpnode));
  TANGOtrans_perform(move);
  TANGOtrans_free(1,move);
}
/*****************************************************************************/
AnimPutInTree(p) /* links node up with the rest of the tree */
bptr p;
{
  int n=loc(p);
  TANGO_IMAGE parent,newlink,newnode=(TANGO_IMAGE)ASSOCretrieve("CurrentNode");
  double x[2],y[2];

  ASSOCstore("Node",n,newnode);
  if (n>0)
  {
    parent=(TANGO_IMAGE)ASSOCretrieve("Node",(n+1)/2-1);
    TANGOloc_inquire(TANGOimage_loc(parent,TANGO_PART_TYPE_S),&x[1],&y[1]);
    TANGOloc_inquire(TANGOimage_loc(newnode,TANGO_PART_TYPE_N),&x[0],&y[0]);
    x[0]-=x[1];y[0]-=y[1];
    newlink=TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE,x[1],y[1],1,
                              TANGO_COLOR_GREEN,2,x,y,0.0,1.0,1);
    TAPshow(newlink);
    ASSOCstore("Link",n,newlink);
  }
}

/*****************************************************************************/
AssocFix() /* copies tmp assosciations */
{
  TANGO_IMAGE node,link;
  int n=0;

  while (ASSOCmember("NUMBER",NULL,n))
  {
    if (ASSOCmember("TMPNode",n))
    {
      if (ASSOCmember("Node",n)) ASSOCdelete("Node",n);
      node=(TANGO_IMAGE)ASSOCretrieve("TMPNode",n);
      ASSOCstore("Node",n,node);
      ASSOCdelete("TMPNode",n);
    }
    if (ASSOCmember("TMPLink",n))
    {
      if (ASSOCmember("Link",n)) ASSOCdelete("Link",n);
      link=(TANGO_IMAGE)ASSOCretrieve("TMPLink",n);
      ASSOCstore("Link",n,link);
      ASSOCdelete("TMPLink",n);
    }
    ++n;
  }
}

/*****************************************************************************/
TANGO_TRANS MoveNode(from,to)
int from,to;
{
  TANGO_IMAGE node=(TANGO_IMAGE)ASSOCretrieve("Node",from);

  ASSOCstore("TMPNode",to,node);
  ASSOCdelete("Node",from);
  return
    TAPmove(node,TANGO_PART_TYPE_C,(TANGO_LOC)ASSOCretrieve("NUMBER",NULL,to));
}

/*****************************************************************************/
TANGO_TRANS MoveLink(from,to)
int from,to;
{
  TANGO_PATH path;
  TANGO_TRANS linkmove,linkswing;
  TANGO_IMAGE link=(TANGO_IMAGE)ASSOCretrieve("Link",from);
  double x=dx(from)-dx(to), y=0;

  ASSOCstore("TMPLink",to,link);
  ASSOCdelete("Link",from);
  path=TANGOpath_create(1,&x,&y);
  path=TANGOpath_interpolate(path,20.0);
  linkswing=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
  TANGOpath_free(1,path);
  path=TreePath((from+1)/2-1,(to+1)/2-1);
  linkmove=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,link,path);
  return TANGOtrans_compose(2,linkmove,linkswing);
}

/*****************************************************************************/
TANGO_TRANS BuildMoveSubtree(root,from,to,error)
int root,from,to;
char *error;
{
  TANGO_TRANS ltreemove,rtreemove,linkmove,linkswing,nodemove,bigmove;
  TANGO_PATH path;
  TANGO_IMAGE node,link;
  TANGO_LOC fromloc,toloc;
  int maxnodes=p(2,maxdepth+1)-1;
  char lerror,rerror;

  if ((from>=maxnodes)||(!InTree(from)))
  {
    *error=FALSE;
    return NULL;
  }
  else
    if (to>=maxnodes) /* 'from' in tree, 'to' not: bad move */
    {
      *error=TRUE;
 printf("Bad move in BuildMoveSubtree, root %d: from %d to %d\n",root,from,to);
      return NULL;
    }
    else /* no error at ths level: both from & to in tree */
    {
      ltreemove=BuildMoveSubtree(root,(from+1)*2-1,(to+1)*2-1,&lerror);
      rtreemove=BuildMoveSubtree(root,(from+1)*2,(to+1)*2,&rerror);
      if (!(*error=(lerror||rerror))) /* if no lower level error */
      {
        node=(TANGO_IMAGE)ASSOCretrieve("Node",from);
        ASSOCstore("TMPNode",to,node);
        nodemove=MoveNode(from,to);
        if (from>root)
	{
          link=(TANGO_IMAGE)ASSOCretrieve("Link",from);
          ASSOCstore("TMPLink",to,link);
          linkmove=MoveLink(from,to);
          if ((ltreemove)&&(rtreemove))
           bigmove=TANGOtrans_compose(4,ltreemove,rtreemove,nodemove,linkmove);
          else if (ltreemove)
           bigmove=TANGOtrans_compose(3,ltreemove,nodemove,linkmove);
          else if (rtreemove)
           bigmove=TANGOtrans_compose(3,rtreemove,nodemove,linkmove);
          else bigmove=TANGOtrans_compose(2,nodemove,linkmove);
	}
        else /* root of move */
        {
          if ((ltreemove)&&(rtreemove))
           bigmove=TANGOtrans_compose(3,ltreemove,rtreemove,nodemove);
          else if (ltreemove)
           bigmove=TANGOtrans_compose(2,ltreemove,nodemove);
          else if (rtreemove)
           bigmove=TANGOtrans_compose(2,rtreemove,nodemove);
          else bigmove=nodemove;
	}
      }
      else bigmove=NULL;
      return bigmove;
    }
}

/*****************************************************************************/
AnimBinaryDelete(z,x,y) /* make sure nonempty tree */
bptr z,x,y;
{
  char error;
  int xn,zn=loc(z), yn=loc(y), fixn;
  TANGO_TRANS movetree=NULL,grabtree=NULL,pulltree=NULL,
              chucknode=NULL,nodemove=NULL,dellink=NULL,anim=NULL,tmp;
  TANGO_IMAGE delnode,replacenode=NULL,link=NULL;
  TANGO_LOC pt;
  TANGO_PATH path;

  
  if (x!=NULL) xn=loc(x); else xn= -1;
  delnode=(TANGO_IMAGE)ASSOCretrieve("Node",zn);
  if (y!=z) replacenode=(TANGO_IMAGE)ASSOCretrieve("Node",yn);
  pt=TANGOimage_loc(delnode,TANGO_PART_TYPE_C);
  chucknode=TAPmove(delnode,TANGO_PART_TYPE_C,
               TANGOloc_create(TANGOloc_X(pt),(1-treeheight)/2.0));
  chucknode=TANGOtrans_concatenate(2,chucknode,DeleteImage(delnode));
  ASSOCdelete("Node",zn);
  anim=chucknode;

  if (replacenode)
  {
    nodemove=MoveNode(yn,zn);
    anim=TANGOtrans_concatenate(2,anim,nodemove);
  }
  if (x!=NULL)
  {
    link=(TANGO_IMAGE)ASSOCretrieve("Link",xn);
    dellink=DeleteImage(link);
    ASSOCdelete("Link",xn);
  }
  else
    if (y->parent)
    {
      link=(TANGO_IMAGE)ASSOCretrieve("Link",yn);
      dellink=DeleteImage(link);
      ASSOCdelete("Link",yn);
    }
  if (dellink) anim=TANGOtrans_concatenate(2,dellink,anim);

  if ((y->parent)&&(x!=NULL))
  {
    link=(TANGO_IMAGE)ASSOCretrieve("Link",yn);
    path=TreePath(yn,xn);
    grabtree=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
    pulltree=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,
                                         TANGOpath_reverse(path));
    TANGOpath_free(1,path);
  }
  if (x!=NULL)
  {
    movetree=BuildMoveSubtree(xn,xn,yn,&error);
  }
  if (grabtree)
  {
    anim=TANGOtrans_concatenate(2,anim,grabtree);
    tmp=TANGOtrans_compose(2,movetree,pulltree);
    anim=TANGOtrans_concatenate(2,anim,tmp);
    TANGOtrans_free(1,tmp);
  }
  else
    if (movetree) anim=TANGOtrans_concatenate(2,anim,movetree);

  TANGOtrans_perform(anim);
  TANGOtrans_free(1,anim);
  if (yn>zn) fixn=zn; else fixn=yn;
  if ((movetree)||(replacenode)) AssocFix();
  if (replacenode) ASSOCstore("Node",zn,replacenode);
}
