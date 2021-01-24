#include <stdio.h>
#include <xtango.h>

int max = 0;

/*  Create an association */

void
ANIMInit()
{
   ASSOCinit();
   ASSOCmake("DATA",2);
}





/* save the values in the array */

void
ANIMInput(id,index,val)
   int id;
   int index;
   int val;
{
   ASSOCstore("DATA",id,index,val);  /* save the values in the array */
   if (val > max) max = val;
}




void
ANIMDraw(id,n)
   int id;
   int n;
{
   int	       i;
   double      vals[200];
   double      between,height,maxwidth,spacing;
   TANGO_PATH  path;
   TANGO_TRANS trans;

   for (i=0; i<n; ++i)
      { vals[i] = (double)((int)ASSOCretrieve("DATA",id,i)) / (double) max;
      }

   spacing = height = 0.8/ (double)(2*n-1);
   if (height < 0.0) {
     printf("Too many elements to display in window (not enough space)\n");
     exit(0);
   }

   maxwidth = 0.8;

   TWISTcreate_image_array(NULL,id,n,TANGO_IMAGE_TYPE_RECTANGLE,0,1,0.5,0.1,
              vals,maxwidth,NULL,height,spacing,1,TANGO_COLOR_BLACK,1.0);

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
   TANGO_LOC loc1, loc2;
   TANGO_IMAGE rect1, rect2;
   TANGO_PATH onepath, path1, path2;
   TANGO_TRANS move1, move2, flip;

   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID", p1, p2);
   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID", p3, p4);
   loc1 = TANGOimage_loc(rect1, TANGO_PART_TYPE_C );
   loc2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_C );
   onepath = TANGOpath_null(1);
   path1 = TANGOpath_example(loc1, loc2, onepath);
   path2 = TANGOpath_example(loc2, loc1, onepath);
   move1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect1, path1);
   move2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, path2);
   flip = TANGOtrans_compose(2, move1, move2);
   TANGOtrans_perform(flip);
   ASSOCstore("ID", p3, p4, rect1);
   ASSOCstore("ID", p1, p2, rect2);
   TANGOpath_free(3, onepath, path1, path2);
   TANGOtrans_free(3, move1, move2, flip);
}



