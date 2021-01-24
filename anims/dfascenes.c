/*****************************************************************************

	XTango scene code for DFA simulator.

	Scott Robertson

*****************************************************************************/

#include <stdio.h>
#include <math.h>
#include "xtango.h"
#include "dfa.h"

#define   SIZE	 	0.05
#define	  PATHINCR	0.05
#define   FCOLOR   	TANGO_COLOR_RED

TANGO_IMAGE  stop;
TANGO_IMAGE  A;
TANGO_IMAGE  B;
TANGO_IMAGE  A_B;
TANGO_IMAGE  dot;

typedef struct {
  TANGO_IMAGE edge;
  TANGO_PATH path;
  enum outputType output;
} Edge_t;

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
   ASSOCmake("START",1);
   ASSOCmake("FINAL",1);

   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.05,0.025,1,TANGO_COLOR_RED,NULL,"STOP",1);
   stop = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,0.0,1,TANGO_COLOR_RED,
			       0.1,0.05,0.0);

   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.16,0.025,1,TANGO_COLOR_ORANGE,NULL,"A",1);
   A = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.11,0.00,1,TANGO_COLOR_ORANGE,
			       0.1,0.05,0.0);

   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.27,0.025,1,TANGO_COLOR_BLUE,NULL,"B",1);
   B = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.22,0.00,1,TANGO_COLOR_BLUE,
			       0.1,0.05,0.0);

   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.38,0.025,1,TANGO_COLOR_MAROON,NULL,"A/B",1);
   A_B = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.33,0.00,1,TANGO_COLOR_MAROON,
			       0.1,0.05,0.0);

   dot = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.0,0.0,0,TANGO_COLOR_RED,
	.02,1.0);

}




/* get the locations of the vertices */

char *
ANIMVertex(index)
   int index;
{
   TANGO_LOC	 loc;
   TANGO_IMAGE	 node,label,start_arrow;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;
   char 	 str[3];
   double	 x,y;

   if (TANGOinput_coord(&x,&y))
      { if ((0.0<=x) && (x<=0.1) && (0.0<=y) && (y<=0.05))  /* chose to stop */
	   return(NULL);
	loc = TANGOloc_create(x,y);
	ASSOCstore("VERTEX",index,loc);
	node = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x,y,1,
				    TANGO_COLOR_BLACK,SIZE,0.0);
	/* label start symbol */
	if (index == 0) {
	  start_arrow = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x-SIZE-.05,y,
		1,TANGO_COLOR_RED,.05,0.0,1.0,1.0,1);
	}
	ASSOCstore("NODE",index,node);
	ASSOCstore("INPUT",node,index);
	ASSOCstore("EDGE",index,index,NULL);
	ASSOCstore("FINAL", index, 0);
	sprintf(str,"%d",index);
	label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,
				     "variable",str,1);
	ASSOCstore("LABEL",index,label);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,node,path);
	TANGOtrans_perform(trans);
	return((char *)1);
      }
   else
      return(NULL);
}



/* get the the adjacency matrix */

char *
ANIMEdge()
{
   TANGO_LOC	 v1,v2;
   TANGO_IMAGE	 node1,node2,outType,transOut;
   TANGO_PATH	 path, subpath[2];
   TANGO_TRANS	 trans;
   Edge_t	 *Edge;
   int		 index1,index2;
   double	 x1,y1,x2,y2;
   double	 vx[2], vy[2];
   double	 x,y,x_offset[2],y_offset[2];
   char 	 *ret;

   if (TANGOinput_image(&node1))
      { if (node1 == stop)
	   { ret = (char *) malloc(8);
	     sprintf(ret,"-1 -1 -1");
	     return(ret);
	   }

	TANGOtrans_perform(TAPcolor(node1,TANGO_COLOR_RED));

	if (!TANGOinput_image(&node2))
	   return(NULL);
	if (node2 == stop)
	   { ret = (char *) malloc(8);
	     sprintf(ret,"-1 -1 -1");
	     return(ret);
	   }

	TANGOtrans_perform(TAPcolor(node2,TANGO_COLOR_RED));

        TANGOinput_coord(&x,&y);

	index1 = ASSOCretrieve("INPUT",node1);
	index2 = ASSOCretrieve("INPUT",node2);
	v1 = (TANGO_LOC) ASSOCretrieve("VERTEX",index1);
	v2 = (TANGO_LOC) ASSOCretrieve("VERTEX",index2);
	TANGOloc_inquire(v1,&x1,&y1);
	TANGOloc_inquire(v2,&x2,&y2);

        x_offset[0] = (x - x1) * (SIZE / sqrt ((x-x1)*(x-x1)+(y-y1)*(y-y1)));
        y_offset[0] = (y - y1) * (SIZE / sqrt ((x-x1)*(x-x1)+(y-y1)*(y-y1)));
        x_offset[1] = (x2 - x) * (SIZE / sqrt ((x2-x)*(x2-x)+(y2-y)*(y2-y)));
        y_offset[1] = (y2 - y) * (SIZE / sqrt ((x2-x)*(x2-x)+(y2-y)*(y2-y)));
        x1 += x_offset[0];
        y1 += y_offset[0];
        x2 -= x_offset[1];
        y2 -= y_offset[1];
        vx[0] = x - x1; vx[1] = x2 - x1;
        vy[0] = y - y1; vy[1] = y2 - y1;

	Edge = (Edge_t *) malloc (sizeof (Edge_t));
	Edge->edge = TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE,x1,y1,
		1,TANGO_COLOR_BLACK,3,vx,vy,0.0,1.0,1);
	subpath[0] = TANGOpath_distance(TANGOimage_loc(node1,TANGO_PART_TYPE_C),
		TANGOloc_create(x,y),PATHINCR);
	subpath[1] = TANGOpath_distance(TANGOloc_create(x,y),
		TANGOimage_loc(node2,TANGO_PART_TYPE_C),PATHINCR);
	Edge->path = TANGOpath_concatenate(2,subpath[0],subpath[1]);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER,Edge->edge,path);
	TANGOtrans_perform(trans);
	TANGOtrans_perform(TAPcolor(node1,TANGO_COLOR_BLACK));
	TANGOtrans_perform(TAPcolor(node2,TANGO_COLOR_BLACK));

	/* annotate edges with output characters (A|B) */

	TANGOinput_image(&outType);
	if (outType == B) {
   	  transOut = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,
		TANGO_COLOR_BLUE,NULL,"B",1);
	  Edge->output = Bout;
	}
	else if (outType == A_B) {
   	  transOut = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,
		TANGO_COLOR_MAROON,NULL,"A/B",1);
	  Edge->output = ABout;
	}
	else {
   	  transOut = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,
		TANGO_COLOR_ORANGE,NULL,"A",1);
	  Edge->output = Aout;
	}
	TAPshow(transOut);

	ASSOCstore("EDGE",index1,index2,Edge);

	ret = (char *) malloc(12);
	sprintf(ret,"%d %d %d",index1,index2, Edge->output);
	return(ret);
      }
   else
      return(NULL);
}

char *
ANIMFinal()
{
   TANGO_IMAGE node;
   int index;
   double x, y;
   char *ret;

   if (TANGOinput_image(&node)) {
      if (node == stop)
	return NULL;
   }

   index = ASSOCretrieve("INPUT",node);
   ASSOCstore("FINAL", index, 1);

   TANGOloc_inquire((TANGO_LOC) ASSOCretrieve("VERTEX",index),&x,&y);

   node = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x,y,1,
        TANGO_COLOR_BLACK,SIZE-.005,0.0);
   TAPshow(node);

   ret = (char *) malloc (sizeof (int));
   sprintf (ret, "%d", index);
   return ret;
}

void
ANIMBeginTraverse(from)
int from;
{
  static first_call = 1;
  TANGO_IMAGE node;
  TANGO_TRANS jump;

  node = (TANGO_IMAGE) ASSOCretrieve("NODE",from);

  jump = TAPjump(dot,TANGO_PART_TYPE_C,TANGOimage_loc(node,TANGO_PART_TYPE_C));
  TANGOtrans_perform(jump);

  if (first_call) {
    TANGOtrans_perform(TAPvis_toggle(dot));
    first_call = 0;
  }
  TAPshow(dot);
}

void
ANIMFlashDot()
{
  TANGOtrans_perform(TAPflash(dot,20));
}

void
ANIMTraverse(from, to)
int from, to;
{
  Edge_t *Edge;

  Edge = (Edge_t*) ASSOCretrieve("EDGE",from,to);
  
  TANGOtrans_perform(TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,dot,Edge->path));
}

