/* Polygon clipping    by Kevin Haulk */

#include <stdio.h>
#include "xtango.h"
#include "clip.h"

#define DOTTED 0.3
#define DASHED 0.6
#define SOLID  1.0
#define VISIBLE 1
#define INVISIBLE 0
#define CURSORSIZE 0.01
#define DIST 0.01
#define MARKERSIZE 0.04

int NextNode();
void LookNode();

typedef struct
   {
   TANGO_IMAGE im1;
   TANGO_IMAGE im2;
    } MARK; 

TANGO_IMAGE stop; 
TANGO_IMAGE border[4][3]; 
TANGO_IMAGE *edgelist;
static int num_edges;
TANGO_IMAGE cursor;
MARK *markers;
static int num_vert = 0;


void ANIMInit(left, right, top, bottom)
double left, right, top, bottom;
{
int i, j;
TANGO_PATH null_path;
TANGO_TRANS plot_border;

/* Set up stop symbol */
TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.05, 0.025, 1, TANGO_COLOR_RED,
                  NULL, "STOP", 1);
stop = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.0, 0.0, 1,
                         TANGO_COLOR_RED, 0.1, 0.05, 0.0);

/* Left border */
border[0][0] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, left, 0.0, VISIBLE,
                  TANGO_COLOR_ORANGE, 0.0, top, 0.0, DOTTED, 0);
 border[0][1] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, left, top, VISIBLE,
                  TANGO_COLOR_ORANGE, 0.0, bottom - top, 0.0, SOLID, 0);
border[0][2] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, left, bottom, VISIBLE,
                  TANGO_COLOR_ORANGE, 0.0, 1.0 - bottom, 0.0, DOTTED, 0); 
/* Right border */
border[1][0] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, right, 0.0, VISIBLE,
                  TANGO_COLOR_ORANGE, 0.0, top, 0.0, DOTTED, 0);
border[1][1] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, right, top, VISIBLE,
                  TANGO_COLOR_ORANGE, 0.0, bottom - top, 0.0, SOLID, 0);
border[1][2] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, right, bottom, VISIBLE,
                  TANGO_COLOR_ORANGE, 0.0, 1.0 - bottom, 0.0, DOTTED, 0);
/* Top border */
border[2][0] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.0, top, VISIBLE,
                  TANGO_COLOR_ORANGE, left, 0.0, 0.0, DOTTED, 0);
border[2][1] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, left, top, VISIBLE,
                  TANGO_COLOR_ORANGE, right - left, 0.0, 0.0, SOLID, 0);
border[2][2] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, right, top, VISIBLE,
                  TANGO_COLOR_ORANGE, 1.0 - right, 0.0, 0.0, DOTTED, 0);
/* Bottom border */
border[3][0] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.0, bottom, VISIBLE,
                  TANGO_COLOR_ORANGE, left, 0.0, 0.0, DOTTED, 0);
border[3][1] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, left, bottom, VISIBLE,
                  TANGO_COLOR_ORANGE, right - left, 0.0, 0.0, SOLID, 0);
border[3][2] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, right, bottom, VISIBLE,
                  TANGO_COLOR_ORANGE, 1.0 - right, 0.0, 0.0, DOTTED, 0);

null_path = TANGOpath_null(1);
for (i = 0; i < 4; i++)
   for (j = 0; j < 3; j++)
      {
      plot_border = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,
                                border[i][j], null_path);
      TANGOtrans_perform(plot_border);
      }

cursor = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.0, 0.0,
                           INVISIBLE, TANGO_COLOR_RED, CURSORSIZE, 1.0);
markers = (MARK *)malloc(sizeof(MARK));
edgelist = (TANGO_IMAGE *)malloc(sizeof(TANGO_IMAGE));
}




char *ANIMVertex()
{
double x,y;
char *ret;
char str[3];
int i;
TANGO_PATH null_path;
TANGO_TRANS disp_trans;

if (TANGOinput_coord(&x, &y))
   {
   if ((x >= 0.0) && (x <= 0.1) && (y >= 0.0) && (y <= 0.05) && 
              (num_vert >= 3))
         return(NULL);
   num_vert++;
   markers = (MARK *)realloc(markers, num_vert * sizeof(MARK));
   markers[num_vert - 1].im1 = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
                  x, y, VISIBLE, TANGO_COLOR_BLACK,
                  0.005, 1.0);
   sprintf(str, "%d", num_vert - 1);
   markers[num_vert - 1].im2 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, 
                  y - .015, 1, TANGO_COLOR_BLACK, "6x9", str, 1);
   null_path = TANGOpath_null(1);
   disp_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, 
                  markers[num_vert-1].im1, null_path);
   TANGOtrans_perform(disp_trans);
   disp_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,
                  markers[num_vert-1].im2, null_path);
   TANGOtrans_perform(disp_trans);
   ret = (char *)malloc(64 * sizeof(char));
   sprintf(ret, "%lf %lf", x, y);
   return(ret);
   }
else
   return(NULL);
}





void ANIMFreeMark()
{
int i, j;
TANGO_PATH null_path;
TANGO_TRANS delete_trans;

null_path = TANGOpath_null(1);
for (i = 0; i < num_vert; i++)
   {
   delete_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, markers[i].im1,
                     null_path);
   TANGOtrans_perform(delete_trans);
   delete_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, markers[i].im2,
                     null_path);
   TANGOtrans_perform(delete_trans);
   free(markers[i]);
   }
free(markers);
}





void ANIMHilite(side)
char side;

/* Highlights a side of the border for clipping */

{
TANGO_PATH color_path;
TANGO_TRANS color_trans;
int i, j;

/* Make sure all sides are black */
color_path = TANGOpath_color(TANGO_COLOR_ORANGE); 
for (i = 0; i < 4; i++)
   for (j = 0; j < 3; j++)
      {
      color_trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, border[i][j],
                        color_path);
      TANGOtrans_perform(color_trans);
      }
      
/* Highlight appropriate side */
color_path = TANGOpath_color(TANGO_COLOR_GREEN);
if (side == 'l') i = 0;
else if (side == 'r') i = 1;
else if (side == 't') i = 2;
else if (side == 'b') i = 3;

if (side != 'n')
   for (j = 0; j < 3; j++)
      {
      color_trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, border[i][j],
                                    color_path);
      TANGOtrans_perform(color_trans);
      }
}





void ANIMUpdatePoly(polygon)
NODE *polygon;

/* Updates the polygon on the screen */

{
VERTEX currvert;
VERTEX nextvert; 
NODE *next; 
NODE *current;
TANGO_PATH null_path;
TANGO_TRANS delete_trans;
TANGO_TRANS show_trans;
int i, j;

null_path = TANGOpath_null(1);

if (edgelist != NULL)
   {
   for (i = 0; i < num_edges; i++)
      {
      delete_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, edgelist[i],
                                    null_path);
      TANGOtrans_perform(delete_trans);
      }

   num_edges = 0;
   free(edgelist);
   }

if (polygon != NULL)
   {
   edgelist = (TANGO_IMAGE *)malloc(sizeof(TANGO_IMAGE));

   current = polygon;
   NextNode(polygon, current, &next);
   do
      {
      LookNode(current, &currvert);
      LookNode(next, &nextvert);
      edgelist = (TANGO_IMAGE *)realloc(edgelist, ++num_edges * 
                                              sizeof(TANGO_IMAGE));
      edgelist[num_edges - 1] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
                             currvert.xval, currvert.yval, VISIBLE,
                             TANGO_COLOR_BLACK, 
                             nextvert.xval - currvert.xval,
                             nextvert.yval - currvert.yval,
                             0.5, SOLID, 0);
      current = next;
      } while (NextNode(polygon, current, &next));
   
   for (i = 0; i < num_edges; i++)
      {
      show_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, edgelist[i],
                                    null_path);
      TANGOtrans_perform(show_trans);
      }
   }
}





void ANIMSetCursor(pos, isinside)
VERTEX pos;
int isinside;

/* Sets up the cursor */

{
TANGO_LOC from, to;
TANGO_PATH cursor_path;
TANGO_TRANS cursor_trans;
TANGO_PATH null_path;
TANGO_TRANS visi_trans;
TANGO_PATH color_path;
TANGO_TRANS color_trans;
TANGO_TRANS total_trans;

double xoffset[2], yoffset[2];

xoffset[0] = yoffset[0] = 0.0;
null_path = TANGOpath_null(1);
visi_trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, cursor, null_path);

from = TANGOimage_loc(cursor, TANGO_PART_TYPE_C);
xoffset[1] = pos.xval - TANGOloc_X(from);
yoffset[1] = pos.yval - TANGOloc_Y(from);
cursor_path = TANGOpath_create(2, xoffset, yoffset);
cursor_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cursor, cursor_path);
color_path = TANGOpath_color((isinside) ? TANGO_COLOR_BLUE : TANGO_COLOR_RED);
color_trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, cursor, color_path);

total_trans = TANGOtrans_concatenate(3, cursor_trans, color_trans, visi_trans);
TANGOtrans_perform(total_trans);
}





void ANIMHideCursor()

/* Hides the cursor */
{
TANGO_PATH null_path;
TANGO_TRANS visi_trans;

null_path = TANGOpath_null(1);
visi_trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, cursor, null_path);
TANGOtrans_perform(visi_trans);
}





void ANIMMoveInIn(from, to)
VERTEX from, to;

/* Moves the cursor inside the window */

{
TANGO_LOC fromloc;
TANGO_LOC toloc;
TANGO_PATH cursor_path;
TANGO_TRANS cursor_trans;

fromloc = TANGOloc_create(from.xval, from.yval);
toloc = TANGOloc_create(to.xval, to.yval);

cursor_path = TANGOpath_distance(fromloc, toloc, DIST);
cursor_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cursor, cursor_path);

TANGOtrans_perform(cursor_trans);
}





void ANIMMoveInOut(from, to, interx, intery)
VERTEX from, to;
double interx, intery;

{
TANGO_LOC fromloc, toloc, interloc;
TANGO_PATH color_path;
TANGO_TRANS color_trans;
TANGO_PATH cursor_path1;
TANGO_TRANS cursor_trans1;
TANGO_PATH cursor_path2;
TANGO_TRANS cursor_trans2;
TANGO_TRANS total_trans;

fromloc = TANGOloc_create(from.xval, from.yval);
interloc = TANGOloc_create(interx, intery);
toloc = TANGOloc_create(to.xval, to.yval);
 
TANGOimage_create(TANGO_IMAGE_TYPE_LINE, interx, intery, VISIBLE, 
                  TANGO_COLOR_BLACK, to.xval - interx, to.yval - intery,
                  0.0, DASHED, 0);
color_path = TANGOpath_color(TANGO_COLOR_RED);
color_trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, cursor, color_path);
cursor_path1 = TANGOpath_distance(fromloc, interloc, DIST);
cursor_trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cursor, cursor_path1);
cursor_path2 = TANGOpath_distance(interloc, toloc, DIST);
cursor_trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cursor, cursor_path2);

total_trans = TANGOtrans_concatenate(3, cursor_trans1, color_trans,
                                        cursor_trans2);
TANGOtrans_perform(total_trans);
}





void ANIMMoveOutOut(from, to)
VERTEX from, to;

{
TANGO_LOC fromloc, toloc;
TANGO_PATH cursor_path;
TANGO_TRANS cursor_trans;

fromloc = TANGOloc_create(from.xval, from.yval);
toloc = TANGOloc_create(to.xval, to.yval);
 
TANGOimage_create(TANGO_IMAGE_TYPE_LINE, from.xval, from.yval, VISIBLE, 
                  TANGO_COLOR_BLACK, to.xval - from.xval, to.yval - from.yval,
                  0.0, DASHED, 0);
cursor_path = TANGOpath_distance(fromloc, toloc, DIST);
cursor_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cursor, cursor_path);

TANGOtrans_perform(cursor_trans);
}





void ANIMMoveOutIn(from, to, interx, intery)
VERTEX from, to;
double interx, intery;

{
TANGO_LOC fromloc, toloc, interloc;
TANGO_PATH color_path;
TANGO_TRANS color_trans;
TANGO_PATH cursor_path1;
TANGO_TRANS cursor_trans1;
TANGO_PATH cursor_path2;
TANGO_TRANS cursor_trans2;
TANGO_TRANS total_trans;

fromloc = TANGOloc_create(from.xval, from.yval);
interloc = TANGOloc_create(interx, intery);
toloc = TANGOloc_create(to.xval, to.yval);
 
TANGOimage_create(TANGO_IMAGE_TYPE_LINE, from.xval, from.yval, VISIBLE, 
                  TANGO_COLOR_BLACK, interx-from.xval, intery-from.yval,
                  0.0, DASHED, 0);
color_path = TANGOpath_color(TANGO_COLOR_BLUE);
color_trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, cursor, color_path);
cursor_path1 = TANGOpath_distance(fromloc, interloc, DIST);
cursor_trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cursor, cursor_path1);
cursor_path2 = TANGOpath_distance(interloc, toloc, DIST);
cursor_trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, cursor, cursor_path2);

total_trans = TANGOtrans_concatenate(3, cursor_trans1, color_trans,
                                        cursor_trans2);
TANGOtrans_perform(total_trans);
}


