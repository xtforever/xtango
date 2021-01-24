/*****************************************************************************/
/*                                ghost.c                                    */
/*                                                                           */
/*                  Match ghost with ghostbusters demo                       */
/*           Written for XTANGO at RPI by Aaron Candib 7/23/92               */
/*****************************************************************************/

#include <stdio.h>
#include "ghost.h"
#include "xtango.h"

void AnimDrawRoom(), AnimInitLeftmost(), AssocShift(), AssocSwap(), AnimInputPts(), AnimMoveLine();

static NAME_FUNCT fcn[] = { {"DrawRoom",1,{VOID, (FPTR)AnimDrawRoom}},
			    {"InitLeftmost",1,{VOID, (FPTR)AnimInitLeftmost}},
			    {"Shift",1,{VOID, (FPTR)AssocShift}},
			    {"Swap",1,{VOID, (FPTR)AssocSwap}},
			    {"InputPts",1,{VOID, (FPTR)AnimInputPts}},
			    {"MoveLine",1,{VOID, (FPTR)AnimMoveLine}},
			    {NULL,0,NULL,NULL}
			  };


double angle(x1,y1,xv,yv,x2,y2)
int x1,y1,xv,yv,x2,y2;

/* Computes the angle determined by two line segments with a common endpoint */
{
  int a1=x1-xv;
  int a2=y1-yv;
  int b1=x2-xv;
  int b2=y2-yv;
  return acos((double)(a1*b1+a2*b2)/sqrt((double)(a1*a1+a2*a2)*(b1*b1+b2*b2)));
} /* end angle */


int leftmost(arr,left,right)
pointlist arr;
int left,right;

/* Returns index of element in array portion with smallest x-value */
{
  int i,minx=MAX_XY,p;

  for (i=left;i<=right;++i)
    { if (arr[i].x<minx) { minx=arr[i].x; p=i; } }
  return p;
} /* end leftmost */


arraydump(a,first,last)
pointlist a; int first,last;
{
  int i;
  for (i=first;i<=last;++i)
  {
    printf("Lbl: %d  X: %d  Y: %d  Type: %c  Angle: %f\n",i,a[i].x,a[i].y,a[i].who,a[i].angle);
  }
  printf("\n");
}

void compute_angles(arr,left,right,center)
pointlist *arr;
int left,right,center;

/* Calculates the angle between each point in the array and a vertical drawn 
from a given point, 'center'. */
{
  int ct;

  (*arr)[center].angle= -1.0;
  for (ct=left;ct<=right;++ct)
    { if (ct!=center) 
        { (*arr)[ct].angle=angle((*arr)[center].x,(*arr)[center].y+1,(*arr)[center].x,(*arr)[center].y,(*arr)[ct].x,(*arr)[ct].y); }
    }
} /* end compute_angles */


anglesort(a,first,last)
pointlist *a;
int first,last;

/* Performs an insertion sort on the array segment on the angle key. */
{
  int i=first+1,j,ct;
  pointtype tmp;

  while (((*a)[i-1].angle<=(*a)[i].angle)&&(i<last)) ++i; /* start with 1'st out-of*/
                                                    /* order element. */
  for (i;i<=last;++i)
  {
    j=first;
    while ((*a)[j].angle<(*a)[i].angle) ++j;
    if (j<i)
      {
        tmp=(*a)[i];
        for (ct=i;ct>j;--ct) { (*a)[ct]=(*a)[ct-1]; }
        (*a)[j]=tmp;
        TANGOalgoOp(fcn,"Shift",i,j); /* do same swaps on XTANGO image assosciations */
      }
  }
} /* end anglesort */


void pairup(a,first,last)
pointlist *a;
int first,last;

/* The main recursive function.  Operating on the list of points, it will */
/* reorder the array to produce a list of paired ghosts and ghostbusters. */

{
  int gcount=0,bcount=0,i=first;
  pointtype tmp;

  if (last-first>1) /* if there are more than 2 elements in the list */
    {
      compute_angles(a,first,last,leftmost(a,first,last));
      anglesort(a,first,last);
      TANGOalgoOp(fcn,"InitLeftmost",first,(*a)[first].who);
      do
	{
          if ((*a)[i].who=='G') { ++gcount; }
          else { ++bcount; }
          if (i>first) TANGOalgoOp(fcn,"MoveLine",i);
          ++i;
        } while ((gcount!=bcount)||((*a)[first].who==(*a)[i-1].who));
      --i;
      if ((first+1)!=i)
      { 
        tmp=(*a)[first+1]; (*a)[first+1]=(*a)[i]; (*a)[i]=tmp;
        TANGOalgoOp(fcn,"Swap",first+1,i);
      }
/* make first 2 elements in array segment a pair to be connected */      
      pairup(a,first+2,i);
      pairup(a,i+1,last);

    }
  else if (last-first==1)
    {
      compute_angles(a,first,last,leftmost(a,first,last));
      anglesort(a,first,last);
      TANGOalgoOp(fcn,"InitLeftmost",first,(*a)[first].who);
      TANGOalgoOp(fcn,"MoveLine",last);
    }
} /* end pairup */


main()
{
  pointlist point;                     /* The array of points */
  int x,y,ct,size,minx,maxx,miny,maxy; /* Yes, we'll need them all. */

  minx=miny=MAX_XY;maxx=maxy=ct=size=0;
  TANGOalgoOp(fcn,"BEGIN");
/*
  while (scanf("%d%d",&x,&y)!=EOF)
    {
      ++ct;
      point[ct].x=x; point[ct].y=y;
      if (ct%2==0) {point[ct].who='G';}
      else {point[ct].who='B';}
      if (x<minx) minx=x;
      if (x>maxx) maxx=x;
      if (y<miny) miny=y;
      if (y>maxy) maxy=y;
    }
  size=ct;
  TANGOalgoOp(fcn,"DrawRoom",point,size,minx,maxx,miny,maxy);
*/
  TANGOalgoOp(fcn,"InputPts",&point,&size,&minx,&maxx,&miny,&maxy);
  pairup(&point,1,size);
  TANGOalgoOp(fcn,"END");
} /* end main */
