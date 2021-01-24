/*****************************************************************************/
/*                                ghost.c                                    */
/*                                                                           */
/*                  Match ghost with ghostbusters demo                       */
/*           Written for XTANGO at RPI by Aaron Candib 7/23/92               */
/*****************************************************************************/

#include <stdio.h>
#include "xtango.h"
#include "ghost.h"

#define TRUE 1
#define FALSE 0

void AnimDrawRoom(pts,npts,minx,maxx,miny,maxy)
pointlist pts;
int npts,minx,maxx,miny,maxy;
{
  int i;
  double x,y;
  TANGO_IMAGE ptimage[MAXPTS],ghost,buster,line,button;
  TANGO_LOC ptloc;
/*
  static TANGO_IMAGE_COMPONENT gcmp[3] = {
    { TANGO_IMAGE_TYPE_CIRCLE, "0.0 0.0 TANGO_COLOR_RED 0.015 0.0" },
    { TANGO_IMAGE_TYPE_TEXT, "0.0 0.005 TANGO_COLOR_RED NULL G 1" },
    { TANGO_IMAGE_TYPE_COMPOSITE, NULL } };
  static TANGO_IMAGE_COMPONENT bcmp[3] = {
    { TANGO_IMAGE_TYPE_CIRCLE, "0.0 0.0 TANGO_COLOR_BLUE 0.015 0.0" },
    { TANGO_IMAGE_TYPE_TEXT, "0.0 0.005 TANGO_COLOR_BLUE NULL B 1" },
    { TANGO_IMAGE_TYPE_COMPOSITE, NULL } };

  ghost=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,0.5,0.5,0,gcmp);
  buster=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,0.5,0.5,0,bcmp);
*/

  ASSOCinit();
  ASSOCmake("Line",0);
  ASSOCmake("Point",1);
  ASSOCmake("Button",0);

  ghost=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.5,0.5,0,TANGO_COLOR_RED,NULL,"G",1);
  buster=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.5,0.5,0,TANGO_COLOR_BLUE,NULL,"B",1);
  button=TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.5-0.05,0.05-0.025,0,TANGO_COLOR_GREEN,0.05,0.025,1);
  ASSOCstore("Button",button);

/*
  TANGOtrans_perform(TAPflash(ghost,10));
  TANGOtrans_perform(TAPflash(buster,10));
*/
  for (i=1;i<=npts;++i)
  {
    if (pts[i].who=='G') ptimage[i]=TANGOimage_copy(ghost);
    else ptimage[i]=TANGOimage_copy(buster);
    ptloc=TANGOloc_create(((double)pts[i].x)/((double)MAX_XY),((double)pts[i].y)/((double)MAX_XY));
    TANGOtrans_perform(TAPjump(ptimage[i],TANGO_PART_TYPE_C,ptloc));
    TANGOtrans_perform(TAPvis_toggle(ptimage[i]));
    ASSOCstore("Point",i,ptimage[i]);
  }
}

char Collin(x1,y1,x2,y2,x3,y3) /* check if (x1,y1),(x2,y2),(x3,y3) are collinear */
long int x1,y1,x2,y2,x3,y3;
{
  return ((y1-y2)*(x1-x3)==(y1-y3)*(x1-x2));
}

char Collinear(pts,n,px,py)
pointlist pts;
int n,px,py;
{
  int i,j;
  char line=FALSE;

  for (i=1;(i<n)&&(!line);++i)
    for (j=i+1;(j<=n)&&(!line);++j)
      if (Collin(pts[i].x,pts[i].y,pts[j].x,pts[j].y,px,py)) line=TRUE;
  return line;
}

void AnimInputPts(pts,size,minx,maxx,miny,maxy)
pointlist *pts;
int *size,*minx,*maxx,*miny,*maxy;
{
  double x,y,lx,ly,rx,ry;
  int ct=0,scale=MAX_XY,intx,inty;
  char done=FALSE;
  TANGO_IMAGE button,point,ghost,buster,line;
  TANGO_LOC ptloc;

  *minx= *miny = 1000000000.0;
  *maxx= *maxy = -10.0;
  ASSOCinit();
  ASSOCmake("Line",0);
  ASSOCmake("Point",1);
  ghost=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.5,0.5,0,TANGO_COLOR_RED,NULL,"G",1);
  buster=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.5,0.5,0,TANGO_COLOR_BLUE,NULL,"B",1);
  button=TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.5-0.05,0.05-0.025,1,TANGO_COLOR_GREEN,0.05,0.025,1.0);
  TANGOloc_inquire(TANGOimage_loc(button,TANGO_PART_TYPE_NW),&lx,&ly);
  TANGOloc_inquire(TANGOimage_loc(button,TANGO_PART_TYPE_SE),&rx,&ry);
  TAPshow(button);
  printf("Input points with the mouse.  Click the box at the top of the screen to finish.\n");

  while (!done)
  {
    while (!TANGOinput_coord(&x,&y)) ;
    intx=(int)(x*scale);
    inty=(int)(y*scale);
    if ((lx<=x)&&(x<=rx)&&(ly<=y)&&(y<=ry)) done=TRUE;
    else
    {
      if (!Collinear(*pts,ct,intx,inty))
      {
        (*pts)[++ct].x=intx;
        (*pts)[ct].y=inty;
        if (ct%2==0) { (*pts)[ct].who='G'; point=TANGOimage_copy(ghost); }
        else { (*pts)[ct].who='B'; point=TANGOimage_copy(buster); }
        ptloc=TANGOloc_create(x,y);
        TANGOtrans_perform(TAPjump(point,TANGO_PART_TYPE_C,ptloc));
        TANGOtrans_perform(TAPvis_toggle(point));
        ASSOCstore("Point",ct,point);
        if (intx<*minx) *minx=intx;
        if (intx>*maxx) *maxx=intx;
        if (inty<*miny) *miny=inty;
        if (inty>*maxy) *maxy=inty;
      }
      else printf("The point you selected is collinear with two other points.  Try somewhere else.\n");
    }
    if (done && (ct%2==1))
    {
      printf("You must enter an even number of points.  Add at least 1 more.\n");
      done=FALSE;
    }
  }
  TANGOtrans_perform(TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,button,TANGOpath_null(1)));
  *size=ct;
}

void AnimInitLeftmost(lmost,type)/* Call after anglesort when leftmost=first */
int lmost;
char type;
{
  TANGO_IMAGE line,point;
  TANGO_PATH path;
  TANGO_TRANS trans;
  TANGO_LOC fromloc,toloc;
  double x,y,vx[2],vy[2];
  int arrowdir;

  vx[0]=0;vy[0]=0;vx[1]=0;vy[1]=0;
  if (type=='B') arrowdir=1; else arrowdir=2;
  point=(TANGO_IMAGE)ASSOCretrieve("Point",lmost);
  TANGOloc_inquire(TANGOimage_loc(point,TANGO_PART_TYPE_C),&x,&y);
  line=TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE,x,y,1,TANGO_COLOR_MAROON,2,vx,vy,0.0,1.0,arrowdir);
  if (ASSOCmember("Line")) ASSOCdelete("Line");
  ASSOCstore("Line",line);
  TANGOtrans_perform(TAPflash(point,10));
}

void AssocShift(i,j) /* need j<i */
int i,j;
{
  int ct;
  TANGO_IMAGE tmp;

  tmp=(TANGO_IMAGE)ASSOCretrieve("Point",i);
  for (ct=i;ct>j;--ct)
  {
    ASSOCdelete("Point",ct);
    ASSOCstore("Point",ct,(TANGO_IMAGE)ASSOCretrieve("Point",ct-1));
  }
  ASSOCdelete("Point",j);
  ASSOCstore("Point",j,tmp);
}

void AssocSwap(i,j)
int i,j;
{
  TANGO_IMAGE tmp;

  tmp=(TANGO_IMAGE)ASSOCretrieve("Point",j);
  ASSOCdelete("Point",j);
  ASSOCstore("Point",j,(TANGO_IMAGE)ASSOCretrieve("Point",i));
  ASSOCdelete("Point",i);
  ASSOCstore("Point",i,tmp);
}

void AnimMoveLine(i) /* move arrowhead from 'i-1' to 'i' Arrowhead better be at 'i-1' */
int i;
{
  TANGO_IMAGE line,point;
  TANGO_PATH path;
  TANGO_TRANS trans;
  TANGO_LOC fromloc,toloc;

  line=(TANGO_IMAGE)ASSOCretrieve("Line");
  point=(TANGO_IMAGE)ASSOCretrieve("Point",i);
  fromloc=TANGOimage_loc((TANGO_IMAGE)ASSOCretrieve("Point",i-1),TANGO_PART_TYPE_C);
  toloc=TANGOimage_loc(point,TANGO_PART_TYPE_C);
  path=TANGOpath_motion(fromloc,toloc,TANGO_PATH_TYPE_STRAIGHT);
  trans=TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,line,path);
  trans=TANGOtrans_compose(2,trans,TAPflash(point,10));
  TANGOtrans_perform(trans);
  TANGOtrans_free(1,trans);
  TANGOpath_free(1,path);
}
