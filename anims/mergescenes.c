#include <stdio.h>
#include <xtango.h>



/* create the data association */

void
ANIMInit()
{
   TANGO_LOC loc;

   ASSOCinit();
   ASSOCmake("DATA",2);
   ASSOCmake("LOCATION",2);
   ASSOCmake("CENTER",1);
   loc = TANGOloc_create(0.495,0.5);
   ASSOCstore("CENTER",0,loc);
   loc = TANGOloc_create(0.505,0.5);
   ASSOCstore("CENTER",1,loc);
}




/* save the values of the two arrays to be merged (their ids will be different) */

void
ANIMInput(id,index,val)
   int id;
   int index;
   int val;
{
   ASSOCstore("DATA",id,index,val);  /* save values of the a and b arrays */
}





/* draw the two arrays to be merged in the top half of the window, and */
/* create an array of locs at the bottom where the "merged" elements   */
/* will go.							       */

void
ANIMDraw(ida,sizea,idb,sizeb,idc,sizec)
   int ida,sizea;
   int idb,sizeb;
   int idc,sizec;
{
   double width;   /* width of a rectangle image */
   int i;
   int item,max_val;
   int a[50],b[50];
   double yvals[50];   /* holds scaling factors */
   TANGO_PATH path;
   TANGO_TRANS trans;

#define   AX	   0.1	   /* starting window positions */
#define   AY	   0.45
#define   BX	   0.55
#define   BY	   0.45
#define   CX	   0.1
#define   CY	   0.9
#define   ROOM	   0.8
#define   MAX_HT   0.35

   width = ROOM /  (2.0 * (double)sizec);

   max_val = 0;
   for (i=0; i<sizea; ++i)   /* find largest data value in a */
      { if ((item = (int)ASSOCretrieve("DATA",ida,i)) > max_val)
	   max_val = item;
	a[i] =	item;
      }
   for (i=0; i<sizeb; ++i)   /* find largest data value in b */
      { if ((item = (int)ASSOCretrieve("DATA",idb,i)) > max_val)
	   max_val = item;
	b[i] =	item;
      }

   for (i=0; i<sizea; ++i)
      yvals[i] = (double)a[i] / (double)max_val;

   TWISTcreate_image_array(NULL,ida,sizea,TANGO_IMAGE_TYPE_RECTANGLE,1,0,AX,AY,NULL,width,
		     yvals,MAX_HT,width,1,TANGO_COLOR_RED,0.0);

   TWISTcreate_loc_array("LOCATION",ida,sizea,1,AX,AY,width*2.0);

   for (i=0; i<sizeb; ++i)
      yvals[i] = (double)b[i] / (double)max_val;

   TWISTcreate_image_array(NULL,idb,sizeb,TANGO_IMAGE_TYPE_RECTANGLE,1,0,BX,BY,NULL,width,
		     yvals,MAX_HT,width,1,TANGO_COLOR_BLUE,0.0);

   TWISTcreate_loc_array("LOCATION",idb,sizeb,1,BX,BY,width*2.0);

   TWISTcreate_loc_array("LOCATION",idc,sizec,1,CX,CY,width*2.0);

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}





/* make the two rectangles move to the middle of the screen next to each  */
/* other								  */

void
ANIMCompare(id1,n1,id2,n2)
   int id1;
   int n1;
   int id2;
   int n2;
{
   TANGO_LOC	 loc1,loc2,center1,center2;
   TANGO_IMAGE	 rect1,rect2;
   TANGO_PATH	 ptype1,ptype2,rot1,rot2,path1,path2;
   TANGO_TRANS	 move[2],compose;

   center1 = (TANGO_LOC) ASSOCretrieve("CENTER",0);
   center2 = (TANGO_LOC) ASSOCretrieve("CENTER",1);
   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID",id1,n1);
   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID",id2,n2);
   loc1 = TANGOimage_loc(rect1,TANGO_PART_TYPE_SE);
   loc2 = TANGOimage_loc(rect2,TANGO_PART_TYPE_SW);
   ptype1 = TANGOpath_type(TANGO_PATH_TYPE_COUNTERCLOCKWISE);
   ptype2 = TANGOpath_type(TANGO_PATH_TYPE_CLOCKWISE);
   rot1 = TANGOpath_rotate(ptype1,180);
   rot2 = TANGOpath_rotate(ptype2,180);
   path1 = TANGOpath_example(loc1,center1,rot1);
   path2 = TANGOpath_example(loc2,center2,rot2);
   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect1,path1);
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect2,path2);
   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);

   TANGOpath_free(6,ptype1,ptype2,rot1,rot2,path1,path2);
   TANGOtrans_free(3,move[0],move[1],compose);
}





/* make the smaller rectangle move to its destination spot in the c array */
/* at the bottom portion of the window, then fill it.  make the larger	  */
/* rectangle return to its place in its original array			  */

void
ANIMSmaller(smallid,smallidx,bigid,bigidx,cid,cidx)
   int smallid,smallidx;
   int bigid,bigidx;
   int cid,cidx;
{
   TANGO_LOC	  at1,to1,at2,to2;
   TANGO_IMAGE	  rect1,rect2;
   TANGO_PATH	  path1,path2,fillpath;
   TANGO_TRANS	  move[2],compose,fill;
   double	  x=1.0,y=0.0;

   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID",smallid,smallidx);
   at1 = TANGOimage_loc(rect1,TANGO_PART_TYPE_SW);
   to1 = (TANGO_LOC) ASSOCretrieve("LOCATION",cid,cidx);
   path1 = TANGOpath_distance(at1,to1,0.03);
   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect1,path1);

   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID",bigid,bigidx);
   at2 = TANGOimage_loc(rect2,TANGO_PART_TYPE_SW);
   to2 = (TANGO_LOC) ASSOCretrieve("LOCATION",bigid,bigidx);
   path2 = TANGOpath_distance(at2,to2,0.03);
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect2,path2);

   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);

   fillpath = TANGOpath_create(1,&x,&y);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect1,fillpath);
   TANGOtrans_perform(fill);

   TANGOpath_free(3,path1,path2,fillpath);
   TANGOtrans_free(4,move[0],move[1],compose,fill);
}








/* make the rectangle move to its destination spot in the c array at the */
/* bottom portion of the window, then fill it.				 */

void
ANIMFix(id1,n1,id2,n2)
   int *id1;
   int n1;
   int *id2;
   int n2;
{
   TANGO_LOC	  frompt,topt;
   TANGO_IMAGE	  rect;
   TANGO_PATH	  movepath,fillpath;
   TANGO_TRANS	  move,fill;
   double	  x=1.0,y=0.0;

   rect = (TANGO_IMAGE) ASSOCretrieve("ID",id1,n1);
   frompt = TANGOimage_loc(rect,TANGO_PART_TYPE_SW);
   topt = (TANGO_LOC) ASSOCretrieve("LOCATION",id2,n2);
   movepath = TANGOpath_distance(frompt,topt,0.03);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect,movepath);
   TANGOtrans_perform(move);

   fillpath = TANGOpath_create(1,&x,&y);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect,fillpath);
   TANGOtrans_perform(fill);

   TANGOpath_free(2,movepath,fillpath);
   TANGOtrans_free(2,move,fill);
}


