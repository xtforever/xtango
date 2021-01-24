#include <stdio.h>
#include "xtango.h"

#define   SIZE	 0.04
#define   FCOLOR   TANGO_COLOR_RED

TANGO_IMAGE  stop;
TANGO_IMAGE  oldedge=NULL;

/* create location and adj matrix associations */

void
ANIMInit()
{
   ASSOCinit();
   ASSOCmake("VERTEX",1);
   ASSOCmake("NODE",1);
   ASSOCmake("INPUT",1);
   ASSOCmake("LABEL",1);
   ASSOCmake("EDGE",2);

   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.05,0.025,1,TANGO_COLOR_RED,NULL,"STOP",1);
   stop = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,0.0,1,TANGO_COLOR_RED,
			       0.1,0.05,0.0);
}




/* get the locations of the vertices */

char *
ANIMVertex(index)
   int index;
{
   TANGO_LOC	 loc;
   TANGO_IMAGE	 node,label;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;
   char 	 str[3],*ret;
   double	 x,y;

   if (TANGOinput_coord(&x,&y))
      { if ((0.0<=x) && (x<=0.1) && (0.0<=y) && (y<=0.05))  /* chose to stop */
	   return(NULL);
	loc = TANGOloc_create(x,y);
	ASSOCstore("VERTEX",index,loc);
	node = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x-SIZE/2.0,y-SIZE/2.0,1,
				    TANGO_COLOR_BLACK,SIZE,SIZE,0.0);
	ASSOCstore("NODE",index,node);
	ASSOCstore("INPUT",node,index);
	ASSOCstore("EDGE",index,index,NULL);
	sprintf(str,"%d",index);
	label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,
				     "variable",str,1);
	ASSOCstore("LABEL",index,label);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,node,path);
	TANGOtrans_perform(trans);
	ret = (char *) malloc(64);
	sprintf(ret,"%lf %lf",x,y);
	return(ret);
      }
   else
      return(NULL);
}



/* get the the adjacency matrix */

char *
ANIMEdge()
{
   TANGO_LOC	 v1,v2;
   TANGO_IMAGE	 node1,node2,edge;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;
   int		 index1,index2;
   double	 x1,y1,x2,y2;
   char 	 *ret;

   if (TANGOinput_image(&node1))
      { if (node1 == stop)
	   { ret = (char *) malloc(8);
	     sprintf(ret,"-1 -1");
	     return(ret);
	   }

	if (!TANGOinput_image(&node2))
	   return(NULL);
	if (node2 == stop)
	   { ret = (char *) malloc(8);
	     sprintf(ret,"-1 -1");
	     return(ret);
	   }

	index1 = ASSOCretrieve("INPUT",node1);
	index2 = ASSOCretrieve("INPUT",node2);
	v1 = (TANGO_LOC) ASSOCretrieve("VERTEX",index1);
	v2 = (TANGO_LOC) ASSOCretrieve("VERTEX",index2);
	TANGOloc_inquire(v1,&x1,&y1);
	TANGOloc_inquire(v2,&x2,&y2);
	edge = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,TANGO_COLOR_BLACK,
				    x2-x1,y2-y1,0.0,1.0,0);
	ASSOCstore("EDGE",index1,index2,edge);
	ASSOCstore("EDGE",index2,index1,edge);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER,edge,path);
	TANGOtrans_perform(trans);
	ret = (char *) malloc(8);
	sprintf(ret,"%d %d",index1,index2);
	return(ret);
      }
   else
      return(NULL);
}



void
ANIMShorter(from,to)
   int from,to;
{
   double x = 1.0;
   TANGO_IMAGE edge;
   TANGO_PATH fpath,path2;
   TANGO_TRANS fringe,wider,both;

   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE",from,to);
   fpath = TANGOpath_color(FCOLOR);
   path2 = TANGOpath_create(1,&x,&x);
   fringe = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, fpath);
   wider = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, edge, path2);
   both = TANGOtrans_compose(2,fringe,wider);
   TANGOtrans_perform(both);
}

