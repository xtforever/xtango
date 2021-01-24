#include <stdio.h>
#include <xtango.h>

#define IDVAL 13
#define SPEED 0.05

void
ANIMInit(edges)
   int edges;
{
   TANGO_IMAGE loz;

   ASSOCinit();
   ASSOCmake("TEXT",0);
   ASSOCmake("CURRLOC",0);
   ASSOCmake("LOZENGE",0);
   loz = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,0,TANGO_COLOR_BLUE,
			      0.05,0.0);
   ASSOCstore("LOZENGE",loz);
   TWISTcreate_bintree(IDVAL,0.1,0.9,0.9,0.1,edges);
}



void
ANIMNewVal(val)
   int val;
{
   char        str[5];
   TANGO_LOC   from,to;
   TANGO_IMAGE text,loz;
   TANGO_PATH  path;
   TANGO_TRANS move,appear,both;

   /* add a new value's image */
   sprintf(str,"%d",val);
   text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.1,0.1,1,
				   TANGO_COLOR_BLACK,"variable",str,1);
   ASSOCstore("TEXT",text);

   /* move the lozenge to the root and make it visible */
   loz = (TANGO_IMAGE) ASSOCretrieve("LOZENGE");
   from = TANGOimage_loc(loz,TANGO_PART_TYPE_C);
   to = (TANGO_LOC) ASSOCretrieve("NUMBER",IDVAL,0);  /* root from TWISTcreatebintree */
   path = TANGOpath_distance(from,to,1.0);  /* make it in 1 frame */
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,loz,path);
   appear = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,loz,path);
   both = TANGOtrans_compose(2,move,appear);
   TANGOtrans_perform(both);

   /* save where we're at */
   ASSOCstore("CURRLOC",to);
}



void
ANIMAddNode()
{
   double	  sx,sy,nx,ny;
   TANGO_LOC	  from,to,parloc;
   TANGO_IMAGE	  text,loz,parnode;
   TANGO_PATH	  movepath,onepath;
   TANGO_TRANS	  move,vis;

   /* move the value to its proper spot in the tree */
   text = (TANGO_IMAGE) ASSOCretrieve("TEXT");
   from = TANGOimage_loc(text,TANGO_PART_TYPE_C);
   to = (TANGO_LOC) ASSOCretrieve("CURRLOC");
   movepath = TANGOpath_distance(from,to,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,movepath);
   TANGOtrans_perform(move);
   /* save the text image via this particular LOCATION */
   ASSOCstore("IMAGE_AT",IDVAL,to,text);

   /* draw an edge to its parent value */
   parloc = (TANGO_LOC) ASSOCretrieve("PARENT",IDVAL,to);
   if (parloc)	/* not the root */
      { parnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,parloc);
	TANGOloc_inquire(TANGOimage_loc(parnode,TANGO_PART_TYPE_S),&sx,&sy);
	TANGOloc_inquire(TANGOimage_loc(text,TANGO_PART_TYPE_N),&nx,&ny);
	TANGOimage_create(TANGO_IMAGE_TYPE_LINE,sx,sy,1,TANGO_COLOR_BLACK,
				    nx-sx,ny-sy,1.0,1.0,0);
      }

   /* make the lozenge invisible */
   loz = (TANGO_IMAGE) ASSOCretrieve("LOZENGE");
   onepath = TANGOpath_null(1);
   vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,loz,onepath);
   TANGOtrans_perform(vis);
}


void
ANIMCompAndMove(goleft)
   int goleft;
{
   TANGO_LOC	currloc,to;
   TANGO_IMAGE	currval,loz;
   TANGO_PATH	red,black,flash,movepath;
   TANGO_TRANS	compare,move;

   /* flash the value at the current position to indicate a "compare" */
   currloc = (TANGO_LOC) ASSOCretrieve("CURRLOC");
   currval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,currloc);
   red = TANGOpath_color(TANGO_COLOR_RED);
   black = TANGOpath_color(TANGO_COLOR_BLACK);
   flash = TANGOpath_concatenate(7,red,red,black,black,red,red,black);
   compare = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,currval,flash);
   TANGOtrans_perform(compare);

   /* move the lozenge to the next spot in the tree */
   loz = (TANGO_IMAGE) ASSOCretrieve("LOZENGE");
   if (goleft)
      to = (TANGO_LOC) ASSOCretrieve("LCHILD",IDVAL,currloc);
   else
      to = (TANGO_LOC) ASSOCretrieve("RCHILD",IDVAL,currloc);
   movepath = TANGOpath_distance(currloc,to,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,loz,movepath);
   TANGOtrans_perform(move);
   /* update our current position */
   ASSOCstore("CURRLOC",to);
}


