/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (xt)		      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*		John T. Stasko and Doug Hayes		      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/11/10 JDH       Baselined source.			      */
/*							      */
/**************************************************************/
/* NOTE:   These are Xt functions and callbacks which do NOT  */
/*	   depend on the underlying widget set.		      */
/**************************************************************/

/**************************************************************/
/*****************	 include files       ******************/ 
/**************************************************************/

#include "xtangolocal.h"

#include <X11/cursorfont.h>

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

/**************************************************************/
/*****************       entry points        ******************/
/**************************************************************/

void pan_callback(), refresh_callback(), resize_callback();
void select_point(), quit_callback(),    zoom_callback();


void TANGOend();
int  TANGOinput_coord();
int  TANGOinput_image();

/**************************************************************/
/* TANGO_check_X_events -- Handle all pending X events.       */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TANGO_check_X_events()
{
   XEvent xev;

   while (XtPending() || TANGO__data->paused) {
      XtNextEvent(&xev);
      XtDispatchEvent(&xev);
      } 
}



/**************************************************************/
/* TANGOend -- Force user to select 'quit' button to exit.    */
/*	       (ie. allow all X events to be processed)       */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TANGOend()
{
   XEvent xev;

   XFlush(TANGO__data->display);
   while (1) {				/* User must press 'quit' button */
      XtNextEvent(&xev);
      XtDispatchEvent(&xev);
      }
}



/**************************************************************/
/* TANGOinput_coord -- Allow the user to select an arbitrary  */
/*		       window location.			      */
/*							      */
/* RETURNS:  TRUE(1) since a valid coordinate is always	      */
/*	     returned.					      */
/**************************************************************/
int
TANGOinput_coord(x,y)
   WIN_COORD *x,*y;
{
   XEvent event;
   //   int	  xpt, ypt;

   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOinput_coord(0x%p, 0x%p)\n", x, y);

   /* Wait for button press event handler to change "getMouse" */
   for (TANGO__data->getMouse = TRUE; TANGO__data->getMouse; ) {
      XtNextEvent(&event);
      XtDispatchEvent(&event);
      }

   *x = (double)TANGO__data->mousex / TANGO__data->x_WIN_COORD_to_int
	                                 + (double)TANGO__data->lx;

   *y = (double)TANGO__data->mousey / TANGO__data->y_WIN_COORD_to_int
	                                 + (double)TANGO__data->ty;

   return(1);
}



/**************************************************************/
/* TANGOinput_image -- Allow the user to select a position in */
/*		       the window, and return the uppermost   */
/*		       image object whose bounding box	      */
/*		       contains that position.		      */
/*							      */
/* RETURNS:  TRUE(1) or FALSE(0) based on whether or not an   */
/*	     image was found.				      */
/**************************************************************/
int
TANGOinput_image(im)
   TANGO_IMAGE *im;
{
  //   int	       xpt,ypt;
   WIN_COORD   x,y,lx,by,rx,ty;
   IMAGE_PTR   i;
   TANGO_IMAGE image;
   XEvent      event;

   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOinput_image(0x%p)\n", *im);

   /* Wait for button press event handler to change "getMouse" */
   for (TANGO__data->getMouse = TRUE; TANGO__data->getMouse; ) {
      XtNextEvent(&event);
      XtDispatchEvent(&event);
      }

   x = (double)TANGO__data->mousex / TANGO__data->x_WIN_COORD_to_int
                                         + (double)TANGO__data->lx;

   y = (double)TANGO__data->mousey / TANGO__data->y_WIN_COORD_to_int
                                         + (double)TANGO__data->ty;


   for (i = TANGO__data->confighead; i; i = i->nexti) {
      image = i->image;

      /* only want to check visible images */
      if (image->visible) {
	 TANGO_bounding_box(image,&lx,&by,&rx,&ty);

#define ERR_FACTOR 0.015
/* this is useful for horiz and vert lines */
	 if ((lx-ERR_FACTOR <= x) && (x <= rx+ERR_FACTOR) && 
             (ty-ERR_FACTOR <= y) && (y <= by+ERR_FACTOR))
	    break;
	 }
      }

   if (i) {
      *im = i->image;
      return(1);
      }

   *im = NULL;
   return(0);
}



/**************************************************************/
/* pan_callback -- Pan easel in specified direction.	      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
pan_callback(w, direction, call_data)
   Widget  w;
   int     direction;
   caddr_t call_data;
{
   double panfactor = 0.2,change;

   switch (direction) {
      case 0:	change = (TANGO__data->rx - TANGO__data->lx) * panfactor;
                TANGO__data->lx -= change;
         	TANGO__data->rx -= change;
         	break;
      case 1:	change = (TANGO__data->rx - TANGO__data->lx) * panfactor;
                TANGO__data->lx += change;
		TANGO__data->rx += change;
		break;
      case 2:	change = (TANGO__data->by - TANGO__data->ty) * panfactor;
                TANGO__data->ty -= change;
 		TANGO__data->by -= change;
		break;
      case 3:	change = (TANGO__data->by - TANGO__data->ty) * panfactor;
                TANGO__data->by += change;
		TANGO__data->ty += change;
		break;
      default: printf("???\n");
      }

   TANGO__data->x_WIN_COORD_to_int = ((WIN_COORD) TANGO__data->width) /
       (TANGO__data->rx - TANGO__data->lx);
   TANGO__data->y_WIN_COORD_to_int = ((WIN_COORD) TANGO__data->height) /
       (TANGO__data->by - TANGO__data->ty);

   TANGO_refresh();
}



/**************************************************************/
/* refresh_callback -- Redraw all images onscreen.	      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
refresh_callback(w, client_data, call_data)
   Widget  w;
   caddr_t client_data, call_data;
{
   if (TANGO__data->pixmap) /* Avoid resize when widget initially displayed */
			/* -- *before* TANGO__data->pixmap has been created. */
      TANGO_refresh();
}



/**************************************************************/
/* resize_callback -- Resize widgets.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
resize_callback(w, client_data, call_data)
   Widget  w;
   caddr_t client_data, call_data;
{
   if (TANGO__data->pixmap) {/* Avoid resize when widget initially displayed */
			/* -- *before* TANGO__data->pixmap has been created. */
      TANGO_anim_setup(w);
      TANGO_refresh();
      }
}



/**************************************************************/
/* quit_callback -- Exit TANGO.				      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
quit_callback(w, client_data, call_data)
   Widget  w;
   caddr_t client_data, call_data;
{
   exit(0);
}



/**************************************************************/
/* zoom_callback -- Zoom in/out.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
zoom_callback(w, direction, call_data)
   Widget  w;
   int     direction;
   caddr_t call_data;
{
#define  ZOOMFACTOR  0.8
   WIN_COORD change;

   switch (direction) {
      case 0:
         change = (TANGO__data->rx - TANGO__data->lx) * (1.0-ZOOMFACTOR);
         TANGO__data->rx -= change/2.0;
         TANGO__data->lx += change/2.0;
         change = (TANGO__data->by - TANGO__data->ty) * (1.0-ZOOMFACTOR);
         TANGO__data->by -= change/2.0;
         TANGO__data->ty += change/2.0;
         break;
      case 1:
         change = (TANGO__data->rx - TANGO__data->lx) * (1.0/ZOOMFACTOR - 1.0);
         TANGO__data->rx += change/2.0;
         TANGO__data->lx -= change/2.0;
         change = (TANGO__data->by - TANGO__data->ty) * (1.0/ZOOMFACTOR - 1.0);
         TANGO__data->by += change/2.0;
         TANGO__data->ty -= change/2.0;
         break;
      default:
         printf("???\n");
   }

   TANGO__data->x_WIN_COORD_to_int = ((WIN_COORD) TANGO__data->width) /
       (TANGO__data->rx - TANGO__data->lx);
   TANGO__data->y_WIN_COORD_to_int = ((WIN_COORD) TANGO__data->height) /
       (TANGO__data->by - TANGO__data->ty);

   TANGO_refresh();
}



/**************************************************************/
/* TANGOset_coord - allows the user to modify the window      */
/*   viewing coords.			                      */
/*							      */
/* RETURNS:  1 if valid coords entered, 0 otherwise.	      */
/**************************************************************/
int
TANGOset_coord(lx, by, rx, ty)
   double lx, by, rx, ty;
{
   if (lx >= rx) {
      printf("rx (%lf) > lx (%lf) passed to TANGOset_coord: illegal\n",
            rx,lx);
      return(0);
      }
   if (by <= ty) {
      printf("by (%lf) < ty (%lf) passed to TANGOset_coord: illegal\n",
            by,ty);
      return(0);
      }
   TANGO__data->lx = lx;
   TANGO__data->rx = rx;
   TANGO__data->by = by;
   TANGO__data->ty = ty;

   TANGO__data->x_WIN_COORD_to_int = ((WIN_COORD) TANGO__data->width) /
       (TANGO__data->rx - TANGO__data->lx);
   TANGO__data->y_WIN_COORD_to_int = ((WIN_COORD) TANGO__data->height) /
       (TANGO__data->by - TANGO__data->ty);

   TANGO_refresh();
   return(1);
}





/**************************************************************/
/* TANGOinq_coord - returns current values of window coords   */
/*							      */
/* RETURNS:  None.	                                      */
/**************************************************************/
void
TANGOinq_coord(lx, by, rx, ty)
   double *lx, *by, *rx, *ty;
{
   *lx = TANGO__data->lx;
   *rx = TANGO__data->rx;
   *by = TANGO__data->by;
   *ty = TANGO__data->ty;
}



/**************************************************************/
/* select_point -- (callback) Save user-selected coordinates. */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
select_point(w, notused, event)
   Widget w;
   void   *notused;
   XEvent *event;
{
   TANGO__data->getMouse = FALSE;
   TANGO__data->mousex = event->xbutton.x;
   TANGO__data->mousey = event->xbutton.y;
}

/**************************************************************/
/***************       end of xtangoxt.c       ****************/
/**************************************************************/
