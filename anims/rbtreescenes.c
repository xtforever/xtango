/* Red-Black Trees by Rammohan Kordale */


#include <stdio.h>
#include <xtango.h>

#define IDVAL 13
#define SPEED 0.05
#define COLOR 1
#define INORDER 0

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
/*
	printf("Observe change - Press RETURN\n");
	getchar();
*/

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
   double      sx,sy,nx,ny;
   TANGO_LOC      from,to,parloc,lloc,rloc;
   TANGO_IMAGE      text,loz,parnode,red;
   TANGO_PATH      movepath,onepath;
   TANGO_TRANS      move,vis;

   /* move the value to its proper spot in the tree */
   text = (TANGO_IMAGE) ASSOCretrieve("TEXT");
   from = TANGOimage_loc(text,TANGO_PART_TYPE_C);
   to = (TANGO_LOC) ASSOCretrieve("CURRLOC");
   movepath = TANGOpath_distance(from,to,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,movepath);
   TANGOtrans_perform(move);
#if COLOR == 1
   red = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_RED,
                  0.05,0.0);
#else
   red = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_BLACK,
                  0.05,0.0);
#endif
   from = TANGOimage_loc(red,TANGO_PART_TYPE_C);
   to = (TANGO_LOC) ASSOCretrieve("CURRLOC");
   movepath = TANGOpath_distance(from,to,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,red,movepath);
   TANGOtrans_perform(move);
   ASSOCstore("IMAGE_AT",IDVAL,to,red);
   /* save the red image via this particular LOCATION */

   /* draw an edge to its parent value */
   parloc = (TANGO_LOC) ASSOCretrieve("PARENT",IDVAL,to);
   if (parloc)    /* not the root */
      { parnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,parloc);
    TANGOloc_inquire(TANGOimage_loc(parnode,TANGO_PART_TYPE_S),&sx,&sy);
    TANGOloc_inquire(TANGOimage_loc(red,TANGO_PART_TYPE_N),&nx,&ny);
    TANGOimage_create(TANGO_IMAGE_TYPE_LINE,sx,sy,1,TANGO_COLOR_BLACK,
                    nx-sx,ny-sy,0.0,1.0,0);
      }

   /* make the lozenge invisible */
   loz = (TANGO_IMAGE) ASSOCretrieve("LOZENGE");
   onepath = TANGOpath_null(1);
   vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,loz,onepath);
   TANGOtrans_perform(vis);
	printf("Tree BEFORE balancing is done - Press RETURN\n");
	getchar();
}


void
ANIMCompAndMove(goleft)
   int goleft;
{
   TANGO_LOC    currloc,to;
   TANGO_IMAGE    currval,loz;
   TANGO_PATH    red,black,flash,movepath;
   TANGO_TRANS    compare,move;

   /* flash the value at the current position to indicate a "compare" */
   currloc = (TANGO_LOC) ASSOCretrieve("CURRLOC");
   currval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,currloc);

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

void
ANIMDrawscenes(val,in_order,color)
    int val,in_order,color;
{
   char str[5];
   double      sx,sy,nx,ny;
   TANGO_LOC   from,to,parloc;
   TANGO_IMAGE text,red,parnode,image;
   TANGO_PATH  path,movepath,onepath;
   TANGO_TRANS move,vis;

#if INORDER == 1
    printf("val is %d, in_order is %d and color is %d\n",val,in_order,color);
#endif
   /* add a new value's image */
   sprintf(str,"%d",val);
   text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.1,0.1,1,
                   TANGO_COLOR_BLACK,"variable",str,1);
   ASSOCstore("TEXT",text);
   from = TANGOimage_loc(text,TANGO_PART_TYPE_C);
    to = (TANGO_LOC)ASSOCretrieve("NUMBER",IDVAL,in_order);
   movepath = TANGOpath_distance(from,to,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,movepath);
   TANGOtrans_perform(move);
   ASSOCstore("CURRLOC",to);
    if (color)/*if color is RED - create the circle*/
   { 
#if COLOR == 1
      red = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_RED,
                  0.05,0.0);
#else
      red = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_BLACK,
                  0.05,0.0);
#endif
          from = TANGOimage_loc(red,TANGO_PART_TYPE_C);
       to = (TANGO_LOC) ASSOCretrieve("CURRLOC");
          movepath = TANGOpath_distance(from,to,SPEED);
         move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,red,movepath);
    }
	else/*If color is black*/
    {  
#if COLOR == 1
       red = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_BLACK,
                  0.05,0.0);
#else
       red = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_WHITE,
                  0.05,0.0);
#endif
          from = TANGOimage_loc(red,TANGO_PART_TYPE_C);
       to = (TANGO_LOC) ASSOCretrieve("CURRLOC");
          movepath = TANGOpath_distance(from,to,SPEED);
         move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,red,movepath);
    }
	TANGOtrans_perform(move);
    ASSOCstore("IMAGE_AT",IDVAL,to,red);
	parloc = (TANGO_LOC)ASSOCretrieve("PARENT",IDVAL,to);
   	if (parloc)    /* not the root */
   	{	parnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,parloc);
       	TANGOloc_inquire(TANGOimage_loc(parnode,TANGO_PART_TYPE_S),&sx,&sy);
       	TANGOloc_inquire(TANGOimage_loc(red,TANGO_PART_TYPE_N),&nx,&ny);
       	image = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,sx,sy,0,TANGO_COLOR_BLACK,
                       nx-sx,ny-sy,0.0,1.0,0);
   		onepath = TANGOpath_null(1);
   		vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,image,onepath);
   		TANGOtrans_perform(vis);
   	}
    
}

void
ANIMClearscenes(in_order)
    int in_order;
{
   char str[5];
   double      sx,sy,nx,ny;
   TANGO_LOC   from,to,parloc,lloc,rloc;
   TANGO_IMAGE text,red,white,parnode,loz,line;
   TANGO_PATH  path,movepath,nullpath,onepath;
   TANGO_TRANS move,appear,both,vis;

#if INORDER == 1
    printf("in_order is %d \n",in_order);
#endif
   white = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_WHITE,
                  0.05,1.0);
	from = TANGOimage_loc(white,TANGO_PART_TYPE_C);
    to = (TANGO_LOC)ASSOCretrieve("NUMBER",IDVAL,in_order);
	path = TANGOpath_distance(from,to,1.0);
	move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,white,path);
	TANGOtrans_perform(move);
/*6 lines added*/
   white = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,1,TANGO_COLOR_WHITE,
                  0.05,0.0);
	from = TANGOimage_loc(white,TANGO_PART_TYPE_C);
    to = (TANGO_LOC)ASSOCretrieve("NUMBER",IDVAL,in_order);
	path = TANGOpath_distance(from,to,1.0);
	move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,white,path);
	TANGOtrans_perform(move);
	ASSOCstore("IMAGE_AT",IDVAL,to,white);
	parloc = (TANGO_LOC)ASSOCretrieve("PARENT",IDVAL,to);
   	if (parloc)
   	{	parnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,parloc);
       	TANGOloc_inquire(TANGOimage_loc(parnode,TANGO_PART_TYPE_S),&sx,&sy);
       	TANGOloc_inquire(TANGOimage_loc(white,TANGO_PART_TYPE_N),&nx,&ny);
       	TANGOimage_create(TANGO_IMAGE_TYPE_LINE,sx,sy,1,TANGO_COLOR_WHITE,
                       nx-sx,ny-sy,1.0,1.0,0);
/*
       	line = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,sx,sy,0,TANGO_COLOR_WHITE,
                       nx-sx,ny-sy,0.0,1.0,0);
   		onepath = TANGOpath_null(1);
		move = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,line,path);
*/
   	}
}

/*The following was a FUTILE attempt to clean the whole screen by drawing the
white square over the entire area in which the tree is drawn. DESPARATION 
manifests itself in many stupid statements - not removed due to lack of time.*/

void
ANIMCleardiag()
{
	TANGO_IMAGE white;
	TANGO_PATH  onepath,path;
	TANGO_PATH red,black,flash;
	TANGO_TRANS vis,move,appear,both;
	TANGO_TRANS compare;
	TANGO_LOC from,to;
	white = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,1.0,0,TANGO_COLOR_WHITE,1.0,0.0,1.0);
	from = TANGOimage_loc(white,TANGO_PART_TYPE_C);
   red = TANGOpath_color(TANGO_COLOR_WHITE);
   black = TANGOpath_color(TANGO_COLOR_BLACK);
   flash = TANGOpath_concatenate(7,red,red,black,black,red,red,black);
   compare = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,from,flash);
   TANGOtrans_perform(compare);
	to = TANGOloc_create(0.1,0.1);
	path = TANGOpath_distance(from,to,1.0);
	move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,white,path);
	appear = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,white,path);
	both = TANGOtrans_compose(2,move,appear);
	TANGOtrans_perform(both);
	white = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.1,0.9,0,TANGO_COLOR_WHITE,0.9,0.1,1.0);
	onepath = TANGOpath_null(1);
	vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,white,onepath);
	TANGOtrans_perform(vis);
}

