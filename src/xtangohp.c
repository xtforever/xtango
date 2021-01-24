/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (hp)		      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*		John T. Stasko and Doug Hayes		      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/10/26 JDH       Baselined source.			      */
/* 90/11/09 JDH       Converted hard-coded resources to file. */
/*							      */
/**************************************************************/

/**************************************************************/
/* NOTE:  To use a widget set other than HP X widgets, just   */
/*	  replace this file with equivalent functions.	      */
/**************************************************************/

/**************************************************************/
/*****************	 include files       ******************/ 
/**************************************************************/

#include "xtangolocal.h"

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>

#include <X11/Xw/Xw.h>
#include <X11/Xw/Form.h>
#include <X11/Xw/SText.h>
#include <X11/Xw/ScrollBar.h>
#include <X11/Xw/RCManager.h>
#include <X11/Xw/PButton.h>
#include <X11/Xw/Arrow.h>
#include <X11/Xw/WorkSpace.h>
#include <X11/Xw/MenuBtn.h>
#include <X11/Xw/PopupMgr.h>
#include <X11/Xw/Cascade.h>

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/* Fake command-line arguments for XtInitialize() */
static int  argc = 1;
static char *argv[] = {"xtango"};

static Widget lastModeW = 0;    /* Keep track of Mode menu's checkmark    */
static Widget lastDebugW = 0;   /* Keep track of Debug menu's checkmark   */
static Widget lastPatternW = 0;	/* Keep track of Pattern menu's checkmark */

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

void delayset_callback(),    pause_callback();
void changeDebug_callback(), changeMode_callback(), patternsMode_callback();
void directSlider_callback();

/**************************************************************/
/*****************       entry points        ******************/
/**************************************************************/

/*----------------------------------------------------------------------------*/
/*                      XTANGO Widgets -- HP X Version                        */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                              toplevel                                      */
/*                                 |                                          */
/*                                form                                        */
/*        /---------------/--------+---\-------\-----\-----\------\-----\     */
/*      title           rcleft         sbar   pause mode debug refresh quit   */
/*                /----/---\---\---\--\                                       */
/*              left right up down in out				      */
/*                                                                            */
/*                                                                            */
/*		modeMgrShell			debugMgrShell		      */
/*		      |				      |			      */
/*		modeMenuMgr			debugMenuMgr		      */
/*		      |				      |			      */
/*		 modeShell			 debugShell		      */
/*		      |				      |			      */
/*		  Refresh			    Debug		      */
/*	    /---------|-------\			    /-+-\		      */
/*      *by frame  by scene  none		  *Off  On		      */
/*----------------------------------------------------------------------------*/

/**************************************************************/
/* TANGO_layout_window -- Create, init (hardcoded), place, &  */
/*			  realize all widgets.		      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TANGO_layout_window()
{
   Widget      toplevel, form, rcleft, wgt, mode, debug;
   Widget      mgrShell, menuMgr, paneShell, menuPane, mbutton;    /* Menus */
   Display     *dpy;
   int         scr;
   Arg	       args[5];
   int         n;

   toplevel = XtInitialize(argv[0], "XTango", NULL, 0, &argc, argv);
      dpy = XtDisplay(toplevel);
      scr = DefaultScreen(dpy);

   form = XtCreateManagedWidget("form", XwformWidgetClass, toplevel, NULL, 0);

   XtCreateManagedWidget("title", XwstatictextWidgetClass, form, NULL, 0);

   rcleft = XtCreateManagedWidget("rcleft", XwrowColWidgetClass, form, NULL, 0);

   wgt = XtCreateManagedWidget("left", XwarrowWidgetClass, rcleft, NULL, 0);
      XtAddCallback(wgt, XtNselect, pan_callback, 0);

   wgt = XtCreateManagedWidget("right", XwarrowWidgetClass, rcleft, NULL, 0);
      XtAddCallback(wgt, XtNselect, pan_callback, 1);

   wgt = XtCreateManagedWidget("up", XwarrowWidgetClass, rcleft, NULL, 0);
      XtAddCallback(wgt, XtNselect, pan_callback, 2);

   wgt = XtCreateManagedWidget("down", XwarrowWidgetClass, rcleft, NULL, 0);
      XtAddCallback(wgt, XtNselect, pan_callback, 3);

   wgt = XtCreateManagedWidget("in", XwpushButtonWidgetClass, rcleft, NULL, 0);
      XtAddCallback(wgt, XtNselect, zoom_callback, 0);

   wgt = XtCreateManagedWidget("out", XwpushButtonWidgetClass, rcleft, NULL, 0);
      XtAddCallback(wgt, XtNselect, zoom_callback, 1);

   wgt = XtCreateManagedWidget("sbar", XwscrollbarWidgetClass, form, NULL, 0);
      XtAddCallback(wgt, XtNsliderMoved, delayset_callback, NULL);
      XtAddCallback(wgt, XtNareaSelected, directSlider_callback, NULL);

   wgt = XtCreateManagedWidget("pause", XwpushButtonWidgetClass, form, NULL, 0);
      XtAddCallback(wgt, XtNselect, pause_callback, NULL);

   mode=XtCreateManagedWidget("mode", XwpushButtonWidgetClass, form, NULL, 0);

   debug=XtCreateManagedWidget("debug", XwpushButtonWidgetClass, form, NULL, 0);

   wgt = XtCreateManagedWidget("refresh", XwpushButtonWidgetClass, form,NULL,0);
      XtAddCallback(wgt, XtNselect, refresh_callback, NULL);
/*    --- reset button doesn't make sense in this iteration of TANGO ---
   XtCreateManagedWidget("reset", XwpushButtonWidgetClass, form, NULL, 0);
*/

   wgt = XtCreateManagedWidget("quit", XwpushButtonWidgetClass, form, NULL, 0);
      XtAddCallback(wgt, XtNselect, quit_callback, NULL);

   /* Patterns menu to select whethers patterns represent colors */
   /*    or fill-styles (only visible on mono machines).         */
   if (!TANGO__data->color_screen) {
      wgt=XtCreateManagedWidget("patterns", XwpushButtonWidgetClass, form,
				NULL,0);
      /* "Patterns" menu */
      mgrShell = XtCreatePopupShell("patternsMgrShell", shellWidgetClass, wgt,
                                    NULL, 0);
      menuMgr = XtCreateManagedWidget("patternsMenuMgr", XwpopupmgrWidgetClass,
                                      mgrShell, NULL, 0);
      paneShell = XtCreatePopupShell("patternsShell", shellWidgetClass, menuMgr,
                                     NULL, 0);
         n = 0;
         XtSetArg(args[n], XtNattachTo, "patternsMenuMgr"); n++;
      menuPane = XtCreateManagedWidget("Represent", XwcascadeWidgetClass,
                                       paneShell, args, n);
         n = 0;
         XtSetArg(args[n], XtNsetMark, True); n++;
      wgt = XtCreateManagedWidget("Colors", XwmenuButtonWidgetClass,
                                  menuPane, args, n);
         XtAddCallback(wgt, XtNselect, patternsMode_callback, 0);
         lastPatternW = wgt;		/* Current checkmarked widget in menu */
      wgt = XtCreateManagedWidget("Fill Styles", XwmenuButtonWidgetClass,
                                  menuPane, NULL, 0);
         XtAddCallback(wgt, XtNselect, patternsMode_callback, 1);
      }

   TANGO__data->easel = XtCreateManagedWidget("easel", 
                            XwworkSpaceWidgetClass, form, NULL,0);
      XtAddCallback(TANGO__data->easel, XtNresize, resize_callback, NULL);
      XtAddCallback(TANGO__data->easel, XtNexpose, refresh_callback, NULL);
      XtAddEventHandler(TANGO__data->easel, ButtonPressMask, FALSE, 
                         select_point, NULL);

   /* "Mode" menu */
   mgrShell = XtCreatePopupShell("modeMgrShell", shellWidgetClass, mode,
                                 NULL, 0);
   menuMgr = XtCreateManagedWidget("modeMenuMgr", XwpopupmgrWidgetClass,
                                   mgrShell, NULL, 0);
   paneShell = XtCreatePopupShell("modeShell", shellWidgetClass, menuMgr,
                                  NULL, 0);
      n = 0;
      XtSetArg(args[n], XtNattachTo,    "modeMenuMgr"); n++;
   menuPane = XtCreateManagedWidget("Refresh", XwcascadeWidgetClass,
                                    paneShell, args, n);
      n = 0;
      XtSetArg(args[n], XtNsetMark,     True); n++;
   wgt = XtCreateManagedWidget("by frame", XwmenuButtonWidgetClass,
                               menuPane, args, n);
      XtAddCallback(wgt, XtNselect, changeMode_callback, TANGO_MODE_SLOW);
      lastModeW = wgt;		/* Current checkmarked widget in menu */
   wgt = XtCreateManagedWidget("by scene", XwmenuButtonWidgetClass,
                               menuPane, NULL, 0);
      XtAddCallback(wgt, XtNselect, changeMode_callback, TANGO_MODE_FAST);
   wgt = XtCreateManagedWidget("none", XwmenuButtonWidgetClass,
			       menuPane, NULL, 0);
      XtAddCallback(wgt, XtNselect, changeMode_callback,TANGO_MODE_DEFAULT);

   /* "Debug" menu */
   mgrShell = XtCreatePopupShell("debugMgrShell", shellWidgetClass, debug,
                                 NULL, 0);
   menuMgr = XtCreateManagedWidget("debugMenuMgr", XwpopupmgrWidgetClass,
                                   mgrShell, NULL, 0);
   paneShell = XtCreatePopupShell("debugShell", shellWidgetClass, menuMgr,
                                  NULL, 0);
      n = 0;
      XtSetArg(args[n], XtNattachTo,    "debugMenuMgr"); n++;
   menuPane = XtCreateManagedWidget("Debug", XwcascadeWidgetClass,
                                    paneShell, args, n);
      n = 0;
      XtSetArg(args[n], XtNsetMark,     True); n++;
   wgt = XtCreateManagedWidget("Off", XwmenuButtonWidgetClass, menuPane,args,n);
      XtAddCallback(wgt, XtNselect, changeDebug_callback, 0);
      lastDebugW = wgt;		/* Current checkmarked widget in menu */
   wgt = XtCreateManagedWidget("On", XwmenuButtonWidgetClass, menuPane, NULL,0);
      XtAddCallback(wgt, XtNselect, changeDebug_callback, TANGO_DEBUG_EXTERNAL);

   XtRealizeWidget(toplevel);
   }



/***************************************************************/
/* delayset_callback -- Adjust the delay setting according     */
/*	       to the scrollbar position.		       */
/*							       */
/* RETURNS:  None.					       */
/***************************************************************/
void
delayset_callback(w, client_data, sliderpos)
   Widget  w;
   caddr_t client_data;
   int sliderpos;
{
   TANGO__data->delay = 1000 * sliderpos;
}



/**************************************************************/
/* pause_callback -- Pause/unpause animation and change	      */
/*		     button label to reflect current state.   */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
pause_callback(w, client_data, call_data)
   Widget  w;
   caddr_t client_data, call_data;
{
   Arg args[1];

   TANGO__data->paused ^= 1;	/* Initially paused as "run animation" */
   XtSetArg(args[0], XtNlabel, (TANGO__data->paused ? "unpause" : "pause"));
   XtSetValues(w, args, 1);
}



/**************************************************************/
/* changeDebug_callback -- Change debug mode and set	      */
/*			   checkmark.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
changeDebug_callback(w, newDebug, call_data)
   Widget  w;
   int     newDebug;
   caddr_t call_data;
{
   Arg args[2];

   if (lastDebugW) {		/* Remove last checkmark */
      XtSetArg(args[0], XtNsetMark, False);
      XtSetValues(lastDebugW , args, 1);
      }
   XtSetArg(args[0], XtNsetMark, True);
   XtSetValues(w, args, 1);
   lastDebugW = w;

   TANGO__data->debug = newDebug;
}



/**************************************************************/
/* changeMode_callback -- Change refresh mode and set	      */
/*			  checkmark.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
changeMode_callback(w, newMode, call_data)
   Widget  w;
   int     newMode;
   caddr_t call_data;
{
   Arg args[2];

   if (lastModeW) {		/* Remove last checkmark */
      XtSetArg(args[0], XtNsetMark, False);
      XtSetValues(lastModeW, args, 1);
      }
   XtSetArg(args[0], XtNsetMark, True);
   XtSetValues(w, args, 1);
   lastModeW = w;

}



/**************************************************************/
/* patternsMode_callback -- Change whether patterns represent */
/*			    colors or fillstyle on mono	      */
/*			    machines.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
patternsMode_callback(w, patternMode, call_data)
   Widget  w;
   int     patternMode;
   caddr_t call_data;
{
   Arg args[2];

   if (lastPatternW) {		/* Remove last checkmark */
      XtSetArg(args[0], XtNsetMark, False);
      XtSetValues(lastPatternW, args, 1);
      }
   XtSetArg(args[0], XtNsetMark, True);
   XtSetValues(w, args, 1);
   lastPatternW = w;

   TANGO__data->mono_fillstyle = patternMode;
}



/**************************************************************/
/* directSlider_callback -- Directly position scrollbar	      */
/*			    slider.			      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
directSlider_callback(w, client_data, sliderpos)
   Widget w;
   caddr_t client_data;
   int sliderpos;
{
   Arg args[1];

   XtSetArg(args[0], XtNsliderOrigin, sliderpos);
   XtSetValues(w, args, 1);
   XtCallCallbacks(w, XtNsliderMoved, sliderpos);
}

/**************************************************************/
/*****************     end of xtangohp.c     ******************/
/**************************************************************/
