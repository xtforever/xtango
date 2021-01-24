/*---------------------------------------------------------------------------*/
/*		  XTANGO Widget Resource File -- Athena Version		     */
/*---------------------------------------------------------------------------*/
/*									     */
/*				toplevel				     */
/*				   |					     */
/*				  form					     */
/*	  /----------/-----/-------|-----\-----\----\----\-------\	     */
/*	title	leftbox	 sbar    quit refresh debug mode pause  easel	     */
/*                 |							     */
/*	  /----/---|---\---\--\						     */
/*	left right up down in out		  			     */
/*									     */
/*			modeMenu		debugMenu		     */
/*			   |			    |			     */
/*                /--------|-------\		  /-+-\			     */
/*          *by frame   by scene   none	       *off    on		     */
/*									     */
/*  Widget Class   Name     Widget Class   Name     Widget Class   Name      */
/*  ---------------------   ---------------------   ---------------------    */
/*  Shell	   toplevel menuButton     mode     simple	   easel     */
/*  form	   form	    command	   pause    simpleMenu	   modeMenu  */
/*  label	   title    command	   left     smeBSBobject   by frame  */
/*  box		   leftbox  command	   right    smeBSBobject   by scene  */
/*  scrollbar	   sbar	    command	   up	    smeBSBobject   none      */
/*  command	   quit	    command	   down     simpleMenu	   debugMenu */
/*  command	   refresh  command	   in	    smeBSBobject   off	     */
/*  menuButton	   debug    command	   out	    smeBSBobject   on	     */
/* 									     */
/*---------------------------------------------------------------------------*/
/* NOTE:  There are MANY magic numbers in this file.  This was a first,      */
/*        quick attempt with the Athena set.  If you find a better way of    */
/*	  implementing these resources or would just like to rearrange	     */
/*	  things on the screen, please feel free.			     */
/*									     */
/*  ****** This file MUST be run through cpp before xrdb can use it. ******  */
/*	   Try "cpp <athena.res >$HOME/app-defaults/xtango"		     */
/*---------------------------------------------------------------------------*/

/*
 * NOTE: ** MUST ** use FULL font names (ie. those listed from 'xlsfont')
 */

/*
 * Set HOME to the absolute path of the xtango subdirectory.
 */
#define HOME /home/my/src/xtango

#define TITLE           XTANGO
#define TITLEFONT	-*-helvetica-bold-r-normal--14-*-*-*-*-*-*-*
#define TITLECOLOR	Yellow
#define BUTTONFONT	-*-helvetica-bold-o-normal--12-*-*-*-*-*-*-*
#define BACKGROUND	White
#define FOREGROUND	Black
#define BORDER_WIDTH    1
#define INITIAL_PAUSE_LABEL run animation

XTango*geometry:		=512x512

XTango*background:		BACKGROUND
XTango*foreground:		FOREGROUND

XTango*borderWidth:		BORDER_WIDTH

XTango*Command.font:		BUTTONFONT
XTango*MenuButton.font:		BUTTONFONT

xtango*form.defaultDistance:	0

xtango*title.label:		TITLE
xtango*title.justify:		JustifyCenter
xtango*title.font:		TITLEFONT
xtango*title.background:	TITLECOLOR
xtango*title.resizable:		TRUE
xtango*title.right:		ChainRight
xtango*title.bottom:		ChainTop

xtango*leftbox.orientation:	vertical
xtango*leftbox.fromVert:	title
xtango*leftbox.top:		ChainTop
xtango*leftbox.right:		ChainLeft
xtango*leftbox.bottom:		ChainBottom
xtango*leftbox*width:		42
xtango*leftbox*height:		35

xtango*left.bitmap:		HOME/bitmaps/left.bm
xtango*right.bitmap:		HOME/bitmaps/right.bm
xtango*up.bitmap:		HOME/bitmaps/up.bm
xtango*down.bitmap:		HOME/bitmaps/down.bm
xtango*in.bitmap:		HOME/bitmaps/in.bm
xtango*out.bitmap:		HOME/bitmaps/out.bm

xtango*sbar.fromVert:		title
xtango*sbar.horizDistance:	49
xtango*sbar.length:		250
xtango*sbar.thickness:		20
xtango*sbar.top:		ChainTop
xtango*sbar.left:		ChainRight
xtango*sbar.right:		ChainRight
xtango*sbar.shown:		0.1

xtango*quit.fromHoriz:		sbar
xtango*quit.top:		ChainBottom
xtango*quit.bottom:		ChainBottom
xtango*quit.horizDistance:	-12
xtango*quit.vertDistance:	235
xtango*quit.width:		5
xtango*quit.height:		30

xtango*refresh.fromHoriz:	quit
xtango*refresh.top:		ChainBottom
xtango*refresh.bottom:		ChainBottom
xtango*refresh.horizDistance:	-15
xtango*refresh.vertDistance:	235
xtango*refresh.width:		6
xtango*refresh.height:		30

xtango*debug.fromHoriz:		refresh
xtango*debug.top:		ChainBottom
xtango*debug.bottom:		ChainBottom
xtango*debug.horizDistance:	-16
xtango*debug.vertDistance:	235
xtango*debug.width:		6
xtango*debug.height:		30
xtango*debug.menuName:		debugMenu

xtango*mode.fromHoriz:		debug
xtango*mode.top:		ChainBottom
xtango*mode.bottom:		ChainBottom
xtango*mode.horizDistance:	-16
xtango*mode.vertDistance:	235
xtango*mode.width:		6
xtango*mode.height:		30
xtango*mode.menuName:		modeMenu

xtango*pause.label:		INITIAL_PAUSE_LABEL
xtango*pause.fromHoriz:		mode
xtango*pause.top:		ChainBottom
xtango*pause.bottom:		ChainBottom
xtango*pause.horizDistance:	-34
xtango*pause.vertDistance:	235
xtango*pause.width:		11
xtango*pause.height:		30

xtango*patterns.fromHoriz:	mode
xtango*patterns.top:		ChainBottom
xtango*patterns.bottom:		ChainBottom
xtango*patterns.horizDistance:	-16
xtango*patterns.vertDistance:	235
xtango*patterns.width:		6
xtango*patterns.height:		30
xtango*patterns.menuName:	patternsMenu

xtango*easel.fromVert:		title
xtango*easel.fromHoriz:		leftbox
xtango*easel.top:		ChainTop
xtango*easel.left:		ChainLeft
xtango*easel.right:		ChainRight
xtango*easel.bottom:		ChainBottom
xtango*easel.width:		7
xtango*easel.height:		200

xtango*modeMenu.geometry:	64x48
xtango*debugMenu.geometry:	64x30
xtango*patternsMenu.geometry:	83x30

/**************************************************************/
/*****************  end of xtangoathena.res  ******************/
/**************************************************************/
