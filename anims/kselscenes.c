

/********  K-Selection problem by Oliver Wang ****************/


#include <stdio.h>
#include "xtango.h"

void
ANIMInit()
{
   ASSOCinit();
   ASSOCmake("DATA",2);
   ASSOCmake("TEXT",1);  
   ASSOCmake("POINTER",0);
   ASSOCmake("PTRLOC",2);
   ASSOCmake("RECT", 1);
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
	    

void
ANIMDraw(id,n)
   int id;
   int n;
{
   double      width;	/* width of a rectangle image */
   int	       i;
   int	       item;
   int	       max_val;
   int	       vals[100];
   double      yvals[100];   /* holds scaling factors */
   TANGO_LOC	center;
   TANGO_IMAGE	rect,text,pointer;
   TANGO_PATH	path;
   TANGO_TRANS	delay;
   double      x,y;
   char        str[5];
   TANGO_COLOR c;


#define   SX	   0.02
#define   SY	   0.95
#define   SIDE	   0.2


   width = 2.4 *SIDE /  ( (double)n);  /* each rectangle's width */

   max_val = 0;
   for (i=0; i<n; ++i)	   /* find largest array elt */
      { if ((item = (int)ASSOCretrieve("DATA",id,i)) > max_val)
	   max_val = item;
	vals[i] = item;
      }

   for (i=0; i<n; ++i)	   /* scale array elts from 0.0->1.0 */
      yvals[i] = (double)vals[i] / (double)max_val;

   TWISTcreate_image_array(NULL,id,n,TANGO_IMAGE_TYPE_RECTANGLE,1,0,SX,SY,NULL,width,
		     yvals,SIDE,width,1,TANGO_COLOR_BLACK,0.0);

   TWISTcreate_loc_array("PTRLOC",id,n,1,SX+(width/2.0),SY+(width/2.0),2.0*width);
   pointer = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,2.0,2.0,1,
				  TANGO_COLOR_MAROON,width/2.0,1.0);
   ASSOCstore("POINTER",pointer);

/*   for (i=0; i<n; ++i)
      { rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,i);
	center = TANGOimage_loc(rect,TANGO_PART_TYPE_C);
	TANGOloc_inquire(center,&x,&y);
	sprintf(str,"%d",vals[i]);
	text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,NULL,str,1);
	ASSOCstore("TEXT",rect,text);
      } */

   path = TANGOpath_null(10);
   delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,text,path);
   TANGOtrans_perform(delay);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,delay);
}


/* move a location pointer to reference one of the array elements */

void
ANIMPointTo(id,n)
   int id,n;
{
   TANGO_IMAGE	 pointer;
   TANGO_LOC	 center, toloc;
   TANGO_PATH	 onepath, movepath;
   TANGO_TRANS	 move;

   pointer = (TANGO_IMAGE) ASSOCretrieve("POINTER");
   onepath = TANGOpath_null(1);

   center = TANGOimage_loc(pointer, TANGO_PART_TYPE_C);
   toloc = (TANGO_LOC) ASSOCretrieve("PTRLOC", id, n);
   movepath = TANGOpath_example(center, toloc, onepath);

   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, pointer, movepath);
   TANGOtrans_perform(move);

   TANGOpath_free(2, onepath, movepath);
   TANGOtrans_free(1, move);
}


void
ANIMFill(id,n)
int id;
int n;
{
  TANGO_IMAGE rect;
  TANGO_TRANS fill;

  rect = (TANGO_IMAGE) ASSOCretrieve("ID", id, n) ;
  fill = TAPfill(rect) ;

  TANGOtrans_perform(fill) ;
  
  TANGOtrans_free(1, fill);
}
  


/* make two rectangles simultaneously flash */

void
ANIMCompare(id1,n1,id2,n2)
   int id1;
   int n1;
   int id2;
   int n2;
{
   TANGO_IMAGE rect1,rect2;
   TANGO_PATH path,fillpath;
   TANGO_TRANS fill[2],compose;
   double flash[4];

   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID",id1,n1);
   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID",id2,n2);
   flash[0] = 0.5; /* halffill */
   flash[2] = -1.0; /* outline */
   flash[1] = flash[3] = 0.0;
   path = TANGOpath_create(4,flash,flash);
   fillpath = TANGOpath_iterate(path,4.0);
   fill[0] = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect1,fillpath);
   fill[1] = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect2,fillpath);
   compose = TANGOtrans_compose(2,fill[0],fill[1]);
   TANGOtrans_perform(compose);

   TANGOpath_free(2,path,fillpath);
   TANGOtrans_free(3,fill[0],fill[1],compose);
}





/* make the two rectangles exchange positions on the screen in one */
/* simultaneous movement					   */

void
ANIMExchange(id1,n1,id2,n2)
   int id1;
   int n1;
   int id2;
   int n2;
{
   TANGO_IMAGE rect1,rect2,text1,text2;
   TANGO_LOC   loc1,loc2;
   TANGO_PATH  expath1,expath2,path1,path2;
   TANGO_TRANS move[4],compose;

   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID",id1,n1);
   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID",id2,n2);
   loc1 = TANGOimage_loc(rect1,TANGO_PART_TYPE_SW);
   loc2 = TANGOimage_loc(rect2,TANGO_PART_TYPE_SW);
/*   text1 = (TANGO_IMAGE) ASSOCretrieve("TEXT",rect1);
   text2 = (TANGO_IMAGE) ASSOCretrieve("TEXT",rect2); */
   path1 = TANGOpath_motion(loc1,loc2,TANGO_PATH_TYPE_CLOCKWISE);
   path2 = TANGOpath_rotate(path1,180);
   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect1,path1);
/*   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text1,path1); */
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect2,path2);
/*   move[3] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text2,path2); */
   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);

   ASSOCstore("ID",id1,n1,rect2);  /* reset assocs due to exchange */
   ASSOCstore("ID",id2,n2,rect1);

   TANGOpath_free(2,path1,path2);
   TANGOtrans_free(3,move[0],move[1],compose);
}

void
ANIMFlash(id,n)
   int id;
   int n;
{
   TANGO_IMAGE  rect;
   TANGO_PATH path,fillpath;
   TANGO_TRANS fill ;
   double flash[4];

   rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,n);
   flash[0] = 0.5; /* halffill */
   flash[2] = -1.0; /* outline */
   flash[1] = flash[3] = 0.0;
   path = TANGOpath_create(4,flash,flash);
   fillpath = TANGOpath_iterate(path,4.0);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect,fillpath);

 /*  compose = TANGOtrans_compose(2,fill[0],fill[1]); */
   TANGOtrans_perform(fill);

   TANGOpath_free(2,path,fillpath);
   TANGOtrans_free(1,fill);
}



void
ANIMMove(id, n, dir)
int id;
int n;
double  dir;
{
   TANGO_IMAGE rect ;
   TANGO_LOC loc1, loc2;
   TANGO_PATH path;
   TANGO_TRANS move;

  rect =(TANGO_IMAGE)ASSOCretrieve("ID", id, n);
  loc1 = TANGOimage_loc(rect, TANGO_PART_TYPE_SW);
  loc2 = TANGOloc_create(TANGOloc_X(loc1), TANGOloc_Y(loc1)-0.4*dir) ;

  move = TAPjump(rect, TANGO_PART_TYPE_SW, loc2);
  TANGOtrans_perform(move); 
  TANGOtrans_free(1, move);
 }

void
ANIMRemove(id, n)
int id;
int n;
{  
   TANGO_IMAGE rect ;
   TANGO_TRANS move;

  rect =(TANGO_IMAGE)ASSOCretrieve("ID", id, n);

  move = TAPvis_toggle(rect);
  TANGOtrans_perform(move); 
  TANGOtrans_free(1, move);
 }
  
void 
ANIMShiftToLeft(id, j, i)
int id;
int j;
int i;

{
   TANGO_IMAGE rect1 ;
   TANGO_TRANS move;
   TANGO_LOC loc1,  loc ;
   double  x;
 
  rect1 =(TANGO_IMAGE)ASSOCretrieve("ID", id, j);

  loc1 = TANGOimage_loc(rect1, TANGO_PART_TYPE_SW);
     x = (TANGOloc_X(loc1)-0.02)*(double)(i+1)/(double)(j+1) + 0.02 ;
  loc = TANGOloc_create(x, 0.95) ;
  ASSOCstore("ID",id, i, rect1);

  move = TAPjump(rect1, TANGO_PART_TYPE_SW, loc);  
  TANGOtrans_perform(move); 
  TANGOtrans_free(1, move);
 
 }

/* alter the fill style of the rectangle to show it is "in-place" */

void
ANIMIn_place(id,n)
   int id;
   int n;
{
   TANGO_IMAGE rect,text;
   TANGO_PATH  path,col;
   TANGO_TRANS fill,color, clean;
   double f = 1.0;

   rect = (TANGO_IMAGE) ASSOCretrieve("ID",id,n);
   path = TANGOpath_create(1,&f,&f);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect,path);
   TANGOtrans_perform(fill);


   clean = TANGOtrans_create(TANGO_TRANS_TYPE_REFRESH,NULL,path);
   TANGOtrans_perform(clean);

   TANGOpath_free(1,path);
   TANGOtrans_free(1,fill);
}

void 
ANIMColor(id,n,cn)
int id;
int n;
int cn;
{ TANGO_IMAGE rect ;
  TANGO_TRANS move;
  TANGO_COLOR c ;
  
  rect =(TANGO_IMAGE) ASSOCretrieve("ID", id, n) ;

 if (cn==1)
  move = TAPcolor(rect, TANGO_COLOR_RED);
 if (cn==2)
  move = TAPcolor(rect, TANGO_COLOR_BLACK);
  TANGOtrans_perform(move); 
  TANGOtrans_free(1, move);
 }

  
void
ANIMPutText(str, lx,  ly, i)
char *str;
double lx;
double ly;
int i ;
{  TANGO_IMAGE text;
  TANGO_TRANS delay ;
  TANGO_PATH path ;
  TANGO_COLOR c;
   
   text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, lx, ly,1, TANGO_COLOR_RED,
                 NULL, str, 1) ;
  ASSOCstore("TEXT", i, text) ;
  path = TANGOpath_null(1) ;
  
  delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, text, path);
  TANGOtrans_perform(delay);
  TANGOpath_free(1, path) ;
  TANGOtrans_free(1,delay) ;
 }

void
ANIMOutline(id,i)
int id;
int i;
{ double lx ;
  double ly;
  double sx;
  TANGO_LOC loc1, loc2 ;
  TANGO_IMAGE rect, rect0 ;
  TANGO_PATH path;
  TANGO_TRANS delay ;
  TANGO_COLOR c ;

 rect0 = (TANGO_IMAGE) ASSOCretrieve("ID", id , i*5 - 5 ) ;
 loc2 = TANGOimage_loc(rect0, TANGO_PART_TYPE_SW) ;
 loc1 = TANGOimage_loc(rect0, TANGO_PART_TYPE_SE) ;


 lx = TANGOloc_X(loc2) ;
 sx = 9.5 * (TANGOloc_X(loc1) - TANGOloc_X(loc2)) ;
 ly = TANGOloc_Y(loc1) - 0.2;

 rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, lx, ly, 1,
  TANGO_COLOR_RED, sx,0.2, 0.0 ) ;
 ASSOCstore("RECT", i, rect );

  path = TANGOpath_null(1) ;
  
  delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,rect, path);
  TANGOtrans_perform(delay);
  TANGOpath_free(1, path) ;
  TANGOtrans_free(1,delay) ;
  
}

void
ANIMRemoveRect(i)
int i;
{ TANGO_IMAGE rect ;
  TANGO_TRANS move;
  
  rect = (TANGO_IMAGE) ASSOCretrieve("RECT", i) ;

  move = TAPvis_toggle(rect);
  TANGOtrans_perform(move); 
  TANGOtrans_free(1, move);
 }



void
ANIMRemoveText(i)
int i;
{ TANGO_IMAGE text ;
  TANGO_TRANS move;
  
  text = (TANGO_IMAGE) ASSOCretrieve("TEXT", i) ;

  move = TAPvis_toggle(text);
  TANGOtrans_perform(move); 
  TANGOtrans_free(1, move);
 }


void
ANIMCompare2(p1,p2,p3,p4)
   int	p1;
   int	p2;
   int	p3;
   int	p4;
{
   double x1,y1,x2,y2;
   TANGO_LOC loc1, loc2;
   TANGO_IMAGE rect1, rect2, l1, l2;
   TANGO_PATH fivepath, onepath;
   TANGO_TRANS appear, invis1, invis2, ender;

   rect1 = (TANGO_IMAGE) ASSOCretrieve("ID", p1, p2);
   rect2 = (TANGO_IMAGE) ASSOCretrieve("ID", p3, p4);
   loc1 = TANGOimage_loc(rect1, TANGO_PART_TYPE_S);
   loc2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_S);
   TANGOloc_inquire(loc1,&x1,&y1);
   TANGOloc_inquire(loc2,&x2,&y2);
   l1 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, x1,y1, 1, TANGO_COLOR_BLACK, 0.000000, 0.06, 1.000000, 1.000000, 0);

   l2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, x2, y2,1, TANGO_COLOR_BLACK, 0.000000, 0.06, 1.000000, 1.000000, 0);
   fivepath = TANGOpath_null(5);
   appear = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, rect1, fivepath);
   TANGOtrans_perform(appear);
   onepath = TANGOpath_null(1);
   invis1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, l1, onepath);
   invis2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, l2, onepath);
   ender = TANGOtrans_compose(2, invis1, invis2);
   TANGOtrans_perform(ender);
}





