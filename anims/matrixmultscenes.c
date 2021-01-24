/* Matrix mutliplication */
/* Developed by Joshua Bleier */

#include <stdio.h>
#include "xtango.h"

#define   SIZE	   0.04
#define   FCOLOR   TANGO_COLOR_RED
#define   TCOLOR   TANGO_COLOR_BLUE
#define   XCOLOR   TANGO_COLOR_YELLOW
#define   ROW	   0
#define   COLUMN   1

static double   	offset, gap;
static int		curchar;


TANGO_IMAGE  stop;
TANGO_IMAGE  oldedge=NULL;

/* create location and adj matrix associations */

void
ANIMInit()
{
  ASSOCinit();
  ASSOCmake("Array1",2);
  ASSOCmake("Array1Locs",2);
  ASSOCmake("Array2",2);
  ASSOCmake("Array2Locs",2);
  ASSOCmake("Value",2);
  ASSOCmake("ValueLocs",2);
  ASSOCmake("Row",1);
  ASSOCmake("Column",1);
  ASSOCmake("Vals",1);
  ASSOCmake("Chars",1);
  ASSOCmake("Mults",1);
  offset   = .040;
  gap      = .020;
  curchar  = 0;
}


/* Display the 3x3 Array, saving it's contents in the corresponding Assoc Area */

void
ANIMDisplayArray(array, aname, startx, starty)
int array[][3];
char *aname;
double startx, starty;
{
   TANGO_LOC	 loc;
   TANGO_IMAGE	 label;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;
   char 	 str[4], alocs[82];
   double	 x,y;
   int		 i,j;

   sprintf(alocs, "%sLocs", aname);
   y = starty;

   for (i=0; i < 3; i++)
     {
       x = startx;

       for (j=0; j < 3; j++)
	 {
	   ASSOCstore(aname,i,j,array[i][j]);
	   sprintf(str, "%d", array[i][j]);
	   loc = TANGOloc_create(x,y);
	   ASSOCstore(alocs,i,j,loc);
	   label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK, "variable",str,1);
	   path =  TANGOpath_null(1);
	   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,label,path);
	   TANGOtrans_perform(trans);

	   x = x + offset + gap;
	 }
       y = y + offset + gap;
     }
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}



/* Animate the Row of the array */

void
ANIMRow(aname, row, finalx, finaly)
char    *aname;
int      row;
double   finalx, finaly;
{
   TANGO_LOC	 loc;
   TANGO_PATH	 path[3];
   TANGO_IMAGE	 mrow[3];
   TANGO_TRANS	 trans[3];
   TANGO_TRANS	 finalRowTrans;
   int		 j, value;
   double	 x1,y1,x2,y2;
   char		 aloc[15],str[10];
   
   sprintf(aloc, "%sLocs", aname);
   x2 = finalx;
   y2 = finaly;
   for (j=0; j < 3; j++)
     {
       value     = ASSOCretrieve(aname, row, j);
       sprintf(str, "%d", value);
       loc       = (TANGO_LOC) ASSOCretrieve(aloc,  row, j);
       TANGOloc_inquire	(loc, &x1, &y1);
       mrow[j] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x1,y1,1,TANGO_COLOR_BLACK, "variable",str,1);
       ASSOCstore("Row", j, mrow[j]);

       path[j]   = TANGOpath_motion  (TANGOloc_create(x1, y1),
				      TANGOloc_create(x2, y2),
				      TANGO_PATH_TYPE_STRAIGHT);
       trans[j]  = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, mrow[j], path[j]);
       x2    	 = x2 + offset + gap;
     }
   finalRowTrans = TANGOtrans_compose(3, trans[0], trans[1], trans[2]);
   TANGOtrans_perform(finalRowTrans);
}


void
ANIMEraseRow()
{
  TANGO_IMAGE row[3];
  TANGO_PATH  path[3];
  TANGO_TRANS trans[3];
  TANGO_TRANS finalRowTrans;
  int	      j;

  for (j=0; j<3; j++)
    {
      row[j]	= (TANGO_IMAGE) ASSOCretrieve("Row", j);
      path[j]   = TANGOpath_null(1);
      trans[j]  = TANGOtrans_create (TANGO_TRANS_TYPE_VISIBLE, row[j], path[j]);
    }
  
   finalRowTrans = TANGOtrans_compose(3, trans[0], trans[1], trans[2]);
   TANGOtrans_perform(finalRowTrans);

  for (j=0; j<3; j++)
    {
      TANGOpath_free(1,path[j]);
      TANGOtrans_free(1,trans[j]);
    }
}


/* Animate the column of the array */
void
ANIMColumn(aname, col, finalx, finaly)
char    *aname;
int      col;
double   finalx, finaly;
{
   TANGO_LOC	 loc;
   TANGO_PATH	 path[3];
   TANGO_IMAGE	 mcol[3];
   TANGO_TRANS	 trans[3];
   TANGO_TRANS	 finalColTrans;
   int		 j, value;
   double	 x1,y1,x2,y2;
   char		 aloc[15],str[10];
   
   sprintf(aloc, "%sLocs", aname);
   x2 = finalx;
   y2 = finaly - (offset + gap);
   for (j=0; j < 3; j++)
     {
       value     = ASSOCretrieve(aname, j, col);
       sprintf(str, "%d", value);
       loc       = (TANGO_LOC) ASSOCretrieve(aloc,  j, col);
       TANGOloc_inquire	(loc, &x1, &y1);
       mcol[j] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x1,y1,1,TANGO_COLOR_BLACK, "variable",str,1);
       ASSOCstore("Column", j, mcol[j]);

       path[j]   = TANGOpath_motion  (TANGOloc_create(x1, y1),
				      TANGOloc_create(x2, y2),
				      TANGO_PATH_TYPE_STRAIGHT);
       trans[j]  = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, mcol[j], path[j]);
       y2    	 = y2 + offset + gap;
     }
   finalColTrans = TANGOtrans_compose(3, trans[0], trans[1], trans[2]);
   TANGOtrans_perform(finalColTrans);
}


/* Erase the Column */
void
ANIMEraseColumn()
{
  TANGO_IMAGE col[3];
  TANGO_PATH  path[3];
  TANGO_TRANS trans[3];
  TANGO_TRANS finalColTrans;
  int	      j;

  for (j=0; j<3; j++)
    {
      col[j]	= (TANGO_IMAGE) ASSOCretrieve("Column", j);
      path[j]   = TANGOpath_null(1);
      trans[j]  = TANGOtrans_create (TANGO_TRANS_TYPE_VISIBLE, col[j], path[j]);
    }
  
   finalColTrans = TANGOtrans_compose(3, trans[0], trans[1], trans[2]);
   TANGOtrans_perform(finalColTrans);

  for (j=0; j<3; j++)
    {
      TANGOpath_free(1,path[j]);
      TANGOtrans_free(1,trans[j]);
    }
}


/* Multiply the appropriate values of the row and the column */

void
ANIMMultiply(i, finalx, finaly, finalval)
int	i;
double	finalx, finaly;
int	finalval;
{
   TANGO_IMAGE 	rval, cval;
   TANGO_PATH 	rpath, cpath;
   TANGO_LOC 	rloc, cloc;
   TANGO_TRANS 	rtrans, ctrans, final;
   char		str[10];
   double	rx, ry, cx, cy;
   int 		j;

   sprintf(str, "%d", finalval);
   /* Get the row and column values to multiply */
   rval = TANGOimage_copy((TANGO_IMAGE) ASSOCretrieve("Row", i));
   rloc = TANGOimage_loc(rval);
   cval = TANGOimage_copy((TANGO_IMAGE) ASSOCretrieve("Column", i));
   cloc = TANGOimage_loc(cval);
   TANGOloc_inquire	(cloc, &cx, &cy);

   /* Move them to the same spot */
   rpath = TANGOpath_motion(rloc,  TANGOloc_create(finalx, finaly), TANGO_PATH_TYPE_CLOCKWISE);
   cpath = TANGOpath_motion(cloc,  TANGOloc_create(finalx, finaly), TANGO_PATH_TYPE_CLOCKWISE);
   rtrans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rval, rpath);
   ctrans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cval, cpath);
   final = TANGOtrans_compose(2, rtrans, ctrans);
   TANGOtrans_perform(final);
   
   /* Make them invisible */
   rpath = TANGOpath_null(1);
   cpath = TANGOpath_null(1);
   rtrans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, rval, rpath);
   ctrans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, cval, cpath);
   final = TANGOtrans_compose(2, rtrans, ctrans);
   TANGOtrans_perform(final);

   /* Put the new value there */
   rval  = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,finalx,finaly,1,
			     TANGO_COLOR_BLACK, "variable",str,1);
   ASSOCstore("Vals", i, rval);

   rpath = TANGOpath_null(1);
   final = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rval, rpath);
   TANGOtrans_perform(final);
   ASSOCstore("Mults", i, rval);
}


/* Multiply the appropriate values of the row and the column */

void
ANIMEraseMultiply()
{
   TANGO_IMAGE 	mvals[3];
   TANGO_PATH 	mpaths[3];
   TANGO_TRANS 	mtrans[3], final;
   int 		j;

   for (j=0; j<3; j++)
     {
       mvals[j]  = (TANGO_IMAGE) ASSOCretrieve("Mults", j);
       mpaths[j] = TANGOpath_null(1);
       mtrans[j] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, mvals[j], mpaths[j]);
     }
   final = TANGOtrans_compose(3, mtrans[0], mtrans[1], mtrans[2]);
   TANGOtrans_perform(final);


  for (j=0; j<3; j++)
    {
      TANGOpath_free (1,mpaths[j]);
      TANGOtrans_free(1,mtrans[j]);
    }
}


/* Sum the Multiplied Values */
void
ANIMSum(sum, tempx, tempy, finalx, finaly)
int	sum;
double	tempx, tempy;
double	finalx, finaly;
{
   TANGO_IMAGE 	tvals[3], sval;
   TANGO_LOC	tlocs[3], sloc;
   TANGO_PATH 	tpaths[3], spath;
   TANGO_TRANS 	ttrans[3], strans, final;
   char		str[10];
   int		i;

   /* Move the values to the initial location */
   sprintf(str, "%d", sum);
   for (i=0; i<3; i++)
     {
       tvals[i] = TANGOimage_copy((TANGO_IMAGE) ASSOCretrieve("Vals", i, ROW));
       tlocs[i] = TANGOimage_loc(tvals[i], TANGO_PART_TYPE_C);
     }
   sloc = TANGOloc_create(tempx, tempy);

   for (i=0; i<3; i++)
     {
       tpaths[i] = TANGOpath_motion(tlocs[i], sloc, TANGO_PATH_TYPE_CLOCKWISE);
       ttrans[i] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, tvals[i], tpaths[i]);
     }
   final  = TANGOtrans_compose(3, ttrans[0], ttrans[1], ttrans[2]);
   TANGOtrans_perform(final);

   /* Erase the Temp Values */
   for (i=0; i<3; i++)
     {
       tpaths[i] = TANGOpath_null(1);
       ttrans[i] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, tvals[i], tpaths[i]);
     }
   final  = TANGOtrans_compose(3, ttrans[0], ttrans[1], ttrans[2]);
   TANGOtrans_perform(final);

   /* Display the Sum */
   sval  = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,tempx,tempy,1,
			     TANGO_COLOR_BLACK, "variable",str,1);
   spath  = TANGOpath_null(1);
   strans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, sval, spath);
   TANGOtrans_perform(strans);

   /* Move the Sum to its final destination */
   spath  = TANGOpath_motion(sloc, TANGOloc_create(finalx, finaly), TANGO_PATH_TYPE_STRAIGHT);
   strans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, sval, spath);
   TANGOtrans_perform(strans);
}


/* Multiply the appropriate values of the row and the column */
void
ANIMDisplayChar(dchar, x, y, storep)
char    *dchar;
double	x,y;
int	storep;
{
   TANGO_IMAGE text;
   TANGO_PATH  npath;
   TANGO_TRANS t1;

   text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK, "variable", dchar,1);
   npath = TANGOpath_null(1);
   t1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, text, npath);
   TANGOtrans_perform(t1);
   if (storep == 1)
     {
       ASSOCstore("Chars", curchar);
       curchar = curchar + 1;
     }
}


/* Erase all Characters constructed as Erasable */
void
ANIMEraseChars()
{
   TANGO_IMAGE  text;
   TANGO_PATH   npath;
   TANGO_TRANS  t1;
   int		i;

   for (i=0; i<curchar; i++)
     {
       text = (TANGO_IMAGE) ASSOCretrieve("Chars", i);
       npath = TANGOpath_null(1);
       t1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, text, npath);
       TANGOtrans_perform(t1);

       TANGOpath_free (1,npath);
       TANGOtrans_free(1,t1);
     }
}


/* Place the new value in the Value Array */
void
ANIMPlaceValue(from,to)
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



