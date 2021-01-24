/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (athena)	      */
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

/**************************************************************/
/* NOTE:  To use a widget set other than Athena widgets, just */
/*	  replace this file with equivalent functions.	      */
/**************************************************************/

/**************************************************************/
/*****************	 include files       ******************/ 
/**************************************************************/

#include "xtangolocal.h"

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Simple.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/* Fake command-line arguments for XtInitialize() */
static int  argc = 1;
static char *argv[] = {"xtango"};

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

void debug_callback(),   delayset_callback();
void mode_callback(),    pause_callback();
void pattern_callback(), resize_handler();
void refresh_handler();

/**************************************************************/
/*****************       entry points        ******************/
/**************************************************************/

/**************************************************************/
/* TANGO_layout_window -- Create & realize all widgets.	      */
/*			  All widget resources are set in an  */
/*			  appropriate resource file.	      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TANGO_layout_window()
{
   Widget      toplevel, form, leftbox, wgt, modeMenu, debugMenu, patMenu;
   //   Display     *dpy;
   //   int	       scr;

   toplevel = XtInitialize(argv[0], "XTango", NULL, 0, &argc, argv);
   //   dpy = XtDisplay(toplevel);
   //   scr = DefaultScreen(dpy);

   form = XtCreateManagedWidget("form", formWidgetClass, toplevel, NULL, 0);

   XtCreateManagedWidget("title", labelWidgetClass, form, NULL, 0);

   leftbox = XtCreateManagedWidget("leftbox", boxWidgetClass, form, NULL, 0);

   wgt = XtCreateManagedWidget("left",  commandWidgetClass, leftbox, NULL, 0);
      XtAddCallback(wgt, XtNcallback, pan_callback, 0);
   wgt = XtCreateManagedWidget("right", commandWidgetClass, leftbox, NULL, 0);
   XtAddCallback(wgt, XtNcallback, pan_callback, (XtPointer)1);
   wgt = XtCreateManagedWidget("up",    commandWidgetClass, leftbox, NULL, 0);
      XtAddCallback(wgt, XtNcallback, pan_callback, (XtPointer)2);
   wgt = XtCreateManagedWidget("down",  commandWidgetClass, leftbox, NULL, 0);
      XtAddCallback(wgt, XtNcallback, pan_callback, (XtPointer)3);
   wgt = XtCreateManagedWidget("in",    commandWidgetClass, leftbox, NULL, 0);
      XtAddCallback(wgt, XtNcallback, zoom_callback, 0);
   wgt = XtCreateManagedWidget("out",   commandWidgetClass, leftbox, NULL, 0);
      XtAddCallback(wgt, XtNcallback, zoom_callback, (XtPointer)1);

   wgt = XtCreateManagedWidget("sbar", scrollbarWidgetClass, form, NULL, 0);
      XtAddCallback(wgt, XtNjumpProc, delayset_callback, NULL);
/*    XtAddCallback(wgt, XtNscrollProc, delayset_callback, NULL); */
 
   wgt = XtCreateManagedWidget("quit", commandWidgetClass, form, NULL, 0);
      XtAddCallback(wgt, XtNcallback, quit_callback, NULL);

   wgt = XtCreateManagedWidget("refresh", commandWidgetClass, form, NULL, 0);
      XtAddCallback(wgt, XtNcallback, refresh_callback, NULL);

   XtCreateManagedWidget("debug", menuButtonWidgetClass, form, NULL, 0);
   debugMenu = XtCreatePopupShell("debugMenu", simpleMenuWidgetClass, toplevel,
  				   NULL, 0);
   wgt = XtCreateManagedWidget("   off  ", smeBSBObjectClass, debugMenu,NULL,0);
      XtAddCallback(wgt, XtNcallback, debug_callback, 0);
   wgt = XtCreateManagedWidget("   on   ", smeBSBObjectClass, debugMenu,NULL,0);
      XtAddCallback(wgt, XtNcallback, debug_callback, (XtPointer)TANGO_DEBUG_EXTERNAL);

   XtCreateManagedWidget("mode", menuButtonWidgetClass, form, NULL, 0);
   modeMenu = XtCreatePopupShell("modeMenu", simpleMenuWidgetClass, toplevel,
  				 NULL, 0);
   wgt = XtCreateManagedWidget("by frame", smeBSBObjectClass, modeMenu, NULL,0);
      XtAddCallback(wgt, XtNcallback, mode_callback, (XtPointer)TANGO_MODE_SLOW);
   wgt = XtCreateManagedWidget("by scene", smeBSBObjectClass, modeMenu, NULL,0);
      XtAddCallback(wgt, XtNcallback, mode_callback, (XtPointer)TANGO_MODE_FAST);
   wgt = XtCreateManagedWidget("none", smeBSBObjectClass, modeMenu, NULL, 0);
      XtAddCallback(wgt, XtNcallback, mode_callback, (XtPointer)TANGO_MODE_DEFAULT);

   wgt = XtCreateManagedWidget("pause", commandWidgetClass, form, NULL, 0);
      XtAddCallback(wgt, XtNcallback, pause_callback, NULL);

/*    --- reset button doesn't make sense in this iteration of TANGO --- 
   XtCreateManagedWidget("reset", commandWidgetClass, form, NULL, 0);
 */

   if (!TANGO__data->color_screen) {
      XtCreateManagedWidget("patterns", menuButtonWidgetClass, form, NULL, 0);
      patMenu = XtCreatePopupShell("patternsMenu", simpleMenuWidgetClass,
				   toplevel, NULL, 0);
      wgt = XtCreateManagedWidget("colors", smeBSBObjectClass, patMenu, NULL,0);
         XtAddCallback(wgt, XtNcallback, pattern_callback, 0);
      wgt=XtCreateManagedWidget("fill styles",smeBSBObjectClass,patMenu,NULL,0);
         XtAddCallback(wgt, XtNcallback, pattern_callback, (XtPointer)1);
      }

   TANGO__data->easel = XtCreateManagedWidget("easel", simpleWidgetClass, 
                              form, NULL, 0);
      XtAddEventHandler(TANGO__data->easel, ButtonPressMask, FALSE, 
                          select_point, NULL);
      XtAddEventHandler(TANGO__data->easel, StructureNotifyMask, False, 
                          resize_handler,NULL);
      XtAddEventHandler(TANGO__data->easel, ExposureMask, False, 
                          refresh_handler,NULL);

   XtRealizeWidget(toplevel);
   }



/**************************************************************/
/* debug_callback -- Change debug mode.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
debug_callback(w, newDebug, call_data)
   Widget  w;
   int     newDebug;
   caddr_t call_data;
{
   TANGO__data->debug = newDebug;
}



/***************************************************************/
/* delayset_callback -- Adjust the delay setting according to  */
/*			the scrollbar position.		       */
/*							       */
/* RETURNS:  None.					       */
/***************************************************************/
void
delayset_callback(w, client_data, sliderpos)
   Widget  w;
   caddr_t client_data;
   XtPointer sliderpos;	/* 0.0 - 1.0 */
{
   TANGO__data->delay = 1000 * (int)(100.0 * *(float *)sliderpos);
}



/**************************************************************/
/* mode_callback -- Change refresh mode.		      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
mode_callback(w, newMode, call_data)
   Widget  w;
   int     newMode;
   caddr_t call_data;
{
}



/**************************************************************/
/* pattern_callback -- Change whether patterns represent      */
/*		       colors or fill styles for mono	      */
/*		       machines.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
pattern_callback(w, newPatMode, call_data)
   Widget  w;
   int     newPatMode;
   caddr_t call_data;
{
   TANGO__data->mono_fillstyle = newPatMode;
}



/**************************************************************/
/* pause_callback -- Pause/unpause animation and change       */
/*		     button label to reflect current state.   */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
pause_callback(w, client_data, call_data)
   Widget  w;
   caddr_t client_data, call_data;
{
   Arg args[2];

   TANGO__data->paused ^= 1;	/* Initially paused as "run animation" */
   XtSetArg(args[0], XtNlabel, (TANGO__data->paused ? "unpause" : "pause"));
   XtSetValues(w, args, 1);
}



/**************************************************************/
/* resize_handler -- Kludge to resize window under Athena set */
/*		     since Athena doesn't provide direct      */
/*		     call-back.				      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
resize_handler(w, client_data, xev)
   Widget  w;
   caddr_t client_data;
   XEvent  *xev;
{
   if (TANGO__data && xev->type == ConfigureNotify)
      resize_callback(w, client_data, NULL);
}




/**************************************************************/
/* refresh_handler -- Kludge to refresh window under Athena set */
/*		     since Athena doesn't provide direct      */
/*		     call-back.				      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
refresh_handler(w, client_data, xev)
   Widget  w;
   caddr_t client_data;
   XEvent  *xev;
{
   if (TANGO__data && xev->type == Expose)
      refresh_callback(w, client_data, NULL);
}

/**************************************************************/
/***************     end of xtangoathena.c     ****************/
/**************************************************************/
