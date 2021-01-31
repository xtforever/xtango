/**************************************************************/
/*							      */
/*			TWIST PACKAGE			      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*			John T. Stasko			      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/10/26 JDH	      Baselined source.                       */
/*							      */
/**************************************************************/

/**************************************************************/
/*****************	 include files       ******************/
/**************************************************************/

#include  "xtangolocal.h"

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

typedef struct TREENODE {
	WIN_COORD x;
	WIN_COORD y;
	int level;
	int count;
	TANGO_LOC loc;
	struct TREENODE *next;
} *TREENODE_PTR;

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

#define   BOTTOM     0
#define   LEFT	     0
#define   MIDDLE     1
#define   TOP	     2
#define   RIGHT      2

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

void create_rect_array();
void create_line_array();
void create_circle_array();
TREENODE_PTR allocate_node();

/**************************************************************/
/*****************	  entry points       ******************/
/**************************************************************/

/**************************************************************/
/* TWISTcreate_loc_array -- Create a line of TANGO_LOCS       */
/*			    subject to the given parameters.  */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void TWISTcreate_loc_array(assoc, id, num, horiz, x, y, spacing)
char *assoc;
int id;
int num;
int horiz;
WIN_COORD x, y;
WIN_COORD spacing;
{
	int i;
	TANGO_LOC loc;

	DEBUG("TWISTcreate_loc_array(\"%s\", %d, %d, %d, %lf, %lf, %lf)\n",
	      assoc, id, num, horiz, x, y, spacing);

	for (i = 0; i < num; ++i) {
		loc = TANGOloc_create(x, y);
		if (!assoc)
			ASSOCstore("ID", id, i, loc);
		else
			ASSOCstore(assoc, id, i, loc);
		if (horiz)
			x += spacing;
		else
			y += spacing;
	}
}

/**************************************************************/
/* TWISTcreate_2d_loc_array -- Create a grid of TANGO_LOCS    */
/*			       subject to the given	      */
/*			       parameters.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void TWISTcreate_2d_loc_array(assoc, id, rows, cols, x, y, xspacing, yspacing)
char *assoc;
int id;
int rows, cols;
WIN_COORD x, y;
WIN_COORD xspacing, yspacing;
{
	int r, c;
	WIN_COORD px, py;
	TANGO_LOC loc;

	DEBUG
	    ("TWISTcreate_2d_loc_array(\"%s\", %d, %d, %d, %lf, %lf, %lf, %lf)\n",
	     assoc, id, rows, cols, x, y, xspacing, yspacing);

	py = y;
	for (r = 0; r < rows; ++r) {
		px = x;
		for (c = 0; c < cols; ++c) {
			loc = TANGOloc_create(px, py);
			if (!assoc)
				ASSOCstore("ID3", id, r, c, loc);
			else
				ASSOCstore(assoc, id, r, c, loc);
			px += xspacing;
		}
		py += yspacing;
	}
}

/**************************************************************/
/* TWISTcreate_image_array -- Create a row or column of       */
/*     			      TANGO_IMAGEs subject to the     */
/*			      given parameters.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
TWISTcreate_image_array(assoc, id, num, type, horiz, just, x, y, xvals, xfactor,
			yvals, yfactor, spacing, vis, color, fill, width, style)
char *assoc;
int id;
int num;
TANGO_IMAGE_TYPE type;
int horiz;			/*  1-horiz, 0-vert                     */
int just;			/*  justification: 0,1, or 2            */
WIN_COORD x, y;			/*  starting placement loc              */
double xvals[];			/*  width factors (if NULL, constant)   */
WIN_COORD xfactor;		/*  multiply by xvals to get widths     */
double yvals[];			/*  height factors (if NULL, constant   */
WIN_COORD yfactor;		/*  multiply by yvals to get heights    */
WIN_COORD spacing;		/*  spacing between images              */
int vis;
int color;
double fill;
double width;
double style;
{
	DEBUG
	    ("TWISTcreate_image_array(\"%s\",%d,%d,0x%d,%d,%d,%lf,%lf,0x%p,%lf,0x%p,%lf,%lf,%d,%d,%lf,%lf,%lf)\n",
	     assoc, id, num, type, horiz, just, x, y, xvals, xfactor, yvals,
	     yfactor, spacing, vis, color, fill, width, style);

	switch (type) {
	case TANGO_IMAGE_TYPE_LINE:
		create_line_array(assoc, id, num, horiz, just, x, y, xvals,
				  xfactor, yvals, yfactor, spacing, vis, color,
				  width, style);
		break;
	case TANGO_IMAGE_TYPE_RECTANGLE:
		create_rect_array(assoc, id, num, horiz, just, x, y, xvals,
				  xfactor, yvals, yfactor, spacing, vis, color,
				  fill);
		break;
	case TANGO_IMAGE_TYPE_CIRCLE:
		create_circle_array(assoc, id, num, horiz, just, x, y, xvals,
				    xfactor, yvals, yfactor, spacing, vis,
				    color, fill);
		break;
	default:;
	}
	return;
}

/**************************************************************/
/* TWISTcreate_graph -- Create an array of TANGO_LOC 	      */
/*			locations and an adjacency matrix,    */
/*			lay out a graph subject to the other  */
/*			parameters.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void TWISTcreate_graph(assoc, id, num, loc, adj, im_type, size, fill)
char *assoc;
int id;
int num;
TANGO_LOC loc[];
int adj[][50];
TANGO_IMAGE_TYPE im_type;
WIN_COORD size;
double fill;
{
	WIN_COORD hs;
	int v, w;
	WIN_COORD x, y, x0, y0, x1, y1;
	TANGO_IMAGE im;

	struct _IMAGE image;
	struct _TANGO_CIRCLE circ;
	struct _TANGO_RECTANGLE rec;
	struct _TANGO_LINE line;

	DEBUG("TWISTcreate_graph(\"%s\", %d,% d, 0x%p, 0x%p, %d, %lf, %lf)\n",
	      assoc, id, num, loc, adj, im_type, size, fill);

	hs = size / 2.0;
	for (v = 0; v < num; ++v) {
		TANGOloc_inquire(loc[v], &x, &y);
		if (im_type == TANGO_IMAGE_TYPE_RECTANGLE) {
			image.type = TANGO_IMAGE_TYPE_RECTANGLE;
			image.loc[0] = x - hs;
			image.loc[1] = y - hs;
			image.visible = 1;
			rec.color = TANGO_COLOR_BLACK;
			rec.fill = fill;
			image.object = &rec;
			im = TANGOimage_create(&image);
//                       two times size ?? 
//         im = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x-hs,y-hs,1,TANGO_COLOR_BLACK,size,size,fill);

		} else if (im_type == TANGO_IMAGE_TYPE_CIRCLE) {
			image.type = TANGO_IMAGE_TYPE_CIRCLE;
			image.loc[0] = x;
			image.loc[1] = y;
			image.visible = 1;
			circ.color = TANGO_COLOR_BLACK;
			circ.radius = size;
			circ.fill = fill;
			image.object = &circ;
			im = TANGOimage_create(&image);
			//       im = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x,y,1,TANGO_COLOR_BLACK,size,fill);                             
		} else
			return;

		ASSOCstore("IMAGE_AT", id, loc[v], im);
	}

	ASSOCmake("EDGE_BETWEEN", 3);
	for (v = 0; v < num - 1; ++v)
	  for (w = v + 1; w < num; ++w) {

	    if (!adj[v][w]) continue; 

	      TANGOloc_inquire(loc[v], &x0, &y0);
	      TANGOloc_inquire(loc[w], &x1, &y1);

	      image.type = TANGO_IMAGE_TYPE_LINE;
	      image.loc[0] = x0;
	      image.loc[1] = y0;
	      image.visible = 1;
	      line.color = TANGO_COLOR_BLACK;
	      line.size[0] = x1 - x0;
	      line.size[1] = y1 - y0;
	      line.width = 0;
	      line.style = 1;
	      line.arrow = 0;

	      image.object = &line;

	      im = TANGOimage_create(&image);
//            im = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x0,y0,1,TANGO_COLOR_BLACK,x1-x0,y1-y0,0.0,1.0,0);

	      ASSOCstore("EDGE_BETWEEN", id, v, w, im);
	      ASSOCstore("EDGE_BETWEEN", id, w, v, im);

	  }
}

/**************************************************************/
/* TWISTcreate_bintree -- This routine lays out a binary tree */
/*			  of TANGO_LOCs and sets up some      */
/*			  associations so they can be         */
/*			  retrieved.  It does a breadth-first */
/*			  search basically.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void TWISTcreate_bintree(treeid, lx, by, rx, ty, edgelevels)
int treeid;			/* used in association */
WIN_COORD lx, by, rx, ty;
int edgelevels;
{
	int i, nextlevel, number;
	int numonlevel[16];
	WIN_COORD width[16];
	TREENODE_PTR node, lnode, rnode, Qbeg, Qend, old;
	WIN_COORD x, y, wid, dy;
	TANGO_LOC loc;

	DEBUG("TWISTcreate_bintree(%d, %lf, %lf, %lf, %lf, %d)\n",
	      treeid, lx, by, rx, ty, edgelevels);

	if ((rx <= lx) || (by <= ty)) {
		fprintf(stderr,
			"Illegal box coords passed to TWISTcreate_bintree\n");
		return;
	}

	ASSOCmake("NODE", 3);
	ASSOCmake("NUMBER", 2);
	ASSOCmake("PARENT", 2);
	ASSOCmake("LCHILD", 2);
	ASSOCmake("RCHILD", 2);

	for (i = 0; i < 16; ++i)
		numonlevel[i] = 0;
	width[0] = (rx - lx) / 2.0;
	for (i = 1; i < 16; ++i)
		width[i] = width[i - 1] / 2.0;

	number = 0;
	dy = (by - ty) / edgelevels;
	loc = TANGOloc_create(lx + width[0], ty);
	node = allocate_node(lx + width[0], ty, 0, 0, loc);
	ASSOCstore("PARENT", treeid, loc, NULL);
	ASSOCstore("NODE", treeid, 0, 0, loc);
	ASSOCstore("NUMBER", treeid, number++, loc);
	Qbeg = Qend = node;

	while (Qbeg) {
		if (Qbeg->level == edgelevels) {	/* at the leaf level */
			ASSOCstore("LCHILD", treeid, Qbeg->loc, NULL);
			ASSOCstore("RCHILD", treeid, Qbeg->loc, NULL);
			old = Qbeg;
			Qbeg = Qbeg->next;
			free(old);
		} else {
			nextlevel = Qbeg->level + 1;
			wid = width[nextlevel];
			x = Qbeg->x - wid;
			y = Qbeg->y + dy;
			loc = TANGOloc_create(x, y);
			lnode =
			    allocate_node(x, y, nextlevel,
					  numonlevel[nextlevel]++, loc);
			ASSOCstore("PARENT", treeid, loc, Qbeg->loc);
			ASSOCstore("LCHILD", treeid, Qbeg->loc, loc);
			ASSOCstore("NODE", treeid, lnode->level,
				   lnode->count, loc);
			ASSOCstore("NUMBER", treeid, number++, loc);

			x = Qbeg->x + wid;
			loc = TANGOloc_create(x, y);
			rnode =
			    allocate_node(x, y, nextlevel,
					  numonlevel[nextlevel]++, loc);
			ASSOCstore("PARENT", treeid, loc, Qbeg->loc);
			ASSOCstore("RCHILD", treeid, Qbeg->loc, loc);
			ASSOCstore("NODE", treeid, lnode->level,
				   lnode->count, loc);
			ASSOCstore("NUMBER", treeid, number++, loc);

			Qend->next = lnode;	/* add the 2 new ones to the queue */
			lnode->next = rnode;
			Qend = rnode;
			old = Qbeg;
			Qbeg = Qbeg->next;
			free(old);
		}
	}
}

/*
 *  FIXME:
 *  this is overcomplicated, it can be condenced
 *  into one function 
 *  1. make image+data structure
 *  2. use general routine to move around
 */

/**************************************************************/
/* create_line_array -- Create a row or column of lines       */
/*			according to the given parameters.    */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void create_line_array(assoc, id, num, horiz, just, x, y, xvals,
		       xfactor, yvals, yfactor, spacing, vis, color,
		       width, style)
char *assoc;
int id;
int num;
int horiz;			/*  1-horiz, 0-vert                     */
int just;			/*  justification: 0,1, or 2            */
WIN_COORD x, y;			/*  starting placement loc              */
double xvals[];			/*  width factors (if NULL, constant)   */
WIN_COORD xfactor;		/*  multiply by xvals to get widths     */
double yvals[];			/*  height factors (if NULL, constant   */
WIN_COORD yfactor;		/*  multiply by yvals to get heights    */
WIN_COORD spacing;		/*  spacing between images              */
int vis;
int color;
double width;
double style;
{
	int i;
	WIN_COORD px, py;	/* running "location" of next image */
	WIN_COORD sx, sy;	/* sizes */
	TANGO_IMAGE im;
	struct _IMAGE image;
	struct _TANGO_LINE line;

	px = x;
	py = y;

	image.type = TANGO_IMAGE_TYPE_LINE;
	image.loc[0] = px;
	image.loc[1] = py;
	image.visible = vis;
	line.color = color;
	line.size[0] = 0;
	line.size[1] = 0;
	line.width = width;
	line.style = style;
	line.arrow = 0;

	image.object = &line;

	for (i = 0; i < num; ++i) {
		if (horiz) {
			if (yvals)
				sy = yvals[i] * yfactor;
			else
				sy = yfactor;

			line.size[0] = sx;
			line.size[1] = sy;
			image.loc[0] = px;
			image.loc[1] = py;

			if (just == BOTTOM) {
				image.loc[1] = py - sy;
				im = TANGOimage_create(&image);
//              im = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,px,py-sy,vis,       color,0.0,sy,width,style,0);
			} else if (just == MIDDLE) {
				image.loc[1] = py - (sy / 2.0);
				im = TANGOimage_create(&image);
//              im =  TANGOimage_create(TANGO_IMAGE_TYPE_LINE,px,py-(sy/2.0),vis,color,0.0,sy,width,style,0);
			} else if (just == TOP) {
				image.loc[1] = py;
				im = TANGOimage_create(&image);
				//im = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,px,py,vis,          color,0.0,sy,width,style,0);
			}
			px += spacing;
		} else {	/* vertical */
			if (xvals)
				sx = xvals[i] * xfactor;
			else
				sx = xfactor;

			if (just == LEFT) {
				image.loc[0] = px;
				im = TANGOimage_create(&image);
				//im = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,px,py,vis,color,sx,0.0,width,style,0);
			} else if (just == MIDDLE) {
				image.loc[0] = px - (sx / 2.0);
				im = TANGOimage_create(&image);
				//im =  TANGOimage_create(TANGO_IMAGE_TYPE_LINE,px-(sx/2.0),py,vis,color,sx,0.0,width,style,0);
			} else if (just == RIGHT) {
				image.loc[0] = px - sx;
				im = TANGOimage_create(&image);
				//im = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,px-sx,py,vis,color,sx,0.0,width,style,0);
			}
			py += spacing;
		}

		if (!assoc)
			ASSOCstore("ID", id, i, im);
		else
			ASSOCstore(assoc, id, i, im);
	}
}

/**************************************************************/
/* create_rect_array -- Create a row or column of rectangles  */
/*			according to the given parameters.    */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void create_rect_array(assoc, id, num, horiz, just, x, y, xvals,
		       xfactor, yvals, yfactor, spacing, vis, color, fill)
char *assoc;
int id;
int num;
int horiz;			/*  1-horiz, 0-vert                     */
int just;			/*  justification: 0,1, or 2            */
WIN_COORD x, y;			/*  starting placement loc              */
double xvals[];			/*  width factors (if NULL, constant)   */
WIN_COORD xfactor;		/*  multiply by xvals to get widths     */
double yvals[];			/*  height factors (if NULL, constant   */
WIN_COORD yfactor;		/*  multiply by yvals to get heights    */
WIN_COORD spacing;		/*  spacing between images              */
int vis;
int color;
double fill;
{
	int i;
	WIN_COORD px, py;	/* running "location" of next image */
	WIN_COORD sx, sy;	/* sizes */
	TANGO_IMAGE im;
	struct _IMAGE image;
	struct _TANGO_RECTANGLE rec;;

	px = x;
	py = y;

	image.type = TANGO_IMAGE_TYPE_RECTANGLE;
	image.loc[0] = px;
	image.loc[1] = py;
	image.visible = vis;
	rec.color = color;
	rec.fill = 0;

	image.object = &rec;

	for (i = 0; i < num; ++i) {
		if (xvals)
			sx = xvals[i] * xfactor;
		else
			sx = xfactor;
		if (yvals)
			sy = yvals[i] * yfactor;
		else
			sy = yfactor;

		rec.size[0] = sx;
		rec.size[1] = sy;
		image.loc[0] = px;
		image.loc[1] = py;

		if (horiz) {
			if (just == BOTTOM) {
				image.loc[1] = py - sy;
				im = TANGOimage_create(&image);
//              im = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,px,py-sy,vis, color,sx,sy,fill);
			} else if (just == MIDDLE) {
				image.loc[1] = py - (sy / 2.0);
				im = TANGOimage_create(&image);
//              im =  TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,px,py-(sy/2.0),vis,color,sx,sy,fill);
			} else if (just == TOP) {
				image.loc[1] = py;
				im = TANGOimage_create(&image);
//              im = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,px,py,vis,color,sx,sy,fill);
			}

			px += sx + spacing;
		} else {	/* vertical */
			if (just == LEFT) {
				image.loc[0] = px;
				im = TANGOimage_create(&image);
//              im = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,px,py,vis,color,sx,sy,fill);
			} else if (just == MIDDLE) {
				image.loc[0] = px - (sx / 2.0);
				im = TANGOimage_create(&image);
//              im =  TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,px-(sx/2.0),py,vis,color,sx,sy,fill);
			} else if (just == RIGHT) {	//px-color makes no sense, changed to px-sx   
				image.loc[0] = px - sx;
				im = TANGOimage_create(&image);
//              im = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,px-color,sx,py,vis,sx,sy,fill);
			}

			py += sy + spacing;
		}

		if (!assoc)
			ASSOCstore("ID", id, i, im);
		else
			ASSOCstore(assoc, id, i, im);
	}
}

/**************************************************************/
/* create_circle_array -- Create a row or column of circles   */
/*			  according to the given parameters.  */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void create_circle_array(assoc, id, num, horiz, just, x, y, xvals,
			 xfactor, yvals, yfactor, spacing, vis, color, fill)
char *assoc;
int id;
int num;
int horiz;			/*  1-horiz, 0-vert                     */
int just;			/*  justification: 0,1, or 2            */
WIN_COORD x, y;			/*  starting placement loc              */
double xvals[];			/*  width factors (if NULL, constant)   */
WIN_COORD xfactor;		/*  multiply by xvals to get widths     */
double yvals[];			/*  height factors (if NULL, constant   */
WIN_COORD yfactor;		/*  multiply by yvals to get heights    */
WIN_COORD spacing;		/*  spacing between images              */
int vis;
int color;
double fill;
{
	int i;
	WIN_COORD px, py;	/* running "location" of next image */
	WIN_COORD rad;		/* radius of circles */
	TANGO_IMAGE im;
	struct _IMAGE image;
	struct _TANGO_CIRCLE circ;

	px = x;
	py = y;

	image.type = TANGO_IMAGE_TYPE_CIRCLE;
	image.loc[0] = px;
	image.loc[1] = py;
	image.visible = vis;
	circ.color = color;
	circ.radius = 0;
	circ.fill = 0;

	image.object = &circ;;

	for (i = 0; i < num; ++i) {
		if (xvals)
			rad = xvals[i] * xfactor;
		else
			rad = xfactor;

		circ.radius = rad;
		image.loc[0] = px;
		image.loc[1] = py;

		if (horiz) {
			if (just == BOTTOM) {
				image.loc[1] = py - rad;
				im = TANGOimage_create(&image);
//            im = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,px,py-rad,vis,color,rad,fill);
			}

			else if (just == MIDDLE) {
				image.loc[1] = py;
				im = TANGOimage_create(&image);
//      im =  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,px,py,vis,color,rad,fill);
			}

			else if (just == TOP) {
				image.loc[1] = py + rad;
				im = TANGOimage_create(&image);
//      im = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,px,py+rad,vis,color,rad,fill);
			}

			px += (2.0 * rad) + spacing;
		} else {	/* vertical */
			if (just == LEFT) {
				image.loc[0] = px + rad;
				im = TANGOimage_create(&image);
//              im = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,px+rad,py,vis,color,rad,fill);
			} else if (just == MIDDLE) {
				image.loc[0] = px;
				im = TANGOimage_create(&image);
//im =  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,px,py,vis,color,rad,fill);
			} else if (just == RIGHT) {
				image.loc[0] = px - rad;
				im = TANGOimage_create(&image);
//im = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,px-rad,py,vis,color,rad,fill);
			}

			py += (2.0 * rad) + spacing;
		}

		if (!assoc)
			ASSOCstore("ID", id, i, im);
		else
			ASSOCstore(assoc, id, i, im);
	}
}

/**************************************************************/
/* allocate_node -- Allocate a node that will go on the queue */
/*		    of tree nodes being set up.		      */
/* 							      */
/* RETURNS:  Node to go on the queue of tree nodes.	      */
/**************************************************************/
TREENODE_PTR allocate_node(x, y, level, count, loc)
WIN_COORD x;
WIN_COORD y;
int level; 
int count;
TANGO_LOC loc;
{
	TREENODE_PTR node;

	node = (TREENODE_PTR) malloc(sizeof(struct TREENODE));
	node->x = x;
	node->y = y;
	node->level = level;
	node->count = count;
	node->loc = loc;
	node->next = NULL;
	return (node);
}

/**************************************************************/
/*****************   end of xtangotwist.c    ******************/
/**************************************************************/
