/*----------------------------------------------------------------------------*/
/*                      XTANGO Widgets -- Motif Version                       */
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

/*
 * NOTE: ** MUST ** use FULL font names (ie. those listed from 'xlsfont')
 */

#define TITLE           XTANGO
#define TITLECOLOR	Yellow
#define TITLEFONT	-*-helvetica-bold-r-normal--14-*-*-*-*-*-*-*
#define BUTTONFONT	-*-helvetica-bold-o-normal--12-*-*-*-*-*-*-*
#define BACKGROUND	White
#define FOREGROUND	Black
#define BORDER_WIDTH	1
#define INIT_PAUSE_STR	run animation

/* Positions of widgets relative to the form (in percentages of the form) */

XTango*geometry:		=512x512

/* Use the standard Motif background blue...change for different colors.
 * XTango*background:		BACKGROUND
 */
XTango*foreground:		FOREGROUND

XTango*borderWidth:		BORDER_WIDTH
XTango*traversalOn:		TRUE
XTango*highlightOnEnter:	TRUE
XTango*keyboardFocusPolicy:	pointer

XTango*fontList:		BUTTONFONT

xtango*title.alignment:		ALIGNMENT_CENTER
xtango*title.fontList:		TITLEFONT
xtango*title.labelString:	TITLE
xtango*title.marginHeight:	4
xtango*title.background:	TITLECOLOR

xtango*up.arrowDirection:	ARROW_UP
xtango*down.arrowDirection:	ARROW_DOWN
xtango*left.arrowDirection:	ARROW_LEFT
xtango*right.arrowDirection:	ARROW_RIGHT

/*---------------------------------------------------------------------------*/

xtango*pause*labelString:	INIT_PAUSE_STR

/**************************************************************/
/*****************    end of xtangomotif.res    ***************/
/**************************************************************/
