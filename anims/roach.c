#include <stdio.h>
#include "xtango.h"

/************************************************************************

	Program:  The 'Drunken Coackroach' Animation program.

	Description:  This program re-creates the actions of a drunk cock-
		      roach as it walks upon a grid.

	Class: ICS 8113 J Program Visualization

	Author: Michael Burns

*************************************************************************/

#define NUMOFSQRS 6     /* cockroach will walk on a 6 x 6 screen */

void ANIMInit(), ANIMMove();

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
			    {"Move", 1, {VOID, (FPTR)ANIMMove}},
			    {NULL,   NULL, NULL, NULL} };


main()
{
   void get_new_location();	/* call to get a new location for roach */

   int xint, yint;	/* floating point co-ords mapped to int co-ords */
   int i, j;		/* General Loop-Control variables */
   int xcur, ycur;	/* Current Coordinates of cockroach */
   int xold, yold;
   int seed;

   TANGOalgoOp(fcn, "BEGIN");

   printf("What is the random number seed (int)?\n");
   scanf("%d",&seed);
   srandom(seed);

       TANGOalgoOp(fcn, "Init", NUMOFSQRS);

       /* Set the current location for the cockroach */
       xcur = 0;
       ycur = 0;

       /* now make the coackroach walk about 2500 times */
       for (i = 0; i <= 2500; i++)
       {
	  /* save the old location of the coackroach */
	  xold = xcur;
	  yold = ycur;

	  /* pick a new direction for the coackroach to go */
	  get_new_location(&xcur, &ycur);

	  /* now, make a path between those two locations */
	  /* from is where we are currently at */
          TANGOalgoOp(fcn,"Move",xold,yold,xcur,ycur);

	 /* Need to sleep for a second to provide a short wait */
	}
   TANGOalgoOp(fcn, "END");
}



void
get_new_location(xcur, ycur)
int *xcur, *ycur;
{
   int dir;
   int xtemp, ytemp;

   /* loop until we find a valid direction to go */
   while(1)
   {
      /* get a copy of the current location to work on */
      xtemp = *xcur;
      ytemp = *ycur;

      /* pick a random number between 0 and 7 */
      dir = random() % 8;

      /* do the cases */
      switch (dir)
      {
	case         0: ytemp--; break;
	case         1: ytemp++; break;
	case         2: xtemp--; break;
	case         3: xtemp++; break;
	case         4: ytemp--; xtemp--; break;
	case         5: ytemp--; xtemp++; break;
	case         6: ytemp++; xtemp--; break;
	case         7: ytemp++; xtemp++; break;
	default: break;
      }

      /* is it valid */
      if ( (ytemp < 0) || (ytemp == NUMOFSQRS) ||
	   (xtemp < 0) || (xtemp == NUMOFSQRS) )
       ; /* do nothing, will loop around */
      else
      {
	*xcur = xtemp;
	*ycur = ytemp;
	return;
      }
   } /* end while */
} /* end of function get_new_location */



