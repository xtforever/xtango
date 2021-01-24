/* AVL Trees by Nick Duncan */

#include <stdio.h>
#include <xtango.h>

#define IDVAL 13
#define SPEED 0.05
#define NULL_IMAGE 9999

void
ANIMInit(edges)
   int edges;
{
   TANGO_IMAGE loz, pivot;

   ASSOCinit();
   ASSOCmake("TEXT",0);
   ASSOCmake("CURRLOC",0);
   ASSOCmake("LOZENGE",0);
   loz = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.1,0.1,0,TANGO_COLOR_BLUE,
			      0.05,0.0);
   ASSOCstore("LOZENGE",loz);

   ASSOCmake("PIVLOC",0);
   ASSOCmake("PIVOT",0);
   pivot = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.1, 0.1, 1, 
             TANGO_COLOR_GREEN, 0.0450, 0.0);
   ASSOCstore("PIVOT",pivot);

   TWISTcreate_bintree(IDVAL,0.1,0.9,0.9,0.1,edges);
}



void
ANIMNewVal(val)
   int val;
{
   char        str[5];
   TANGO_LOC   from,to;
   TANGO_IMAGE text,loz,pivot;
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

   /* move the pivot indicator to the root and make it visible */
   pivot = (TANGO_IMAGE) ASSOCretrieve("PIVOT");
   from = TANGOimage_loc(pivot,TANGO_PART_TYPE_C);
   path = TANGOpath_distance(from,to,1.0);  /* make it in 1 frame */
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pivot,path);
   appear = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,pivot,path);
   both = TANGOtrans_compose(2,move,appear);
   TANGOtrans_perform(both);

   /* keep the pivot where it is */
   ASSOCstore("PIVLOC",to);

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


void
ANIMErasePivot()
{
   TANGO_IMAGE	pivot;
   TANGO_PATH	onepath;
   TANGO_TRANS	vis;

   /* make the pivot invisible */
   pivot = (TANGO_IMAGE) ASSOCretrieve("PIVOT");
   onepath = TANGOpath_null(1);
   vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,pivot,onepath);
   TANGOtrans_perform(vis);
}


void
ANIMMovePivot()
{
   TANGO_LOC	pivloc,to;
   TANGO_IMAGE	pivot;
   TANGO_PATH	movepath;
   TANGO_TRANS	move;

   /* move the pivot to the correct location */
   pivot = (TANGO_IMAGE) ASSOCretrieve("PIVOT");
   to = (TANGO_LOC) ASSOCretrieve("CURRLOC");
   pivloc = (TANGO_LOC) ASSOCretrieve("PIVLOC");
   movepath = TANGOpath_distance(pivloc,to,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pivot,movepath);
   TANGOtrans_perform(move);

   /* update our current pivot position */
   ASSOCstore("PIVLOC",to);

}

/*----------------------------------------------------------------------------

Internal procedures for moving nodes of subtrees: not directly called by avl

-----------------------------------------------------------------------------*/

void
ANIMMoveSubTree(p,newp)
   TANGO_LOC	p,newp;
{
   TANGO_LOC	pleft,newpleft,pright,newpright;
   TANGO_IMAGE	val;
   TANGO_PATH	path;
   TANGO_TRANS	move;

   /* generic node mover, used when we have no information except
      source location and destination */
   val = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,p);
   path = TANGOpath_distance(p,newp,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,val,path);
   TANGOtrans_perform(move);

   /* store new value at location and negate value at old location */
   ASSOCstore("IMAGE_AT",IDVAL,newp,val);
   ASSOCstore("IMAGE_AT",IDVAL,p,NULL_IMAGE);

   /* move left child, if necessary */
   if (ASSOCmember("LCHILD", IDVAL, p)) {
      pleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pleft) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pleft) != NULL_IMAGE) {
         newpleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, newp);
         ANIMMoveSubTree(pleft,newpleft);
       }
    }

   /* move right child, if necessary */
   if (ASSOCmember("RCHILD", IDVAL, p)) {
      pright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pright) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pright) != NULL_IMAGE) {
         newpright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, newp);
         ANIMMoveSubTree(pright,newpright);
       }
    }

}

void
ANIMMoveDownLeft(p,pleft)
   TANGO_LOC	p,pleft;
{
   TANGO_LOC	newpleft,pright,newpright;
   TANGO_IMAGE	val;
   TANGO_PATH	path;
   TANGO_TRANS	move;

   /* for moving a node into position of its left child, first
      move left child out of the way */
   if (ASSOCmember("IMAGE_AT",IDVAL,pleft) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pleft) != NULL_IMAGE) {
      newpleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pleft);
      ANIMMoveDownLeft(pleft,newpleft);
    }

   /* now move our node */
   val = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,p);
   path = TANGOpath_distance(p,pleft,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,val,path);
   TANGOtrans_perform(move);
   ASSOCstore("IMAGE_AT",IDVAL,pleft,val);
   ASSOCstore("IMAGE_AT",IDVAL,p,NULL_IMAGE);

   /* then move right child using generic procedure */
   if (ASSOCmember("RCHILD", IDVAL, p)) {
      pright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pright) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pright) != NULL_IMAGE) {
         newpright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pleft);
         ANIMMoveSubTree(pright,newpright);
       }
    }

}

void
ANIMMoveDownRight(p,pright)
   TANGO_LOC	p,pright;
{
   TANGO_LOC	pleft,newpleft,newpright;
   TANGO_IMAGE	val;
   TANGO_PATH	path;
   TANGO_TRANS	move;

   /* for moving a node into position of its right child, first
      move right child out of the way */
   if (ASSOCmember("IMAGE_AT",IDVAL,pright) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pright) != NULL_IMAGE) {
      newpright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pright);
      ANIMMoveDownRight(pright,newpright);
    }

   /* now move our node */
   val = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,p);
   path = TANGOpath_distance(p,pright,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,val,path);
   TANGOtrans_perform(move);
   ASSOCstore("IMAGE_AT",IDVAL,pright,val);
   ASSOCstore("IMAGE_AT",IDVAL,p,NULL_IMAGE);

   /* then move left child using generic procedure */
   if (ASSOCmember("LCHILD", IDVAL, p)) {
      pleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pleft) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pleft) != NULL_IMAGE) {
         newpleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pright);
         ANIMMoveSubTree(pleft,newpleft);
       }
    }

}

void
ANIMMoveUpLeft(p,parent)
   TANGO_LOC	p,parent;
{
   TANGO_LOC	pleft,pright,newpright;
   TANGO_IMAGE	val;
   TANGO_PATH	path;
   TANGO_TRANS	move;

   /* to move left child into parent location, first move our node */
   val = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,p);
   path = TANGOpath_distance(p,parent,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,val,path);
   TANGOtrans_perform(move);
   ASSOCstore("IMAGE_AT",IDVAL,parent,val);
   ASSOCstore("IMAGE_AT",IDVAL,p,NULL_IMAGE);

   /* then move left child, if any */
   if (ASSOCmember("LCHILD",IDVAL,p)) {
      pleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pleft) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pleft) != NULL_IMAGE) {
         ANIMMoveUpLeft(pleft,p);
       }
    }
   /* then use generic procedure to move right child */
   if (ASSOCmember("RCHILD", IDVAL, p)) {
      pright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pright) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pright) != NULL_IMAGE) {
         newpright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, parent);
         ANIMMoveSubTree(pright,newpright);
       }
    }

}

void
ANIMMoveUpRight(p,parent)
   TANGO_LOC	p,parent;
{
   TANGO_LOC	pleft,newpleft,pright;
   TANGO_IMAGE	val;
   TANGO_PATH	path;
   TANGO_TRANS	move;

   /* to move right child into parent location, first move our node */
   val = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,p);
   path = TANGOpath_distance(p,parent,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,val,path);
   TANGOtrans_perform(move);
   ASSOCstore("IMAGE_AT",IDVAL,parent,val);
   ASSOCstore("IMAGE_AT",IDVAL,p,NULL_IMAGE);

   /* then move right child, if any */
   if (ASSOCmember("RCHILD", IDVAL, p)) {
      pright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pright) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pright) != NULL_IMAGE) {
         ANIMMoveUpRight(pright,p);
       }
    }

   /* then use generic procedure to move left child */
   if (ASSOCmember("LCHILD", IDVAL, p)) {
      pleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, p);
      if (ASSOCmember("IMAGE_AT",IDVAL,pleft) && 
          ASSOCretrieve("IMAGE_AT", IDVAL, pleft) != NULL_IMAGE) {
         newpleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, parent);
         ANIMMoveSubTree(pleft,newpleft);
       }
    }

}

/*-----------------------------------------------------------------------

Animation procedures actually called by avl

------------------------------------------------------------------------*/

void
ANIMLeftLeft()
{
   TANGO_LOC	pivloc,ploc,pleft,pivright,newpivright,qloc,newqloc;
   TANGO_IMAGE	pivval,pval,qval;
   TANGO_PATH	pivpath,ppath,qpath;
   TANGO_TRANS	pivmove,pmove,qmove;

   /* get locations we need */
   pivloc = (TANGO_LOC) ASSOCretrieve("PIVLOC");
   pivright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivloc);
   ploc = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivloc);
   pleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, ploc);
   qloc = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, ploc);

   /* move right child of pivot out of the way */
   if (ASSOCmember("IMAGE_AT",IDVAL,pivright) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pivright) != NULL_IMAGE) {
      newpivright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivright);
      ANIMMoveDownRight(pivright,newpivright);
   }

   /* move pivot to right child's position */
   pivval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,pivloc);
   pivpath = TANGOpath_distance(pivloc,pivright,SPEED);
   pivmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pivval,pivpath);
   TANGOtrans_perform(pivmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivright,pivval);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,NULL_IMAGE);

   /* move left child to pivot position */
   pval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,ploc);
   ppath = TANGOpath_distance(ploc,pivloc,SPEED);
   pmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pval,ppath);
   TANGOtrans_perform(pmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,pval);
   ASSOCstore("IMAGE_AT",IDVAL,ploc,NULL_IMAGE);

   /* if necessary, move the left child's right subtree */
   if (ASSOCmember("IMAGE_AT",IDVAL,qloc) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, qloc) != NULL_IMAGE) {
      newqloc = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivright);
      ANIMMoveSubTree(qloc,newqloc);
    }

   /* move the left subtree of the left child up a level */
   if (ASSOCmember("IMAGE_AT",IDVAL,pleft) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pleft) != NULL_IMAGE) {
      ANIMMoveUpLeft(pleft,ploc);
   }
}


void
ANIMLeftRight()
{
   TANGO_LOC	pivloc,pivleft,pivright,newpivleft,qloc,qleft,qright,newqleft;
   TANGO_IMAGE	pivval,qval;
   TANGO_PATH	pivpath,qpath;
   TANGO_TRANS	pivmove,qmove;

   /* get locations we need */
   pivloc = (TANGO_LOC) ASSOCretrieve("PIVLOC");
   pivright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivloc);
   pivleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivloc);
   qloc = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivright);
   qright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, qloc);
   qleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, qloc);

   /* move left child of pivot out of the way */
   if (ASSOCmember("IMAGE_AT",IDVAL,pivleft) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pivleft) != NULL_IMAGE) {
      newpivleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivleft);
      ANIMMoveDownLeft(pivleft,newpivleft);
    }

   /* move pivot to left child's position */
   pivval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,pivloc);
   pivpath = TANGOpath_distance(pivloc,pivleft,SPEED);
   pivmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pivval,pivpath);
   TANGOtrans_perform(pivmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivleft,pivval);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,NULL_IMAGE);

   /* move left child of pivot's right child to pivot position */
   qval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,qloc);
   qpath = TANGOpath_distance(qloc,pivloc,SPEED);
   qmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,qval,qpath);
   TANGOtrans_perform(qmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,qval);
   ASSOCstore("IMAGE_AT",IDVAL,qloc,NULL_IMAGE);

   /* move left subtree of q to be right subtree of pivot in new location */
   if (ASSOCmember("IMAGE_AT",IDVAL,qleft) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, qleft) != NULL_IMAGE) {
      newqleft = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivleft);
      ANIMMoveSubTree(qleft,newqleft);
    }

   /* move right subtree of q up a level */
   if (ASSOCmember("IMAGE_AT",IDVAL,qright) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, qright) != NULL_IMAGE) {
      ANIMMoveUpRight(qright,qloc);
    }
}


void
ANIMRightRight()
{
   TANGO_LOC	pivloc,ploc,pright,pivleft,newpivleft,qloc,newqloc;
   TANGO_IMAGE	pivval,pval;
   TANGO_PATH	pivpath,ppath;
   TANGO_TRANS	pivmove,pmove;

   /* get locations we need */
   pivloc = (TANGO_LOC) ASSOCretrieve("PIVLOC");
   ploc = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivloc);
   pright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, ploc);
   pivleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivloc);
   qloc = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, ploc);

   /* move left child of pivot out of the way */
   if (ASSOCmember("IMAGE_AT",IDVAL,pivleft) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pivleft) != NULL_IMAGE) {
      newpivleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivleft);
      ANIMMoveDownLeft(pivleft,newpivleft);
   }

   /* move pivot to left child's position */
   pivval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,pivloc);
   pivpath = TANGOpath_distance(pivloc,pivleft,SPEED);
   pivmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pivval,pivpath);
   TANGOtrans_perform(pivmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivleft,pivval);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,NULL_IMAGE);

   /* move right child to pivot position */
   pval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,ploc);
   ppath = TANGOpath_distance(ploc,pivloc,SPEED);
   pmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pval,ppath);
   TANGOtrans_perform(pmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,pval);
   ASSOCstore("IMAGE_AT",IDVAL,ploc,NULL_IMAGE);

   /* if necessary, move the right child's left subtree */
   if (ASSOCmember("IMAGE_AT",IDVAL,qloc) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, qloc) != NULL_IMAGE) {
      newqloc = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivleft);
      ANIMMoveSubTree(qloc,newqloc);
    }

   /* move the right subtree of the right child up a level */
   if (ASSOCmember("IMAGE_AT",IDVAL,pright) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pright) != NULL_IMAGE) {
      ANIMMoveUpRight(pright,ploc);
   }
}

void
ANIMRightLeft()
{
   TANGO_LOC	pivloc,pivleft,pivright,newpivright,qloc,qleft,qright,newqright;
   TANGO_IMAGE	pivval,qval;
   TANGO_PATH	pivpath,qpath;
   TANGO_TRANS	pivmove,qmove;

   /* get locations we need */
   pivloc = (TANGO_LOC) ASSOCretrieve("PIVLOC");
   pivleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivloc);
   pivright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivloc);
   qloc = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivleft);
   qleft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, qloc);
   qright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, qloc);

   /* move right child of pivot out of the way */
   if (ASSOCmember("IMAGE_AT",IDVAL,pivright) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, pivright) != NULL_IMAGE) {
      newpivright = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, pivright);
      ANIMMoveDownRight(pivright,newpivright);
    }

   /* move pivot to right child's position */
   pivval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,pivloc);
   pivpath = TANGOpath_distance(pivloc,pivright,SPEED);
   pivmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pivval,pivpath);
   TANGOtrans_perform(pivmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivright,pivval);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,NULL_IMAGE);

   /* move right child of pivot's left child to pivot position */
   qval = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,qloc);
   qpath = TANGOpath_distance(qloc,pivloc,SPEED);
   qmove = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,qval,qpath);
   TANGOtrans_perform(qmove);
   ASSOCstore("IMAGE_AT",IDVAL,pivloc,qval);
   ASSOCstore("IMAGE_AT",IDVAL,qloc,NULL_IMAGE);

   /* move right subtree of q to be left subtree of pivot in new location */
   if (ASSOCmember("IMAGE_AT",IDVAL,qright) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, qright) != NULL_IMAGE) {
      newqright = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, pivright);
      ANIMMoveSubTree(qright,newqright);
    }

   /* move left subtree of q up a level */
   if (ASSOCmember("IMAGE_AT",IDVAL,qleft) && 
       ASSOCretrieve("IMAGE_AT", IDVAL, qleft) != NULL_IMAGE) {
      ANIMMoveUpLeft(qleft,qloc);
    }
}









