#include <stdio.h>
#include "xtangolocal.h"

#define   SIZE	 0.04
#define   FCOLOR   TANGO_COLOR_RED
#define   TCOLOR   TANGO_COLOR_BLUE
#define   XCOLOR   TANGO_COLOR_YELLOW

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

struct _IMAGE ximage; 
struct _TANGO_TEXT xtext;
 ximage.type = TANGO_IMAGE_TYPE_TEXT;
 ximage.loc[0] = 0.05;
 ximage.loc[1] =  0.025;
 ximage.visible =1;
 xtext.color=TANGO_COLOR_RED;
  xtext.font_name[0]=0;

strcpy(xtext.text,"STOP");
 xtext.orient=1;
ximage.object = &xtext; 
stop=TANGOimage_create(&ximage);

 struct _TANGO_RECTANGLE rec;
 ximage.type = TANGO_IMAGE_TYPE_RECTANGLE;
 ximage.loc[0] = 0.0;
 ximage.loc[1] = 0.0;
 ximage.visible = 1;
 rec.color=TANGO_COLOR_RED;
 rec.size[0]=0.1;
 rec.size[1]=0.05;
 rec.fill = 0;
 ximage.object = &rec;   
 stop=TANGOimage_create(&ximage); 



//   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.05,0.025,1,TANGO_COLOR_RED,NULL,"STOP",1);
//   stop = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,0.0,1,TANGO_COLOR_RED,
//			       0.1,0.05,0.0);
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


struct _IMAGE ximage; 
struct _TANGO_RECTANGLE rec;
 ximage.type = TANGO_IMAGE_TYPE_RECTANGLE;
 ximage.loc[0] = x-SIZE/2.0;
ximage.loc[1] = y-SIZE/2.0;
 ximage.visible = 1;
 rec.color=TANGO_COLOR_BLACK;
 rec.size[0]=SIZE;
rec.size[1]=SIZE;
 rec.fill = 0;
ximage.object = &rec;   
node=TANGOimage_create(&ximage); 


//	node = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x-SIZE/2.0,y-SIZE/2.0,1,
//				    TANGO_COLOR_BLACK,SIZE,SIZE,0.0);
	ASSOCstore("NODE",index,node);
	ASSOCstore("INPUT",node,index);
	ASSOCstore("EDGE",index,index,NULL);
	sprintf(str,"%d",index);


struct _TANGO_TEXT xtext;
 ximage.type = TANGO_IMAGE_TYPE_TEXT;
 ximage.loc[0] = x;
 ximage.loc[1] = y;
 ximage.visible =1;
 xtext.color=TANGO_COLOR_BLACK;

strcpy(xtext.font_name,"variable");
strcpy(xtext.text,str);
 xtext.orient=1;
ximage.object = &xtext; 
label=TANGOimage_create(&ximage);



//	label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,
//				     "variable",str,1);
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

struct _IMAGE ximage; 
struct _TANGO_LINE line;
 ximage.type = TANGO_IMAGE_TYPE_LINE;
 ximage.loc[0] = x1;
 ximage.loc[1] = y1;
 ximage.visible =1;
 line.color=TANGO_COLOR_BLACK;
 line.size[0]=x2-x1;
 line.size[1]=y2-y1;
 line.width=0.5;
 line.style=1.0;
 line.arrow=0;
ximage.object = &line;   
edge=TANGOimage_create(&ximage); 


//	edge = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,TANGO_COLOR_BLACK,
//				    x2-x1,y2-y1,0.5,1.0,0);
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
   //   TANGO_PATH new,old;
   TANGO_TRANS t1, t2, flash, repeat;

   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE", from, to);
/* new = TANGOpath_color(TANGO_COLOR_BLACK);
   old = TANGOpath_color(FCOLOR);
   t1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,edge,new);
   t2 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,edge,old);  */
   dashed = TANGOpath_create(1, dashed_x, dashed_y);
   solid = TANGOpath_create(1, solid_x, solid_y);
   t1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, edge, dashed);
   t2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, edge, solid);
   flash = TANGOtrans_concatenate(2, t1, t2);
   repeat = TANGOtrans_iterate(flash, 4);
   TANGOtrans_perform(repeat);
}



void
ANIMNewMin(from,to)
   int	from, to;
{
   TANGO_IMAGE edge;
   TANGO_PATH  path1, path2;
   TANGO_TRANS color1, color2, both;

   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE", from, to);
   path2 = TANGOpath_color(XCOLOR);
   color2 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, path2);

   if (oldedge)
      { path1 = TANGOpath_color(FCOLOR);
	color1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, oldedge, path1);

	both = TANGOtrans_compose(2,color1,color2);
	TANGOtrans_perform(both);
      }
   else
      TANGOtrans_perform(color2);

   oldedge = edge;
}



void
ANIMAddFringe(from,to)
   int from,to;
{
   static double fillpath_x[1] = { 1.0 };
   static double fillpath_y[1] = { 0.0 };
   TANGO_IMAGE edge, node, label;
   TANGO_PATH fillpath, colorpath, onepath, whitepath, fpath;
   TANGO_TRANS filler, colorer, raiser, white, fringe, all;

   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE",from,to);
   if (edge)
      { fpath = TANGOpath_color(FCOLOR);
	fringe = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, fpath);
	TANGOtrans_perform(fringe);
      }

   node = (TANGO_IMAGE) ASSOCretrieve("NODE", to);
   fillpath = TANGOpath_create(1, fillpath_x, fillpath_y);
   filler = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, node, fillpath);

   colorpath = TANGOpath_color(FCOLOR);
   colorer = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, node, colorpath);

   label = (TANGO_IMAGE) ASSOCretrieve("LABEL", to);
   onepath = TANGOpath_null(1);
   raiser = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, label, onepath);

   whitepath = TANGOpath_color(TANGO_COLOR_WHITE);
   white = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,label,whitepath);

   all = TANGOtrans_compose(4, filler, colorer, raiser, white);
   TANGOtrans_perform(all);
}



void
ANIMShorter(from,to)
   int from,to;
{
   TANGO_IMAGE edge;
   TANGO_PATH fpath;
   TANGO_TRANS fringe;

   edge = (TANGO_IMAGE) ASSOCretrieve("EDGE",from,to);
   fpath = TANGOpath_color(FCOLOR);
   fringe = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, edge, fpath);
   TANGOtrans_perform(fringe);
}



void
ANIMAdd(n)
   int	n;
{
   TANGO_IMAGE node;
   TANGO_PATH  color, thickpath;
   TANGO_TRANS ncolor, ecolor, ethicken, all;
   static double px[1] = { 1.0 };
   static double py[1] = { 0.0 };

   node = (TANGO_IMAGE) ASSOCretrieve("NODE", n);
   color = TANGOpath_color(TCOLOR);
   ncolor = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, node, color);

   if (oldedge)
      { ecolor = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,oldedge,color);

	thickpath = TANGOpath_create(1, px, py);
	ethicken = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,oldedge,thickpath);

	all = TANGOtrans_compose(3,ncolor,ecolor,ethicken);
	TANGOtrans_perform(all);
      }
   else
      TANGOtrans_perform(ncolor);

   oldedge = NULL;
}



