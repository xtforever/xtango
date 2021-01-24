/*======================================================================*/
/*----------------------------------------------------------------------*/
/* linescenes.c - Line Scan Conversion					*/
/* 	    An XTango animation of Bresenham's Midpoint Line Algorithm	*/
/* 	    by Anthony G. Francis, Jr.					*/
/*----------------------------------------------------------------------*/
/* References:	_Computer Graphics, Principles and Practice_, 2e,	*/
/*		Foley, et. al.						*/
/*----------------------------------------------------------------------*/
/* Animations File							*/
/*----------------------------------------------------------------------*/
/*======================================================================*/

/*======================================================================*/
/* #include Files                                                       */
/*======================================================================*/
#include <stdio.h>
#include <math.h>
#include "xtango.h"


/*======================================================================*/
/* #define Statements                                                   */
/*======================================================================*/
#define   SIZE	   0.04
#define   FCOLOR   TANGO_COLOR_RED
#define   TCOLOR   TANGO_COLOR_BLUE
#define   XCOLOR   TANGO_COLOR_YELLOW


/*======================================================================*/
/* XTango Declarations                                                  */
/*======================================================================*/
TANGO_IMAGE  tiCanvas;
TANGO_IMAGE  tiDeltaX;
TANGO_IMAGE  tiDeltaY;
TANGO_IMAGE  tiDecide;
TANGO_IMAGE  tiIncrE;
TANGO_IMAGE  tiIncrNE;
TANGO_IMAGE  tiLineE;
TANGO_IMAGE  tiLineNE;
TANGO_IMAGE  tiTheLine;
TANGO_IMAGE  tiALine;

/*======================================================================*/
/* ANIMInit								*/
/* 	create location and adj matrix associations; display screen	*/
/*======================================================================*/
void
ANIMInit()
{
  TANGO_IMAGE  tiTemp;
  int i,j;

  /*-Make the associations for further use------------------------------*/
  ASSOCinit();
  ASSOCmake("VERTEX",2);
  ASSOCmake("XCOORD",1);
  ASSOCmake("YCOORD",1);

  /*-Create the Decision Variable Box-----------------------------------*/
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.10, 0.04, 1,
		    TANGO_COLOR_BLACK, NULL, "DECISION", 1);
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.04, 0.43, 1,
		    TANGO_COLOR_BLACK, NULL, "-", 1);
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.04, 0.39, 1,
		    TANGO_COLOR_BLACK, NULL, "+", 1);
  TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.02, 0.02, 1,
		    TANGO_COLOR_BLACK, 0.16, 0.76, 0.0);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.02, 0.40, 1,
		    TANGO_COLOR_BLACK, 0.16, 0.0, 0.0, 1.0, 0);

  /*-Create the Drawing Direction Box-----------------------------------*/
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.10, 0.81, 1,
		    TANGO_COLOR_BLACK, NULL, "DIRECTION", 1);
  TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.02, 0.82, 1,
		    TANGO_COLOR_BLACK, 0.16, 0.16, 0.0);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.06, 0.82, 1,
		    TANGO_COLOR_BLUE, 0.0, 0.16, 0.0, 1.0, 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.14, 0.82, 1,
		    TANGO_COLOR_BLUE, 0.0, 0.16, 0.0, 1.0, 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.02, 0.86, 1,
		    TANGO_COLOR_BLUE, 0.16,  0.0, 0.0, 1.0, 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.02, 0.94, 1,
		    TANGO_COLOR_BLUE, 0.16,  0.0, 0.0, 1.0, 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.12, 0.90, 1,
		    TANGO_COLOR_BLACK, 0.04,  0.0, 1.0, 1.0, 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
		    0.14, 0.94, 1,TANGO_COLOR_BLACK,.02,0.0);
  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
		    0.14, 0.86, 1,TANGO_COLOR_BLACK,.02,0.0);

  /*-Create the Drawing Direction Arrows--------------------------------*/
  tiLineE  = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.06, 0.94, 1,
			       TANGO_COLOR_RED, 0.08, 0.0, 0.5, 1.0, 1);
  tiLineNE = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.06, 0.94, 1,
			       TANGO_COLOR_RED, 0.08, -0.08, 0.5, 1.0, 1);
  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
		    0.06, 0.94, 1,TANGO_COLOR_BLACK,.02,1.0);


  /*-Create the Screen Canvas Box---------------------------------------*/
  tiCanvas = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.22, 0.02, 1,
			   TANGO_COLOR_BLUE, 0.76, 0.76, 1);

  /*-Create the Gridlines and Points------------------------------------*/
  for (i=0; i<=20; i++) {
    for (j=0; j<=20; j++) {
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.24+i*0.035, 0.76, 1,
			TANGO_COLOR_BLUE, 0.0, -0.72, 0.0,1.0,0);
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.24, 0.76-j*0.035, 1,
			TANGO_COLOR_BLUE, 0.72, 0.0, 0.0,1.0,0);
    }
  }

  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.24, 0.78, 1,
		    TANGO_COLOR_RED, 0.0, -0.76, 0.5, 1.0, 1);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.22, 0.76, 1,
		    TANGO_COLOR_RED, 0.76, 0.0, 0.5, 1.0, 1);
  TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.22, 0.78, 1,
		    TANGO_COLOR_RED, 0.76, -0.76, 0.0, 1.0, 0);

  for (i=0; i<=20; i++) {
    for (j=0; j<=20; j++) {
      tiTemp = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
				 0.24+i*0.035, 0.76-j*0.035, 1,
				 TANGO_COLOR_YELLOW,.01,1.0);
      ASSOCstore("VERTEX",i,j,tiTemp);
      ASSOCstore("XCOORD",tiTemp,i);
      ASSOCstore("YCOORD",tiTemp,j);
    }
  }

  /*-Create the Screen Canvas Text Box----------------------------------*/
  TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.26, 0.02, 1,
		    TANGO_COLOR_BLACK, 0.30, 0.04, 1.0);
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.27, 0.05, 1,
		    TANGO_COLOR_WHITE, NULL, "Screen Canvas", 0);

  /*-Create the Algorithm Variables Text Boxes--------------------------*/
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.25, 0.81, 1,
		    TANGO_COLOR_BLACK, NULL, "Northeast Increment", 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.25, 0.91, 1,
		    TANGO_COLOR_BLACK, NULL, "East Increment", 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.75, 0.81, 1,
		    TANGO_COLOR_BLACK, NULL, "Delta X", 0);
  TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.75, 0.91, 1,
		    TANGO_COLOR_BLACK, NULL, "Delta Y", 0);


  /*-Display all the structures used so far-----------------------------*/
  TANGOtrans_perform 
    (TANGOtrans_create
     (TANGO_TRANS_TYPE_REFRESH,tiCanvas,TANGOpath_null(1)));
}




/*======================================================================*/
/* ANIMVertex								*/
/* 	get the vetex corresponding to a user pick, using Tango's 	*/
/*	association function.						*/
/*======================================================================*/
void
ANIMVertex(ipX,ipY)
   int *ipX, *ipY;
{
   TANGO_IMAGE	 tiVertex;

   if (TANGOinput_image(&tiVertex))
     if (ASSOCmember ("XCOORD",tiVertex) && 
	 ASSOCmember ("YCOORD",tiVertex)) {
       *ipX = (int) ASSOCretrieve ("XCOORD",tiVertex);
       *ipY = (int) ASSOCretrieve ("YCOORD",tiVertex);
     }

 }



/*----------------------------------------------------------------------*/
/* ANIMDrawLine								*/
/* 	Draw the ideal line (and all of the setup variables for the	*/
/*	scan-converted line).						*/
/*----------------------------------------------------------------------*/
void
ANIMDrawLine(x,y)
     int x,y;
{
  /*-Setup Temporary Variables------------------------------------------*/
  TANGO_IMAGE tiTempX, tiTempY0, tiTempY1, tiTempY2;
  TANGO_PATH  tpTempX, tpTempY, tpSizeX,  tpSizeY;
  TANGO_TRANS ttMove, ttMoveX, ttMoveY, ttSizeX, ttSizeY;

  /*-Draw the Ideal Line (for Comparison)-------------------------------*/
  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.24, 0.76, 1,
		    TANGO_COLOR_BLUE, 0.02, 0.0);
  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.24+x*0.035, 0.76-y*0.035, 1,
		    TANGO_COLOR_BLUE, 0.02, 0.0);
  tiTheLine =TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.24, 0.76, 1,
			       TANGO_COLOR_BLUE, x*0.035, -y*0.035, 
			       0.5, 1.0, 0);
  TAPshow(tiTheLine);

  /*-Build Delta X and Delta Y------------------------------------------*/
  tiDeltaX =  TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 
				0.24, 0.76, 1,
				TANGO_COLOR_BLUE, x*0.035,.02, 1.0);
  tiDeltaY =  TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 
				0.24, 0.76-y*0.035, 1,
				TANGO_COLOR_BLUE, 0.02, y*0.035, 1.0);
  TANGOtrans_perform
    (TANGOtrans_compose
     (2, TAPflash(tiDeltaX,8), TAPflash(tiDeltaY,8)));
      

  /*-Move Delta X and Delta Y to Status Bar Area------------------------*/
  tpTempX  = TANGOpath_motion (TANGOloc_create (0.24, 0.76),
			       TANGOloc_create (0.75, 0.83),
			       TANGO_PATH_TYPE_STRAIGHT);
  tpTempY  = TANGOpath_motion (TANGOloc_create (0.24, 0.76),
			       TANGOloc_create (0.75, 0.93+y*0.035),
			       TANGO_PATH_TYPE_STRAIGHT);
  tpSizeX  = TANGOpath_motion (TANGOloc_create (x*0.035, 0.02),
			       TANGOloc_create (x*0.01, 0.02),
			       TANGO_PATH_TYPE_STRAIGHT);
  tpSizeY  = TANGOpath_motion (TANGOloc_create (0.02, y*0.035),
			       TANGOloc_create (y*0.01, 0.02),
			       TANGO_PATH_TYPE_STRAIGHT);

  TANGOtrans_perform
    (TANGOtrans_compose 
     (4,
      TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, tiDeltaX, tpTempX),
      TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, tiDeltaY, tpTempY),
      TANGOtrans_create (TANGO_TRANS_TYPE_RESIZE, tiDeltaX, tpSizeX),
      TANGOtrans_create (TANGO_TRANS_TYPE_RESIZE, tiDeltaY, tpSizeY)));


  /*-Build the Increments Visually--------------------------------------*/
  tiIncrE  = TANGOimage_copy (tiDeltaY);
  tiIncrNE = TANGOimage_copy (tiDeltaX);

  tiTempX  = TANGOimage_copy (tiDeltaX);
  tiTempY0 = TANGOimage_copy (tiDeltaY);
  tiTempY1 = TANGOimage_copy (tiDeltaY);
  tiTempY2 = TANGOimage_copy (tiDeltaY);

  tpTempX  = TANGOpath_motion (TANGOloc_create (0.75, 0.83),
			       TANGOloc_create (0.25, 0.83),
			       TANGO_PATH_TYPE_COUNTERCLOCKWISE);
  tpTempY  = TANGOpath_motion (TANGOloc_create (0.75, 0.93),
			       TANGOloc_create (0.25, 0.93),
			       TANGO_PATH_TYPE_COUNTERCLOCKWISE);

  TANGOtrans_perform
    (TANGOtrans_concatenate
     (6,
      TANGOtrans_compose
      (2,
       TANGOtrans_concatenate
       (2,
	TANGOtrans_create (TANGO_TRANS_TYPE_MOVE,tiIncrE, tpTempY),
	TANGOtrans_create (TANGO_TRANS_TYPE_MOVE,tiTempY0,tpTempY)),

       TANGOtrans_concatenate
       (2,
	TANGOtrans_compose
	(3,
	 TAPcolor (tiIncrNE, TANGO_COLOR_RED),
	 TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, tiIncrNE, tpTempY),
	 TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, tiTempY1, 
			    TANGOpath_motion
			    (TANGOloc_create (0.75, 0.93),
			     TANGOloc_create (0.25, 0.83),
			     TANGO_PATH_TYPE_COUNTERCLOCKWISE))),
	TANGOtrans_compose
	(3, 
	 TAPcolor (tiTempX, TANGO_COLOR_RED),
	 TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, tiTempX, tpTempY),
	 TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, tiTempY2, 
			    TANGOpath_motion
			    (TANGOloc_create (0.75, 0.93),
			     TANGOloc_create (0.25, 0.83),
			     TANGO_PATH_TYPE_COUNTERCLOCKWISE))))),
      
      TANGOtrans_create (TANGO_TRANS_TYPE_VISIBLE, tiTempX,
			 TANGOpath_null(1)),
      TANGOtrans_create (TANGO_TRANS_TYPE_VISIBLE, tiTempY1, 
			 TANGOpath_null(1)),
      TANGOtrans_create (TANGO_TRANS_TYPE_VISIBLE, tiTempY2, 
			 TANGOpath_null(1)),
      TANGOtrans_create (TANGO_TRANS_TYPE_VISIBLE, tiTempY0, 
			 TANGOpath_null(1)),
      
      TANGOtrans_compose
      (2,
       TANGOtrans_create (TANGO_TRANS_TYPE_RESIZE, tiIncrE, 
			  TANGOpath_motion (TANGOloc_create (y*0.01, 0.02),
					    TANGOloc_create (y*0.02, 0.02),
					    TANGO_PATH_TYPE_STRAIGHT)),
       TANGOtrans_create (TANGO_TRANS_TYPE_RESIZE, tiIncrNE, 
			  TANGOpath_motion (TANGOloc_create (x*0.01, 0.02),
					    TANGOloc_create ((x-y)*0.02, 0.02),
					    TANGO_PATH_TYPE_STRAIGHT)))));

  /*-Create the Decision Variable Line--------------------------*/
  tiDecide = TANGOimage_create (TANGO_IMAGE_TYPE_LINE, 0.10, 0.40, 1,
			        TANGO_COLOR_BLUE, 0.0, -(2.0*y-x)*0.01, 
				1.0, 1.0, 1);
  TAPshow (tiDecide);
}


/*----------------------------------------------------------------------*/
/* ANIMDrawPoint							*/
/* 	Sets a point on the "screen canvas" to black			*/
/*----------------------------------------------------------------------*/
void
ANIMDrawPoint(x,y)
     int x,y;
{
  TANGOtrans_perform
    ( TAPcolor
     ( (TANGO_IMAGE) ASSOCretrieve("VERTEX", x, y), TANGO_COLOR_BLACK ) );
}



/*----------------------------------------------------------------------*/
/* ANIMGoEast								*/
/* 	Changes the highlighting of the current direction arrow		*/
/*----------------------------------------------------------------------*/
void ANIMGoEast()
{
  TANGOtrans_perform
    ( TANGOtrans_compose
     ( 2, 
       TAPcolor (tiLineE, TANGO_COLOR_BLACK),
       TAPcolor (tiLineNE,TANGO_COLOR_YELLOW)));
}

/*----------------------------------------------------------------------*/
/* ANIMGoNorthEast							*/
/* 	Changes the highlighting of the current direction arrow		*/
/*----------------------------------------------------------------------*/
void ANIMGoNorthEast()
{
  TANGOtrans_perform
    ( TANGOtrans_compose
     ( 2, 
       TAPcolor (tiLineE, TANGO_COLOR_YELLOW),
       TAPcolor (tiLineNE,TANGO_COLOR_BLACK)));
}


/*----------------------------------------------------------------------*/
/* ANIMChangeDecision							*/
/* 	Changes the value of the decision pointer, graphically		*/
/*	illustrating where the change values are.			*/
/*----------------------------------------------------------------------*/
void ANIMChangeDecision(old,new,direction)
     int old,new,direction;
{
  TANGO_IMAGE tiTemp;

  if (direction==1) {
    /* Update the Decision Variable for the Northeast */
    tiTemp = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.14, 0.86, 1,
			       TANGO_COLOR_RED, .02, 1.0);
    TANGOtrans_perform
      (TANGOtrans_concatenate
       (3,
	TANGOtrans_create
	(TANGO_TRANS_TYPE_MOVE, tiTemp,
	 TANGOpath_distance
	 (TANGOloc_create (0.14, 0.86),
	  TANGOloc_create (0.25, 0.83),
	  0.05)),
	TANGOtrans_create
	(TANGO_TRANS_TYPE_MOVE, tiTemp,
	 TANGOpath_distance
	 (TANGOloc_create (0.25, 0.83),
	  TANGOloc_create (0.10, 0.40),
	  0.05)),
	TANGOtrans_create
	(TANGO_TRANS_TYPE_DELETE, tiTemp, TANGOpath_null(1))));
  } else {
    /* Update the Decision Variable for the Northeast */
    tiTemp = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.14, 0.94, 1,
				TANGO_COLOR_BLUE, .02, 1.0);
    TANGOtrans_perform
      (TANGOtrans_concatenate
       (3,
	 TANGOtrans_create
	(TANGO_TRANS_TYPE_MOVE, tiTemp,
	 TANGOpath_distance
	 (TANGOloc_create (0.14, 0.94),
	  TANGOloc_create (0.25, 0.93),
	  0.05)),
	TANGOtrans_create
	(TANGO_TRANS_TYPE_MOVE, tiTemp,
	 TANGOpath_distance
	 (TANGOloc_create (0.25, 0.93),
	  TANGOloc_create (0.10, 0.40),
	  0.05)),
	TANGOtrans_create
	(TANGO_TRANS_TYPE_DELETE, tiTemp, TANGOpath_null(1))));
  }

  /* Adjust the size of the line */
  TANGOtrans_perform
    (TANGOtrans_create 
     (TANGO_TRANS_TYPE_RESIZE, 
      tiDecide, 
      TANGOpath_distance (TANGOloc_create (0.0,0),
			TANGOloc_create (0.0,-(new-old)*0.01),
			0.05)));

  /* Adjust the color; blue for positive, red for negative */
  if (new <0) {
    TANGOtrans_perform (TAPcolor(tiDecide, TANGO_COLOR_RED));
  } else {
    TANGOtrans_perform (TAPcolor(tiDecide, TANGO_COLOR_BLUE));
  }
      
}

/*----------------------------------------------------------------------*/
/* ANIMCleanup								*/
/*	Flash the ideal line when done					*/
/*----------------------------------------------------------------------*/
void
ANIMCleanup()
{
  TANGOtrans_perform (TAPflash (tiTheLine,8));
}

