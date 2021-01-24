#include <stdio.h>
#include "xtango.h"

#define cosine cos
#define sine   sin

#define  PI   3.1415

#define  PRODUCER   0	      /* ids for associations */
#define  MONITOR    1
#define  CONSUMER   2
#define  IN_PTR     0
#define  OUT_PTR    1
#define  NULL_PTR   2

#define  ELT_SIZE   0.03
#define  SPEED	    0.04

double cosine();
double sine();

double fillval = 1.0;
double unfillval = -1.0;


/* draw the 3 main regions, draw the ring buffer (it's tricky */
/* to delimit the buffer positions), and set up the locs      */
/* for elements and ptrs to go into the buffer		      */

void
ANIMInit(buffer,n)
   int buffer,n;
{
   int qspot,i;
   double delta,angle;
   double x,y,sx,sy;
   TANGO_IMAGE rect,nullspot,inptr,outptr;
   TANGO_LOC loc,ploc,mloc,cloc;
   TANGO_PATH path;
   TANGO_TRANS trans;

#define BUFX  0.5
#define BUFY  0.6

#define INNER_RAD  0.1
#define MIDDLE_RAD 0.15
#define OUTER_RAD  0.2
#define INPTR_RAD  0.25
#define OUTPTR_RAD 0.275

   ASSOCinit();
   ASSOCmake("ITEM_TEXT",1);
   ASSOCmake("REGIONLOC",1);
   ASSOCmake("REGIONRECT",1);
   ASSOCmake("PTR_POS",2);
   ASSOCmake("PTR_ICONS",1);

   ploc = TANGOloc_create(0.1,0.15);  /* create the three regions */
   mloc = TANGOloc_create(0.5,0.15);
   cloc = TANGOloc_create(0.9,0.15);
   ASSOCstore("REGIONLOC",PRODUCER,ploc);
   ASSOCstore("REGIONLOC",MONITOR,mloc);
   ASSOCstore("REGIONLOC",CONSUMER,cloc);
   rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,0.0,1,TANGO_COLOR_BLACK,0.2,0.2,0.0);
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.1,0.05,1,TANGO_COLOR_RED,"variable","PRODUCER",1);
   ASSOCstore("REGIONRECT",PRODUCER,rect);
   rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.4,0.0,1,TANGO_COLOR_BLACK,0.2,0.2,0.0);
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.5,0.05,1,TANGO_COLOR_RED,"variable","MONITOR",1);
   ASSOCstore("REGIONRECT",MONITOR,rect);
   rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.8,0.0,1,TANGO_COLOR_BLACK,0.2,0.2,0.0);
   TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.9,0.05,1,TANGO_COLOR_RED,"variable","CONSUMER",1);
   ASSOCstore("REGIONRECT",CONSUMER,rect);

   /* draw the ring buffers two circles */
   TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,BUFX,BUFY,1,TANGO_COLOR_BLACK,INNER_RAD,0.0);
   TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,BUFX,BUFY,1,TANGO_COLOR_BLACK,OUTER_RAD,0.0);

   qspot = n-1;
   angle = 0.0;
   delta = PI/ (double)n;
   for (i=1; i<=n; ++i)
      { angle += delta;
	x = cosine(angle) * MIDDLE_RAD; /* initialize positions (TANGO_LOCS) */
	y = sine(angle) * MIDDLE_RAD; /* in the ring buffer		    */
	loc = TANGOloc_create(BUFX+x,BUFY-y);
	ASSOCstore("ID",buffer,qspot,loc);

	x = cosine(angle) * INPTR_RAD;	/* initialize positions outside buffer */
	y = sine(angle) * INPTR_RAD;	/* for IN ptr			       */
	loc = TANGOloc_create(BUFX+x,BUFY-y);
	ASSOCstore("PTR_POS",IN_PTR,qspot,loc);

	x = cosine(angle) * OUTPTR_RAD;  /* initialize positions outside buffer */
	y = sine(angle) * OUTPTR_RAD;	 /* for OUT ptr 			*/
	loc = TANGOloc_create(BUFX+x,BUFY-y);
	ASSOCstore("PTR_POS",OUT_PTR,qspot,loc);

	angle += delta; 	     /* put lines in to divide buffer regions */
	x = cosine(angle) * INNER_RAD;
	y = sine(angle) * INNER_RAD;
	sx = (cosine(angle) * OUTER_RAD) - x;
	sy = (sine(angle) * OUTER_RAD) - y;
	TANGOimage_create(TANGO_IMAGE_TYPE_LINE,BUFX+x,BUFY+y,1,TANGO_COLOR_BLACK,sx,sy,0.0,1.0,0);

	qspot--;
      }

   loc = (TANGO_LOC) ASSOCretrieve("ID",buffer,n-1);
   TANGOloc_inquire(loc,&x,&y);
   nullspot = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x-ELT_SIZE,y-ELT_SIZE,
				 1,TANGO_COLOR_BLACK,2.0*ELT_SIZE,2.0*ELT_SIZE,1.0);
   ASSOCstore("PTR_ICONS",NULL_PTR,nullspot);

   loc = (TANGO_LOC) ASSOCretrieve("PTR_POS",IN_PTR,0);
   TANGOloc_inquire(loc,&x,&y);
   inptr = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_RED,"variable","In",1);
   loc = (TANGO_LOC) ASSOCretrieve("PTR_POS",OUT_PTR,0);
   TANGOloc_inquire(loc,&x,&y);
   outptr = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_RED,"variable","Out",1);

   ASSOCstore("PTR_ICONS",IN_PTR,inptr);
   ASSOCstore("PTR_ICONS",OUT_PTR,outptr);

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}






/* the PRODUCER is waiting, fill its region */

void
ANIMP_wait()
{
   TANGO_IMAGE pimage;
   TANGO_PATH  onepath;
   TANGO_TRANS fill;

   pimage = (TANGO_IMAGE) ASSOCretrieve("REGIONRECT",PRODUCER);
   onepath = TANGOpath_create(1,&fillval,&fillval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,pimage,onepath);
   TANGOtrans_perform(fill);

   TANGOpath_free(1,onepath);
   TANGOtrans_free(1,fill);
}





/* the PRODUCER is done waiting, unfill its region */

void
ANIMP_active()
{
   TANGO_IMAGE pimage;
   TANGO_PATH  onepath;
   TANGO_TRANS fill;

   pimage = (TANGO_IMAGE) ASSOCretrieve("REGIONRECT",PRODUCER);
   onepath = TANGOpath_create(1,&unfillval,&unfillval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,pimage,onepath);
   TANGOtrans_perform(fill);

   TANGOpath_free(1,onepath);
   TANGOtrans_free(1,fill);
}





/* the CONSUMER is waiting; fill its region */

void
ANIMC_wait()
{
   TANGO_IMAGE cimage;
   TANGO_PATH  onepath;
   TANGO_TRANS fill;

   cimage = (TANGO_IMAGE) ASSOCretrieve("REGIONRECT",CONSUMER);
   onepath = TANGOpath_create(1,&fillval,&fillval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,cimage,onepath);
   TANGOtrans_perform(fill);

   TANGOpath_free(1,onepath);
   TANGOtrans_free(1,fill);
}






/* the CONSUMER is done waiting; unfill its region */

void
ANIMC_active()
{
   TANGO_IMAGE cimage;
   TANGO_PATH  onepath;
   TANGO_TRANS fill;

   cimage = (TANGO_IMAGE) ASSOCretrieve("REGIONRECT",CONSUMER);
   onepath = TANGOpath_create(1,&unfillval,&unfillval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,cimage,onepath);
   TANGOtrans_perform(fill);

   TANGOpath_free(1,onepath);
   TANGOtrans_free(1,fill);
}





/* introduce a new item in the PRODUCER region */

void
ANIMNew_item(id,num)
   int id,num;
{
   TANGO_LOC ploc;
   TANGO_IMAGE node,text;
   TANGO_PATH path;
   TANGO_TRANS trans;
   double x,y;
   char str[5];

   ploc = (TANGO_LOC) ASSOCretrieve("REGIONLOC",PRODUCER);
   TANGOloc_inquire(ploc,&x,&y);
   sprintf(str,"%d",num);
   node = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x,y,1,TANGO_COLOR_BLUE,ELT_SIZE,0.0);
   text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLUE,NULL,str,1);
   ASSOCstore("ID",id,num,node);   /* save it for later references */
   ASSOCstore("ITEM_TEXT",node,text);

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);	 /* make it appear */
}







/* the queue is full, so have the given item move to the monitor */
/* then back to the producer					 */

void
ANIMFull(id,num)
   int id,num;
{
   TANGO_LOC	 atpt,mloc;
   TANGO_IMAGE	 node,text;
   TANGO_PATH	 p1[2],fullpath;
   TANGO_TRANS	 move[2],compose;

   node = (TANGO_IMAGE) ASSOCretrieve("ID",id,num);   /* get the item */
   text = (TANGO_IMAGE) ASSOCretrieve("ITEM_TEXT",node);

   atpt = TANGOimage_loc(node,TANGO_PART_TYPE_C);
   mloc = (TANGO_LOC) ASSOCretrieve("REGIONLOC",MONITOR);
   p1[0] = TANGOpath_distance(atpt,mloc,SPEED);
   p1[1] = TANGOpath_distance(mloc,atpt,SPEED);
   fullpath = TANGOpath_concatenate(2,p1[0],p1[1]);
   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,node,fullpath);
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,fullpath);
   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);

   TANGOpath_free(3,p1[0],p1[1],fullpath);
   TANGOtrans_free(3,move[0],move[1],compose);
}







/* have the given item move through the monitor to the designated */
/* inspot in the ring buffer.  update the IN ptr to the given	  */
/* buffer spot							  */

void
ANIMIn(itemid,inum,buffer,inspot,ptrspot)
   int itemid,inum;
   int buffer,inspot,ptrspot;
{
   TANGO_LOC	 atpt,mloc,ipt,inpt,newpt;
   TANGO_IMAGE	 node,text,inptr;
   TANGO_PATH	 p1[2],fullpath,inpath;
   TANGO_TRANS	 move[2],compose,ptrtrans;

   node = (TANGO_IMAGE) ASSOCretrieve("ID",itemid,inum);  /* get the item */
   text = (TANGO_IMAGE) ASSOCretrieve("ITEM_TEXT",node);
   atpt = TANGOimage_loc(node,TANGO_PART_TYPE_C);
   mloc = (TANGO_LOC) ASSOCretrieve("REGIONLOC",MONITOR);
   ipt = (TANGO_LOC) ASSOCretrieve("ID",buffer,inspot);

   p1[0] = TANGOpath_distance(atpt,mloc,SPEED);
   p1[1] = TANGOpath_distance(mloc,ipt,SPEED);
   fullpath = TANGOpath_concatenate(2,p1[0],p1[1]);   /* move the item to the buffer */
   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,node,fullpath);
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,fullpath);
   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);

   inptr = (TANGO_IMAGE) ASSOCretrieve("PTR_ICONS",IN_PTR);
   inpt = TANGOimage_loc(inptr,TANGO_PART_TYPE_C);  /* update the IN ptr */
   newpt = (TANGO_LOC) ASSOCretrieve("PTR_POS",IN_PTR,ptrspot);
   inpath = TANGOpath_distance(inpt,newpt,SPEED);
   ptrtrans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,inptr,inpath);
   TANGOtrans_perform(ptrtrans);

   TANGOpath_free(4,p1[0],p1[1],fullpath,inpath);
   TANGOtrans_free(4,move[0],move[1],compose,ptrtrans);
}






/* move the given item out of the ring buffer, through the monitor, */
/* and to the consumer, then make it vanish.  update the OUT ptr in */
/* the buffer, and move the null buffer position to the given spot  */

void
ANIMOut(itemid,inum,buffer,outspot,nullspot)
   int itemid,inum;
   int buffer,outspot,nullspot;
{
   TANGO_LOC	 ipt,mloc,cloc,outpt,newpt,nullpt;
   TANGO_IMAGE	 node,text,outptr,nullptr;
   TANGO_PATH	 p1[2],fullpath,onepath,outpath,nullpath;
   TANGO_TRANS	 move[2],compose,vis[3],t1[2],ptrtrans;

   node = (TANGO_IMAGE) ASSOCretrieve("ID",itemid,inum);
   text = (TANGO_IMAGE) ASSOCretrieve("ITEM_TEXT",node);
   ipt = TANGOimage_loc(node,TANGO_PART_TYPE_C);
   mloc = (TANGO_LOC) ASSOCretrieve("REGIONLOC",MONITOR);
   cloc = (TANGO_LOC) ASSOCretrieve("REGIONLOC",CONSUMER);

   p1[0] = TANGOpath_distance(ipt,mloc,SPEED);
   p1[1] = TANGOpath_distance(mloc,cloc,SPEED);
   fullpath = TANGOpath_concatenate(2,p1[0],p1[1]);
   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,node,fullpath);
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,fullpath);
   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);
   onepath = TANGOpath_null(1);
   vis[0] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,node,onepath);
   vis[1] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,text,onepath);
   vis[2] = TANGOtrans_compose(2,vis[0],vis[1]);
   TANGOtrans_perform(vis[2]);

   outptr = (TANGO_IMAGE) ASSOCretrieve("PTR_ICONS",OUT_PTR);
   outpt = TANGOimage_loc(outptr,TANGO_PART_TYPE_C);
   newpt = (TANGO_LOC) ASSOCretrieve("PTR_POS",OUT_PTR,outspot);
   outpath = TANGOpath_distance(outpt,newpt,SPEED);
   t1[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,outptr,outpath);

   nullptr = (TANGO_IMAGE) ASSOCretrieve("PTR_ICONS",NULL_PTR);
   nullpt = TANGOimage_loc(nullptr,TANGO_PART_TYPE_C);
   newpt = (TANGO_LOC) ASSOCretrieve("ID",buffer,nullspot);
   nullpath = TANGOpath_distance(nullpt,newpt,SPEED);
   t1[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,nullptr,nullpath);

   ptrtrans = TANGOtrans_compose(2,t1[0],t1[1]);
   TANGOtrans_perform(ptrtrans);

   TANGOpath_free(6,p1[0],p1[1],fullpath,onepath,outpath,nullpath);
   TANGOtrans_free(9,move[0],move[1],compose,vis[0],vis[1],vis[2],t1[0],t1[1],ptrtrans);
}



