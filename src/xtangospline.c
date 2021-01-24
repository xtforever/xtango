/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (spline)	      */
/*							      */
/*     Written by Steven P. Reiss.  Used with permission.     */
/**************************************************************/
/*	Copyright 1989 Brown University -- Steven P. Reiss    */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/10/26 JDH	      Baselined source.                       */
/*							      */
/**************************************************************/

#include <stdio.h>
#include "xtangolocal.h"

/**************************************************************/
/*	Spline drawing definitions			      */
/**************************************************************/

#define NUMPOINTS	200
#define SMOOTHNESS	1.0

typedef struct _LINELIST {
   int size;
   int count;
   int *x;
   int *y;
} LINELIST;

/**************************************************************/
/*	Spline Macro routines				      */
/**************************************************************/

#define DOUBLE(x)	((double)(x))
//#define ROUND(x)	((int)((x)+0.5))

#define ThirdPoint(x0,y0,x1,y1,tx,ty) { 		\
   tx = (2*x0 + x1) / 3.0;				\
   ty = (2*y0 + y1) / 3.0;				\
}

#define Midpoint(x0,y0,x1,y1,mx,my) {			\
   mx = (x0 + x1) / 2.0;				\
   my = (y0 + y1) / 2.0;				\
}

#define INIT_LINELIST(ll,ct) {			       \
   (ll)->size = ct;				       \
   (ll)->count = 0;				       \
   (ll)->x = (int *) malloc(sizeof(int)*ct);  	       \
   (ll)->y = (int *) malloc(sizeof(int)*ct);	       \
}

#define FREE_LINELIST(ll) {			       \
   if ((ll)->x) { 	        		       \
      free((ll)->x);				       \
      free((ll)->y);				       \
      (ll)->x = (int *) NULL;			       \
      (ll)->y = (int *) NULL;			       \
    };						       \
}

#define AddLine(ll,x0,y0,x1,y1) {			\
   if (ll->count >= ll->size) { 			\
      GrowBuf(ll);					\
    };							\
   if (ll->count == 0) {				\
      ll->x[0] = ROUND(x0);				\
      ll->y[0] = ROUND(y0);				\
      ll->count = 1;					\
    };							\
   ll->x[ll->count] = ROUND(x1);			\
   ll->y[ll->count] = ROUND(y1);			\
   ++(ll->count);					\
}

#define CanApproxWithLine(x0,y0,x2,y2,x3,y3,fg) {			\
   double triangleArea, sideSquared, dx, dy;				\
									\
   triangleArea = x0*y2 - x2*y0 + x2*y3 - x3*y2 + x3*y0 - x0*y3;	\
   triangleArea *= triangleArea;					\
   dx = x3 - x0;							\
   dy = y3 - y0;							\
   sideSquared = dx*dx + dy*dy; 					\
   fg = triangleArea <= SMOOTHNESS * sideSquared;			\
};

/**************************************************************/
/*	Forward references				      */
/**************************************************************/

static	void		CreateOpenLineList();
static	void		CalcBSpline();
static	void		AddBezierArc();
static	void		GrowBuf();


/**************************************************************/
/*	TANGO_spline -- draw a spline through given points    */
/**************************************************************/
void
TANGO_Spline(count,x,y)
   int count;
   int x[];
   int y[];
{
   LINELIST ll;

   INIT_LINELIST(&ll,NUMPOINTS);

   CreateOpenLineList(&ll, x, y, count);
   TANGO_Polyline(ll.count, ll.x, ll.y);

   FREE_LINELIST(&ll);
}



/***************************************************************/
/*      CreateOpenLineList -- create line list for open spline */
/***************************************************************/
static void
CreateOpenLineList(ll, cpx,cpy,cpcount)
   LINELIST * ll;
   int cpx[];
   int cpy[];
   int cpcount;
{
    register int cpi;

    ll->count = 0;

    CalcBSpline(ll, cpx[0], cpy[0], cpx[0], cpy[0], cpx[0], cpy[0], cpx[1], cpy[1] );
    CalcBSpline(ll, cpx[0], cpy[0], cpx[0], cpy[0], cpx[1], cpy[1], cpx[2], cpy[2] );

    for (cpi = 1; cpi < cpcount - 2; ++cpi) {
	CalcBSpline(ll, cpx[cpi - 1], cpy[cpi - 1], cpx[cpi], cpy[cpi],
			cpx[cpi + 1], cpy[cpi + 1], cpx[cpi + 2], cpy[cpi + 2] );
     };

    CalcBSpline(ll, cpx[cpi - 1], cpy[cpi - 1], cpx[cpi], cpy[cpi],
		    cpx[cpi + 1], cpy[cpi + 1], cpx[cpi + 1], cpy[cpi + 1] );
    CalcBSpline(ll, cpx[cpi], cpy[cpi], cpx[cpi + 1], cpy[cpi + 1],
		    cpx[cpi + 1], cpy[cpi + 1], cpx[cpi + 1], cpy[cpi + 1] );
}



/**************************************************************/
/*	CalcBSpline -- add points for spline to list	      */
/**************************************************************/
static void
CalcBSpline(ll, cminus1x, cminus1y, cx, cy, cplus1x, cplus1y, cplus2x, cplus2y )
   LINELIST * ll;
   int cminus1x,cminus1y;
   int cx,cy;
   int cplus1x,cplus1y;
   int cplus2x,cplus2y;
{
   double p0x, p1x, p2x, p3x, tempx, p0y, p1y, p2y, p3y, tempy;
   double fcx,fcy,fc1x,fc1y,fcm1x,fcm1y,fc2x,fc2y;

   fcx = cx;
   fcy = cy;
   fc1x = cplus1x;
   fc1y = cplus1y;
   fcm1x = cminus1x;
   fcm1y = cminus1y;
   fc2x = cplus2x;
   fc2y = cplus2y;

   ThirdPoint(fcx,fcy,fc1x,fc1y, p1x,p1y );
   ThirdPoint(fc1x,fc1y,fcx,fcy, p2x,p2y );
   ThirdPoint(fcx,fcy,fcm1x,fcm1y, tempx,tempy);
   Midpoint(tempx, tempy, p1x, p1y, p0x, p0y);
   ThirdPoint(fc1x,fc1y,fc2x,fc2y, tempx,tempy );
   Midpoint(tempx, tempy, p2x, p2y, p3x, p3y);

   AddBezierArc(ll, p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y);
}

/**************************************************************/
/*	AddBezierArc -- add arc to			      */
/**************************************************************/
static void
AddBezierArc(ll,x0,y0,x1,y1,x2,y2,x3,y3)
   LINELIST * ll;
   double x0,y0;
   double x1,y1;
   double x2,y2;
   double x3,y3;
{
   double midx01, midx12, midx23, midlsegx, midrsegx, cx;
   double midy01, midy12, midy23, midlsegy, midrsegy, cy;
   char fg;

   Midpoint(x0, y0, x1, y1, midx01, midy01);
   Midpoint(x1, y1, x2, y2, midx12, midy12);
   Midpoint(x2, y2, x3, y3, midx23, midy23);
   Midpoint(midx01, midy01, midx12, midy12, midlsegx, midlsegy);
   Midpoint(midx12, midy12, midx23, midy23, midrsegx, midrsegy);
   Midpoint(midlsegx, midlsegy, midrsegx, midrsegy, cx, cy);

   CanApproxWithLine(x0, y0, midlsegx, midlsegy, cx, cy, fg);
   if (fg) {
      AddLine(ll, x0, y0, cx, cy);
    }
   else if ( (midx01 != x1) || (midy01 != y1) ||
		(midlsegx != x2) || (midlsegy != y2) ||
		(cx != x3) || (cy != y3) ) {
	AddBezierArc(ll, x0, y0, midx01, midy01, midlsegx, midlsegy, cx, cy );
    };

   CanApproxWithLine(cx, cy, midx23, midy23, x3, y3, fg);
   if (fg) {
      AddLine(ll, cx, cy, x3, y3);
    }
   else if ( (cx != x0) || (cy != y0) ||
		(midrsegx != x1) || (midrsegy != y1) ||
		(midx23 != x2) || (midy23 != y2) ) {
	AddBezierArc(ll, cx, cy, midrsegx, midrsegy, midx23, midy23, x3, y3 );
    };
}



/**************************************************************/
/*	GrowBuf -- augment a line list			      */
/**************************************************************/
static void
GrowBuf(ll)
   LINELIST * ll;
{
   ll->size = (ll->size) * 2;

   ll->x = (int *) realloc(ll->x, (ll->size)*sizeof(int));
   ll->y = (int *) realloc(ll->y, (ll->size)*sizeof(int));
}

/**************************************************************/
/*****************   end of xtangospline.c   ******************/
/**************************************************************/
