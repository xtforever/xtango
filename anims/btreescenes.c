                  /* B-Trees */
/* (written by: Paul Rowland) Project #1 Animating Computer Algorithms

   This portion of the code runs the interface with XTANGO. The code
maintains a btree image on the screen. The tree is represented by text
for each key and the keys in the same node are grouped next to each other
so as to give the impression of a node. Given more time, more elaborate
text and shape combinations could be added for better visual representation
but now the actual node and lines connecting the nodes are left to the
viewer to imagine. */

#include "btree.h"

double sig = 10000.0, hspace = 0.05; /* used to get image references */
extern int lastlevel;
extern int children[maxlevels];

/* vertical spacing between levels of the tree */
static double levspc = (1.0/((double) (maxlevels + 1))); 
static int plevel = 0;    /* previous depth of the tree */

double MakeX(left, pos,  children)

int left, pos, children;
/* This function returns the physical x coordinate of the image given
   the number of children to the left of it, the position and the number
   of children on the level. The horizontal spacing requires this 
   information to optimize the available space, since with increased depth
   the number of children at that level increases exponentially */

{
   double newx = 0.0, d = 1.0;
  
   d = 1.0/((double) (children + 1)); /* partition width based on children */
   newx = ( (double) (left + 0.5))*d; /* get node's partition with indent */
   newx += ( ((double) pos) * hspace);/* move to appropriate key */ 
   return (newx);
}

double MakeY(level, bottomlevel)

int bottomlevel, level;
/* NewY determines the vertical location of the image given the depth of
   the tree and the level the node is at */

{
   double y;
   
   y = ((double) (maxlevels - bottomlevel + level))*levspc;
   return (y);
}
 
void ANIMInit()
/* This initializes the screen and association lists to be used */
{
  ASSOCinit();
  ASSOCmake("NEW", 0);
  ASSOCmake("AT", 2);
}

void ANIMNew(k)

int k;
/* ANIMNew displays the new entry to be added to the tree in the upper left
   corner of the screen */

{
   TANGO_IMAGE text;
   char str[5];

  sprintf(str, "%d", k);
  text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, hspace, 0.2, 1, 
             TANGO_COLOR_BLUE, NULL, str, 1);
  ASSOCstore("NEW", text);
}
 
void ANIMInsPt(frleft, inspt, n, level)

int frleft, inspt, n, level;

/* ANIMInsPt shifts the keys from inspt to n of the node at level and
   is the frleft + 1 child from the left. The shift is used to make a 
   space in the node to insert a new key into the node */
{ 
   int i, j;
   TANGO_LOC loc1, loc2;
   TANGO_IMAGE image;
   TANGO_PATH path;
   TANGO_TRANS comptrans, trans;
   double x1, x2, y;

   /* make a dummy transition so we can compose with it later */
   path = TANGOpath_color(TANGO_COLOR_BLUE);
   comptrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOpath_free(1, path);

   y = MakeY(level, plevel); 
   for (i = n - 1; i > (inspt - 1); i--) {
/* move the key at position x1, y to 1 position to the right of its current
         position */
     x1 = MakeX(frleft, i, children[level]);
     loc1 = TANGOloc_create(x1,y);

     x2 = MakeX(frleft, i+1, children[level]); 
     loc2 = TANGOloc_create(x2,y); 

/* form a path and transition to move the key from (x1,y) to (x2,y) and
   then compose it with all other transitions moving the other keys so
   all the keys move at once */

     path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
     image = (TANGO_IMAGE) ASSOCretrieve("AT", (int) (x1*sig), (int) (y*sig)); 
     trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, path);

/* store image at its new location and clean up variables */
     ASSOCstore("AT", (int) (x2*sig), (int) (y*sig), image);
     comptrans = TANGOtrans_compose(2, comptrans, trans);
     TANGOtrans_free(1, trans); 
     TANGOpath_free(1,path); 
   }

/* perform transition and clean up */

  TANGOtrans_perform(comptrans);
  TANGOtrans_free(1, comptrans);
}

void ANIMSplit(frleft, pfrleft, ploc, level)

int frleft, pfrleft, ploc, level;

/* ANIMSplit moves all the nodes at level to their new locations because
  of the addition of a new node to the level. It also moves the center
  key of node to the ploc key of the node at prleft in the level above. */

{
   TANGO_IMAGE image;
   TANGO_LOC loc1, loc2;
   TANGO_PATH path;
   TANGO_TRANS trans, comptrans;
   int i,j, left, diff = lastlevel - plevel;
   double x1, x2, y1, y2;

/* make dummy transition so we can compose things with to make everything
     move at once */
   path = TANGOpath_color(TANGO_COLOR_BLUE);
   comptrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOpath_free(1, path); 

/* get previous y position of nodes at level */
   y1 = MakeY(level - diff, plevel);
   y2 = MakeY(level, lastlevel); 

/* move all keys in nodes to the left of the split node to their new 
   locations */
   for (i = j = 0; i < frleft; i++, j = 0)
     do {
          x1 = MakeX(i, j, children[level]);
          loc1 = TANGOloc_create(x1,y1);
      /* check to see if a key is there  and if it is move it */
          if ( left = ASSOCmember("AT", (int) (x1*sig), (int) (y1*sig))) {
             image = (TANGO_IMAGE) ASSOCretrieve("AT",(int) (x1*sig),
                                 (int) (y1*sig));
             x2 = MakeX(i, j, children[level] + 1);
             loc2 = TANGOloc_create(x2,y2);
  
     /* store image at new location and then make a transition to move it */

             ASSOCstore("AT", (int) (x2*sig), (int) (y2*sig), image);
             path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
             trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, path);
             comptrans = TANGOtrans_compose(2, comptrans, trans);

         /* clean up variables */
             TANGOpath_free(1, path); 
             TANGOtrans_free(1, trans);
             j++;
           }
        } while (left && (j < maxc));

/* move left side of split node to its new location */

   for (i = 0; i < t - 1; i++) {
     x1 = MakeX(frleft, i, children[level]);
     loc1 = TANGOloc_create(x1,y1);
     x2 = MakeX(frleft, i, children[level] + 1);
     loc2 = TANGOloc_create(x2,y2);
 
     image = (TANGO_IMAGE) ASSOCretrieve("AT", (int) (x1*sig), (int) (y1*sig));
     path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
     trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image,path);

     ASSOCstore("AT", (int) (x2*sig), (int) (y2*sig), image);
     comptrans = TANGOtrans_compose(2, comptrans, trans);
     TANGOpath_free(1, path);
     TANGOtrans_free(1, trans);
   }

/* move right side of split node to its new location */

 for (i = t; i < maxkey; i++) {
    x1 = MakeX(frleft, i, children[level]);
    loc1 = TANGOloc_create(x1,y1);
    x2 = MakeX( (frleft + 1), (i - t), children[level] + 1);
    loc2 = TANGOloc_create(x2,y2);

    image = (TANGO_IMAGE) ASSOCretrieve("AT", (int) (x1*sig),(int) (y1*sig) );
    path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
    trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, path);

    ASSOCstore("AT", (int) (x2*sig), (int) (y2*sig), image);
    path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
    comptrans = TANGOtrans_compose(2, comptrans, trans);
    TANGOpath_free(1, path);
    TANGOtrans_free(1, trans);
  }

/* move nodes to the right of the split node to their new locations */

   for (i = children[level], j = 0; i > frleft; i--, j = 0)
     do {
          x1 = MakeX(i, j, children[level]);
          loc1 = TANGOloc_create(x1,y1);

     /* check to see if a key exists at (x1, y1) and move it if it does */
          if ( left = ASSOCmember("AT", (int) (x1*sig), (int) (y1*sig))) {
             image = (TANGO_IMAGE) ASSOCretrieve("AT",(int) (x1*sig),
                            (int) (y1*sig));
             x2 = MakeX(i+1, j, children[level] + 1);
             loc2 = TANGOloc_create(x2,y2);
             path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);

             ASSOCstore("AT", (int) (x2*sig), (int) (y2*sig), image);
             trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, path);
             comptrans = TANGOtrans_compose(2, comptrans, trans);
             TANGOpath_free(1, path);
             TANGOtrans_free(1, trans);
             j++;
           }
       } while ((j < maxc) && left);
  
/* move value in center of split node up to the next level */

  x1 = MakeX(frleft, t - 1, children[level]);
  loc1 = TANGOloc_create(x1,y1);
  image = (TANGO_IMAGE) ASSOCretrieve("AT", (int) (x1*sig), (int) (y1*sig));

  x2 = MakeX(pfrleft, ploc, children[level-1]);
  y2 = MakeY(level - 1, lastlevel);
  loc2 = TANGOloc_create(x2,y2);

  path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
  ASSOCstore("AT", (int) (x2*sig), (int) (y2*sig), image);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, path);
  comptrans = TANGOtrans_compose(2, comptrans, trans); 

/* move everything and clean up variables */

  TANGOtrans_perform(comptrans);
  TANGOtrans_free(2, trans, comptrans);
  TANGOpath_free(1, path);
  plevel = lastlevel;
}

void ANIMAddNew(frleft, loc, level)

int frleft, loc, level;
/* ANIMAddNew inserts the new key into the node at loc in the frleft + 1
   child from the left on level */

{
   TANGO_LOC loc1, loc2;
   TANGO_PATH path;
   TANGO_IMAGE image;
   TANGO_TRANS trans;
   double x,y;

   x = MakeX(frleft, loc, children[level]);
   y = MakeY(level, lastlevel);
   loc1 = TANGOloc_create(x,y);

/* get old location and move it counterclockwise to new location (x,y) */
   image = (TANGO_IMAGE) ASSOCretrieve("NEW");
   loc2 = TANGOimage_loc(image, TANGO_PART_TYPE_C);
   path = TANGOpath_motion(loc2, loc1, TANGO_PATH_TYPE_COUNTERCLOCKWISE);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, path);
 
   /* save at new location, perform transition, and clean up */ 
   ASSOCstore("AT", (int) (x*sig), (int) (y*sig), image);
   TANGOtrans_perform(trans);
   TANGOtrans_free(1, trans);
   TANGOpath_free(1, path);
} 
