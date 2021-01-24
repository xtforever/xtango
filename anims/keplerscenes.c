#include <stdio.h>
#include "xtango.h"

/*--------------------------------------------------------------------*/
/*                                                                    */
/*                     KEPLER'S LAW IN MOTION                         */
/*                  ANIMATED BY SCOTT MCCRICKARD                      */
/*                          WITH XTANGO                               */
/*  CS 8115 Project 1 -- project idea submitted by Dr. John Stasko.   */
/*                                                                    */
/*  This program animates Kepler's Law of Planetary Motion, which     */
/*  states that the square of a planet's orbital period is            */
/*  proportional to its distance from the sun.  By watching two       */
/*  bodies in orbit, we can visualize this comparison.                */
/*                                                                    */
/*--------------------------------------------------------------------*/

  TANGO_IMAGE planet1;
  TANGO_IMAGE planet2;
  TANGO_IMAGE planet3;

void ANIMInit(x1,x2,x3)
  double x1,x2,x3;
{
  TANGO_PATH null_path;
  TANGO_TRANS draw_sun;

  TANGOinit();

  /* create the sun */
  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
                        0.5,0.5,1,TANGO_COLOR_YELLOW,0.065,1.0);
                         /* x,y,vis,color,radius,fill */

  /* move the sun -- it must be moved or else it will remain invisible */
  null_path=TANGOpath_null(1);
  draw_sun=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,null_path);
  TANGOtrans_perform(draw_sun);

  /* create the first planet */
  planet1=TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
			 x1,0.5,1,TANGO_COLOR_BLUE,0.02,1.0);
                         /* x,y,vis,color,radius,fill */

  /* create the second planet */
  planet2=TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
                           x2,0.5,1,TANGO_COLOR_RED,0.03,1.0);

  /* create the third planet */
  planet3=TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
                           x3,0.5,1,TANGO_COLOR_GREEN,0.06,1.0);
}


void
ANIMMove(x1,y1,x2,y2,x3,y3)
  double x1,y1,x2,y2,x3,y3;
{
  TANGO_LOC f1, t1;           /* from, to locations for planet1 */
  TANGO_PATH planet1_path;
  TANGO_TRANS planet1_trans;

  TANGO_LOC f2, t2;           /* from, to locations for planet2 */
  TANGO_PATH planet2_path;
  TANGO_TRANS planet2_trans;

  TANGO_LOC f3, t3;           /* from, to locations for planet3 */
  TANGO_PATH planet3_path;
  TANGO_TRANS planet3_trans;

  TANGO_TRANS planets;

  /* find the from, to locations, create a path, create a transition action */
  /* for planet1... */

  f1 = TANGOimage_loc(planet1,TANGO_PART_TYPE_C); /* c as in center */
  t1 = TANGOloc_create(x1,y1);

  planet1_path = TANGOpath_motion(f1,t1,TANGO_PATH_TYPE_STRAIGHT);

  planet1_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,planet1,planet1_path);


  /* ...and again for planet2... */

  f2 = TANGOimage_loc(planet2,TANGO_PART_TYPE_C); /* c as in center */
  t2 = TANGOloc_create(x2,y2);

  planet2_path = TANGOpath_motion(f2,t2,TANGO_PATH_TYPE_STRAIGHT);

  planet2_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,planet2,planet2_path);

  /* ...and again for planet3... */

  f3 = TANGOimage_loc(planet3,TANGO_PART_TYPE_C); /* c as in center */
  t3 = TANGOloc_create(x3,y3);

  planet3_path = TANGOpath_motion(f3,t3,TANGO_PATH_TYPE_STRAIGHT);

  planet3_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,planet3,planet3_path);

  /* Now put them together and perform the transition. */

  planets = TANGOtrans_compose(3,planet1_trans,planet2_trans,planet3_trans);

  TANGOtrans_perform(planets);
}


