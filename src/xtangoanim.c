/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (anim)		      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*		John T. Stasko and Doug Hayes		      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/10/26 JDH	      Baselined source.                       */
/*							      */
/**************************************************************/

/**************************************************************/
/*****************	 include files       ******************/ 
/**************************************************************/

#include "xtangolocal.h"

#include <X11/StringDefs.h>

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

/******************************************************************/
/* TANGO_anim_setup -- Create Pixmaps to hold animation frame(s). */
/*		       Whenever the frame dimensions are changed  */
/*		       (eg. by resizing the window) call this     */
/*		       function to create new Pixmaps and update  */
/*		       global TANGO__data information.		  */
/*								  */
/* RETURNS:  NULL.                                         	  */
/******************************************************************/
void
TANGO_anim_setup(widgetid)
   Widget widgetid;
{
   int	     old;
   Arg	     wargs[2];
   Dimension w ,h;
   Display  *_display;

   _display = TANGO__data->display;

	/* Each frame gets copied here for viewing */
   TANGO__data->display_window = XtWindow(widgetid);

   /* Create offscreen frame(s)...Pixmap(s) */

   if (TANGO__data->pixmap) XFreePixmap(_display, TANGO__data->pixmap);	
                       /* Resizing old frame? */

   XtSetArg(wargs[0], XtNwidth, &w);
   XtSetArg(wargs[1], XtNheight, &h);
   XtGetValues(TANGO__data->easel, wargs, 2);

   TANGO__data->width = w;	/* Update global data */
   TANGO__data->height = h;

   TANGO__data->x_WIN_COORD_to_int = ((WIN_COORD) w) 
                                       / (TANGO__data->rx - TANGO__data->lx);
   TANGO__data->y_WIN_COORD_to_int = ((WIN_COORD) h) 
                                       / (TANGO__data->by - TANGO__data->ty);

   TANGO__data->pixmap = XCreatePixmap(_display, 
                             DefaultRootWindow(_display), 
                             (unsigned int) w, (unsigned int) h,
  	         	     DefaultDepth(_display, DefaultScreen(_display)));
   
   /* Pixmaps always start in an unknown state...clear it */

   old = TANGO_color(TANGO__data->bgcolor);
   XSetClipMask(_display, TANGO_gc(), None);
   XFillRectangle(_display, TANGO__data->pixmap, TANGO_gc(), 0, 0, 
                      (unsigned int) w, (unsigned int) h);
   TANGO_color(old);
}



/*******************************************************************/
/* TANGO_anim_next_damage_frame -- Display next animation frame by */
/*			    PixBlting offscreen Pixmap to onscreen */
/*			    window.				   */
/*								   */
/* RETURNS:  NULL                                       	   */
/*******************************************************************/
void
TANGO_anim_next_damage_frame()
{
   XCopyArea(TANGO__data->display, TANGO__data->pixmap, 
             TANGO__data->display_window, TANGO_gc(),  
	     TANGO__data->damage_x, TANGO__data->damage_y,
	     TANGO__data->damage_width, TANGO__data->damage_height,
	     TANGO__data->damage_x, TANGO__data->damage_y);
}

/**************************************************************/
/*****************    end of xtangoanim.c    ******************/
/**************************************************************/
