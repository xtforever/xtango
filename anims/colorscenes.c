
/* this program implements the graph coloring algorithm */


/* this is the program which makes calls to tango */

#include <stdio.h>
#include "xtango.h"

#define SPEED 0.05


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
				    x2-x1,y2-y1,0.5,1.0,0);
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
ANIMCG_Color(i,c)
int i,c;
{
   TANGO_IMAGE cur_node,cur_label;
   TANGO_TRANS m1,m2,m3,m4,m5,all;
   TANGO_PATH  p1,p2,p3,p4;
   double f = 1.0;
   double u = -1.0;

   cur_node = (TANGO_IMAGE) ASSOCretrieve("NODE",i);
   cur_label = (TANGO_IMAGE) ASSOCretrieve("LABEL",i);

   /* node image is flashed thrice before changing its color */
   p1 = TANGOpath_null(6);
   m1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,cur_node,p1);
   TANGOtrans_perform(m1);

   switch(c){
   case 0:
        /* the color assignment is cancelled and put back to black */
   	p3 = TANGOpath_color(TANGO_COLOR_BLACK);
	break;
   case 1:
   	p3 = TANGOpath_color(TANGO_COLOR_RED);
   	break;
   case 2:
   	p3 = TANGOpath_color(TANGO_COLOR_YELLOW);
   	break;
   case 3:
   	p3 = TANGOpath_color(TANGO_COLOR_GREEN);
   	break;
   case 4:
   	p3 = TANGOpath_color(TANGO_COLOR_ORANGE);
   	break;
   case 5:
   	p3 = TANGOpath_color(TANGO_COLOR_BLUE);
   	break;
   case 6:
   	p3 = TANGOpath_color(TANGO_COLOR_MAROON);
   	break;
   default: printf("Error in CG-Color\n");
   }
   if (c==0)
     p2 = TANGOpath_create(1,&u,&u);  /* unfill it */
   else
      p2 = TANGOpath_create(1,&f,&f);   /* fill it*/
   m2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,cur_node,p2);
   m3=TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,cur_node,p3);
   m4 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,cur_node,p3);
   m5 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,cur_label,p3);
   all = TANGOtrans_compose(4,m2,m3,m4,m5);

   TANGOtrans_perform(all);
}

