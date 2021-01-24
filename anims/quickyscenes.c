/* Quicksort developed by Charles Hardnett */

#include <stdio.h>
#include <xtango.h>

#define SX 	0.0
#define SY	0.85
#define PDX     0.05
#define   MAXHEIGHT .3



/*  create an association */

void
ANIMInit()
{
   ASSOCinit();
   ASSOCmake("DATA",2);
   ASSOCmake("ITEMS",0);
}





/* save the values in the array */

void
ANIMInput(id,index,val)
   int id;
   int index;
   int val;
{
   ASSOCstore("DATA",id,index,val);  /* save the values in the array */
}



void
ANIMDraw(id,n)
   int id;
   int n;
{
   int	       i,value,vals[100], max_val,j;
   double      side,x,y,side2,yvals[100],width;
   
   TANGO_LOC   loc, ctr;
   TANGO_IMAGE image;
   TANGO_PATH  path;
   TANGO_TRANS trans;

   side = 1.0 / n;
   side2 = side / 2.0;

vals[0] = max_val = (int) ASSOCretrieve("DATA",id,0);
   for (i=1; i<n; i++) {
      j = (int) ASSOCretrieve("DATA",id,i);
      vals[i] = j;
      if (j > max_val) max_val = j;
      }

   for (i=0; i<n; ++i)     /* scale array elts from 0.0->1.0 */
      yvals[i] = (double)vals[i] / (double)max_val;
  width =side2;

   TWISTcreate_image_array(NULL,id,n,TANGO_IMAGE_TYPE_RECTANGLE,1,0,SX,SY,
                     NULL,width,yvals,MAXHEIGHT,width,1,TANGO_COLOR_BLUE,0.5);

   ASSOCstore("ITEMS",n);

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}


void
ANIMShuffle(id,ind,color)
   int id;
   int ind;
   int color;

{
   int     	value, val2;
   double	x, y;
   
   TANGO_LOC    loc; 
   TANGO_PATH   path;
   TANGO_IMAGE	image;
   TANGO_TRANS  trans;

   
   if (color == 0)
      path = TANGOpath_color(TANGO_COLOR_RED);
   else
      if (color == 1)
         path = TANGOpath_color(TANGO_COLOR_BLUE);
      else
         if (color == 2)
            path = TANGOpath_color(TANGO_COLOR_GREEN);



   image = (TANGO_IMAGE) ASSOCretrieve("ID",id,ind);

   trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,image,path);

   TANGOtrans_perform(trans);

   TANGOtrans_free(1,trans);
   TANGOpath_free(1,path);
}



void
ANIMMove(id, start, end, move)
    int id, start, end, move;

{
   int           i;
   TANGO_LOC     hiloc, loloc;
   TANGO_PATH    thepath;
   TANGO_TRANS   group,helper;
   TANGO_IMAGE   element;


 if (start < end)
 {
   hiloc = TANGOloc_create(SX,SY);
   loloc = TANGOloc_create(SX,SY+0.4);

   if (move)
      thepath = TANGOpath_distance(hiloc, loloc, PDX);
   else
      thepath = TANGOpath_distance(loloc, hiloc, PDX);

   element = (TANGO_IMAGE) ASSOCretrieve("ID",id, start);

   group = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, element, thepath);

   for (i = (start + 1); i <= end; i++) 
   {
      element = (TANGO_IMAGE) ASSOCretrieve("ID",id,i);

      helper = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, element, thepath);

      group = TANGOtrans_compose(2, group, helper);

   }
   TANGOtrans_perform(group);

 TANGOtrans_free(2,group,helper);
 TANGOpath_free(1,thepath);
 }

}



void
ANIMExchange(p1,p2,p3,p4)
   int	p1;
   int	p2;
   int	p3;
   int	p4;
{
   int val1, val2, n;
   TANGO_LOC loc1a, loc1b, loc2a, loc2b;
   TANGO_IMAGE rect1, rect2;
   TANGO_PATH onepath, path1, path2, toright, toleft;
   TANGO_TRANS move1_2, move2_1, flip;

   val1 = (int)ASSOCretrieve("DATA", p1, p2);
   val2 = (int)ASSOCretrieve("DATA", p3, p4);
   n = (int)ASSOCretrieve("ITEMS");

   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID", p1, p2);

   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID", p3, p4);

   loc1a = TANGOimage_loc(rect1, TANGO_PART_TYPE_S);
   loc1b = (TANGO_LOC) ASSOCretrieve("ID", p3, p4);

   loc2a = TANGOimage_loc(rect2, TANGO_PART_TYPE_S);
   loc2b = (TANGO_LOC) ASSOCretrieve("ID", p1, p2);

   toright = TANGOpath_type(TANGO_PATH_TYPE_CLOCKWISE);
   toleft = TANGOpath_type(TANGO_PATH_TYPE_COUNTERCLOCKWISE);

   path1 = TANGOpath_example(loc1a, loc2a, toright);
   path2 = TANGOpath_example(loc2a, loc1a, toleft);

   move1_2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect1, path1);
   move2_1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, path2);
   
   flip = TANGOtrans_compose(2, move1_2, move2_1);


   TANGOtrans_perform(flip);

   ASSOCstore("DATA", p3, p4, val1);
   ASSOCstore("DATA", p1, p2, val2);
   ASSOCstore("ID",p1,p2,rect2);
   ASSOCstore("ID",p3,p4,rect1);
   TANGOpath_free(2, path1, path2);
   TANGOtrans_free(3, move2_1, move1_2, flip);
}
