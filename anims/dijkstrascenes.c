/*======================================================================*/
/*----------------------------------------------------------------------*/
/* ssspscenes.c - Single Source Shortest Paths                          */
/*          An XTango animation of Dijkstra's SSSP algorithm            */
/*          by Anthony G. Francis, Jr.                                  */
/*----------------------------------------------------------------------*/
/*======================================================================*/

/*======================================================================*/
/* #include Files                                                       */
/*======================================================================*/
#include <stdio.h>
#include <math.h>
#include "xtango.h"


/*======================================================================*/
/* #define Statements                                                   */
/*======================================================================*/
#define   SIZE	   0.04
#define   FCOLOR   TANGO_COLOR_RED
#define   TCOLOR   TANGO_COLOR_BLUE
#define   XCOLOR   TANGO_COLOR_YELLOW


/*======================================================================*/
/* XTango Declarations                                                  */
/*======================================================================*/
TANGO_IMAGE  stop;
TANGO_IMAGE  comp;
TANGO_IMAGE  oldedge=NULL;
TANGO_IMAGE  oldnode=NULL;


/*======================================================================*/
/* ANIMInit								*/
/* 	create location and adj matrix associations; display screen	*/
/*======================================================================*/
void
ANIMInit()
{
   /*-Make the associations for further use-----------------------------*/
   ASSOCinit();
   ASSOCmake("VERTEX",1);
   ASSOCmake("NODE",1);
   ASSOCmake("SIZE",1);
   ASSOCmake("INPUT",1);
   ASSOCmake("LABEL",1);
   ASSOCmake("EDGE",2);

   /*-Create the Stop box-----------------------------------------------*/
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.05, 0.025, 1,
		     TANGO_COLOR_RED, NULL, "STOP", 1);
   stop = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.0, 0.0, 1,
			    TANGO_COLOR_RED, 0.1, 0.05, 0.0);

   /*-Create an additional Comparisons box------------------------------*/
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.50, 0.825, 1,
		     TANGO_COLOR_RED, NULL, "Comparison", 1);
   comp = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.4, 0.8, 1,
			    TANGO_COLOR_RED, 0.2, 0.2, 0.0);

   /*-Display all the structures used so far----------------------------*/
   TANGOtrans_perform (
	TANGOtrans_create(
		TANGO_TRANS_TYPE_REFRESH,stop,TANGOpath_null(1)));
}




/*======================================================================*/
/* ANIMVertex								*/
/* 	get the locations of the vertices - virtually unmodified	*/
/*======================================================================*/
char *
ANIMVertex(index)
   int index;
{
   TANGO_LOC	 loc;
   TANGO_IMAGE	 node,label;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;
   char 	 str[3],*ret;
   double	 x,y,temp;

   if (TANGOinput_coord(&x,&y))
      { if ((0.0<=x) && (x<=0.1) && (0.0<=y) && (y<=0.05))  /* chose to stop */
	   return(NULL);
	loc = TANGOloc_create(x,y);
	ASSOCstore("VERTEX",index,loc);
	node = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x,y,1,
				    TANGO_COLOR_BLACK,SIZE/2,0.0);
	ASSOCstore("NODE",index,node);
	ASSOCstore("INPUT",node,index);
	ASSOCstore("EDGE",index,index,NULL);
	temp = (double) SIZE/2.0;
	ASSOCstore("SIZE",index,temp);
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



/*======================================================================*/
/* ANIMEdge								*/
/* 	get the adjacency matrix - virtually unmodified			*/
/*======================================================================*/
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
	edge = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
				 x1,y1,1,TANGO_COLOR_BLACK,
				 x2-x1,y2-y1,0.25,1.0,0);
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



/*======================================================================*/
/* ANIMGetNode								*/
/*	Get the start node and mark it with an identifying ring		*/
/*======================================================================*/
char *
ANIMGetNode()
{
   TANGO_IMAGE 	image;
   TANGO_LOC	loc;
   int 		index;
   char 	*ret;

   /* If we get a user pick on an image ... */
   if (TANGOinput_image(&image)) { 
      /* Then retrieve the index of that image */
      index = ASSOCretrieve("INPUT",image);

      /* And mark it so that it is known as the first node */
      loc   = (TANGO_LOC) ASSOCretrieve("VERTEX",index);
      TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 
     	 TANGOloc_X(loc), TANGOloc_Y(loc),
	 1, TANGO_COLOR_BLACK,SIZE,0.0);

      ret = (char *) malloc(8);
      sprintf(ret,"%d",index);
      return(ret);
      }
   else
      return("0");
}



/*======================================================================*/
/* ANIMExamine								*/
/*	Formerly used as the primary explication tool, this virtually	*/
/*	unmodified function highlights the edge under consideration.	*/
/*======================================================================*/
void
ANIMExamine(from, to)
   int	from, to;
{
   static double dashed_x[1] = { 0.0 };
   static double dashed_y[1] = { -0.5 };
   static double solid_x[1] = { 0.0 };
   static double solid_y[1] = { 0.5 };
   TANGO_IMAGE edge;
   TANGO_PATH dashed, solid;
   TANGO_PATH new,old;
   TANGO_TRANS t1, t2, flash, repeat;

   /* Get the edge */
   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE", from, to);

   /* Set up the line types */
   dashed = TANGOpath_create(1, dashed_x, dashed_y);
   solid = TANGOpath_create(1, solid_x, solid_y);

   /* Set up the transformations */
   t1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, edge, dashed);
   t2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, edge, solid);
   flash = TANGOtrans_concatenate(2, t1, t2);
   repeat = TANGOtrans_iterate(flash, 4);

   /* Make the line flash */
   TANGOtrans_perform(repeat);
}

/*----------------------------------------------------------------------*/
/* ANIMCompare								*/
/*	Make a comparison between the node in the comparison box	*/
/*	and some new node; switch them if neccessary.			*/
/*----------------------------------------------------------------------*/
void
ANIMCompare(iOld,iNew,iSwitch)
   int iOld, iNew, iSwitch;
{
   /*-Define all TANGO terms for this move------------------------------*/
   TANGO_LOC	tlOldStart, tlOldFinish, tlNewStart, tlNewFinish;
   TANGO_IMAGE 	tiOldNode, tiNewNode;
   TANGO_PATH  	tpOldOut, tpOldIn, tpNewOut, tpNewIn, tpColorF, tpColorX;
   TANGO_TRANS 	ttColorF, ttColorX, ttOldMove, ttNewMove, ttCompMove;

   /*-Retrieve the old node and set up its motions----------------------*/
   tiOldNode	= (TANGO_IMAGE) ASSOCretrieve("NODE",iOld);
   tlOldStart	= (TANGO_LOC) ASSOCretrieve("VERTEX",iOld);
   tlOldFinish	= TANGOloc_create (0.5,0.9);
   tpOldOut	= TANGOpath_motion (tlOldStart,tlOldFinish,
				    TANGO_PATH_TYPE_STRAIGHT);
   tpOldIn	= TANGOpath_motion (tlOldFinish,tlOldStart,
				    TANGO_PATH_TYPE_STRAIGHT);

   /*-Retrieve the new node and set up its motions----------------------*/
   tiNewNode	= (TANGO_IMAGE) ASSOCretrieve("NODE",iNew);
   tlNewStart	= TANGOimage_loc (tiNewNode,TANGO_PART_TYPE_C);
   tlNewFinish	= TANGOloc_create (0.5,0.9);
   tpNewOut	= TANGOpath_motion (tlNewStart,tlNewFinish,
				    TANGO_PATH_TYPE_STRAIGHT);
   tpNewIn	= TANGOpath_motion (tlNewFinish,tlNewStart,
				    TANGO_PATH_TYPE_STRAIGHT);

   /*-Move them down to the compare position----------------------------*/
   ttNewMove 	= TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, 
				     tiNewNode, tpNewOut);
   TANGOtrans_perform(ttNewMove);

   /*-Should we switch to the smaller one?------------------------------*/
   if (iSwitch) {
      /*-Yes, perform the swap------------------------------------------*/
      tpColorF 	= TANGOpath_color(FCOLOR);
      tpColorX	= TANGOpath_color(XCOLOR);
      ttColorF	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, 
				    tiOldNode, tpColorF); 
      ttColorX	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, 
				    tiNewNode, tpColorX); 
      ttCompMove= TANGOtrans_compose(2,ttColorF,ttColorX);
      TANGOtrans_perform(ttCompMove);

      /*-Return the oldster to his vertex-------------------------------*/
      ttOldMove = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, 
				     tiOldNode, tpOldIn);
      TANGOtrans_perform(ttOldMove);
      }

    else {
      /*-No, don't perform the swap-------------------------------------*/
      /*-Return the new node to his vertex------------------------------*/
      ttNewMove	= TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, 
				     tiNewNode, tpNewIn);
      TANGOtrans_perform(ttNewMove);
      }
}

/*----------------------------------------------------------------------*/
/* ANIMDump								*/
/*	Dump the first node in each iteration into the comparison box	*/
/*----------------------------------------------------------------------*/
void
ANIMDump(iNode)
   int iNode;
{
   /*-Define all TANGO terms for this move------------------------------*/
   TANGO_LOC	tlNodeStart, tlNodeFinish;
   TANGO_IMAGE 	tiNode;
   TANGO_PATH  	tpNodeOut, tpColorX;
   TANGO_TRANS 	ttColorX, ttNodeMove;

   /*-Retrieve the node and set up its motions--------------------------*/
   tiNode	= (TANGO_IMAGE) ASSOCretrieve("NODE",iNode);
   tlNodeStart	= TANGOimage_loc (tiNode,TANGO_PART_TYPE_C);
   tlNodeFinish	= TANGOloc_create (0.5,0.9);
   tpNodeOut	= TANGOpath_motion (tlNodeStart,tlNodeFinish,
				    TANGO_PATH_TYPE_STRAIGHT);

   /*-Color it badd-----------------------------------------------------*/
   tpColorX	= TANGOpath_color(XCOLOR);
   ttColorX	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,tiNode,tpColorX); 
   TANGOtrans_perform(ttColorX);

   /*-Move it down to the compare position------------------------------*/
   ttNodeMove 	= TANGOtrans_create (TANGO_TRANS_TYPE_MOVE,tiNode,tpNodeOut);
   TANGOtrans_perform(ttNodeMove);
}

/*----------------------------------------------------------------------*/
/* ANIMZuck								*/
/*	Pull the last node out of the comparison box back to its vertex */
/*----------------------------------------------------------------------*/
void
ANIMZuck(iNode)
   int iNode;
{
   /*-Define all TANGO terms for this move------------------------------*/
   TANGO_LOC	tlNodeStart, tlNodeFinish;
   TANGO_IMAGE 	tiNode;
   TANGO_PATH  	tpNodeIn, tpColorX;
   TANGO_TRANS 	ttColorX, ttNodeMove;

   /*-Retrieve the node and set up its motions--------------------------*/
   tiNode	= (TANGO_IMAGE) ASSOCretrieve("NODE",iNode);
   tlNodeStart	= (TANGO_LOC) ASSOCretrieve("VERTEX",iNode);
   tlNodeFinish	= TANGOloc_create (0.5,0.9);
   tpNodeIn	= TANGOpath_motion (tlNodeFinish,tlNodeStart,
				    TANGO_PATH_TYPE_STRAIGHT);

   /*-Color it badd-----------------------------------------------------*/
   tpColorX	= TANGOpath_color(XCOLOR);
   ttColorX	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,tiNode,tpColorX); 
   TANGOtrans_perform(ttColorX);

   /*-Move it down to the compare position------------------------------*/
   ttNodeMove 	= TANGOtrans_create (TANGO_TRANS_TYPE_MOVE,tiNode,tpNodeIn);
   TANGOtrans_perform(ttNodeMove);
}

/*----------------------------------------------------------------------*/
/* ANIMNewMin								*/
/*	We have a new minimum path to a node. If there was no previous 	*/
/*	edge, that's ok, but if there was, then the old edge may	*/
/*	need decoloring.						*/
/*----------------------------------------------------------------------*/
void
ANIMNewMin(from,to)
   int	from, to;
{
   /*-Declare TANGO variables-------------------------------------------*/
   TANGO_IMAGE edge, node;
   TANGO_PATH  path1, path2;
   TANGO_TRANS color1, color2, ncolor1, ncolor2, both;

   /*-Get the edge and node structures associated with this edge--------*/
   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE", from, to);
   node = (TANGO_IMAGE) ASSOCretrieve("NODE", to);

   /*-Set up the colors-------------------------------------------------*/
   path2 	= TANGOpath_color(XCOLOR);
   color2 	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, path2);
   ncolor2 	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, node, path2);


   /*-Do we have an old edge?-------------------------------------------*/
   if (oldedge) { 

      /*-If so, change it-----------------------------------------------*/
      path1 	= TANGOpath_color(FCOLOR);
      color1 	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, oldedge, path1);
      ncolor1 	= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, oldnode, path1);
      both 	= TANGOtrans_compose(4,color1,color2,ncolor1,ncolor2);
      }

   else {
      /*-If not, don't change it----------------------------------------*/
      both 	= TANGOtrans_compose(2,color2,ncolor2);
      }

   /*-Perform the actual transformation---------------------------------*/
   TANGOtrans_perform(both);
   oldedge = edge;
   oldnode = node;
}


/*----------------------------------------------------------------------*/
/* ANIMNewMin								*/
/*	We have a new minimum path to a node. If there was no previous 	*/
/*	edge, that's ok, but if there was, then the old edge may	*/
/*	need decoloring.						*/
/*----------------------------------------------------------------------*/
void
ANIMAddFringe(from,to,size)
   int from,to;
   double size;
{
   /*-TANGO Declarations------------------------------------------------*/
   static double fillpath_x[1]   = { 1.0 };
   static double fillpath_y[1]   = { 0.0 };
   static double resizepath_x[1] = { 0.0 };
   static double resizepath_y[1] = { 0.0 };

   TANGO_LOC backloc;
   TANGO_IMAGE edge, node, label, back;
   TANGO_PATH fillpath, colorpath, resizepath, onepath, whitepath, fpath;
   TANGO_TRANS filler, colorer, resizer, raiser, white, fringe, all;

   /*-Build a black backdrop for each label in the graph----------------*/
   backloc = (TANGO_LOC) ASSOCretrieve("VERTEX",to);
   back = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
			    TANGOloc_X(backloc),
			    TANGOloc_Y(backloc),
			    1, TANGO_COLOR_BLACK,SIZE/2,1.0);
   onepath = TANGOpath_null(1);
   raiser = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER, back, onepath);
   TANGOtrans_perform(raiser);

   /*-Perform the neccessary processing to create a new fringe piece----*/
   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE",from,to);
   if (edge)
      { fpath = TANGOpath_color(FCOLOR);
	fringe = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, fpath);
	TANGOtrans_perform(fringe);
      }

   /*-Fill in the circle------------------------------------------------*/
   node = (TANGO_IMAGE) ASSOCretrieve("NODE", to);
   fillpath = TANGOpath_create(1, fillpath_x, fillpath_y);
   filler = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, node, fillpath);

   /*-Fix the circle's color--------------------------------------------*/
   colorpath = TANGOpath_color(FCOLOR);
   colorer = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, node, colorpath);

   /*-Resize the node to represent total distance from start------------*/
   resizepath_x[0]=(double)size/20.0;
   resizepath = TANGOpath_create(2,resizepath_x,resizepath_y);
   resizer = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,node,resizepath);

   /*-Move the label to the top of the screen---------------------------*/
   label = (TANGO_IMAGE) ASSOCretrieve("LABEL", to);
   onepath = TANGOpath_null(1);
   raiser = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, label, onepath);

   /*-Turn the label white----------------------------------------------*/
   whitepath = TANGOpath_color(TANGO_COLOR_WHITE);
   white = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,label,whitepath);

   /*-Apply all of these transformations--------------------------------*/
   all = TANGOtrans_compose(5, filler, colorer, resizer, raiser, white);
   TANGOtrans_perform(all);
}



/*----------------------------------------------------------------------*/
/* ANIMShorter								*/
/*	We have a new minimum path to a node. If there was no previous 	*/
/*	edge, that's ok, but if there was, then the old edge may	*/
/*	need decoloring.						*/
/*----------------------------------------------------------------------*/
void
ANIMShorter(from,to,size)
   int from,to;
   double size;
{
   /*-TANGO Declarations------------------------------------------------*/
   TANGO_IMAGE edge,   node;
   TANGO_PATH  fpath,  resizepath;
   TANGO_TRANS fringe, resizer, all;

   static double resizepath_x[1] = { 0.0 };
   static double resizepath_y[1] = { 0.0 };

   /*-Get the edge and node structures----------------------------------*/
   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE",from,to);
   node = (TANGO_IMAGE) ASSOCretrieve("NODE",to);

   /*-Build the recoloration paths--------------------------------------*/
   fpath = TANGOpath_color(FCOLOR);
   fringe = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, fpath);

   /*-Build the resize path - we'll have a new distance from the source-*/
   resizepath_x[0]=(double)size/20.0;
   resizepath = TANGOpath_create(2,resizepath_x,resizepath_y);
   resizer = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,node,resizepath);

   /*-Display all paths-------------------------------------------------*/
   all = TANGOtrans_compose(2, fringe, resizer);
   TANGOtrans_perform(all);
}



/*----------------------------------------------------------------------*/
/* ANIMAdd								*/
/* 	Add this node to the list of nodes in S				*/
/*----------------------------------------------------------------------*/
void
ANIMAdd(n)
   int	n;
{
   /*-TANGO Declarations------------------------------------------------*/
   TANGO_IMAGE node;
   TANGO_PATH  color, thickpath;
   TANGO_TRANS ncolor, ecolor, ethicken, all;
   static double px[1] = { 1.0 };
   static double py[1] = { 0.0 };

   /*-Retrieve the node-------------------------------------------------*/
   node = (TANGO_IMAGE) ASSOCretrieve("NODE", n);

   /*-set up the color change paths-------------------------------------*/
   color = TANGOpath_color(TCOLOR);
   ncolor = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, node, color);

   /*-Do we have an old edge?-------------------------------------------*/
   if (oldedge) { 
      /*-Yes, we do and we need to recolor it---------------------------*/
      ecolor = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,oldedge,color);
      thickpath = TANGOpath_create(1, px, py);
      ethicken = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,oldedge,thickpath);
      all = TANGOtrans_compose(3,ncolor,ecolor,ethicken);
      TANGOtrans_perform(all);
      }
   else
      /*-No, just fix the node's color----------------------------------*/
      TANGOtrans_perform(ncolor);

   /*-Reset the Old Edge counter----------------------------------------*/
   oldedge = NULL;
}



