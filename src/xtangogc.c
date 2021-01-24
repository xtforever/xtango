/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (gc)		      */
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
#include "xtangofills.h"  /* 41 Fill styles from clear to solid */

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

typedef struct {
   char *name;
   XFontStruct *xfs;
   } GC_FONT;

typedef struct {
   int  width;		/* 0, 2, 4 		      */
   int  line_style;	/* LineSolid or LineOnOffDash */
   char dash_list[2];
   } GC_LINE;

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

#ifdef ultrix
#define DEFAULTFONT      "fixed"
#else
#define DEFAULTFONT      "8x13"
#endif
#define MAXCOLORS	 100
#define NUMDEFAULTCOLORS  8
#define MAXFONTS	 20
#define MAXFILLS	 41
#define NUMDEFAULTFILLS  41
#define MAXLINESTYLES     9
#define UNINITIALIZED	 -1

static Pixel _colors[MAXCOLORS];
static char  *_colorName[MAXCOLORS] = { "white", "yellow", "green",  "blue",
					"orange", "red",   "maroon", "black" };
static GC_FONT _font[MAXFONTS] = { 0 };
static GC_LINE _lineStyles[MAXLINESTYLES] = {
		  {0,LineSolid,    {4,4}}, {0,LineOnOffDash,{8,8}},
		  {0,LineOnOffDash,{2,2}}, {2,LineSolid,    {4,4}},
		  {2,LineOnOffDash,{8,8}}, {2,LineOnOffDash,{2,2}},
		  {4,LineSolid,    {4,4}}, {4,LineOnOffDash,{8,8}},
		  {4,LineOnOffDash,{2,2}} };

static Pixmap _fillPixmap[MAXFILLS];

static GC  _gc;
static int _curFill, _curLS, _curColor, _curFont;
static int _numColorsAvail, _numFillsAvail, _numFontsAvail;

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/


/**************************************************************/
/* TANGO_gc()		internal interface to outside	      */
/* TANGO_inq_font()					      */
/* TANGO_inq_color()					      */
/**************************************************************/
GC  TANGO_gc()		{ return _gc; }
int TANGO_inq_font()	{ return _curFont; }
int TANGO_inq_color()	{ return _curColor; }



/***************************************************************/
/* TANGOload_color -- Get pixel value of named color.  Loading */
/*		      new colors is not supported for mono     */
/*		      screens since 'colors' are represented   */
/*		      by fill styles.			       */
/*							       */
/* RETURNS:  Index into _colors[] for users to refer to color. */
/***************************************************************/
TANGO_COLOR
TANGOload_color(name)
   char *name;
{
   XColor exact, result;
   int indx;

   DEBUG("TANGOload_color(\"%s\")\n", name);

   if (!TANGO__data) TANGOinit();

   if (!name) return _curColor;
   if (TANGO__data->color_screen) {
      if (_numColorsAvail >= MAXCOLORS) {
         COMPLAIN("ERROR: TANGOload_color: max number (%d) %s\n", MAXCOLORS,
		  "of colors already used");
         return _curColor;
         }
      if ((indx = TANGO_check_color(name)) != -1) /* already loaded */
         return indx;
      if (XAllocNamedColor(TANGO__data->display,
     			   DefaultColormap(TANGO__data->display, 
                                          DefaultScreen(TANGO__data->display)),
			   name, &exact, &result)) {
	 _colors[_numColorsAvail] = result.pixel;
         _colorName[_numColorsAvail] = (char*)malloc(strlen(name)+1);
         strcpy(_colorName[_numColorsAvail],name);
	 return _numColorsAvail++;
	 }
      else {
	 COMPLAIN("WARNING: TANGOload_color: color not available (\"%s\")\n",
		  name);
	 return TANGO_COLOR_BLACK;
	 }
      }
   else {
      COMPLAIN("WARNING: TANGOload_color: new 'colors' %s \"%s\"\n",
	       "not supported, ignoring", name);
      return TANGO_COLOR_BLACK;
      }
}



/***************************************************************/
/* TANGO_check_color -- Check to see if a color has already    */
/*                    been loaded.                             */
/*							       */
/* RETURNS:  Index into _colorName[] or -1 if not there.       */
/***************************************************************/
int
TANGO_check_color(name)
   char *name;
{
   int i;
   for (i=0; i<_numColorsAvail; i++)
       if (strcmp(name,_colorName[i]) == 0)
          return i;
   return -1;
}




/***************************************************************/
/* TANGO_color -- Update graphics context with new color if    */
/*		  necessary.  For monochrome screens, 'colors' */
/*		  are represented by fill styles.	       */
/*							       */
/* RETURNS:  Color in gc that was replaced by new color.       */
/***************************************************************/
TANGO_COLOR
TANGO_color(color)
   TANGO_COLOR color;
{
   int   old = _curColor;
   Pixel fg;

   if (!TANGO__data) TANGOinit();

   if (color == _curColor) return _curColor;

   if (color < 0 || color >= _numColorsAvail) {
      COMPLAIN("ERROR: TANGO_color: undefined color (%d)\n", color);
      return _curColor;
      }
   _curColor = color;

   if (TANGO__data->color_screen)
      fg = _colors[color];
   else {
      fg = (color == TANGO_COLOR_WHITE
     		  ? WhitePixel(TANGO__data->display, 
                                  DefaultScreen(TANGO__data->display))
		  : BlackPixel(TANGO__data->display, 
                                  DefaultScreen(TANGO__data->display)));
      if (color == TANGO_COLOR_WHITE || color == TANGO_COLOR_BLACK)
         XSetFillStyle(TANGO__data->display, TANGO_gc(), FillSolid);
      else	/* 8 distinct fills 0,6,11,17,23,29,34,40 */
         TANGO_fill_style((int)(color*5.7 + 0.5));
      }

   XSetForeground(TANGO__data->display, TANGO_gc(), fg);
   return (old != UNINITIALIZED ? old : _curColor);
}



/***************************************************************/
/* TANGO_line_style -- Update graphics context with new line   */
/*		       style if necessary.		       */
/*							       */
/* RETURNS:  Line style in gc that was replaced by new style.  */
/***************************************************************/
TANGO_LINE_STYLE
TANGO_line_style(style)
   TANGO_LINE_STYLE style;
{
   int old = _curLS;
   
   if (!TANGO__data) TANGOinit();

   if (style < 0 || style >= MAXLINESTYLES) {
      COMPLAIN("ERROR: TANGO_line_style: undefined line style (%d)\n", style);
      return _curLS;
      }

   if (style == _curLS) return _curLS;
   _curLS = style;

   if (style == TANGO_STYLE_SOLID)
      XSetLineAttributes(TANGO__data->display, TANGO_gc(), 0, LineSolid,
			 CapButt, JoinRound);
   else {
      XSetLineAttributes(TANGO__data->display, TANGO_gc(), 
                         _lineStyles[style].width,
                         _lineStyles[style].line_style, CapButt, JoinRound);
      XSetDashes(TANGO__data->display, TANGO_gc(), 0, 
                 _lineStyles[style].dash_list, 2);
      }

   return old;
}



/***************************************************************/
/* TANGO_fill_style -- Update graphics context with new fill   */
/*		       style if necessary.		       */
/*							       */
/* RETURNS:  fill style in gc that was replaced by new style.  */
/***************************************************************/
TANGO_FILL_STYLE
TANGO_fill_style(fill)
   TANGO_FILL_STYLE fill;
{
   int old = _curFill;
   if (!TANGO__data) TANGOinit();

   if (fill < 0 || fill >= _numFillsAvail) {
      COMPLAIN("ERROR: TANGO_fill_style: undefined fill style (%d)\n", fill);
      return _curFill;
      }

   if (fill == _curFill) return _curFill;
   _curFill = fill;

   /* For mono screens, colors are represented by fillstyles  */
   /*    so any change to fillstyle must alert the color      */
   /*    routine that the current "color" is NOT what it used */
   /*    to be.  Force TANGO_color() to reload the color.     */

   if (!TANGO__data->color_screen) _curColor = UNINITIALIZED;

   /* Update the GC with the new fill style */

   if (_fillPixmap[fill] != None) {
      XSetFillStyle(TANGO__data->display, TANGO_gc(), FillOpaqueStippled);
      XSetStipple(TANGO__data->display, TANGO_gc(), _fillPixmap[fill]);
      }
   else  /* Problem getting particular fill Pixmap...displayed warning at bad */
      XSetFillStyle(TANGO__data->display, TANGO_gc(), FillSolid); 
                       /* creation attempt */
   
   return old;
}



/***************************************************************/
/* TANGO_load_font -- Load named font into server if it isn't  */
/*		      already there.			       */
/*		      NOTE:  It does NOT change font in gc.    */
/*							       */
/* RETURNS:  Index into _font[] to allow users a way to        */
/*	     reference the new font.			       */
/***************************************************************/
int
TANGO_load_font(fontname)
   char *fontname;
{
   XFontStruct *xfs;
   int  newfont = 0;	/* Problems?  use default font @ first array pos */
   int  i, found;

   if (!TANGO__data) TANGOinit();

   if (!fontname) return newfont;

   if (_numFontsAvail >= MAXFONTS) {
      COMPLAIN("WARNING: TANGO_load_font: max number (%d) of fonts %s\n",
	       MAXFONTS, "already used");
      return newfont;
      }

   /* Make sure we don't reload an already-used font */

   for (found = i = 0; !found && i < _numFontsAvail; i += !found)
      found = !strcmp(_font[i].name, fontname);
   if (found) return i;

   /* Font hasn't been loaded yet, load it */

   xfs = XLoadQueryFont(TANGO__data->display, fontname);
   if (xfs == None)	/* Problem loading font, use default */
      COMPLAIN("WARNING: TANGO_load_font: font not available (\"%s\")\n",
	       fontname);
   else {
      _font[_numFontsAvail].name = (char *)malloc(strlen(fontname)+1);
      strcpy(_font[_numFontsAvail].name, fontname);
      _font[_numFontsAvail].xfs = xfs;
      newfont = _numFontsAvail++;
      }

   return newfont;
}



/***************************************************************/
/* TANGO_text_info -- Get information about a particular font  */
/*		      such as height and widths.	       */
/*							       */
/* RETURNS:  None.					       */
/***************************************************************/
void
TANGO_text_info(fontid, str, xext, yext, xofs, yofs)
   Font fontid;
   char *str;
   int  *xext, *yext, *xofs, *yofs;
{
   XCharStruct over;
   int dir, asc, des;

   if (!TANGO__data) TANGOinit();

   if (fontid < 0 || fontid >= _numFontsAvail) {
      COMPLAIN("ERROR: TANGO_text_info: undefined font (%ld)\n", fontid);
      *xext = *yext = *xofs = *yofs = 0;
      }
   else if (str) {
      XTextExtents(_font[fontid].xfs, str, strlen(str), &dir, &asc, &des, &over);
      *xext = over.rbearing;
      *yext = asc + des;
      *xofs = over.lbearing;
      *yofs = des;
      }
   else
      *xext = *yext = *xofs = *yofs = 0;
}



/***************************************************************/
/* TANGO_font -- Change current font in graphics context if    */
/*		 necessary.				       */
/*							       */
/* RETURNS:  Font in gc that was replaced by new font.	       */
/***************************************************************/
int
TANGO_font(fontid)
   int fontid;
{
   int old = _curFont;

   if (!TANGO__data) TANGOinit();

   if (fontid == _curFont) return _curFont;

   if (fontid < 0 || fontid >= _numFontsAvail) {
      COMPLAIN("ERROR: TANGO_font: undefined font (%d)\n", fontid);
      return _curFont;
      }

   _curFont = fontid;
   XSetFont(TANGO__data->display, TANGO_gc(), _font[fontid].xfs->fid);

   return old;
}



/***************************************************************/
/* TANGO_initGC -- Initialize the single graphics context used */
/*		   by all window drawing routines.             */
/*		   Create Pixmaps holding fill styles, reserve */
/*		   the 8 default colors, and load the default  */
/*		   font.				       */
/*							       */
/* RETURNS:  None.					       */
/***************************************************************/
void
TANGO_initGC()
{
   int    i;
   XColor exact, result;

   /* Init fill styles ... create pixmaps */
   for (i = 0; i < NUMDEFAULTFILLS; i++) {
      _fillPixmap[i] = XCreateBitmapFromData(TANGO__data->display,
				DefaultRootWindow(TANGO__data->display),
				_fillStyles[i], 8, 8);
      if (_fillPixmap[i] == None) {
         COMPLAIN("WARNING: TANGO_initGC: unable to create fill style #%d\n",
		  i);
         /* recover somehow...here */
         }
      }
   for ( ; i < MAXFILLS; i++)
      _fillPixmap[i] = None;
   _numFillsAvail = NUMDEFAULTFILLS;

   /* Init colors */
   if (TANGO__data->color_screen)
      for (i = 0; i < NUMDEFAULTCOLORS; i++)
         if (XAllocNamedColor(TANGO__data->display, 
                              DefaultColormap(TANGO__data->display,
			           DefaultScreen(TANGO__data->display)), 
                              _colorName[i], &exact, &result)) {
	    _colors[i] = result.pixel;
	    }
	 else {
	    COMPLAIN("WARNING: TANGO_initGC: color not available (\"%s\")\n",
		     _colorName[i]);
	    _colors[i] = BlackPixel(TANGO__data->display, 
                                    DefaultScreen(TANGO__data->display));
	    }
   else
      i = 0;	/* Init all colors to UNITIALIZED since b/w doesn't use them */
   for ( ; i < MAXCOLORS; i++)
      _colors[i] = UNINITIALIZED;
   _numColorsAvail = NUMDEFAULTCOLORS;

   /* Init fonts */
   for (i = 0; i < MAXFONTS; i++) {
      _font[i].name = NULL;
      _font[i].xfs = NULL;
      }
   _numFontsAvail = 0;

   /* Create GC */
   _gc = XCreateGC(TANGO__data->display, 
                    DefaultRootWindow(TANGO__data->display), 0, 0);
   if (_gc == None) {
      COMPLAIN("ERROR: TANGO_initGC: unable to create GC\n");
      /* recover somehow...here */
      exit(1);
      }

   /* Init GC - white background, default font, black */
   /*		foreground, solid fill, solid line    */

   XSetBackground(TANGO__data->display, _gc, 
       WhitePixel(TANGO__data->display,DefaultScreen(TANGO__data->display)));
   _curFont = _curColor = _curFill = _curLS = UNINITIALIZED;
   XSetGraphicsExposures(TANGO__data->display, _gc, False);
   TANGO_font(TANGO_load_font(DEFAULTFONT));	/* _curFont set here  */
   TANGO_fill_style(40);			/* _curFill set here  */
   TANGO_color(TANGO_COLOR_BLACK);		/* _curColor set here */
   TANGO_line_style(TANGO_STYLE_SOLID);		/* _curLS set here    */
}

/**************************************************************/
/*****************     end of xtangogc.c     ******************/
/**************************************************************/
