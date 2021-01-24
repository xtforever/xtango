/* Combsort, implemented by Kenneth Moorman */

#include <stdio.h>
#include "xtango.h"

#define SX   .1
#define SY   .9
#define SIDE .8

/*  create an association */
void
ANIMInit()
{
	ASSOCinit();
	ASSOCmake("DATA1",2);
	ASSOCmake("DATA2",2);
 	ASSOCmake("TEXT1",1);
	ASSOCmake("TEXT2",1);
	ASSOCmake("POINTER1",0);
	ASSOCmake("POINTER2", 0);
	ASSOCmake("PTRLOC1",2);
	ASSOCmake("PTRLOC2",2);
	ASSOCmake("ID1", 2);
	ASSOCmake("ID2", 2);
}


/* save the data values in the array */
void
ANIMInput(id,index,val)
int id, index, val; 
{
	ASSOCstore("DATA1",id,index,val);  
}


void
ANIMInput2(id, index, val)
int id, index, val;
{
	ASSOCstore("DATA2", id, index, val);
}


/* draw the array as a row of rectangles with the heights corresponding */
/* to the value of each array element.					*/
void
ANIMDraw(id,n)
int id, n;
{
   	double      width, 	/* width of a rectangle image */
  		    yvals[50],
		    x, y; 
	int	       i,
   	       item, 
   	       max_val,
   	       vals[50];
	TANGO_LOC	center;
	TANGO_IMAGE	rect,text,pointer;
	TANGO_PATH	path;
	TANGO_TRANS	delay;
	char            str[5];
	TANGO_COLOR     c;

	width = SIDE /  (2.0 * (double)n);  /* each rectangle's width */

   	max_val = 0;
   	for (i=0; i<n; ++i) {   /* find largest array elt */
        	if ((item = (int)ASSOCretrieve("DATA1",id,i)) > max_val)
	   	max_val = item;
		vals[i] = item;
      	}

   	for (i=0; i<n; ++i)	   /* scale array elts from 0.0->1.0 */
      		yvals[i] = (double)vals[i] / (2.0 * (double)max_val);

	TWISTcreate_image_array("ID1",id,n,TANGO_IMAGE_TYPE_RECTANGLE,
				1,0,SX,SY,NULL,width,yvals,SIDE,width,
				1,TANGO_COLOR_RED,0.0);
   
        TWISTcreate_loc_array("PTRLOC1",id,n,1,SX+(width/2.0),
			      SY+(width/2.0),2.0*width);
   
	pointer = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,2.0,2.0,1,
				  TANGO_COLOR_MAROON,width/2.0,1.0);
   	ASSOCstore("POINTER1",pointer);

   	for (i=0; i<n; ++i) {  
      		rect = (TANGO_IMAGE) ASSOCretrieve("ID1",id,i);
		center = TANGOimage_loc(rect,TANGO_PART_TYPE_C);
		TANGOloc_inquire(center,&x,&y);
		sprintf(str,"%d",vals[i]);
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,
					 TANGO_COLOR_BLACK,NULL,str,1);
		ASSOCstore("TEXT1",rect,text);
      }

   	path = TANGOpath_null(10);
   	delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,text,path);
   	TANGOtrans_perform(delay);
  	TANGOpath_free(1,path);
   	TANGOtrans_free(1,delay);
}


/* draw the array as a row of rectangles with the heights corresponding */
/* to the value of each array element.					*/
void
ANIMDraw2(id,n)
int id, n;
{
   	double      width, 	/* width of a rectangle image */
  		    yvals[50],
		    x, y; 
	int	       i,
   	       item, 
   	       max_val,
   	       vals[50];
	TANGO_LOC	center;
	TANGO_IMAGE	rect,text,pointer;
	TANGO_PATH	path;
	TANGO_TRANS	delay;
	char            str[5];
	TANGO_COLOR     c;

	width = SIDE /  (2.0 * (double)n);  /* each rectangle's width */

   	max_val = 0;
   	for (i=0; i<n; ++i) {   /* find largest array elt */
        	if ((item = (int)ASSOCretrieve("DATA2",id,i)) > max_val)
	   	max_val = item;
		vals[i] = item;
      	}

   	for (i=0; i<n; ++i)	   /* scale array elts from 0.0->1.0 */
      		yvals[i] = (double)vals[i] / (2.0 * (double)max_val);

	TWISTcreate_image_array("ID2",id,n,TANGO_IMAGE_TYPE_RECTANGLE,
				1,0,SX,.5*SY,NULL,width,yvals,SIDE,width,
				1,TANGO_COLOR_RED,0.0);
   
        TWISTcreate_loc_array("PTRLOC2",id,n,1,SX+(width/2.0),
			      SY+(width/2.0),2.0*width);
   
	pointer = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,2.0,2.0,1,
				  TANGO_COLOR_MAROON,width/2.0,1.0);
   	ASSOCstore("POINTER2",pointer);

   	for (i=0; i<n; ++i) {  
      		rect = (TANGO_IMAGE) ASSOCretrieve("ID2",id,i);
		center = TANGOimage_loc(rect,TANGO_PART_TYPE_C);
		TANGOloc_inquire(center,&x,&y);
		sprintf(str,"%d",vals[i]);
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,
					 TANGO_COLOR_BLACK,NULL,str,1);
		ASSOCstore("TEXT2",rect,text);
      }

   	path = TANGOpath_null(10);
   	delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,text,path);
   	TANGOtrans_perform(delay);
  	TANGOpath_free(1,path);
   	TANGOtrans_free(1,delay);
}



/* make two rectangles simultaneously flash */

void
ANIMCompare(id1,n1,id2,n2)
int id1, n1, id2, n2;
{
	TANGO_IMAGE rect1,rect2;
	TANGO_PATH path,fillpath;
   	TANGO_TRANS fill[2],compose;
   	double flash[4];
   
	rect1 = (TANGO_IMAGE) ASSOCretrieve("ID1",id1,n1);
   	rect2 = (TANGO_IMAGE) ASSOCretrieve("ID1",id2,n2);
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


void
ANIMCompare2(id1,n1,id2,n2)
int id1, n1, id2, n2;
{
	TANGO_IMAGE rect1,rect2;
	TANGO_PATH path,fillpath;
   	TANGO_TRANS fill[2],compose;
   	double flash[4];
   
	rect1 = (TANGO_IMAGE) ASSOCretrieve("ID2",id1,n1);
   	rect2 = (TANGO_IMAGE) ASSOCretrieve("ID2",id2,n2);
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
int id1, n1, id2, n2;
{
	TANGO_IMAGE rect1,rect2,text1,text2;
   	TANGO_LOC   loc1,loc2;
   	TANGO_PATH  expath1,expath2,path1,path2;
   	TANGO_TRANS move[4],compose;

   	rect1 = (TANGO_IMAGE) ASSOCretrieve("ID1",id1,n1);
   	rect2 = (TANGO_IMAGE) ASSOCretrieve("ID1",id2,n2);
   	loc1 = TANGOimage_loc(rect1,TANGO_PART_TYPE_SW);
   	loc2 = TANGOimage_loc(rect2,TANGO_PART_TYPE_SW);
   	text1 = (TANGO_IMAGE) ASSOCretrieve("TEXT1",rect1);
   	text2 = (TANGO_IMAGE) ASSOCretrieve("TEXT1",rect2);
   	path1 = TANGOpath_motion(loc1,loc2,TANGO_PATH_TYPE_CLOCKWISE);
   	path2 = TANGOpath_rotate(path1,180);
   	move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect1,path1);
   	move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text1,path1);
   	move[2] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect2,path2);
   	move[3] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text2,path2);
   	compose = TANGOtrans_compose(4,move[0],move[1],move[2],move[3]);
   	TANGOtrans_perform(compose);

   	ASSOCstore("ID1",id1,n1,rect2);  /* reset assocs due to exchange */
   	ASSOCstore("ID1",id2,n2,rect1);

   	TANGOpath_free(2,path1,path2);
   	TANGOtrans_free(5,move[0],move[1],move[2],move[3],compose);
}


void
ANIMExchange2(id1,n1,id2,n2)
int id1, n1, id2, n2;
{
	TANGO_IMAGE rect1,rect2,text1,text2;
   	TANGO_LOC   loc1,loc2;
   	TANGO_PATH  expath1,expath2,path1,path2;
   	TANGO_TRANS move[4],compose;

   	rect1 = (TANGO_IMAGE) ASSOCretrieve("ID2",id1,n1);
   	rect2 = (TANGO_IMAGE) ASSOCretrieve("ID2",id2,n2);
   	loc1 = TANGOimage_loc(rect1,TANGO_PART_TYPE_SW);
   	loc2 = TANGOimage_loc(rect2,TANGO_PART_TYPE_SW);
   	text1 = (TANGO_IMAGE) ASSOCretrieve("TEXT2",rect1);
   	text2 = (TANGO_IMAGE) ASSOCretrieve("TEXT2",rect2);
   	path1 = TANGOpath_motion(loc1,loc2,TANGO_PATH_TYPE_CLOCKWISE);
   	path2 = TANGOpath_rotate(path1,180);
   	move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect1,path1);
   	move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text1,path1);
   	move[2] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect2,path2);
   	move[3] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text2,path2);
   	compose = TANGOtrans_compose(4,move[0],move[1],move[2],move[3]);
   	TANGOtrans_perform(compose);

   	ASSOCstore("ID2",id1,n1,rect2);  /* reset assocs due to exchange */
   	ASSOCstore("ID2",id2,n2,rect1);

   	TANGOpath_free(2,path1,path2);
   	TANGOtrans_free(5,move[0],move[1],move[2],move[3],compose);
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

   rect = (TANGO_IMAGE) ASSOCretrieve("ID1",id,n);
   path = TANGOpath_create(1,&f,&f);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect,path);
   TANGOtrans_perform(fill);

   text = (TANGO_IMAGE) ASSOCretrieve("TEXT1",rect);
   col = TANGOpath_color(TANGO_COLOR_WHITE);
   color = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,text,col);
   TANGOtrans_perform(color);

   clean = TANGOtrans_create(TANGO_TRANS_TYPE_REFRESH,NULL,path);
   TANGOtrans_perform(clean);

   TANGOpath_free(1,path);
   TANGOtrans_free(1,fill);
}
