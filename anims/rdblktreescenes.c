/*****************************************************************************/
/*                            rdblktreescenes.c                              */
/*                                                                           */
/*                Red-Black Tree insertion & deletion demo                   */
/*           Written for XTANGO at RPI by Aaron Candib 6/23/92               */
/*****************************************************************************/

#include <stdio.h>
#include <math.h>
#include "xtango.h"
#include "rdblktree.h"
#define TRUE 1
#define FALSE 0
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
rbptr p;
{
  if (p==NULL) {printf("ERROR: Null pointer passed to loc.\n"); return -1; }
  else if (p->parent==NULL) return 0;
  else if (p==p->parent->left) return 2*(loc(p->parent)+1)-1;
  else return 2*(loc(p->parent)+1);
}

/*****************************************************************************/
AnimColorNode(p) /* Node must be in the RB-tree, changes to preset color */
rbptr p;
{
  TANGO_COLOR color;
  TANGO_IMAGE node=(TANGO_IMAGE)ASSOCretrieve("Node",loc(p));

  if (p->color=='B') color=TANGO_COLOR_BLACK;
  else color=TANGO_COLOR_RED;

  TANGOtrans_perform(TAPflash(node,2));
  TANGOtrans_perform(TAPcolor(node,color));
  TANGOtrans_perform(TAPflash(node,2));
}

/*****************************************************************************/
AnimComparison(dat,x,flag) /* Animates comparison and moving down the tree */
int dat; rbptr x; char flag;
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
                                   TANGO_COLOR_BLUE,NULL,"<",1);
        dx*=(width(curnode)/2.0+2*width(lessthan)+width(newnode)/2.0);
        if (dx<0) lx-=(width(lessthan)+width(curnode)/2.0);
        else lx+=(width(lessthan)+width(curnode)/2.0);
        lessthan=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,lx,ly,0,
                                   TANGO_COLOR_BLUE,NULL,"<",1);

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
rbptr p;
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
rbptr p;
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
                              TANGO_COLOR_BLUE,2,x,y,0.0,1.0,1);
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
void AnimLeftRotate(p) /* Shows the red-black rotation around node 'p' */
rbptr p;
{
  TANGO_TRANS t[14],tmp,rotate=NULL,arrange=NULL;
  TANGO_PATH path;
  TANGO_LOC loc1,loc2;
  TANGO_IMAGE node,link;
  char error,mainerror=FALSE;
   /* The following ints indicate key locations in the subtree to be rotated */
  int root=loc(p),i,l=(root+1)*2-1,r=(root+1)*2;
  int ll=(l+1)*2-1,lr=(l+1)*2,rl=(r+1)*2-1,rr=(r+1)*2;

  if (InTree(l))
  {                  /* t[1]: Move the 'l' subtree down to the 'll' position */
    t[1]=BuildMoveSubtree(l,l,ll,&error);
    mainerror=mainerror||error;
    t[6]=MoveLink(l,ll); /* t[6]: and also move its connecting link */
  }
  else t[1]=t[6]=NULL;

  if (InTree(rl)) 
  {
                         /* t[2]: move the 'rl' subtree to the 'lr' position */
    t[2]=BuildMoveSubtree(rl,rl,lr,&error);
    mainerror=mainerror||error;

     /* t[11]: Move the link between 'r' and 'rl' to between 'root' and 'rl' */
    link=(TANGO_IMAGE)ASSOCretrieve("Link",rl);
    path=TreePath(l,rl);
    tmp=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
    t[11]=TANGOtrans_compose(2,MoveLink(rl,l),tmp);

      /* t[8]: After t[11], make the link to the 'rl' subtree stay attatched */
    ASSOCstore("TMPLink",lr,link);
    path=TreePath(root,l);
    tmp=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,link,path);
    path=TANGOpath_reverse(path);
    tmp=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                   link,path));
    t[8]=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                   link,TreePath(rl,lr)));
  }
  else t[2]=t[11]=t[8]=NULL;

  if (InTree(rr))
  {                                 /* t[3]: Move the 'rr' subtree up to 'r' */
    t[3]=BuildMoveSubtree(rr,rr,r,&error);
    mainerror=mainerror||error;
        /* t[7]: and also move the link connecting it to the tree up with it */
    t[7]=MoveLink(rr,r);
  }
  else t[3]=t[7]=NULL;

  if (root>0)          /* t[13] and t[10] move the link to the whole subtree */
  {
    link=(TANGO_IMAGE)ASSOCretrieve("Link",root);
    path=TreePath(r,root);
    t[10]=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);

    path=TreePath(root,r);
    t[13]=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
  }
  else t[10]=t[13]=NULL;

                         /* t[4],t[5]: move 'root' to 'l' and 'r' to 'root'. */
          /* These are the two nodes around which the whole rotation centers */
  t[4]=MoveNode(root,l);
  t[5]=MoveNode(r,root);

                      /* t[12] reverses the direction of the 'root'-'r' link */
                                    /* showing that 'r' will be the new root */
  link=(TANGO_IMAGE)ASSOCretrieve("Link",r);
  path=TANGOpath_motion(TANGOimage_loc(link,TANGO_PART_TYPE_NW),
                        TANGOimage_loc(link,TANGO_PART_TYPE_SE),
                        TANGO_PATH_TYPE_STRAIGHT);
  tmp=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,link,path);
  path=TANGOpath_scale(path,-2.0,-2.0);
  t[12]=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
  t[12]=TANGOtrans_compose(2,tmp,t[12]);

       /* t[9], following t[12], keeps the 'r'-'root' link attatched to both */
  ASSOCstore("TMPLink",l,link);
  ASSOCdelete("Link",r);
  path=TANGOpath_motion(TANGOimage_loc(link,TANGO_PART_TYPE_SE),
                        TANGOimage_loc(link,TANGO_PART_TYPE_NW),
                        TANGO_PATH_TYPE_STRAIGHT);
  tmp=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,link,path);
  path=TANGOpath_scale(path,-2.0,-2.0);
  tmp=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                 link,path));
  t[9]=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                  link,TreePath(r,l)));

  for (i=11;i<=13;++i) /* t[11]-t[13] represent the pointer manipulation */
    if (t[i])
    {
      if (arrange) arrange=TANGOtrans_compose(2,arrange,t[i]);
      else arrange=t[i];
    }
  for (i=1;i<=10;++i) /* t[1]-t[10] represent putting the tree back in order */
    if (t[i])
    {
      if (rotate) rotate=TANGOtrans_compose(2,rotate,t[i]);
      else rotate=t[i];
    }
  rotate=TANGOtrans_concatenate(2,arrange,rotate);
  TANGOtrans_perform(rotate);
  AssocFix();
  TANGOtrans_free(1,rotate);
}

/*****************************************************************************/
AnimRightRotate(p)
rbptr p;
{
  TANGO_TRANS t[14],tmp,rotate=NULL,arrange=NULL;
  TANGO_PATH path;
  TANGO_LOC loc1,loc2;
  TANGO_IMAGE node,link;
  char error,mainerror=FALSE;
  int root=loc(p),i,l=(root+1)*2-1,r=(root+1)*2;
  int ll=(l+1)*2-1,lr=(l+1)*2,rl=(r+1)*2-1,rr=(r+1)*2;

  if (InTree(ll))
  {
    t[1]=BuildMoveSubtree(ll,ll,l,&error);
    mainerror=mainerror||error;
    t[6]=MoveLink(ll,l);
  }
  else t[1]=t[6]=NULL;

  if (InTree(lr)) 
  {
    t[2]=BuildMoveSubtree(lr,lr,rl,&error);
    mainerror=mainerror||error;

    link=(TANGO_IMAGE)ASSOCretrieve("Link",lr);
    ASSOCstore("TMPLink",rl,link);
    path=TreePath(root,r);
    tmp=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,link,path);
    path=TANGOpath_reverse(path);
    tmp=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                   link,path));
    t[8]=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                    link,TreePath(lr,rl)));

    path=TreePath(r,lr);
    tmp=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
    t[11]=TANGOtrans_compose(2,MoveLink(lr,r),tmp);
  }
  else t[2]=t[11]=t[8]=NULL;

  if (InTree(r))
  {
    t[3]=BuildMoveSubtree(r,r,rr,&error);
    mainerror=mainerror||error;
    t[7]=MoveLink(r,rr);
  }
  else t[3]=t[7]=NULL;

  if (root>0)
  {
    link=(TANGO_IMAGE)ASSOCretrieve("Link",root);
    path=TreePath(l,root);
    t[10]=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);

    path=TreePath(root,l);
    t[13]=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
  }
  else t[10]=t[13]=NULL;

  t[4]=MoveNode(l,root);
  t[5]=MoveNode(root,r);

  link=(TANGO_IMAGE)ASSOCretrieve("Link",l);
  ASSOCstore("TMPLink",r,link);
  ASSOCdelete("Link",l);
  path=TANGOpath_motion(TANGOimage_loc(link,TANGO_PART_TYPE_SW),
                        TANGOimage_loc(link,TANGO_PART_TYPE_NE),
                        TANGO_PATH_TYPE_STRAIGHT);
  tmp=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,link,path);
  path=TANGOpath_scale(path,-2.0,-2.0);
  tmp=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                 link,path));
  t[9]=TANGOtrans_compose(2,tmp,TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
                                                  link,TreePath(l,r)));

  path=TANGOpath_motion(TANGOimage_loc(link,TANGO_PART_TYPE_NE),
                        TANGOimage_loc(link,TANGO_PART_TYPE_SW),
                        TANGO_PATH_TYPE_STRAIGHT);
  tmp=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,link,path);
  path=TANGOpath_scale(path,-2.0,-2.0);
  t[12]=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
  t[12]=TANGOtrans_compose(2,tmp,t[12]);

  for (i=1;i<=10;++i)
    if (t[i])
    {
      if (rotate) rotate=TANGOtrans_compose(2,rotate,t[i]);
      else rotate=t[i];
    }
  for (i=11;i<=13;++i)
    if (t[i])
    {
      if (arrange) arrange=TANGOtrans_compose(2,arrange,t[i]);
      else arrange=t[i];
    }
  rotate=TANGOtrans_concatenate(2,arrange,rotate);
  if (!mainerror)
  { 
    TANGOtrans_perform(rotate);
    AssocFix();
    
  }
  else printf("ERROR!  Rotate out of bounds!  Imminent program crash!\n");
  TANGOtrans_free(1,rotate);
}

/*****************************************************************************/
AnimBinaryDelete(z,x,y,nil) /* make sure nonempty tree */
rbptr z,x,y,nil;
{
  char error;
  int xn,zn=loc(z), yn=loc(y), fixn;
  TANGO_TRANS movetree=NULL,grabtree=NULL,pulltree=NULL,
              chucknode=NULL,nodemove=NULL,dellink=NULL,anim=NULL,tmp;
  TANGO_IMAGE delnode,replacenode=NULL,link=NULL;
  TANGO_LOC pt;
  TANGO_PATH path;

  
  if (x!=nil) xn=loc(x); else xn= -1;
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
  if (x!=nil)
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

  if ((y->parent)&&(x!=nil))
  {
    link=(TANGO_IMAGE)ASSOCretrieve("Link",yn);
    path=TreePath(yn,xn);
    grabtree=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,path);
    pulltree=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,link,
                                         TANGOpath_reverse(path));
    TANGOpath_free(1,path);
  }
  if (x!=nil)
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
