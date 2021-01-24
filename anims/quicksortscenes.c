#include <stdio.h>
#include <xtango.h>


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
   int	       i,value;
   double      side,x,y;
   TANGO_LOC   loc;
   TANGO_IMAGE image;
   TANGO_PATH  path;
   TANGO_TRANS trans;

   side = 1.0 / n;

   TWISTcreate_2d_loc_array(NULL,id,n,n,0.0,0.0,side,side);

   for (i=0; i<n; ++i)
      { value = (int)ASSOCretrieve("DATA",id,i);
	loc = (TANGO_LOC) ASSOCretrieve("ID3",id,n-1-value,i);
	TANGOloc_inquire(loc,&x,&y);
	image = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x,y,1,
                                  TANGO_COLOR_BLACK,side,side,1.0);
	ASSOCstore("ID",id,i,image);
      }

   ASSOCstore("ITEMS",n);

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
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
   TANGO_PATH onepath, path1, path2;
   TANGO_TRANS move1, move2, flip;

   val1 = (int)ASSOCretrieve("DATA", p1, p2);
   val2 = (int)ASSOCretrieve("DATA", p3, p4);
   n = (int)ASSOCretrieve("ITEMS");

   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID", p1, p2);
   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID", p3, p4);
   loc1a = TANGOimage_loc(rect1, TANGO_PART_TYPE_NW);
   loc1b = (TANGO_LOC) ASSOCretrieve("ID3", p1, n-1-val2, p2);
   loc2a = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   loc2b = (TANGO_LOC) ASSOCretrieve("ID3", p3, n-1-val1, p4);
   onepath = TANGOpath_null(1);
   path1 = TANGOpath_example(loc1a, loc1b, onepath);
   path2 = TANGOpath_example(loc2a, loc2b, onepath);
   move1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect1, path1);
   move2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, path2);
   flip = TANGOtrans_compose(2, move1, move2);
   TANGOtrans_perform(flip);
   ASSOCstore("DATA", p3, p4, val1);
   ASSOCstore("DATA", p1, p2, val2);
   TANGOpath_free(3, onepath, path1, path2);
   TANGOtrans_free(3, move1, move2, flip);
}



