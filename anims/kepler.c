#include <stdio.h>
#include <math.h>
#include "xtango.h"

/*--------------------------------------------------------------------*/
/* 								      */
/*                     KEPLER'S LAW IN MOTION                         */
/*                  ANIMATED BY SCOTT MCCRICKARD                      */
/*                          WITH XTANGO                               */
/*  CS 8115 Project 1 -- project idea submitted by Dr. John Stasko.   */
/*                       					      */
/*  This program animates Kepler's Law of Planetary Motion, which     */
/*  states that the square of a planet's orbital period is            */
/*  proportional to its distance from the sun.  By watching two       */
/*  bodies in orbit, we can visualize this comparison.                */
/*								      */
/*--------------------------------------------------------------------*/

#define dt .003           /* time interval between calculations */
#define pi 3.141592654    /* ratio of circumference to diameter */

void ANIMInit(), ANIMMove();

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
                            {"Move", 1, {VOID, (FPTR)ANIMMove}},
                            {NULL,NULL,NULL,NULL}};
main()
{
  void newloc();
  int m,n;                /* m is an input variable, n is a counter */
  double r1,theta1,x1,y1; /* radius, angle from center,           */
  double r2,theta2,x2,y2; /* x and y coordinates for both planets */
  double r3,theta3,x3,y3;

  printf("Kepler's Laws of Motion\n\n");
  TANGOalgoOp (fcn, "BEGIN");

  printf("Enter your answers in natural numbers <100.\n");
  printf("What is the inner planet's initial distance from the sun?\n");
  scanf("%d",&m);
  r1=m/200.00000;  /* convert integer into float in range of display */
  printf("What is the middle planet's initial distance from the sun?\n");
  scanf("%d",&m);
  r2=m/200.00000;  /* convert integer into float in range of display */
  printf("What is the outer planet's initial distance from the sun?\n");
  scanf("%d",&m);
  r3=m/200.00000;  /* convert integer into float in range of display */

  theta1=0.0;
  theta2=0.0;
  theta3=0.0;      /* initially, angle is 0 for both planets */

  TANGOalgoOp (fcn, "Init",r1+.5,r2+.5,r3+.5);

  for (n=0; n<=10000; n++) 
     {
      newloc(r1,&theta1,&x1,&y1);          /* calculate new x,y location */
      newloc(r2,&theta2,&x2,&y2);          /* and new theta              */
      newloc(r3,&theta3,&x3,&y3);

      TANGOalgoOp(fcn,"Move",x1,y1,x2,y2,x3,y3); /* move the planets */
     }

  TANGOalgoOp(fcn,"END");
}


void
newloc(r,theta,x,y)
double r,*theta,*x,*y;
{
  *theta = *theta + (2 * pi * dt / r ) / sqrt (r); 
                  /* theta = theta + d(theta) */
                  /* d(theta) = v dt = (2 * pi / r^3/2) dt         */
                  /* v = distance for 1 rotation / time for 1 rotation */
                  /* time for 1 rotation given by Kepler's Law         */

  *x = r * cos(*theta) + 0.5;  /* x component of distance */
  *y = r * sin(*theta) + 0.5;  /* y component of distance */
}


