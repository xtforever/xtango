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
#define SBAR_WIDTH	20
#define YBDR_OFFSET	-BORDER_WIDTH

XTango*geometry:		=512x512

XTango*background:		BACKGROUND
XTango*foreground:		FOREGROUND

XTango*borderWidth:		BORDER_WIDTH

XTango*PushButton.font:		BUTTONFONT

xtango*title.gravity:		CenterGravity
xtango*title.font:		TITLEFONT
xtango*title.String:		TITLE
xtango*title.background:	TITLECOLOR
xtango*title.xResizable:	true
xtango*title.vSpace:		4

xtango*rcleft.forceSize:	true
xtango*rcleft.yRefName:		title
xtango*rcleft.yAddHeight:	true
xtango*rcleft.yResizable:	true
xtango*rcleft.yOffset:		YBDR_OFFSET
xtango*rcleft.yAttachOffset:	37
xtango*rcleft*highlightThickness: 2
xtango*rcleft*traversalType:	highlight_enter

XTango*Arrow.width:		20
XTango*Arrow.height:		30
xtango*up.arrowDirection:	arrow_up
xtango*down.arrowDirection:	arrow_down
xtango*left.arrowDirection:	arrow_left
xtango*right.arrowDirection:	arrow_right

/*----------------------------------------------------------------------------
 * Scrollbar:  Position relative to title bar, start just under title	      
 *		 fill entire left side of window, hide top border under title,
 *		 end just above bottom buttons, wide arrow (better looking)   
 *---------------------------------------------------------------------------*/
xtango*sbar*xRefName:		title
xtango*sbar*width:		SBAR_WIDTH
xtango*sbar*xAddWidth:		true
/*
 * Place on x axis just under title so left edges line up...
 * (-sbar_width - 2*bdr_width)
 */
xtango*sbar*xOffset:		-22
xtango*sbar*yRefName:		title
xtango*sbar*yAddHeight:		true
xtango*sbar*yResizable:		true
xtango*sbar*yOffset:		YBDR_OFFSET
xtango*sbar*yAttachOffset:	37

/*
 * Scrollbar conveniences
 * repeat speed of arrows, initial delay before repeating,
 * slider size, slider granularity (0 - 100)
 */
xtango*sbar*repeatRate:		25
xtango*sbar*initialDelay:	500
xtango*sbar*sliderExtent:	10
xtango*sbar*granularity:	2
/*---------------------------------------------------------------------------*/

/*
 * Initially paused so we don't miss part of animation
 */
xtango*pause*set:		true
xtango*pause*label:		INIT_PAUSE_STR
xtango*pause*xOffset:		4
xtango*pause*yRefName:		rcleft
xtango*pause*yAddHeight:	true
xtango*pause*yOffset:		4
xtango*pause*yAttachOffset:	4
xtango*pause*highlightThickness: 2
xtango*pause*traversalType:	highlight_enter

/*
 * "Stack" bottom buttons from left edge of window (under scroll bar)
 */

/*
 * Mode button/menu
 */
xtango*mode*xRefName:		sbar
xtango*mode*xOffset:		-182	/* make room for all buttons */
xtango*mode*xAttachOffset:	4
xtango*mode*yRefName:		pause
xtango*mode*yAttachOffset:	4
xtango*mode*highlightThickness: 2
xtango*mode*traversalType:	highlight_enter

/*
 * Debug button/menu
 */
xtango*debug*xRefName:		mode
xtango*debug*xAddWidth:		true
xtango*debug*xOffset:		4
xtango*debug*xAttachOffset:	4
xtango*debug*yRefName:		mode
xtango*debug*yAttachOffset:	4
xtango*debug*highlightThickness: 2
xtango*debug*traversalType:	highlight_enter

/*
 * Refresh button
 */
xtango*refresh*xRefName:	debug
xtango*refresh*xAddWidth:	true
xtango*refresh*xOffset:		4
xtango*refresh*xAttachOffset:	4
xtango*refresh*yRefName:	debug
xtango*refresh*yAttachOffset:	4
xtango*refresh*highlightThickness: 2
xtango*refresh*traversalType:	highlight_enter

/*
 * Quit button
 */
xtango*quit*xRefName:		refresh
xtango*quit*xAddWidth:		true
xtango*quit*xOffset:		4
xtango*quit*xAttachOffset:	4
xtango*quit*yRefName:		refresh
xtango*quit*yAttachOffset:	4
xtango*quit*highlightThickness: 2
xtango*quit*traversalType:	highlight_enter

/*
 * Patterns button/menu (used for mono machines only)
 */
xtango*patterns*xRefName:	mode
xtango*patterns*xOffset:	-67
xtango*patterns*xAttachOffset:	4
xtango*patterns*yRefName:	quit
xtango*patterns*yAttachOffset:	4
xtango*patterns*highlightThickness: 2
xtango*patterns*traversalType:	highlight_enter

xtango*easel*foreground:	black
xtango*easel*background:	white
xtango*easel*xRefName:		rcleft
xtango*easel*xAddWidth:		true
xtango*easel*xResizable:	true
xtango*easel*xAttachRight:	true
xtango*easel*xAttachOffset:	21	/* sbar width - border (from left) */
xtango*easel*xOffset:		YBDR_OFFSET
xtango*easel*yRefName:		title
xtango*easel*yAddHeight:	true
xtango*easel*yResizable:	true
xtango*easel*yAttachBottom:	true
xtango*easel*yAttachOffset:	37
xtango*easel*yOffset:		YBDR_OFFSET

xtango*Refresh*font:		BUTTONFONT
xtango*Debug*font:		BUTTONFONT
xtango*Represent*font:		BUTTONFONT

/**************************************************************/
/*****************    end of xtangohp.res    ******************/
/**************************************************************/
