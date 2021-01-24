#include <stdio.h>
#include "xtango.h"

/************************************************************************

	Program:  The 'Drunken Coackroach' Animation program.

	Description:  This program re-creates the actions of a drunk cock-
		      roach as it walks upon a grid.

	Class: ICS 8113 J Program Visualization

	Authors: Michael Burns & John Stasko

*************************************************************************/

#define OFFSET 0.1	/* offset from the borders of the screen */
#define FILL 1.0	/* Fill value for square */
#define NO_FILL 0.0	/* No fill value */
#define VISIBLE 1	/* Item is visible */
#define NOT_VISIBLE 0   /* Item is not visible */


   TANGO_IMAGE squares[20][20];		      /* Images of squares */

   static TANGO_IMAGE_COMPONENT roach[4] = {
     { TANGO_IMAGE_TYPE_CIRCLE, "0.0 0.0 TANGO_COLOR_BLACK 0.02 1.0"},
     { TANGO_IMAGE_TYPE_LINE, "-0.03 -0.03 TANGO_COLOR_BLACK 0.06 0.06 0.5 1.0 0"},
     { TANGO_IMAGE_TYPE_LINE, "-0.03 0.03 TANGO_COLOR_BLACK 0.06 -0.06 0.5 1.0 0"},
     {TANGO_IMAGE_TYPE_COMPOSITE, NULL}
   };

   TANGO_IMAGE cockroach;  		/* Cockroach Image */



/* draws the board that the cockroach is going to walk on */
void
ANIMInit(numsquares)
   int numsquares;
{
   int xint, yint;	/* floating point co-ords mapped to int co-ords */
   int i, j;		/* General Loop-Control variables */
   double x, y; 	/* floating-point point co-ordinates */
   double cx, cy;	/* coackroach location for start */
   double side;		/* caculates length of side */

   TANGO_PATH  null_path; 		/* Path of null length */
   TANGO_TRANS plot_squares; 		/* trans used to plot squares */
   TANGO_LOC   cloc;

   /* First, initialize the TANGO system */
   TANGOinit();

   /* the length of a side of a square is the width of a screen */
   /* minus 2 * offset distance.  This gives the total area that you */
   /* can use for squares.  to find the length of a side of a square */
   /* then divide by the number of squares that you want on the screen */
   side = (1.0 - (2.0 * OFFSET)) / (double) numsquares;

   /* Now loop in x and y direction to determine where the squares are. */
   /* Store these into a TANGO location array.       */
   /* This way, square[0][0] has the location for the first row, and */
   /* first column on the screen */
   for(x = OFFSET, xint = 0; xint < numsquares; x+= side, xint++)
     for(y = OFFSET, yint = 0; yint <numsquares; y+= side, yint++)
     {
       squares[xint][yint] =
	   TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,	/* want a rectangle */
			   x,		/* x location of rectangle */
			   y,		/* y location of rectangle */
			   VISIBLE,	/* rectangle is visible */
			   TANGO_COLOR_BLACK,  /* outline is in black */
			   side,	/* length in X direction */
			   side,	/* length in Y direction */
			   NO_FILL);	/* Do not fill this rectangle */

	/* Create square's outline in a closer plane */
	/*    than the filled-in squares	     */
	TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,	/* want a rectangle */
			   x,		/* x location of rectangle */
			   y,		/* y location of rectangle */
			   VISIBLE,	/* rectangle is visible */
			   TANGO_COLOR_BLACK,  /* outline is in black */
			   side,	/* length in X direction */
			   side,	/* length in Y direction */
			   NO_FILL);	/* Do not fill this rectangle */
     } /* end for */

     /* okay, now make those squares visible */
     /* this is done by doing a transition on a NULL path */
     null_path = TANGOpath_null(1);
     plot_squares = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,null_path);
     TANGOtrans_perform(plot_squares);

       /* Now create the cockroach square */
       /* This is a solid filled in black square that is 80 % */
       /* the size of one of the stepping squares */
       /* Start in the upper left corner */
       cloc = TANGOimage_loc(squares[0][0],TANGO_PART_TYPE_C);
       cockroach = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
                                     TANGOloc_X(cloc), 
                                     TANGOloc_Y(cloc), 1, roach);

     TANGOtrans_perform(plot_squares);
 }



void
ANIMMove(fx,fy,tx,ty)
   int fx,fy,tx,ty;
   {
   /* TANGO stuff */
   TANGO_LOC from, to; 		/* Locations from where roach is, to 
                                   where roach wants to go */
   TANGO_PATH  roach_path; 		/* Path roach will take */
   TANGO_TRANS roach_trans;		/* roach transition */
   TANGO_PATH  fill_path;		/* New fill path for dest. sqr */
   TANGO_TRANS fill_trans;		/* New fill transition for dest. sqr*/
   TANGO_TRANS move_trans;		/* Composition of move & fill */
   double      x[1], y[1];		/* Fill change */

	  /* now, make a path between those two locations */
	  /* from is where we are currently at */
	  from = TANGOimage_loc(squares[fx][fy],
				TANGO_PART_TYPE_C);	/* center */

	  /* To is where we want to go */
	  to = TANGOimage_loc(squares[tx][ty],
				TANGO_PART_TYPE_C);	/* center */


	  /* Now create a path between the two locations */
	  /* This is a straight path between from and to locations */
	  roach_path = TANGOpath_motion(from, to,
					TANGO_PATH_TYPE_STRAIGHT);

	 /* Setup the transition path between objects */
	 roach_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,
					 cockroach,
					 roach_path);

	  /* Slightly darken the visited square */
	  x[0] = 1.0 / 40.0;	/* Forty fill styles */
	  y[0] = 0.0;
          fill_path  = TANGOpath_create(1, x, y);
          fill_trans = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,
					 squares[tx][ty], fill_path);
          
	  /* Move the roach and THEN change the destination square's fill */
	  move_trans = TANGOtrans_concatenate(2, roach_trans, fill_trans);

	 /* Now do the transition */
         TANGOtrans_perform(move_trans);
}



