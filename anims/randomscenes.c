#include <stdio.h>
#include <xtango.h>


/*  create an association */

void
ANIMInit(id,n)
   int id,n;
{
   double width,height,radius,starty;
   double area,space;
   TANGO_IMAGE ptr;
   TANGO_PATH path;
   TANGO_TRANS trans;

   width = 0.8/(double)n;
   height = 0.1;
   radius = width/2.0;
   if (radius > 0.05)
      radius = 0.05;
   starty = 0.2;

   TWISTcreate_image_array(NULL,id,n,TANGO_IMAGE_TYPE_RECTANGLE,1,1,0.1,starty,NULL,width,
		     NULL,height,0.0,1,TANGO_COLOR_BLACK,0.0);

   ASSOCmake("OUTLINE",2);
   TWISTcreate_image_array("OUTLINE",id,n,TANGO_IMAGE_TYPE_RECTANGLE,1,1,0.1,starty,NULL,width,
		     NULL,height,0.0,1,TANGO_COLOR_BLACK,0.0);

   ASSOCmake("PTRPOS",2);
   TWISTcreate_loc_array("PTRPOS",id,n,1,0.1+(width/2.0),starty-(height/2.0)-radius,width);

   area = 1.0 - starty - (height/2.0);
   space = area/40.0;
   TWISTcreate_2d_loc_array(NULL,id,100,n,0.1+(width/2.0),starty+(height/2.0)+space,
			       width,space);

   ptr = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.5,0.05,1,TANGO_COLOR_RED,radius,1.0);
   ASSOCmake("PTR",0);
   ASSOCstore("PTR",ptr);

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}




void
ANIMPrimary(id, index,num)
   int	id,index,num;
{
   double	x, y;
   TANGO_LOC	ptrloc,toloc;
   TANGO_IMAGE	ptr;
   TANGO_PATH	movepath, colorpath;
   TANGO_TRANS	move, color, check;

   ptr = (TANGO_IMAGE) ASSOCretrieve("PTR");
   ptrloc = TANGOimage_loc(ptr, TANGO_PART_TYPE_C );
   toloc = (TANGO_LOC) ASSOCretrieve("PTRPOS", id, index);

   movepath = TANGOpath_distance(ptrloc, toloc, 1.0);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ptr, movepath);
   colorpath = TANGOpath_color(TANGO_COLOR_RED);
   color = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, ptr, colorpath);
   check = TANGOtrans_compose(2, move, color);
   TANGOtrans_perform(check);

   TANGOpath_free(2,movepath,colorpath);
   TANGOtrans_free(3,move,color,check);
}


void
ANIMSecondary(id, index,num)
   int	id,index,num;
{
   double	x, y;
   TANGO_LOC	ptrloc,toloc;
   TANGO_IMAGE	ptr;
   TANGO_PATH	movepath, colorpath;
   TANGO_TRANS	move, color, check;

   ptr = (TANGO_IMAGE) ASSOCretrieve("PTR");
   ptrloc = TANGOimage_loc(ptr, TANGO_PART_TYPE_C );
   toloc = (TANGO_LOC) ASSOCretrieve("PTRPOS", id, index);

   movepath = TANGOpath_distance(ptrloc, toloc, 1.0);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ptr, movepath);
   colorpath = TANGOpath_color(TANGO_COLOR_YELLOW);
   color = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, ptr, colorpath);
   check = TANGOtrans_compose(2, move, color);
   TANGOtrans_perform(check);

   TANGOpath_free(2,movepath,colorpath);
   TANGOtrans_free(3,move,color,check);
}


void
ANIMProbe(id,index,num)
   int	id,index,num;
{
   double	x, y;
   TANGO_LOC	probeloc;
   TANGO_IMAGE	probe, ptr;
   TANGO_PATH	onepath;
   TANGO_TRANS	appear,delay,adder;

   probeloc = (TANGO_LOC) ASSOCretrieve("ID3",id,num,index);
   TANGOloc_inquire(probeloc,&x,&y);
   probe = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x,y,1,TANGO_COLOR_BLACK,0.005,1.0);
   onepath = TANGOpath_null(1);
   appear = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, probe, onepath);
   delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, TANGOpath_iterate(onepath, 2.0));
   adder = TANGOtrans_compose(2,appear,delay);
   TANGOtrans_perform(adder);
}



void
ANIMFill(id,index)
   int	id,index;
{
   static double fillpath_x[1] = { 0.5 };
   static double fillpath_y[1] = { 0.0 };
   TANGO_IMAGE	 box, outline;
   TANGO_PATH	 fillpath;
   TANGO_TRANS	 fill, raise, both;

   box = (TANGO_IMAGE) ASSOCretrieve("ID", id, index);
   fillpath = TANGOpath_create(1, fillpath_x, fillpath_y);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, box, fillpath);

   outline = (TANGO_IMAGE) ASSOCretrieve("OUTLINE", id, index);
   raise = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, outline, fillpath);
   both = TANGOtrans_compose(2,fill,raise);
   TANGOtrans_perform(both);
}



