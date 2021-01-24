#include <stdio.h>
#include "xtango.h"

#define   PLENGTH 10
#define   PDX       .05
#define   SX	   0.1
#define   SY       0.45
#define   MAXHEIGHT .3
#define   SIDE	   0.8

/*  create an association */

void
AInit()
{
   ASSOCinit();
   ASSOCmake("DATA",2);
}

/* save the values in the array */
void
AInput(id,index,val)
   int id;
   int index;
   int val;
{
   ASSOCstore("DATA",id,index,val);  /* save the values in the array */
}


/* draw the array as row of scaled rectangles */
void
ADraw(id,n)
   int id;
   int n;
{
   double	width;
   int		i,j,max_val;
   int		vals[100];
   double	yvals[100];   /* holds scaling factors */
   TANGO_LOC	center,loc;
   TANGO_IMAGE	rect,pointer;

   width = SIDE /  (2.0 * (double)n);  /* rectangle's width */

/* get highest value in array and hold it  */
   vals[0] = max_val = (int) ASSOCretrieve("DATA",id,0);
   for (i=1; i<n; i++) {
      j = (int) ASSOCretrieve("DATA",id,i);
      vals[i] = j;
      if (j > max_val) max_val = j;
      }

   for (i=0; i<n; ++i)	   /* scale array elts from 0.0->1.0 */
      yvals[i] = (double)vals[i] / (double)max_val;

   TWISTcreate_image_array(NULL,id,n,TANGO_IMAGE_TYPE_RECTANGLE,1,0,SX,SY,
	      NULL,width,yvals,MAXHEIGHT,width,1,TANGO_COLOR_BLACK,1.0);
}

void
AUncolor(id,n)
int id, n;
{
int             i;
TANGO_IMAGE     rect;
TANGO_PATH      path;
TANGO_TRANS     trans, helper;

path = TANGOpath_color(TANGO_COLOR_BLACK);
rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,0);
trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,rect,path);

for (i=1; i<n; i++) {
   rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,i);
   helper = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,rect,path);
   trans = TANGOtrans_compose(2, trans, helper); 
   }
TANGOtrans_perform( trans);
}


void
APart(id,shell,start,split, n)
   int	id, shell, start, split, n;
{
   int		 i;
   TANGO_LOC	 hiloc, loloc;
   TANGO_PATH	 downpath, colorpath;
   TANGO_TRANS	 mover,helper;
   TANGO_IMAGE   rect;

   hiloc = TANGOloc_create(0.0,0.45);
   loloc = TANGOloc_create(0.0,0.85);

   if (split) 
      downpath = TANGOpath_distance(hiloc, loloc, PDX);
   else 
      downpath = TANGOpath_distance(loloc, hiloc, PDX);

   rect = (TANGO_IMAGE) ASSOCretrieve("ID",id, start);
   mover = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect, downpath);
 
   for (i= start + shell; i < n; i+=shell) {
      rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,i);
      helper = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect, downpath);
      mover = TANGOtrans_compose(2, mover, helper); 
      }
   TANGOtrans_perform(mover);
   if (split) {
      rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,start);
      colorpath = TANGOpath_color(TANGO_COLOR_RED);
      helper = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,rect,colorpath);
      TANGOtrans_perform(helper);
      }
}



APass(id,left,right, first)
   int id,left,right, first;
{
   TANGO_IMAGE	 lrect, rrect;
   TANGO_PATH	 lpath, rpath;
   TANGO_TRANS	 lmove, rmove;
   TANGO_LOC     lloc,  rloc;

   lrect = (TANGO_IMAGE) ASSOCretrieve("ID",id,left);
   rrect = (TANGO_IMAGE) ASSOCretrieve("ID",id,right); 
   rloc = TANGOimage_loc(rrect,TANGO_PART_TYPE_SW);
   lloc = TANGOimage_loc(lrect,TANGO_PART_TYPE_SW);

   lpath = TANGOpath_distance(lloc, rloc, PDX);
   rpath = TANGOpath_distance(rloc, lloc, PDX);

   lmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, lrect, lpath);
   rmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rrect, rpath);
   rmove = TANGOtrans_compose(2, lmove, rmove);
   TANGOtrans_perform(rmove);
   if (left == first) {
      rpath = TANGOpath_color(TANGO_COLOR_RED);
      rmove = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, rrect, rpath);
      TANGOtrans_perform(rmove);
      }
   ASSOCstore("ID",id,left,rrect);
   ASSOCstore("ID",id,right,lrect);
}

ARebound(id,left,right)
   int id,left,right;
{
   TANGO_IMAGE	 lrect, rrect, rect;
   TANGO_PATH	 lpath, rpath, tpath, colorpath;
   TANGO_TRANS	 lmove, rmove, helper;
   TANGO_LOC     lloc,  rloc, cloc;

   lrect = (TANGO_IMAGE) ASSOCretrieve("ID",id,left);
   rrect = (TANGO_IMAGE) ASSOCretrieve("ID",id,right); 
   rloc = TANGOimage_loc(rrect,TANGO_PART_TYPE_SW);
   lloc = TANGOimage_loc(lrect,TANGO_PART_TYPE_SW);
   cloc = TANGOloc_create((TANGOloc_X(lloc)+TANGOloc_X(rloc))/2,TANGOloc_Y(lloc));
   lpath = TANGOpath_distance(lloc, cloc, PDX);
   tpath = TANGOpath_distance(cloc, lloc, PDX);
   lpath = TANGOpath_concatenate(2, lpath, tpath);
   rpath = TANGOpath_distance(rloc, cloc, PDX);
   tpath = TANGOpath_distance(cloc, rloc, PDX);
   rpath = TANGOpath_concatenate(2, rpath, tpath);

   lmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, lrect, lpath);
   rmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rrect, rpath);
   rmove = TANGOtrans_compose(2, lmove, rmove);
   TANGOtrans_perform(rmove);

   colorpath = TANGOpath_color(TANGO_COLOR_RED);
   helper = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,rrect,colorpath);
   TANGOtrans_perform(helper);
}
