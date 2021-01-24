#include <stdio.h>
#include <math.h>
#include <xtango.h>

#define SPEED 0.01


void
ANIMInit(id,n)
   int id,n;
{
   int edges;

   ASSOCinit();
   ASSOCmake("EDGE",1);
   edges = (int) log2( (double)n );
   TWISTcreate_bintree(id,0.1,0.7,0.9,0.1,edges);
   TWISTcreate_loc_array(NULL,id,n,1,0.1,0.9,0.8/(double)(n-1));
}


void
ANIMAddNode(id,num,val)
   int id,num,val;
{
   char 	  str[10];
   double	  x,y,sx,sy,nx,ny;
   TANGO_LOC	  loc,parloc;
   TANGO_IMAGE	  node,pnode,edge;
   TANGO_PATH	  path;
   TANGO_TRANS	  trans;

   sprintf(str,"%d",val);
   loc = (TANGO_LOC) ASSOCretrieve("NUMBER",id,num-1);
   TANGOloc_inquire(loc,&x,&y);
   node = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,"variable",str,1);
   ASSOCstore("IMAGE_AT",id,loc,node);
   TANGOloc_inquire(TANGOimage_loc(node,TANGO_PART_TYPE_N),&nx,&ny);

   parloc = (TANGO_LOC) ASSOCretrieve("PARENT",id,loc);
   if (parloc)
      { pnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,parloc);
	TANGOloc_inquire(TANGOimage_loc(pnode,TANGO_PART_TYPE_S),&sx,&sy);
	edge = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,sx,sy,1,TANGO_COLOR_BLACK,
				    nx-sx,ny-sy,0.5,1.0,0);
	ASSOCstore("EDGE",loc,edge);
      }

   path = TANGOpath_null(16);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
}


void
ANIMElevate(id,num)
   int id, num;
{
   double	rx,ry,tx,ty,nx,ny;
   TANGO_LOC	rootloc,toloc,nloc;
   TANGO_IMAGE	rootnode,node,edge;
   TANGO_PATH	path,onepath;
   TANGO_TRANS	mover,vis,both;

   rootloc = (TANGO_LOC) ASSOCretrieve("NUMBER",id,0);
   TANGOloc_inquire(rootloc,&rx,&ry);
   toloc = (TANGO_LOC) ASSOCretrieve("ID",id,num-1);
   TANGOloc_inquire(toloc,&tx,&ty);
   rootnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,rootloc);
   path = TANGOpath_distance(rootloc,toloc,SPEED);
   mover = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rootnode,path);
   TANGOtrans_perform(mover);

   if (num != 1)
      { nloc = (TANGO_LOC) ASSOCretrieve("NUMBER",id,num-1);
	TANGOloc_inquire(nloc,&nx,&ny);
	node = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,nloc);
	path = TANGOpath_distance(nloc,rootloc,SPEED);
	mover = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,node,path);

	edge = (TANGO_IMAGE) ASSOCretrieve("EDGE",nloc);
	onepath = TANGOpath_null(1);
	vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,edge,onepath);
	both = TANGOtrans_compose(2,mover,vis);
	TANGOtrans_perform(both);

	ASSOCstore("IMAGE_AT",id,rootloc,node);
      }
}


void
ANIMCompare1(id,num)
   int id,num;
{
   TANGO_LOC	nloc,lloc;
   TANGO_IMAGE	node,lnode;
   TANGO_PATH	red,black,flip,flippath;
   TANGO_TRANS	f1,f2,compare;

   nloc = (TANGO_LOC) ASSOCretrieve("NUMBER",id,num-1);
   node = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,nloc);
   lloc = (TANGO_LOC) ASSOCretrieve("LCHILD",id,nloc);
   lnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,lloc);

   red = TANGOpath_color(TANGO_COLOR_RED);
   black = TANGOpath_color(TANGO_COLOR_BLACK);
   flip = TANGOpath_concatenate(6,red,red,red,black,black,black);
   flippath = TANGOpath_iterate(flip,4.0);

   f1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,node,flippath);
   f2 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,lnode,flippath);
   compare = TANGOtrans_compose(2,f1,f2);
   TANGOtrans_perform(compare);
}




void
ANIMCompare2(id,num)
   int id,num;
{
   TANGO_LOC	nloc,lloc,rloc;
   TANGO_IMAGE	node,lnode,rnode;
   TANGO_PATH	red,black,flip,flippath;
   TANGO_TRANS	f1,f2,f3,compare;

   nloc = (TANGO_LOC) ASSOCretrieve("NUMBER",id,num-1);
   node = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,nloc);
   lloc = (TANGO_LOC) ASSOCretrieve("LCHILD",id,nloc);
   lnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,lloc);
   rloc = (TANGO_LOC) ASSOCretrieve("RCHILD",id,nloc);
   rnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,rloc);

   red = TANGOpath_color(TANGO_COLOR_RED);
   black = TANGOpath_color(TANGO_COLOR_BLACK);
   flip = TANGOpath_concatenate(6,red,red,red,black,black,black);
   flippath = TANGOpath_iterate(flip,4.0);

   f1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,node,flippath);
   f2 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,lnode,flippath);
   f3 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,rnode,flippath);
   compare = TANGOtrans_compose(3,f1,f2,f3);
   TANGOtrans_perform(compare);
}


void
ANIMExchange(id,i,j)
   int id,i,j;
{
   double	x1,y1,x2,y2;
   TANGO_LOC	loc1,loc2;
   TANGO_IMAGE	node1,node2;
   TANGO_PATH	path1,path2;
   TANGO_TRANS	move1,move2,mover;

   loc1 = (TANGO_LOC) ASSOCretrieve("NUMBER",id,i-1);
   TANGOloc_inquire(loc1,&x1,&y1);
   node1 = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,loc1);
   loc2 = (TANGO_LOC) ASSOCretrieve("NUMBER",id,j-1);
   TANGOloc_inquire(loc2,&x2,&y2);
   node2 = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,loc2);

   path1 = TANGOpath_distance(loc1,loc2,SPEED);
   path2 = TANGOpath_distance(loc2,loc1,SPEED);
   move1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,node1,path1);
   move2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,node2,path2);
   mover = TANGOtrans_compose(2,move1,move2);
   TANGOtrans_perform(mover);

   ASSOCstore("IMAGE_AT",id,loc1,node2);
   ASSOCstore("IMAGE_AT",id,loc2,node1);
}
