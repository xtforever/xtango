/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (window)	      */
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

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

void animate_begin();


/**************************************************************/
/* TANGOset_bgcolor -- Sets the background color of the       */
/*                        XTango window                       */
/* 							      */
/* RETURNS:  None.                                            */
/**************************************************************/
void 
TANGOset_bgcolor (char *name)
{
   TANGO_COLOR col;

   col = TANGOload_color(name);
   TANGO__data->bgcolor = col;
   TANGO_refresh();
}
   

/**************************************************************/
/* TANGO_setup_windows -- Open X display, decide whether      */
/*			  color or mono screen, and init all  */
/*			  X stuff.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_setup_windows (void)
{
   Visual     *vis;
//   int        lx,by,rx,ty;

   TANGO__data = (ANIMATION_PTR) malloc( sizeof( struct ANIMATION) );

   TANGO__data->display = XOpenDisplay("");
   if (!TANGO__data->display) {
      COMPLAIN("ERROR: TANGO_setup_windows: unable to open X display\n");
      exit(1);
      }

   vis = DefaultVisual(TANGO__data->display, 
                       DefaultScreen(TANGO__data->display));
   TANGO__data->color_screen = (vis->class != GrayScale && 
                                vis->class != StaticGray);

   TANGO__data->pixmap = 0;  /* needed to short-circuit initial attempt */
                                /* at refreshing window                    */
   TANGO__data->debug = 0;

   TANGO_initGC();

   TANGO_layout_window();

   animate_begin();
}



/**************************************************************/
/* TANGO_flush -- Send any queued X events to the server.     */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_flush (void)
{
   XFlush(TANGO__data->display);
}



/**************************************************************/
/* TANGO_batch_mode -- Explicitly flush or hold X events      */
/*		       (well, almost...)		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_batch_mode (int fg)
{
   static int batch_level = 0;

   if (fg)
      ++batch_level;
   else if (batch_level > 0) {
      --batch_level;
      if (batch_level == 0)
	 XFlush(TANGO__data->display);
      }
}



/**************************************************************/
/* animate_begin -- This routine is used to set up the TANGO  */
/*		    animation window and package.  The	      */
/*		    routine receives the animation window,    */
/*		    its parent window, plus the user-         */
/*		    specified, real-valued window boundary    */
/*		    coords.  The routine initializes all the  */
/*		    fields of the main, GLOBAL animation data */
/*		    structure, TANGO__data.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
animate_begin (void)
{
   TANGO__data->bgcolor = TANGO_COLOR_WHITE;
   TANGO__data->width = 0;	      /* Width & height init'd in anim_setup */
   TANGO__data->height = 0;	      /*    (user may resize window)         */
   TANGO__data->damlx = TANGO__data->rx = 1.0;
   TANGO__data->damrx = TANGO__data->lx = 0.0;
   TANGO__data->damty = TANGO__data->by = 1.0;
   TANGO__data->damby = TANGO__data->ty = 0.0;
   TANGO__data->mono_fillstyle = 0;    /* Patterns represent colors */
   TANGO__data->delay = 100;

   /* Start paused--don't lose initial x stuff */
   /*    (before window/widgets are displayed) */
   TANGO__data->paused = 1;	
   TANGO__data->motionblur = 0;
   TANGO__data->confighead = NULL;
   TANGO__data->configtail = NULL;

   TANGO_anim_setup(TANGO__data->easel);

   /* Wait until widgets are displayed */
   TANGO_flush();
   TANGO_check_X_events();
}

/**************************************************************/
/*****************   end of xtangowindow.c   ******************/
/**************************************************************/
