/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (loc)		      */
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
#include "xtangoversion.h"

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

ANIMATION_PTR	 TANGO__data = NULL;

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

/**************************************************************/
/*****************	  entry points       ******************/
/**************************************************************/

void	  TANGOinit();
TANGO_LOC TANGOloc_create();
WIN_COORD TANGOloc_X();
WIN_COORD TANGOloc_Y();
void	  TANGOloc_inquire();
TANGO_LOC TANGOloc_modify();
int	  TANGOloc_equal();


/**************************************************************/
/* TANGOinit -- Initialize TANGO globals and setup windows.   */
/*		(Only allowed once).			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TANGOinit()
{
   if (TANGO__data) return;

   fprintf(stderr, "\nXTANGO Version %4.02f\n\n", VERSION);

   TANGO_image_init();
   TANGO_setup_windows();
}



/**************************************************************/
/* TANGOloc_create -- Create a loc object given its two       */
/*		      coordinates.			      */
/* 							      */
/* RETURNS:  Newly created TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC
TANGOloc_create(x,y)
   WIN_COORD x,y;
{
   TANGO_LOC loc;

   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOloc_create(%lf, %lf)\n", x, y);

   loc = (TANGO_LOC) malloc( sizeof( struct _LOC));
   loc->x = x;
   loc->y = y;
   return(loc);
}



/**************************************************************/
/* TANGOloc_X -- Return the X component of the given LOC.     */
/* 							      */
/* RETURNS:  The X component of the given LOC.		      */
/**************************************************************/
WIN_COORD
TANGOloc_X(loc)
   TANGO_LOC loc;
{
   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOloc_X(0x%p)\n", loc);

   return(loc->x);
}



/**************************************************************/
/* TANGOloc_Y -- Return the Y component of the given LOC.     */
/* 							      */
/* RETURNS:  The Y component of the given LOC.		      */
/**************************************************************/
WIN_COORD
TANGOloc_Y(loc)
   TANGO_LOC loc;
{
   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOloc_Y(0x%p)\n", loc);

   return(loc->y);
}



/**************************************************************/
/* TANGOloc_inquire -- Get the x and y coordinates of the     */
/*		       given loc.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TANGOloc_inquire(loc,x,y)
   TANGO_LOC loc;
   WIN_COORD *x,*y;
{
   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOloc_inquire(0x%p, 0x%p, 0x%p)\n", loc, x, y);

   *x = loc->x;
   *y = loc->y;
}



/**************************************************************/
/* TANGOloc_modify -- Modify the given loc's coords by the    */
/*		      given x and y values.		      */
/* 							      */
/* RETURNS:  Modified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC
TANGOloc_modify(loc,x,y)
   TANGO_LOC loc;
   WIN_COORD x,y;
{
   TANGO_LOC newloc;

   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOloc_modify(0x%p, %lf, %lf)\n", loc, x, y);

   newloc = (TANGO_LOC) malloc( sizeof( struct _LOC));
   newloc->x = x + loc->x;
   newloc->y = y + loc->y;
   return(newloc);
}



/**************************************************************/
/* TANGOloc_equal -- Return 1 if the two locs have the same   */
/*		     coordinates, 0 if not.		      */
/* 							      */
/* RETURNS:  TRUE(1) or FALSE(0).			      */
/**************************************************************/
int
TANGOloc_equal(loc1,loc2)
   TANGO_LOC loc1,loc2;
{
   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOloc_equal(0x%p, 0x%p)\n", loc1, loc2);

   return( EQUAL(loc1->x,loc2->x) && EQUAL(loc1->y,loc2->y) );
}

/**************************************************************/
/*****************    end of xtangoloc.c     ******************/
/**************************************************************/
