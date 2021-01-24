/*======================================================================*/
/*----------------------------------------------------------------------*/
/* line.c - Line Scan Conversion					*/
/* 	    An XTango animation of Bresenham's Midpoint Line Algorithm	*/
/* 	    by Anthony G. Francis, Jr.					*/
/*----------------------------------------------------------------------*/
/* References:	_Computer Graphics, Principles and Practice_, 2e,	*/
/*		Foley, et. al.						*/
/*----------------------------------------------------------------------*/
/* Algorithm File 							*/
/*----------------------------------------------------------------------*/
/*======================================================================*/

/*======================================================================*/
/* #include Files							*/
/*======================================================================*/
#include <stdio.h>
#include <math.h>
#include "xtango.h"

/*======================================================================*/
/* #define Statements							*/
/*======================================================================*/

/*======================================================================*/
/* XTango Declarations							*/
/*======================================================================*/
void
  ANIMInit(),
  ANIMVertex(),
  ANIMDrawLine(),
  ANIMDrawPoint(),
  ANIMGoEast(),
  ANIMGoNorthEast(),
  ANIMChangeDecision(),
  ANIMCleanup();

static NAME_FUNCT fcn[] = {
  {"Init",		1, {VOID,	(FPTR)ANIMInit}},
  {"Vertex",		1, {VOID,	(FPTR)ANIMVertex}},
  {"DrawLine",	 	1, {VOID,       (FPTR)ANIMDrawLine}},
  {"DrawPoint", 	1, {VOID,       (FPTR)ANIMDrawPoint}},
  {"GoEast", 		1, {VOID,       (FPTR)ANIMGoEast}},
  {"GoNorthEast", 	1, {VOID,       (FPTR)ANIMGoNorthEast}},
  {"DrawPoint", 	1, {VOID,       (FPTR)ANIMDrawPoint}},
  {"ChangeDecision", 	1, {VOID,       (FPTR)ANIMChangeDecision}},
  {"Cleanup",	 	1, {VOID,       (FPTR)ANIMCleanup}},
  {NULL,		NULL, NULL,	NULL} };

/*======================================================================*/
/* Main Function							*/
/*======================================================================*/
main()
{

/*-Data Structures------------------------------------------------------*/
  int	
    iEndpointX, iEndpointY,		/* Destination Points		*/
    iX, iY,				/* Stepping Variable		*/
    iDeltaX, iDeltaY,			/* Delta X and Delta Y		*/
    iDecide,				/* Decision Variable 		*/
    iIncrementE, iIncrementNE,		/* Increment Values		*/
    i,j,k;				/* Loop Variables		*/

  char *cpReply; 			/* Ptr for XTango return vals	*/

  /* Set up the XTango System */
  TANGOalgoOp(fcn, "BEGIN");
  TANGOalgoOp(fcn, "Init");

  /* Brag to the User */
  printf("Bresenham's Midpoint Line Scan-conversion Algorithm\n");
  printf("Adapted for XTango by Anthony G. Francis, Jr.\n");
 
  /*-Input the X and Y coordinates for the algorithm=-------------------*/
  printf ("Select the destination vertex with the mouse.\n");
  printf ("Please select a destination in the first octant.\n");

  iEndpointX = 0;
  iEndpointY = 0;

  /* Note we must stay in the first octant */
  while ((iEndpointX==0 && iEndpointY==0) || iEndpointX>20 ||
	   iEndpointY>iEndpointX) {
    /* Get user input from XTango */
    TANGOalgoOp(fcn, "Vertex", &iEndpointX,&iEndpointY);
  }
  
  
  /*--------------------------------------------------------------------*/
  /* Single Source Shortest Paths Algorithm				*/
  /*--------------------------------------------------------------------*/
  /*-Initialize Data Structures-----------------------------------------*/
  iDeltaX 	= iEndpointX;
  iDeltaY 	= iEndpointY;
  iDecide 	= 2 *  iDeltaY - iDeltaX;
  iIncrementE 	= 2 *  iDeltaY;
  iIncrementNE 	= 2 * (iDeltaY - iDeltaX);
  iX 		= 0;
  iY 		= 0;

  /* Draw the Target Line */
  printf ("Drawing the ideal line and setting up for scan conversion.\n");
  TANGOalgoOp (fcn, "DrawLine",iEndpointX,iEndpointY);

  /* Draw the First Point */
  printf ("Beginning scan conversion;  Drawing first pixel at (%d, %d)\n",
	  iX,iY);
  TANGOalgoOp (fcn, "DrawPoint",0,0);

   /* Loop through all the points on the line */
   while (iX < iEndpointX) {
     if (iDecide <= 0) {
       /* Choose East Direction */
       printf ("East direction chosen;      ");
       TANGOalgoOp (fcn, "GoEast");

       /* Update Point */
       iX++;

       /* Draw Pixel at the Point */
       printf ("Drawing pixel at (%d, %d).\n",iX,iY);
       TANGOalgoOp (fcn, "DrawPoint",iX,iY);

       /* Update Decision Variable */
       printf ("Updating the decision variable from %d to %d.\n",
	       iDecide, iDecide + iIncrementE);
       TANGOalgoOp (fcn, "ChangeDecision",iDecide, iDecide + iIncrementE, 0);
       iDecide += iIncrementE;

     } else {
       /* Choose Northeast Direction */
       printf ("Northeast direction chosen; ");
       TANGOalgoOp (fcn, "GoNorthEast");

       /* Update Point */
       iX++;
       iY++;

       /* Draw Pixel at the Point */
       printf ("Drawing pixel at (%d, %d).\n",iX,iY);
       TANGOalgoOp (fcn, "DrawPoint",iX,iY);

       /* Update Decision Variable */
       printf ("Updating the decision variable from %d to %d.\n",
	       iDecide, iDecide + iIncrementNE);
       TANGOalgoOp (fcn, "ChangeDecision",iDecide, iDecide + iIncrementNE, 1);
       iDecide += iIncrementNE;

     }

   }

   printf("The Line is Complete.\n");
   TANGOalgoOp(fcn, "Cleanup");

   /*-End the Program---------------------------------------------------*/
   TANGOalgoOp(fcn, "END");

}
