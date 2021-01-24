#include <stdio.h>
#include <xtango.h>


/*  create an association */

void
ANIMInit()
{
   ASSOCinit();
   ASSOCmake("DATA",2);
   ASSOCmake("PTRLOC",2);
   ASSOCmake("BALL",0);
   ASSOCmake("RIGHT",0);
   ASSOCmake("LEFT",0);
   ASSOCmake("PREV",0);
   ASSOCmake("TARGET",0);
   ASSOCstore("PREV",0);
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




/* draw the array as a row of rectangles with the heights corresponding */
/* to the value of each array element.					*/

void
ANIMDisplay(id,n)
   int id;
   int n;
{
   double	width,maxheight,max_val;
   int		i;
   int		vals[50];
   double	yvals[50];   /* holds scaling factors */
   TANGO_LOC	center,loc;
   TANGO_IMAGE	ball,rect,text,pointer;
   TANGO_PATH	path;
   TANGO_TRANS	delay;
   double	x,y;
   char 	str[5];


#define   SX	   0.1
#define   SIDE	   0.8

   width = SIDE /  (2.0 * (double)n);  /* each rectangle's width */
   maxheight = 0.4;

   max_val = (int) ASSOCretrieve("DATA",id,n-1);
   for (i=0; i<n; ++i)
      vals[i] = (int) ASSOCretrieve("DATA",id,i);
   for (i=0; i<n; ++i)	   /* scale array elts from 0.0->1.0 */
      yvals[i] = (double)vals[i] / (double)max_val;

   TWISTcreate_image_array(NULL,id,n,TANGO_IMAGE_TYPE_RECTANGLE,1,1,SX,0.5,
	      NULL,width,yvals,maxheight,width,1,TANGO_COLOR_BLACK,0.0);
   ball = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.05,0.5,1,TANGO_COLOR_BLACK,width/2.0,0.0);
   ASSOCstore("BALL",ball);
   for (i=0; i<n; ++i)
      { rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,i);
	center = TANGOimage_loc(rect,TANGO_PART_TYPE_C);
	TANGOloc_inquire(center,&x,&y);
	sprintf(str,"%d",vals[i]);
	text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,NULL,str,1);
      }

   TWISTcreate_loc_array("PTRLOC",id,n,1,SX+(width/2.0),0.5+maxheight/2.0+0.05,
			      2.0*width);
   loc = (TANGO_LOC) ASSOCretrieve("PTRLOC",id,0);
   TANGOloc_inquire(loc,&x,&y);
   pointer = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,"variable","L ",1);
   ASSOCstore("LEFT",pointer);
   loc = (TANGO_LOC) ASSOCretrieve("PTRLOC",id,n-1);
   TANGOloc_inquire(loc,&x,&y);
   pointer = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,"variable"," R",1);
   ASSOCstore("RIGHT",pointer);

   path = TANGOpath_null(5);
   delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,text,path);
   TANGOtrans_perform(delay);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,delay);
}







/* move a location pointer to reference one of the array elements */

void
ANIMTarget(val)
   int val;
{
   char 	 str[5];
   TANGO_IMAGE	 target;
   TANGO_PATH	 path;
   TANGO_TRANS	 show;

   sprintf(str,"%d",val);
   target = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.5,0.2,1,TANGO_COLOR_BLACK,"variable",str,1);
   ASSOCstore("TARGET",target);

   path = TANGOpath_null(3);
   show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(show);
}





void
ANIMTry(id,pos,left)
   int	id, pos, left;
{
   int		 oldspot;
   TANGO_LOC	 center, tocenter;
   TANGO_IMAGE	 ball, tonode;
   TANGO_PATH	 looper, bpath;
   TANGO_TRANS	 bmove;

   ball = (TANGO_IMAGE) ASSOCretrieve("BALL");
   center = TANGOimage_loc(ball, TANGO_PART_TYPE_C );
   tonode = (TANGO_IMAGE) ASSOCretrieve("ID", id, pos);
   tocenter = TANGOimage_loc(tonode, TANGO_PART_TYPE_C );
   oldspot = (int) ASSOCretrieve("PREV");
   if (oldspot < pos)
      looper = TANGOpath_type(TANGO_PATH_TYPE_CLOCKWISE);
   else
      looper = TANGOpath_type(TANGO_PATH_TYPE_COUNTERCLOCKWISE);
   bpath = TANGOpath_example(center, tocenter, looper);
   bmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ball, bpath);
   TANGOtrans_perform(bmove);
   ASSOCstore("PREV", pos);
}



ANIMMiss(id,pos)
   int id,pos;
{
   static double fillpath_x[1] = { 1.0 };
   static double fillpath_y[1] = { 0.0 };
   TANGO_IMAGE	 tonode;
   TANGO_PATH	 redpath, fillpath;
   TANGO_TRANS	 color, fill, trans;

   tonode = (TANGO_IMAGE) ASSOCretrieve("ID", id, pos);
   redpath = TANGOpath_color(TANGO_COLOR_RED);
   fillpath = TANGOpath_create(1, fillpath_x, fillpath_y);
   color = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, tonode, redpath);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, tonode, fillpath);
   trans = TANGOtrans_compose(2, color, fill);
   TANGOtrans_perform(trans);
}



ANIMHit(id,pos)
   int id,pos;
{
   static double fillpath_x[1] = { 1.0 };
   static double fillpath_y[1] = { 0.0 };
   TANGO_IMAGE	 tonode;
   TANGO_PATH	 greenpath, fillpath;
   TANGO_TRANS	 color, fill, trans;

   tonode = (TANGO_IMAGE) ASSOCretrieve("ID", id, pos);
   greenpath = TANGOpath_color(TANGO_COLOR_GREEN);
   fillpath = TANGOpath_create(1, fillpath_x, fillpath_y);
   color = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, tonode, greenpath);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, tonode, fillpath);
   trans = TANGOtrans_compose(2, color, fill);
   TANGOtrans_perform(trans);
}





void
ANIMMovePtr(id,pos,mleft)
   int	id,pos,mleft;
{
   double	 x,y;
   int		 val;
   TANGO_LOC	 fromloc, toloc;
   TANGO_IMAGE	 ptr;
   TANGO_PATH	 path;
   TANGO_TRANS	 movetext;

   if (mleft) { /* move the left */
      ptr = (TANGO_IMAGE) ASSOCretrieve("LEFT");
      val = pos + 1;
      }
   else {   /* move the right */
      ptr = (TANGO_IMAGE) ASSOCretrieve("RIGHT");
      val = pos - 1;
      }
   fromloc = TANGOimage_loc(ptr, TANGO_PART_TYPE_C );
   toloc = (TANGO_LOC) ASSOCretrieve("PTRLOC", id, val);

   path = TANGOpath_distance(fromloc, toloc, 0.020000);
   movetext = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ptr, path);
   TANGOtrans_perform(movetext);
}



void
ANIMCleanUp(id,n)
   int id,n;
{
   int		 i;
   static double x[1] = { -1.0 };
   static double y[1] = {  0.0 };
   TANGO_IMAGE	 node;
   TANGO_LOC	 loc, toloc;
   TANGO_PATH	 onepath, path, cpath;
   TANGO_TRANS	 vis, move1, move2, pmove, filler, colorer, both;

   onepath = TANGOpath_create(1, x, y);
   vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, ASSOCretrieve("TARGET"), onepath);
   TANGOtrans_perform(vis);

   loc = TANGOimage_loc( ASSOCretrieve("BALL"), TANGO_PART_TYPE_C);
   toloc = TANGOloc_create(0.05,0.5);
   path = TANGOpath_distance(loc, toloc, 1.0);
   move1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ASSOCretrieve("BALL"), path);
   TANGOtrans_perform(move1);

   loc = TANGOimage_loc( ASSOCretrieve("LEFT"), TANGO_PART_TYPE_C);
   toloc = (TANGO_LOC) ASSOCretrieve("PTRLOC", id, 0);
   path = TANGOpath_distance(loc, toloc, 1.0);
   move1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ASSOCretrieve("LEFT"), path);

   loc = TANGOimage_loc( ASSOCretrieve("RIGHT"), TANGO_PART_TYPE_C);
   toloc = (TANGO_LOC) ASSOCretrieve("PTRLOC", id, n-1);
   path = TANGOpath_distance(loc, toloc, 1.0);
   move2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ASSOCretrieve("RIGHT"), path);

   pmove = TANGOtrans_compose(2,move1,move2);
   TANGOtrans_perform(pmove);

   cpath = TANGOpath_color(TANGO_COLOR_BLACK);
   for (i=0; i<n; ++i) {
      node = (TANGO_IMAGE) ASSOCretrieve("ID", id, i);
      filler = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, node, onepath);
      colorer = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, node, cpath);
      both = TANGOtrans_compose(2, filler, colorer);
      TANGOtrans_perform(both);
      }

   ASSOCstore("PREV",0);
}
