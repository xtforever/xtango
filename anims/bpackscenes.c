#include <stdio.h>
#include "xtango.h"


/* draw the container and set up scaling values for the weights */

void
AnimInit(id,bins)
   int id;
   int bins;
{
   double *width;
   TANGO_LOC loc;
   TANGO_PATH path;
   TANGO_TRANS trans;

#define  BIN_WD    0.65
#define  BIN_HT    0.5

   ASSOCinit();
   ASSOCmake("TEXT",1);

			/* draw the bin holding area */
   TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.3,0.2,1,TANGO_COLOR_BLACK,BIN_WD,BIN_HT+0.2,0.0);
   TANGOimage_create(TANGO_IMAGE_TYPE_LINE,0.3,0.4,1,TANGO_COLOR_BLACK,BIN_WD,0.0,0.0,1.0,0);

   width = (double *) malloc(sizeof(double));  /* save width of an elt */
   *width =  BIN_WD  / (double) bins;
   ASSOCmake("WIDTH",0);
   ASSOCstore("WIDTH",width);

   loc = TANGOloc_create(0.05,0.9);  /* save place for new elt to appear */
   ASSOCmake("START_PT",0);
   ASSOCstore("START_PT",loc);

   TWISTcreate_loc_array(NULL,id,bins,1,0.3,0.4+BIN_HT,*width);
	/* array of bin locations (where next weight should go) */

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}




/* have a new weight (with height scaled to value) appear */

void
AnimNewWeight(id,idnum,val)
   int id;
   int idnum;
   double val;
{
   double      ht,wd,x,y,cx,cy;
   TANGO_LOC	loc,center;
   TANGO_IMAGE	rect,text;
   TANGO_PATH	path;
   TANGO_TRANS	trans;
   char        str[5];

   ht = val * BIN_HT;	/* scale how big the rect should be */
   wd = *( (double *) ASSOCretrieve("WIDTH") );

   loc = (TANGO_LOC) ASSOCretrieve("START_PT");
   TANGOloc_inquire(loc,&x,&y);
   rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x,y-ht,1,TANGO_COLOR_GREEN,wd,ht,0.5);
   center = TANGOimage_loc(rect,TANGO_PART_TYPE_C);
   TANGOloc_inquire(center,&cx,&cy);
   sprintf(str,"%d",idnum);
   text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,cx,cy,1,TANGO_COLOR_BLACK,NULL,str,1);

   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);

   ASSOCstore("ID",id,idnum,rect);  /* have to remember where weight is */
   ASSOCstore("TEXT",rect,text);
}





/* move the weight to the corresponding bin position */

void
AnimMoveTo(id1,n1,binid,binnum)
   int id1;  /* weight's ids */
   int n1;
   int binid;
   int binnum;
{
   TANGO_LOC	  frompt,topt;
   TANGO_IMAGE	  rect,text;
   TANGO_PATH	  movepath;
   TANGO_TRANS	  move[2],compose;

   rect = (TANGO_IMAGE) ASSOCretrieve("ID",id1,n1);
   text = (TANGO_IMAGE) ASSOCretrieve("TEXT",rect);
   frompt = TANGOimage_loc(rect,TANGO_PART_TYPE_SW);
   topt = (TANGO_LOC) ASSOCretrieve("ID",binid,binnum);
   movepath = TANGOpath_motion(frompt,topt,TANGO_PATH_TYPE_CLOCKWISE);
   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect,movepath);
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,movepath);
   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);

   TANGOpath_free(1,movepath);
   TANGOtrans_free(3,move[0],move[1],compose);
}




/* make the weight move to the given bin, then highlight it to show it */
/* found its place						       */

void
AnimInPlace(id1,n1,binid,binnum)
   int id1;
   int n1;
   int binid;
   int binnum;
{
   TANGO_LOC	  nw,se;
   TANGO_IMAGE	  rect;
   TANGO_PATH	  onepath;
   TANGO_TRANS	  fill;
   double	  x0,y0,x1,y1;
   double	  fval = 1.0;

   rect = (TANGO_IMAGE) ASSOCretrieve("ID",id1,n1);
   nw = TANGOimage_loc(rect,TANGO_PART_TYPE_NW);  /* put a black outline around weight */
   TANGOloc_inquire(nw,&x0,&y0);
   se = TANGOimage_loc(rect,TANGO_PART_TYPE_SE);
   TANGOloc_inquire(se,&x1,&y1);
   TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x0,y0,1,TANGO_COLOR_BLACK,x1-x0,y1-y0,0.0);

   onepath = TANGOpath_create(1,&fval,&fval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect,onepath);
   TANGOtrans_perform(fill);

    /* reset this bin's entry position to the top of the weight */
   ASSOCstore("ID",binid,binnum,nw);

   TANGOpath_free(1,onepath);
   TANGOtrans_free(1,fill);
}



