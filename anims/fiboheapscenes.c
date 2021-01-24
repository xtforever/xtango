#include <stdio.h>
#include <stdlib.h>
#include "fiboheap.h"


TANGO_IMAGE prompt = NULL;

extern pHeap H;
extern pHeap N;


/* ux, uy are offsets in those directions */
double ux = 0.08;
double uy = 0.10;


/* array tc is Tree Components[ 4 * (64 (max nodes) + 1)] */
static TANGO_IMAGE_COMPONENT tc[245];


char *strdupl (a)
     char *a;
{
  char *b;
  b = (char *) malloc(strlen(a)+1);
  strcpy(b,a);
  return(b);
}


double calc_xoffset (n)
     pNode n;
{
   int d = n -> degree;

   if (d == 0) 
      return 0.0;

   return -(ux * (1 << (d - 1)));
}


double calc_treeWidth (n)
     pNode n;
{
   int d = n -> degree;

   if (d == 0)
      return (ux);

   return ux * (1 << (d - 1));
}


void prepTreeImage (X, i, coord)
     pNode X;
     int *i;
     TANGO_LOC coord;
{
   char Args[128];
   double xoff, yoff;
   double m_x, m_y;
   double p_x, p_y;
   int d;

   if (!X)
     return;

   p_x = TANGOloc_X( coord );
   p_y = TANGOloc_Y( coord );

   /* set my *real* coordinates */

   d = X -> degree;

   if (X -> p) 
     {
       xoff = calc_xoffset( X );
       yoff = uy;
       X -> loc = TANGOloc_modify (TANGOloc_create(p_x, p_y), xoff, yoff);
     }
   else 
     {
       X -> loc = TANGOloc_modify (TANGOloc_create(p_x, p_y), 0.0, 0.0);
     }

   m_x = TANGOloc_X( X -> loc);
   m_y = TANGOloc_Y( X -> loc);
   
   /* hit same row */

   if (X -> sibling && X -> p) 
     {
       prepTreeImage (X -> sibling, i, coord);
     }

   /* hit kids */

   if (X -> child) 
     {
       prepTreeImage (X -> child, i, X -> loc);
     }

   /* define vector */

   if (X -> p) 
     {
       sprintf (Args, "%f %f TANGO_COLOR_BLACK %f %f 0.25 1.0 0",
		m_x, m_y, p_x - m_x, p_y - m_y);
       tc[*i].type = TANGO_IMAGE_TYPE_LINE;
       tc[*i].args = strdupl(Args);
       (*i)++;
     }

   /* define circle to cover vectors */

   sprintf (Args, "%f %f TANGO_COLOR_WHITE %f 1.0",
	    m_x, m_y, ux/4);
   tc[*i].type = TANGO_IMAGE_TYPE_CIRCLE;
   tc[*i].args = strdupl(Args);
   (*i)++;

   /* define outlining circle */

   sprintf (Args, "%f %f TANGO_COLOR_BLACK %f 0.0",
	    m_x, m_y, ux/4);
   tc[*i].type = TANGO_IMAGE_TYPE_CIRCLE;
   tc[*i].args = strdupl(Args);
   (*i)++;

   /* define string */

   sprintf (Args, "%f %f TANGO_COLOR_BLACK variable %d 1",
	    m_x, m_y + 0.005, X -> key);
   tc[*i].type = TANGO_IMAGE_TYPE_TEXT;
   tc[*i].args = strdupl(Args);
   (*i)++;
}


TANGO_IMAGE buildTreeImage (thisnode)
     pNode thisnode;
{
   int i = 0;
   TANGO_LOC no_coord;

   double cx, cy;

   cx = TANGOloc_X ( thisnode -> tree_loc );
   cy = TANGOloc_Y ( thisnode -> tree_loc );

   no_coord = TANGOloc_create(0.0, 0.0);

   prepTreeImage (thisnode, &i, no_coord);

   tc[i].type = TANGO_IMAGE_TYPE_COMPOSITE;
   tc[i].args = NULL;

   return TANGOimage_create (TANGO_IMAGE_TYPE_COMPOSITE, cx, cy, 1, tc);
}


void showHeap(h)
     pHeap h;
{
   TANGO_IMAGE im;

   double hx, hy;

   hx = TANGOloc_X ( h -> loc );
   hy = TANGOloc_Y ( h -> loc );

   im = TANGOimage_create (TANGO_IMAGE_TYPE_LINE, 0.0, hy, 1,
			   TANGO_COLOR_RED, 2.0, 0.0, 0.5, 1.0, 0) ;
   TAPshow (im);
}


void showTree(t)
     pNode t;
{
   TAPshow (t -> tree_im);
}


void moveTree(x, sx, sy)
     pNode x;
     double sx;
     double sy;
{
   TANGO_LOC loc;
   TANGO_PATH aPath;
   TANGO_TRANS aTrans;

   loc = x -> tree_loc;
   x -> tree_loc = TANGOloc_create ( sx, sy );    

   aPath = TANGOpath_motion ( loc, x -> tree_loc,
			     TANGO_PATH_TYPE_STRAIGHT );
   aTrans = TANGOtrans_create ( TANGO_TRANS_TYPE_MOVE,
			       x -> tree_im, aPath );

   TANGOtrans_perform ( aTrans );
   TANGOtrans_free( 1, aTrans );
   TANGOpath_free( 1, aPath );
}



void move2Trees(s, sx, sy, t, tx, ty)
     pNode s, t;
     double sx, tx, sy, ty;
{
   TANGO_LOC sloc, tloc;
   TANGO_PATH sPath, tPath;
   TANGO_TRANS sTrans, tTrans, cTrans;

   sloc = s -> tree_loc;
   s -> tree_loc = TANGOloc_create ( sx, sy );    

   sPath = TANGOpath_motion ( sloc, s -> tree_loc,
			     TANGO_PATH_TYPE_STRAIGHT );
   sTrans = TANGOtrans_create ( TANGO_TRANS_TYPE_MOVE,
			       s -> tree_im, sPath );

   tloc = t -> tree_loc;
   t -> tree_loc = TANGOloc_create ( tx, ty );    

   tPath = TANGOpath_motion ( tloc, t -> tree_loc,
			     TANGO_PATH_TYPE_STRAIGHT );
   tTrans = TANGOtrans_create ( TANGO_TRANS_TYPE_MOVE,
			       t -> tree_im, tPath );

   cTrans = TANGOtrans_compose(2, sTrans, tTrans);

   TANGOtrans_perform ( cTrans );
   TANGOtrans_free( 3, cTrans, sTrans, tTrans );
   TANGOpath_free( 2, sPath, tPath );
}


void moveForest(Q)
     pHeap Q;
{
   double qx, qy;
   pNode start, prev_x, x, next_x;

   qx = TANGOloc_X ( Q -> loc );
   qy = TANGOloc_Y ( Q -> loc );

   start = Q -> head;
   x = Q -> head;

   if ( x ) 
     {
       qx += calc_treeWidth(x) - ux;    
       moveTree( x, qx, qy );

       x = x->right;
       
       while (x != start) 
	 {
	   qx += calc_treeWidth(x);    
	   moveTree ( x, qx, qy );
	   x = x->right;
	 }
     }
}


void ANIMDelete(x)
     pNode x;
{
   int d = x -> degree;
   TANGO_PATH path;
   TANGO_TRANS aTran;
   TANGO_IMAGE im;
   double xx, yy;
   double zero = 0.0;
   pNode a, b, c;

   TANGO_TRANS del, show;
   TANGO_PATH p;

   p = TANGOpath_null(1);
   if (prompt) 
     {
       del = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, prompt, p);
       TANGOtrans_perform(del);
       free (del);
     }
   prompt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.95, 1,
               TANGO_COLOR_BLACK, "10x20", "Extract Minimum", 1);
   show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, prompt, p);
   TANGOtrans_perform(show);
   free(p);
   free(show);
   
   path = TANGOpath_create ( 1, &zero, &zero );

   if (d)
     {
       --d;

       a = x -> child;
       a -> p = NULL;
       xx = TANGOloc_X ( x -> tree_loc ) + calc_xoffset(a);
       yy = TANGOloc_Y ( x -> tree_loc ) + uy;
       a -> tree_loc = TANGOloc_create ( xx, yy );
       a -> tree_im = buildTreeImage ( a );
       showTree ( a );

       b = a -> sibling;
       if (b) 
	 {
	   c = b -> sibling;
	   do 
	     {
	       b -> p = NULL;
	       xx = TANGOloc_X ( x -> tree_loc ) + calc_xoffset( b );
	       yy = TANGOloc_Y ( x -> tree_loc ) + uy;
	       b -> tree_loc = TANGOloc_create ( xx, yy );
	       b -> tree_im = buildTreeImage ( b );
	       showTree ( b );
	       
	       a = b;
	       b = c;
	       if (c)
		 c = c -> sibling;
	     } while (b);
	 }
     }
   
   xx = TANGOloc_X ( x -> tree_loc );
   yy = TANGOloc_Y ( x -> tree_loc );

   im = TANGOimage_create (TANGO_IMAGE_TYPE_CIRCLE, xx, yy, 1,
			   TANGO_COLOR_RED, ux/4, 1.0);
   aTran = TAPflash( im, 20 );
   TANGOtrans_perform ( aTran );
   TANGOtrans_free( 1, aTran );

   aTran = TANGOtrans_create ( TANGO_TRANS_TYPE_DELETE, im, path );
   TANGOtrans_perform ( aTran );
   TANGOtrans_free( 1, aTran );

   aTran = TANGOtrans_create ( TANGO_TRANS_TYPE_DELETE, x->tree_im, path );
   TANGOtrans_perform ( aTran );
   TANGOtrans_free( 1, aTran );

   TANGOpath_free( 1, path );

   sleep(1);
}


void ANIMConsolidate()
{
   TANGO_TRANS del, show;
   TANGO_PATH p;

   p = TANGOpath_null(1);
   if (prompt) 
     {
       del = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, prompt, p);
       TANGOtrans_perform(del);
       free (del);
     }
   prompt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.95, 1,
		      TANGO_COLOR_BLACK, "10x20", "Start Consolidation", 1);
   show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, prompt, p);
   TANGOtrans_perform(show);
   free(p);
   free(show);

   sleep(1);
}


void ANIMExtract()
{
   TANGO_TRANS del, show;
   TANGO_PATH p;

   p = TANGOpath_null(1);
   if (prompt) 
     {
       del = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, prompt, p);
       TANGOtrans_perform(del);
       free (del);
     }
   prompt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.95, 1,
			      TANGO_COLOR_BLACK, "10x20", "After MinExtract", 1);
   show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, prompt, p);
   TANGOtrans_perform(show);
   free(p);
   free(show);

   moveForest (H);

   sleep(1);
}


void ANIMLink(z)
     pNode z;
{
   pNode y = z->child;
   char q;

   double y_x, y_y;
   double z_x, z_y;
   double zero = 0.0;
   TANGO_LOC loc;
   TANGO_IMAGE imy, imz;
   TANGO_PATH path;
   TANGO_TRANS aTran1, aTran2, aTran3;
   int i = 0;

   TANGO_TRANS del, show;
   TANGO_PATH p;

   p = TANGOpath_null(1);
   if (prompt)
     {
       del = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, prompt, p);
       TANGOtrans_perform(del);
       free (del);
     }
   prompt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.95, 1,
			      TANGO_COLOR_BLACK, "10x20", "Link", 1);
   show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, prompt, p);
   TANGOtrans_perform(show);
   free(p);
   free(show);

   y_x = TANGOloc_X ( y -> tree_loc );
   y_y = TANGOloc_Y ( y -> tree_loc );

   z_x = TANGOloc_X ( z -> tree_loc );
   z_y = TANGOloc_Y ( z -> tree_loc );

   if ( y_x < z_x ) 
     {
       if ( y -> degree )
	 moveTree( y, y_x, z_y + uy );
       else
	 moveTree( y, z_x, z_y + uy );
     }
   else 
     {
       if ( y -> degree )
	 move2Trees( z, y_x, z_y, y, z_x, z_y + uy );
       else
	 move2Trees( z, y_x, z_y, y, y_x, z_y + uy );
       z -> tree_loc = TANGOloc_create ( y_x, z_y );    
     }

   path = TANGOpath_create ( 1, &zero, &zero );

   imy = y -> tree_im;
   aTran1 = TANGOtrans_create ( TANGO_TRANS_TYPE_DELETE, imy, path );
   y -> tree_im = NULL;

   imz = z -> tree_im;
   aTran2 = TANGOtrans_create ( TANGO_TRANS_TYPE_DELETE, imz, path );

   z -> tree_im = buildTreeImage (z);
   aTran3 = TANGOtrans_create ( TANGO_TRANS_TYPE_RAISE, z -> tree_im, path );

   TANGOtrans_perform ( aTran3 );
   TANGOtrans_perform ( aTran1 );
   TANGOtrans_perform ( aTran2 );

   TANGOtrans_free( 3, aTran1, aTran2, aTran3 );
   TANGOpath_free( 1, path );

   sleep(1);
}


void ANIMInsert(x)
     pNode x;
{
   TANGO_TRANS del, show;
   TANGO_PATH p;
   char str[10];

   p = TANGOpath_null(1);
   if (prompt) 
     {
       del = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, prompt, p);
       TANGOtrans_perform(del);
       free (del);
     }
   sprintf(str,"Insert %d",x->key);
   prompt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.95, 1,
			      TANGO_COLOR_BLACK, "10x20", str, 1);
   show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, prompt, p);
   TANGOtrans_perform(show);
   free(p);
   free(show);

   x -> tree_loc = TANGOloc_create (TANGOloc_X ( N -> loc ),
				    TANGOloc_Y ( N -> loc ));    
   x -> tree_im = buildTreeImage (x);

   showTree (x);

   sleep(1);

   moveForest (H);

   sleep(1);
}


void ANIMInit()
{
   TANGO_TRANS del, show;
   TANGO_PATH p;

   p = TANGOpath_null(1);
   if (prompt) 
     {
       del = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, prompt, p);
       TANGOtrans_perform(del);
       free (del);
     }

   prompt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.95, 1,
			    TANGO_COLOR_BLACK, "10x20", "New Heap Created", 1);
   show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, prompt, p);
   TANGOtrans_perform(show);
   free(p);
   free(show);


   H -> loc = TANGOloc_create ( 0.05, 0.05 );
   N -> loc = TANGOloc_create ( 0.05, 0.80 );

   showHeap (H);
   showHeap (N);
}

