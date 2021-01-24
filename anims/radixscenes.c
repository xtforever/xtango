/*   RADIX SORT  developed by Mary Ann Frogge   */

#include <stdio.h>
#include <xtango.h>
#include "radix.h"

char str[10];
double width=0;
double qfill[10];
extern int total;
extern int largest;
/* arrays for list elements (text), list rectangles, queue labels (text) */
TANGO_IMAGE aelms[25];
TANGO_IMAGE recs[25];
TANGO_IMAGE qs[10];
TANGO_IMAGE dot;

static TANGO_IMAGE_COMPONENT aelement[3] = 
  {
    {TANGO_IMAGE_TYPE_RECTANGLE, "0.0 0.0 TANGO_COLOR_BLACK 0.08 0.09 0.0"},
    {TANGO_IMAGE_TYPE_TEXT, "0.0 0.0 TANGO_COLOR_RED NULL str 0"},
    {TANGO_IMAGE_TYPE_COMPOSITE, NULL}
  };


/* init ANIM and initialize qfill array to unfilled */

void
ANIMInit()
{
  int i;

  ASSOCinit();
  for( i=0; i<10; i++ )
    qfill[i] = 0;
}



/*  Draw initial list, first line position, and queue labels */
void
ANIMDraw()
{
  int i;
  float x, y;
  double tl;
  struct element *cur;
  TANGO_IMAGE tmp[10];
  TANGO_PATH path;
  TANGO_TRANS trans;

  for( width=0, tl=largest; (tl/10.0)>=1.0; width++, tl=tl/10.0 );
  width *= .018; 
  width += .04;
  /* This for loop draws a blue rectangle and puts the list element inside it */
  for( i=0, y=.05, cur=head; i<total; i++,y+=.06, cur=cur->next )
  {
    sprintf(str,"%d",cur->num);
    recs[i] = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.1, y, 1, TANGO_COLOR_BLUE, width, .04, 0);  
    aelms[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.115, y+.03, 1, TANGO_COLOR_BLACK,NULL, str,0);   
  }
  /* This for loop draws green boxes and puts queue labels on them */
  for( i=0, y=.055; i<10; i++, y+=.1 ) 
  {
    sprintf(str,"%d",i);
    tmp[i] = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.29, y-.03, 1, TANGO_COLOR_GREEN, .04, .04, 1.0);
    qs[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.3, y, 1, TANGO_COLOR_BLACK, NULL, str, 0);
  }
  /* This is the red position line */
  dot = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, .1+width-.03, .02, 1, TANGO_COLOR_RED, 0.0, .95, 0.0, 1.0, 0 ); 
  path = TANGOpath_null(3);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
  TANGOtrans_perform(trans);
  TANGOpath_free(1,path);
  TANGOtrans_free(1,trans);
}

/*  This routine moves an element indexed by count to the queue labeled rem */
void ANIMToQueue( count, rem )
  int count;
  int rem;
{
  TANGO_LOC qloc,tloc;
  TANGO_PATH p, path;
  TANGO_TRANS movet;

  qloc = TANGOimage_loc(qs[rem], TANGO_PART_TYPE_NE);
  qloc = TANGOloc_modify( qloc, .03+(width*qfill[rem]), .025 );
  tloc = TANGOimage_loc(aelms[count], TANGO_PART_TYPE_SW);
  p = TANGOpath_type( TANGO_PATH_TYPE_STRAIGHT );
  path = TANGOpath_example( tloc, qloc, p ); 
  movet = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, aelms[count], path );
  TANGOtrans_perform(movet);
  qfill[rem]++;

  TANGOpath_free(2,path,p);
  TANGOtrans_free(1,movet);
}


/*  This routine takes all the elements from the queues in order and puts
    them back in the list to sort again .  Then it moves the position line  */
void ANIMMoveBack()
{
  int i;
  TANGO_LOC locr, now;
  TANGO_IMAGE tmp[25];
  TANGO_PATH p, path, r;
  TANGO_TRANS move;

  struct element *cur;

  p = TANGOpath_type( TANGO_PATH_TYPE_STRAIGHT );
  r = TANGOpath_rotate(p,180);
  for( cur = head, i=0; cur!=NULL; cur=cur->next, i++ )
  {
    tmp[i] = aelms[cur->tag];
    locr = TANGOimage_loc(recs[i], TANGO_PART_TYPE_SE );
    locr = TANGOloc_modify( locr, -.011, -.006 );
    now = TANGOimage_loc( aelms[cur->tag], TANGO_PART_TYPE_SE);
    path = TANGOpath_example( now, locr, r );
    move = TANGOtrans_create( TANGO_TRANS_TYPE_MOVE, aelms[cur->tag], path );
    cur->tag = i;
    TANGOtrans_perform(move);
    TANGOpath_free(1,path);
    TANGOtrans_free(1,move);
  }
  /* move position line */
  now = TANGOimage_loc(dot, TANGO_PART_TYPE_S );
  locr = TANGOloc_modify( now, -.018, 0 );
  path = TANGOpath_example( now, locr, r );
  move = TANGOtrans_create( TANGO_TRANS_TYPE_MOVE, dot, path );
  TANGOtrans_perform(move);
  TANGOtrans_free(1,move);
  TANGOpath_free(3,p,r,path); 
 
  for( i=0; i<total; i++ )
  {
    aelms[i] = tmp[i];
  } 
  /* reset queue offsets */ 
  for( i=0; i<10; i++ )
    qfill[i] = 0.0;
}

