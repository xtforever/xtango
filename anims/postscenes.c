#include <stdio.h>
#include "xtango.h"

#define  ROWS		5

#define  TABLES 	0   /* ids for associations */
#define  WORKERS	1

static TANGO_IMAGE_COMPONENT compdoor[3] = {
   { TANGO_IMAGE_TYPE_RECTANGLE, "-0.025 -0.06 TANGO_COLOR_BLACK 0.05 0.12 1.0"}, /* frame */
   { TANGO_IMAGE_TYPE_CIRCLE, "0.020 0.00 TANGO_COLOR_WHITE 0.006 1.0"},          /* knob  */
   { TANGO_IMAGE_TYPE_COMPOSITE, NULL}
};


static TANGO_IMAGE_COMPONENT compperson[7] = {
   { TANGO_IMAGE_TYPE_CIRCLE,  "0.0  0.0  TANGO_COLOR_ORANGE  0.01  0.0"},   /* head */
   { TANGO_IMAGE_TYPE_LINE,  "0.0  0.010  TANGO_COLOR_BLUE    0.00   0.03  0.5  1.0 0"}, /* body */
   { TANGO_IMAGE_TYPE_LINE,  "0.0  0.025  TANGO_COLOR_ORANGE  0.02  -0.01  0.0  1.0 0"}, /* arms */
   { TANGO_IMAGE_TYPE_LINE,  "0.0  0.025  TANGO_COLOR_ORANGE -0.02  -0.01  0.0  1.0 0"},
   { TANGO_IMAGE_TYPE_LINE,  "0.0  0.040  TANGO_COLOR_ORANGE  0.01   0.02  0.0  1.0 0"}, /* legs */
   { TANGO_IMAGE_TYPE_LINE,  "0.0  0.040  TANGO_COLOR_ORANGE -0.01   0.02  0.0  1.0 0"},
   { TANGO_IMAGE_TYPE_COMPOSITE, NULL }
};


/* draw the entrance and exit doors, the tables and workers, */
/* and create a 2d array of positions corresponding to the   */
/* lines and positions that the patrons will take	     */

void
ANIMInit(lineid,timeid,lines)
   int lineid;
   int timeid;
   int lines;
{
   TANGO_IMAGE enter,exit,text;
   double TABLE_X;
   int i;
   double x,y,rx;
   TANGO_LOC pt;
   TANGO_PATH path;
   TANGO_TRANS trans;

#define  LX		0.1  /* area of post office */
#define  RX		0.7
#define  TY		0.2
#define  BY		0.8

#define  TABLE_Y	0.05
#define  PERSON_SPACE	0.12

   ASSOCinit();
   ASSOCmake("TIME",1);
   ASSOCmake("ENTER",0);
   ASSOCmake("EXIT",0);

   /* draw the entrances and exits */
   enter = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,0.1,0.12,1,compdoor);
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.075,0.05,1,TANGO_COLOR_RED,NULL,"ENTER",0);
   ASSOCstore("ENTER",enter);
   exit = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,0.82,0.35,1,compdoor);
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.8,0.28,1,TANGO_COLOR_RED,NULL,"EXIT",0);
   ASSOCstore("EXIT",exit);

   /* figure how wide a table should be */
   TABLE_X = (RX-LX) / (2.0 * (double)lines - 1.0);

   /* make the tables */
   TWISTcreate_image_array(NULL,TABLES,lines,TANGO_IMAGE_TYPE_RECTANGLE,1,0,LX,BY+TABLE_Y,
			NULL,TABLE_X,NULL,TABLE_Y,TABLE_X,1,TANGO_COLOR_BLACK,1.0);

   /* make the workers */
   x = LX + (TABLE_X/2.0);
   y = BY + TABLE_Y + 0.02;
   for (i=0; i<lines; ++i)
      { TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,x,y,1,compperson);
	x += 2.0 * TABLE_X;
      }

   rx = LX - 0.04;   /* add the spots to show time left for front patron */
   for (i=0; i<lines; ++i)
      { pt = TANGOloc_create(rx,BY+TABLE_Y);
	ASSOCstore("ID",timeid,i,pt);
	text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,rx,BY+TABLE_Y,1,TANGO_COLOR_BLUE,NULL,"0",0);
	ASSOCstore("TIME",i,0);
	ASSOCstore("IMAGE_AT",timeid,pt,text);
	rx += 2.0 * TABLE_X;
      }

   /* create the places for patrons to go */
   TWISTcreate_2d_loc_array(NULL,lineid,ROWS,lines,LX+(TABLE_X/2.0),TY+(PERSON_SPACE/2.0)-0.01,
			      2.0*TABLE_X,PERSON_SPACE);

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}





/* update the value of the amount of time of left for the front patron */
/* in the line. 						       */

void
ANIMSet_time(line,time)
   int line,time;
{
   ASSOCstore("TIME",line,time);
}




/* display all the times of the front patrons in the lines next  */
/* to the table designating each line.				 */

void
ANIMShow_times(timeid,numlines)
   int timeid;
   int numlines;
{
   int tr=0,time,i;
   double x,y;
   char str[10];
   TANGO_LOC   loc;
   TANGO_IMAGE oldtext,text;
   TANGO_PATH  onepath;
   TANGO_TRANS vis,compose[20];

   onepath = TANGOpath_null(1);
   for (i=0; i<numlines; ++i)
      { loc = (TANGO_LOC) ASSOCretrieve("ID",timeid,i);
	oldtext = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",timeid,loc);
	compose[tr++] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,oldtext,onepath);

	time = (int) ASSOCretrieve("TIME",i);
	sprintf(str,"%d",time);
	TANGOloc_inquire(loc,&x,&y);
	text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,0,TANGO_COLOR_BLUE,NULL,str,0);
	compose[tr++] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,text,onepath);

	ASSOCstore("IMAGE_AT",timeid,loc,text);
      }

   vis = TANGOtrans_compose(tr,compose[0],compose[1],compose[2],compose[3],compose[4],
			       compose[5],compose[6],compose[7],compose[8],compose[9]);
   TANGOtrans_perform(vis);

   TANGOpath_free(1,onepath);
   TANGOtrans_free(1,vis);
   for (i=0; i<tr; ++i)
      TANGOtrans_free(1,compose[i]);
}






/* the entrance door opens, a patron walks in and takes his/her */
/* place in the shortest line					*/

void
ANIMEnter(id,line,pos)
   int id;
   int line;
   int pos;
{
   double x,y;
   TANGO_IMAGE enter,person;
   TANGO_LOC   inpt,topt;
   TANGO_PATH  enterpath,onepath,tenpath;
   TANGO_TRANS t1[2],t2[3],compose[2],action;

   enter = (TANGO_IMAGE) ASSOCretrieve("ENTER");
   inpt = TANGOimage_loc(enter,TANGO_PART_TYPE_C);
   TANGOloc_inquire(inpt,&x,&y);    /* a new person appears (invisible) */
   person = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,x,y,0,compperson);

   topt = (TANGO_LOC) ASSOCretrieve("ID3",id,ROWS-pos,line);
   onepath = TANGOpath_null(1);
   t1[0] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,person,onepath);
   enterpath = TANGOpath_distance(inpt,topt,0.02);
   t1[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,person,enterpath); /* person becomes vis and moves */
   compose[0] = TANGOtrans_concatenate(2,t1[0],t1[1]); /* to proper place in line */

   t2[0] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,enter,onepath); /* entrance becomes invisible */
   tenpath = TANGOpath_null(4); 	       /* while person is going through */
   t2[1] = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,enter,tenpath);
   t2[2] = t2[0];
   compose[1] = TANGOtrans_concatenate(3,t2[0],t2[1],t2[2]);

   action = TANGOtrans_compose(2,compose[0],compose[1]); /* both occur at same time */
   TANGOtrans_perform(action);

   ASSOCstore("IMAGE_AT",id,topt,person); /* set the position of this new patron */

   TANGOpath_free(3,enterpath,onepath,tenpath);
   TANGOtrans_free(7,t1[0],t1[1],t2[0],t2[1],compose[0],compose[1],action);
}






/* the front patron in the given line leaves through the exit door */
/* any people left in that line move up one place		   */

void
ANIMExit(id,line,num_left)
   int id;
   int line;
   int num_left;
{
   TANGO_LOC exitpt,frompt,topt;
   TANGO_IMAGE exit,person;
   int offsets,i;
   TANGO_PATH onepath,fourpath,exitpath,nullpath,path[2],advpath;
   TANGO_TRANS t1[2],t2[4],compose[10],action;

   exit = (TANGO_IMAGE) ASSOCretrieve("EXIT");  /* get the exit and patron */
   exitpt = TANGOimage_loc(exit,TANGO_PART_TYPE_C);
   frompt = (TANGO_LOC) ASSOCretrieve("ID3",id,ROWS-1,line);
   person = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,frompt);

   exitpath = TANGOpath_distance(frompt,exitpt,0.02);
   t1[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,person,exitpath);
   onepath = TANGOpath_null(1);
   t1[1] = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,person,onepath);
   compose[0] = TANGOtrans_concatenate(2,t1[0],t1[1]);

   offsets = TANGOpath_length(exitpath);  /* the exit door is briefly open */
   nullpath = TANGOpath_null(offsets-4);  /* (invisible) while the patron  */
   fourpath = TANGOpath_null(4);	  /* exits through it		   */
   t2[0] = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,exit,nullpath);
   t2[1] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,exit,onepath);
   t2[2] = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,exit,fourpath);
   t2[3] = t2[1];
   compose[1] = TANGOtrans_concatenate(4,t2[0],t2[1],t2[2],t2[3]);

   frompt = (TANGO_LOC) ASSOCretrieve("ID3",id,0,0); /* create a path that advances */
   topt = (TANGO_LOC) ASSOCretrieve("ID3",id,1,0);   /* patrons up one position     */
   path[0] = fourpath;
   path[1] = TANGOpath_distance(frompt,topt,0.01);
   advpath = TANGOpath_concatenate(2,path[0],path[1]);

   for (i=1; i<=num_left; ++i)	/* each patron left must move up */
      { frompt = (TANGO_LOC) ASSOCretrieve("ID3",id,ROWS-i-1,line);
	person = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,frompt);
	compose[1+i] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,person,advpath);

	topt = (TANGO_LOC) ASSOCretrieve("ID3",id,ROWS-i,line);
	ASSOCstore("IMAGE_AT",id,topt,person);  /* reset patron's position */
      }

   action = TANGOtrans_compose(num_left+2,compose[0],compose[1],compose[2],compose[3],compose[4],
			       compose[5],compose[6],compose[7],compose[8],compose[9]);
   TANGOtrans_perform(action); /* make it all happen at once */

   TANGOpath_free(7,exitpath,onepath,fourpath,nullpath,
		    path[0],path[1],advpath);
   TANGOtrans_free(7,t1[0],t1[1],t2[0],t2[1],t2[2],compose[0],compose[1]);
   for (i=1; i<=num_left; ++i)
      TANGOtrans_free(1,compose[1+i]);
}
