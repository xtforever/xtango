/*   Drawing Hilbert Curves
     by  Nirupama Mallavarupu
*/


#include <stdio.h>
#include "xtango.h"

void north();
void east();
void west();
void south();

void ANIMInit(), ANIMDraw(); /* THESE ARE THE ANIMATION FUNCTIONS */

/* THE FOLLOWING IS THE MAPPING FUNCTION PASSING  INFORMATION FROM THIS */
/* PROGRAM TO ITS CORRESPONDING ANIMATION  FUNCTIONS IN "newscenes.c"  */

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
			    {"Draw", 1, {VOID, (FPTR)ANIMDraw}},
			    {NULL,   0, NULL, NULL} };          

double  x, y, xo, yo, h; /* xo AND yo ARE THE STARTING COORDINATES FOR DRAWING */
			/*  x AND y ARE THE COORDINATES AT ANY  POINT OF TIME */
			/* h IS THE LENGTH OF A UNIT IN THE CURVE  */  

main()
{
   int i,n = 4;
   double sizX,sizY; 

   TANGOalgoOp(fcn, "BEGIN"); /* THIS IS FIRST STATEMENT OF ANY XTANGO PROGRAM */
   TANGOalgoOp(fcn, "Init");
   i = 0; h = 0.8000; yo = 0.5; xo = 0.5;

      printf(" Please give the order of hilbert curves u want to draw <only natural numbers>\n");
       scanf("%d",&n);
       while (n <= 0)
         {  printf("Give a positive number only for order");
             scanf("%d",&n);
         }
           
    do { 
	i++; h = h/2;
        yo += h/2;
	xo  += h/2;
	  y = yo;
	  x = xo;
	east(i);
     }
    while (i < n);
   TANGOalgoOp(fcn, "END");
}

/*  THE FOLLOWING FUNCTIONS EAST(), WEST(), NORTH() AND SOUTH() ARE USED  TO POSITION*/
/*  THE X AND Y COORDINATES TO DRAW THE HILBERT CURVES. EMBEDDED IN  THESE ROUTINES */
/*  ARE ALSO THE CALLS TO THE ANIMATION FUNCTIONS TO ACTUALLY DRAW THE CURVES.      */


void east(i)
int i;
{
   if (i > 0)
   {
    north(i - 1);
    TANGOalgoOp(fcn,"Draw", x, y, -h, 0.0000 );
    x -= h; 
    east(i - 1);
    TANGOalgoOp(fcn,"Draw", x, y,0.0000,-h );
    y -= h; 
    east(i - 1);
    TANGOalgoOp(fcn,"Draw", x, y, h, 0.0000 );
    x += h;
    south(i -1);
   }
}

void north(i)
int i;
{
  if (i > 0)
    {
      east(i - 1);
      TANGOalgoOp(fcn,"Draw", x, y, 0.0000,-h );
      y -= h;	
      north(i - 1);
      TANGOalgoOp(fcn,"Draw", x, y, -h, 0.0000);
	x -= h;
      north(i - 1);
      TANGOalgoOp(fcn,"Draw", x, y, 0.0000, h );
      y += h;
      west(i -1);
    }
}

void south(i)
int i;
{
   if (i > 0)
   {
        west(i -1);
     TANGOalgoOp(fcn,"Draw",x,y,0.0000,h);
	y += h;
     south(i - 1);
        TANGOalgoOp(fcn,"Draw",x,y,  h, 0.0000);
	x += h;
        south(i - 1);
        TANGOalgoOp(fcn,"Draw",x,y,0.0000, -h);
	y -= h;
        east(i - 1);
        }
}



void west(i)
int i;
{
   if (i > 0)
   {    south(i -1);
        TANGOalgoOp(fcn,"Draw", x, y, h,0.0000);
	x += h;
        west(i - 1);
        TANGOalgoOp(fcn,"Draw",x,y,0.0000, h);
	y += h;
        west(i - 1);
        TANGOalgoOp(fcn,"Draw",x,y,-h,0.0000);
	x -= h;
        north(i - 1);
        }
}


