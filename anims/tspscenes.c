/*----------------------------------------------------------------------

     Scenes for the travelling salesman problem 

     G.R.Hillhouse
----------------------------------------------------------------------*/


#include <stdio.h>
#include "xtango.h"

#define   SIZE	 0.04
#define   FCOLOR   TANGO_COLOR_RED
#define   TCOLOR   TANGO_COLOR_BLUE
#define   XCOLOR   TANGO_COLOR_GREEN

#define   SPEED    0.05

TANGO_IMAGE  stop;
TANGO_IMAGE  oldedge=NULL;

TANGO_COLOR PINK;
TANGO_COLOR PURPLE;
TANGO_COLOR HOTPINK;
TANGO_COLOR WHITE;


void ANIMSalesWalk (from, to)
  int from, to;
{
  TANGO_LOC   v1, v2;
  TANGO_IMAGE salesrep, node1, label1;
  TANGO_PATH  movepath, path1;
  TANGO_TRANS walk, drawpt, drawlabel, draw_vertex;
  double      x1, y1, x2, y2;
  double      xpts[1], ypts[1];


   v1 = (TANGO_LOC) ASSOCretrieve("VERTEX",from);
   v2 = (TANGO_LOC) ASSOCretrieve("VERTEX",to);
   TANGOloc_inquire(v1,&x1,&y1);
   TANGOloc_inquire(v2,&x2,&y2);

   salesrep = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x1-SIZE/4.0,y1-SIZE/4.0,
                                 1, HOTPINK, SIZE/2, 1.0);

   movepath = TANGOpath_distance (v1, v2, SPEED);
   walk = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, salesrep, movepath);
   TANGOtrans_perform(walk);


   node1 = (TANGO_IMAGE) ASSOCretrieve ("NODE", from);
   label1 = (TANGO_IMAGE) ASSOCretrieve ("LABEL", from);
   path1 = TANGOpath_null(1);
   drawpt = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, node1, path1);
   drawlabel = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, label1, path1);
   draw_vertex = TANGOtrans_compose (2, drawpt, drawlabel);
   TANGOtrans_perform (draw_vertex);
}




void ANIMShowPath(from, to)
   int	from, to;
{
   TANGO_IMAGE edge, node1, node2, label1, label2;
   TANGO_PATH  path, path1, path2, path3, path4;
   TANGO_TRANS t, t0, t1,t2,t3, t4, t5;
  
  /* Display path between from & to vertices */ 

   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE", from, to);
   path = TANGOpath_color(TANGO_COLOR_BLUE);
   t = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, path);
   t0 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, edge, path);

   /* Display from & to vertices with their labels */

   node1 = (TANGO_IMAGE) ASSOCretrieve ("NODE", from);
   label1 = (TANGO_IMAGE) ASSOCretrieve ("LABEL", from);

   node2 = (TANGO_IMAGE) ASSOCretrieve ("NODE", to);
   label2 = (TANGO_IMAGE) ASSOCretrieve ("LABEL", to);

   path1 = TANGOpath_null(1);
   path2 = TANGOpath_null(1);
   path3 = TANGOpath_null(1);
   path4 = TANGOpath_null(1);

   t1 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, node1, path1);
   t2 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, node2, path2);
   t3 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, label1, path3);
   t4 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, label2, path4);
  
   t5 = TANGOtrans_compose (6, t, t0, t1, t2, t3, t4);
   TANGOtrans_perform(t5);
}





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
	ASSOCmake("ADJ",3);
	ASSOCmake("TEXT_AT",2);


   PINK = TANGOload_color ("deep pink");
   PURPLE = TANGOload_color ("PURPLE");
   HOTPINK = TANGOload_color ("Hot Pink");
   WHITE = TANGOload_color ("White");
	
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.05,0.025,1,TANGO_COLOR_RED,NULL,"STOP",1);
   stop = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,0.0,1,TANGO_COLOR_RED,
			       0.1, 0.05, 0.0);
}


void
ANIMClearScreen(node)
int node;
{
  TANGO_IMAGE blankscreen;
  TANGO_PATH  path;
  TANGO_TRANS trans;

  blankscreen = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.001, 0.001,
                                  0,TANGO_COLOR_WHITE, 0.99, 0.99, 1.0);

  path = TANGOpath_null(1);
  trans = TANGOtrans_create (TANGO_TRANS_TYPE_VISIBLE, blankscreen, path);
  TANGOtrans_perform (trans);
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
	node = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x-SIZE/2.0,y-SIZE/2.0,
                                 1, TANGO_COLOR_BLACK,SIZE,1.0);
	ASSOCstore("NODE",index,node);
	ASSOCstore("INPUT",node,index);
	ASSOCstore("EDGE",index,index,NULL);
	sprintf(str,"%d",index);
	label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,WHITE,
				     "variable",str,1);
	ASSOCstore("LABEL",index,label);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER,node,path);
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

   double        vxlist[1];
   double        vylist[1];

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
/*
	edge = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,PINK,
				    x2-x1,y2-y1,0.5,1.0,0);
*/
        vxlist[0] = x2-x1;
        vylist[0] = y2-y1;
	edge = TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE,x1,y1,1,PINK,2,
				    vxlist,vylist,0.5,1.0,0);
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



/***** this routine gets a starting node *****/

char *
ANIMGetNode()
{
   TANGO_IMAGE image;
   int index;
   char *ret;

   if (TANGOinput_image(&image))
      { index = ASSOCretrieve("INPUT",image);
	ret = (char *) malloc(8);
	sprintf(ret,"%d",index);
	return(ret);
      }
   else
      return("0");
}


/******** Neede for Prim, but not Kruskal's ****/
void
ANIMExamine(from, to)
   int	from, to;
{
   static double dashed_x[1] = { 0.0 };
   static double dashed_y[1] = { -0.5 };
   static double solid_x[1] = { 0.0 };
   static double solid_y[1] = { 0.5 };
   TANGO_IMAGE edge;
   TANGO_PATH dashed, solid, colorpath, path1, rotatepath;
   TANGO_TRANS t1, t2, flash, repeat, fringe, rotate, dashtrans, colortrans;
   


   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE", from, to);
   dashed = TANGOpath_create(1, dashed_x, dashed_y);
   solid = TANGOpath_create(1, solid_x, solid_y);
   t1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, edge, dashed);
   t2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, edge, solid);
   flash = TANGOtrans_concatenate(2, t1, t2);
   repeat = TANGOtrans_iterate(flash, 4);
   TANGOtrans_perform(repeat);
}


void
ANIMShorter(from,to)
   int from,to;
{
   TANGO_IMAGE edge;
   TANGO_PATH fpath, raise;
   TANGO_TRANS colors, top, highlight;

   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE",from,to);
   fpath = TANGOpath_color(XCOLOR);
   colors = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, fpath);

   raise = TANGOpath_null(1);
   top   = TANGOtrans_create (TANGO_TRANS_TYPE_RAISE, edge, raise);

   highlight = TANGOtrans_compose (2, colors, top);

   TANGOtrans_perform(colors);
}



void 
ANIMInput_vert(index,x,y)
   int index;
   double	x,y;

{
   TANGO_LOC	 loc;
   TANGO_IMAGE	 node,label;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;
   char 	 str[3];

	loc = TANGOloc_create(x,y);
	ASSOCstore("VERTEX",index,loc);
	node = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x,y,1,
				    PURPLE,SIZE,1.0);
	ASSOCstore("NODE",index,node);
	ASSOCstore("INPUT",node,index);
	ASSOCstore("EDGE",index,index,NULL);
	sprintf(str,"%d",index);
	label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,WHITE,
				     "variable",str,1);
	ASSOCstore("LABEL",index,label);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER,node,path);
	TANGOtrans_perform(trans);
}


/* get the the adjacency matrix */
void
ANIMInput_Edge(from,to)
	int from,to;
{
   TANGO_LOC	 v1,v2;
   TANGO_IMAGE	 node1,node2,edge;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;
   double	 x1,y1,x2,y2;
   double        vxlist[1];
   double        vylist[1];

	v1 = (TANGO_LOC) ASSOCretrieve("VERTEX",from);
	v2 = (TANGO_LOC) ASSOCretrieve("VERTEX",to);
	TANGOloc_inquire(v1,&x1,&y1);
	TANGOloc_inquire(v2,&x2,&y2);
/*
	edge = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,PINK,
				    x2-x1,y2-y1,0.5,1.0,0);
*/
        vxlist[0] = x2-x1;
        vylist[0] = y2-y1;
	edge = TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE,x1,y1,1,PINK,2,
				    vxlist,vylist,0.5,1.0,0);
	ASSOCstore("EDGE",from,to,edge);
	ASSOCstore("EDGE",to,from,edge);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER,edge,path);
	TANGOtrans_perform(trans);
}
