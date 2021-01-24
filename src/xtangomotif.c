/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (motif)	      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*		John T. Stasko and Doug Hayes		      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 91/05/09 JDH       Baselined source.			      */
/*							      */
/**************************************************************/

/**************************************************************/
/* NOTE:  To use a widget set other than Motif widgets, just  */
/*	  replace this file with equivalent functions.	      */
/**************************************************************/

/**************************************************************/
/*****************	 include files       ******************/ 
/**************************************************************/

#include "xtangolocal.h"

#include <X11/Shell.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>

#define _NO_PROTO
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/CascadeB.h>
#include <Xm/ToggleB.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <Xm/ScrollBar.h>
#include <Xm/Separator.h>

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

/* Fake radio buttons...keep currently set widget */
Widget lastPattern, lastMode, lastDebug;

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

void ManageButtons();
void delayset_callback(),    pause_callback();
void changeDebug_callback(), changeMode_callback(), patternsMode_callback();
void newToggle();

/**************************************************************/
/*****************       entry points        ******************/
/**************************************************************/

/*----------------------------------------------------------------------------*/
/*                      XTANGO Widgets -- Motif Version                       */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*                          toplevel					      */
/*                             |					      */
/*                            form					      */
/*    /-----/---------/--------+------\---------\			      */
/*  title easel    rcleft            sbar      menubot			      */
/*              /----/---\---\---\--\    /------\-----\-----\-------\-----\   */
/*           left right up down in out pause patterns mode debug refresh quit */
/*                                                                            */
/*                                                                            */
/*		modeMenu		debugMenu	patternsMenu	      */
/*		    |			    |		     |		      */
/*	    /---------\-------\		  /-+-\		   /-+-\	      */
/*      *by frame  by scene  none	*Off  On      *colors fillstyles      */
/*----------------------------------------------------------------------------*/

typedef enum { ARROW, PUSHB, TOGGLEB, LABEL, SEPARATOR } buttonType;
typedef enum { UNSET, DEFAULT } initialType;
typedef struct {
   char	       *name;
   initialType setting;
   buttonType  btnType;
   String      callbackType;
   void        (*callback)();
   int         calldata;
   } buttonList;

static buttonList leftButtons[] = {
	{ "left",  UNSET, ARROW, XmNactivateCallback, pan_callback,  0 },
	{ "right", UNSET, ARROW, XmNactivateCallback, pan_callback,  1 },
	{ "up",	   UNSET, ARROW, XmNactivateCallback, pan_callback,  2 },
	{ "down",  UNSET, ARROW, XmNactivateCallback, pan_callback,  3 },
	{ "in",	   UNSET, PUSHB, XmNactivateCallback, zoom_callback, 0 },
	{ "out",   UNSET, PUSHB, XmNactivateCallback, zoom_callback, 1 }
   };

static buttonList patButtons[] = {
	{ "Represent",   UNSET,	  LABEL,     NULL, NULL, NULL },
	{ "separator1",  UNSET,	  SEPARATOR, NULL, NULL, NULL },
	{ "Colors",      DEFAULT, TOGGLEB,   XmNvalueChangedCallback,
					     patternsMode_callback, 0 },
	{ "Fill Styles", UNSET,	  TOGGLEB,   XmNvalueChangedCallback,
					     patternsMode_callback, 1 }
   };

static buttonList modeButtons[] = {
	{ "Refresh",	UNSET,	 LABEL,	    NULL, NULL, NULL },
	{ "separator2", UNSET,	 SEPARATOR, NULL, NULL, NULL },
	{ "by frame",	DEFAULT, TOGGLEB,   XmNvalueChangedCallback,
				 changeMode_callback, TANGO_MODE_SLOW },
	{ "by scene",	UNSET,	 TOGGLEB,   XmNvalueChangedCallback,
				 changeMode_callback, TANGO_MODE_FAST },
	{ "none",	UNSET,	 TOGGLEB,   XmNvalueChangedCallback,
				 changeMode_callback, TANGO_MODE_DEFAULT }
   };

static buttonList debugButtons[] = {
	{ "Off", DEFAULT, TOGGLEB, XmNvalueChangedCallback,
				   changeDebug_callback, 0 },
	{ "On",  UNSET,   TOGGLEB, XmNvalueChangedCallback,
				   changeDebug_callback, TANGO_DEBUG_EXTERNAL }
   };



/**************************************************************/
/* TANGO_layout_window -- Create, init (hardcoded), place, &  */
/*			  realize all widgets.		      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TANGO_layout_window()
{
   Widget      toplevel, form, rcleft, menubot, title, sbar, wgt;
   Widget      menuPane;    /* Menus */
   Display     *dpy;
   int         scr;
   Arg	       args[10];
   int         n;

   toplevel = XtInitialize(argv[0], "XTango", NULL, 0, &argc, argv);
      dpy = XtDisplay(toplevel);
      scr = DefaultScreen(dpy);

   form = XtCreateManagedWidget("form", xmFormWidgetClass, toplevel, NULL, 0);

      n = 0;
      XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
      XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
   title = XtCreateManagedWidget("title", xmLabelWidgetClass, form, args, n);

      n = 0;
      XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
   menubot = XmCreateMenuBar(form, "menubot", args, n);
   XtManageChild(menubot);

   wgt = XtCreateManagedWidget("pause", xmCascadeButtonWidgetClass, menubot,
			       NULL, 0);
      XtAddCallback(wgt, XmNactivateCallback, pause_callback, NULL);

   /* Patterns menu to select whethers patterns represent colors */
   /*    or fill-styles (only visible on mono machines).         */
   if (!TANGO__data->color_screen) {
      /* "Patterns" menu */
      menuPane = XmCreatePulldownMenu(menubot, "patternsMenu", NULL, 0);
      ManageButtons(menuPane, patButtons, XtNumber(patButtons), &lastPattern);
	 n = 0;
         XtSetArg(args[n], XmNsubMenuId, menuPane); n++;
      XtCreateManagedWidget("patterns", xmCascadeButtonWidgetClass, menubot,
			    args,n);
      }

   /* "Mode" menu */
   menuPane = XmCreatePulldownMenu(menubot, "modeMenu", NULL, 0);
   ManageButtons(menuPane, modeButtons, XtNumber(modeButtons), &lastMode);
      n = 0;
      XtSetArg(args[n], XmNsubMenuId, menuPane); n++;
   XtCreateManagedWidget("mode", xmCascadeButtonWidgetClass, menubot, args, n);

   /* "Debug" menu */
   menuPane = XmCreatePulldownMenu(menubot, "debugMenu", NULL, 0);
   ManageButtons(menuPane, debugButtons, XtNumber(debugButtons), &lastDebug);
      n = 0;
      XtSetArg(args[n], XmNsubMenuId, menuPane); n++;
   XtCreateManagedWidget("debug", xmCascadeButtonWidgetClass, menubot, args, n);

   wgt = XtCreateManagedWidget("refresh", xmCascadeButtonWidgetClass, menubot,
			       NULL, 0);
      XtAddCallback(wgt, XmNactivateCallback, refresh_callback, NULL);
/*    --- reset button doesn't make sense in this iteration of TANGO ---
   XtCreateManagedWidget("reset", xmCascadeButtonWidgetClass, menubot, NULL, 0);
*/

   wgt = XtCreateManagedWidget("quit", xmCascadeButtonWidgetClass, menubot,
			       NULL, 0);
      XtAddCallback(wgt, XmNactivateCallback, quit_callback, NULL);

      n = 0;
      XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNtopWidget, title); n++;
      XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNbottomWidget, menubot); n++;
   rcleft = XtCreateManagedWidget("rcleft", xmRowColumnWidgetClass, form,
				  args, n);
   ManageButtons(rcleft, leftButtons, XtNumber(leftButtons), &wgt);

      n = 0;
      XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNtopWidget, title); n++;
      XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNbottomWidget, menubot); n++;
      XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
      XtSetArg(args[n], XmNinitialDelay, 500); n++;
      XtSetArg(args[n], XmNrepeatDelay, 25); n++;
   sbar = XtCreateManagedWidget("sbar", xmScrollBarWidgetClass, form, args, n);
      XtAddCallback(sbar, XmNvalueChangedCallback, delayset_callback, NULL);

      n = 0;
      XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNtopWidget, title); n++;
      XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNbottomWidget, menubot); n++;
      XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNrightWidget, sbar); n++;
      XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNleftWidget, rcleft); n++;
   TANGO__data->easel = XtCreateManagedWidget("easel", 
                          xmDrawingAreaWidgetClass, form, args, n);
      XtAddCallback(TANGO__data->easel, XmNresizeCallback, resize_callback, 
                            NULL);
      XtAddCallback(TANGO__data->easel, XmNexposeCallback, refresh_callback, 
                            NULL);
      XtAddEventHandler(TANGO__data->easel, ButtonPressMask, FALSE, 
                            select_point, NULL);

   XtRealizeWidget(toplevel);
   }



/***************************************************************/
/* ManageButtons -- Add buttons and callbacks to a manager     */
/*		    (ie. RowColumn, PulldownMenu, etc) widget. */
/*							       */
/* RETURNS:  None.					       */
/***************************************************************/
void
ManageButtons(manager, buttons, numButtons, defaultWidget)
   Widget     manager;
   buttonList *buttons;
   int	      numButtons;
   Widget     *defaultWidget;
{
   WidgetList buttonsW;
   Arg	      arg[3];
   int	      i, argCount;

   buttonsW = (WidgetList)XtMalloc(numButtons * sizeof(Widget));
   
   for (i = 0; i < numButtons; i++) {
      switch (buttons[i].btnType) {
	 case ARROW     : buttonsW[i] = XtCreateManagedWidget(buttons[i].name,
					xmArrowButtonWidgetClass, manager,
					NULL, 0);
			  break;
	 case PUSHB     : buttonsW[i] = XtCreateManagedWidget(buttons[i].name,
					xmPushButtonWidgetClass, manager,
				        NULL, 0);
			  break;
	 case TOGGLEB   : argCount = 2;
			  if (buttons[i].setting == DEFAULT)
			     XtSetArg(arg[0], XmNset, TRUE);
			  else
			     XtSetArg(arg[0], XmNset, FALSE);
                          XtSetArg(arg[1], XmNindicatorType, XmONE_OF_MANY);
			  buttonsW[i] = XtCreateManagedWidget(buttons[i].name,
					xmToggleButtonWidgetClass, manager,
					arg, argCount);
			  if (buttons[i].setting == DEFAULT)
			     *defaultWidget = buttonsW[i];
			  break;
	 case LABEL     : buttonsW[i] = XtCreateManagedWidget(buttons[i].name,
					xmLabelWidgetClass, manager, NULL, 0);
			  break;
	 case SEPARATOR : buttonsW[i] = XtCreateManagedWidget(buttons[i].name,
					xmSeparatorWidgetClass, manager,
					NULL, 0);
			  break;
	 default	: break;
         }
      if (buttons[i].callback)
	 XtAddCallback(buttonsW[i], buttons[i].callbackType,
		       buttons[i].callback, buttons[i].calldata);
      }
   XtManageChildren(buttonsW, numButtons);
   }



/***************************************************************/
/* delayset_callback -- Adjust the delay setting according     */
/*	       to the scrollbar position.		       */
/*							       */
/* RETURNS:  None.					       */
/***************************************************************/
void
delayset_callback(w, client_data, slider)
   Widget  w;
   caddr_t client_data;
   XmScrollBarCallbackStruct *slider;
{
   TANGO__data->delay = 1000 * slider->value;
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
   XmString newLabel;

   TANGO__data->paused ^= 1;	/* Initially paused as "run animation" */
   newLabel = XmStringCreate((TANGO__data->paused ? "unpause" : "pause"),
			     XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, newLabel);
   XtSetValues(w, args, 1);
   XmStringFree(newLabel);
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
   newToggle(&lastDebug, w);
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
   newToggle(&lastMode, w);
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
   newToggle(&lastPattern, w);
   TANGO__data->mono_fillstyle = patternMode;
}



/**************************************************************/
/* newToggle -- Kludge to simulate radio buttons in a pull-   */
/*		down menu.  Unset the last toggle button and  */
/*		set the new toggle button; save the currently */
/*		set toggle button in "lastSet."		      */
/*							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
newToggle(lastSet, nowSet)
Widget *lastSet, nowSet;
{
   Arg args[2];

   XtSetArg(args[0], XmNset, FALSE);
   XtSetValues(*lastSet, args, 1);
   XtSetArg(args[0], XmNset, TRUE);
   XtSetValues(nowSet, args, 1);

   *lastSet = nowSet;
   }

/**************************************************************/
/****************     end of xtangomotif.c     ****************/
/**************************************************************/
