/*
 * treapscenes.c -- the anim routines for the treap XTango project are
 *            contain in this module.
 *
 *  AUTHOR: Lyman S. Taylor ( lyman@cc.gatech.edu ) 
 *
 *
 *   NOTES:
 *         Why no arcs?
 *                    Because:
 *                 1. The tree is squeezed  for space as it is.
 *                 2. It would "way" complicate the code. By also calling for
 *                      the deletion of archs to. And the resizing of arcs
 *                      since each level is not equal spaced.
 *                 3. ran out of time :-)
 *       
 *         Although derived from treescenes.c this module keeps the Lozenge
 *         visible at all times.  
 *
 *   BUGS:
 *        Does NO garbage collection.  Run it long enough and it shall surely
 *             barf. :(  [ I think ]
 *        
 */
#include <stdio.h>
#include <xtango.h>

#define IDVAL 13
#define SPEED 0.05
#define FAST  1.0
#define SLOWMO 0.01



/* Module gobals */

static  TANGO_IMAGE  nullImage;



/* Module routines */

static void 
BFill_Component(/*int skey,int hkey,TANGO_IMAGE_COMPONET temp*/);
static void fill_component(/* real,imag,temp*/);

static TANGO_LOC  childLoc ( /* char* child, TANGO_LOC parentLoc */ );
static void ANIMMoveMe( /* TANGO_IMAGE me  , TANGO_LOC from,to  */ );
static void ANIMMoveSubTreeUp ( /* TANGO_LOC from , TANGO_LOC to */ );
static void ANIMMoveSubTreeDown( /* TANGO_LOC from , TANGO_LOC to */ );




/**************** section 1 ******************/

/* 
 * Module utility routines
*/


/* BUGGY Ignore this routine. see fill_component at end of file */
static void 
BFill_Component(skey,hkey,temp)
     int skey,hkey;
     TANGO_IMAGE_COMPONENT *temp;
/*
 * ANIM_Fill_Component -- fill in the  textComposite componet.
 *
 *   Credits: inspireation from fftscenes.c and Tim Dean.
 *   IGNORE see fill_component at end of file.  This is the biffed version.
 *   I'm only keeping so that I can debug later.
*/
{
       
  /* the BST Key Text  */
  temp[0].type=TANGO_IMAGE_TYPE_TEXT;
  temp[0].args=(char *)calloc(80,sizeof(char));
  sprintf( temp[0].args, "0.1 0.1 1 TANGO_COLOR_BLACK variable  %d 1", skey);

  
  /*  the Heap Key Text */
  temp[1].type=TANGO_IMAGE_TYPE_TEXT;
  temp[1].args=(char *)calloc(80,sizeof(char));   
  sprintf(temp[1].args, "0.1 0.15  1 TANGO_COLOR_BLACK variable  %d 1", hkey);

  temp[2].type=TANGO_IMAGE_TYPE_COMPOSITE;
  temp[2].args=NULL;

  /* END Fill_Component */
} 



static TANGO_LOC  
childLoc (  child , parentLoc )
     char* child;
     TANGO_LOC parentLoc;
/* 
 * childLoc -- return the TANGO_LOC of a child, if there is one.
 *         o.w. return NULL
 *
 *  child should either be the string "LCHILD" or "RCHILD".
 * Algorithm:
 *            find out if there is a TWIST-defined left subtree node 
 *                    of this node (not down too far)
 *                       if so retrieve it. Then find out is there is anything
 *                       done there ( either nothing ever entered. or nothing
 *                       there now ).
 *                       return this location.
 * 
 *
 *
*/
{
  TANGO_LOC loc;
  
 if (ASSOCmember(child, IDVAL, parentLoc)) {
   loc = (TANGO_LOC ) ASSOCretrieve(child , IDVAL, parentLoc );
   if (( ASSOCmember("IMAGE_AT",IDVAL,loc)) &&
	( (( TANGO_IMAGE)ASSOCretrieve("IMAGE_AT", IDVAL,loc)) != nullImage)) {
     return  loc;
   }
 }

 return NULL;


}



static void
ANIMMoveMe( me  , from ,  to   )
     TANGO_IMAGE me;
     TANGO_LOC  from , to;
/*
 * ANIMMove Me
 *
 *  move an image to a new location. Store the NULL
 *
 */
{
  TANGO_PATH path;
  TANGO_TRANS  move;

  /* add flash 
  move = TAPflash( me , 4 );
  TANGOtrans_perform(move);
   */

   /* catch errors of use */
   if ( (from == NULL ) || ( to == NULL )) {
     fprintf(stderr,"MoveMe: attempt to move node to/from nonexistant loc\n");
     fprintf(stderr,"    Probably moving subtree down too far.\n");
     fprintf(stderr,"    Will leave in place. Node now probably \"biffed\"\n");
     return;
   }

  /* move to new position */
  path = TANGOpath_distance(from, to, FAST);
  move =  TANGOtrans_create(TANGO_TRANS_TYPE_MOVE , me , path);
  TANGOtrans_perform(move);
  
  /* store this result back into TANGO-tree data structure */
  ASSOCstore("IMAGE_AT", IDVAL, to , me );
  ASSOCstore("IMAGE_AT", IDVAL, from , nullImage);

  return;
} 


static void 
ANIMMoveSubTreeUp ( from , to )
     TANGO_LOC from, to;
/* 
 * ANIMMoveSubTreeUp -- move a whole subtree from one location to another.
 *        The from/to locations are the location of the old and new root.
 * 
 * Algorithm:
 *            1. move the root image.
 *            2.   find out if there is a TWIST-defined left subtree node 
 *                    of this node (not down too far)
 *                       if so retrieve it. Then find out is there is anything
 *                       done there ( either nothing ever entered. or nothing
 *                       there now ).
 *                       if so move it.
 *            3. ditto right child.
 * 
 * NOTE:  Assumes that given a "Real" Root to start with.
 *
 */
{
 TANGO_IMAGE root;
 TANGO_LOC   oldLeft, oldRight;
 TANGO_LOC   newLeft, newRight;

 
 root = (TANGO_IMAGE) ASSOCretrieve( "IMAGE_AT", IDVAL, from );
 ANIMMoveMe( root , from , to );


 oldLeft = childLoc ( "LCHILD", from );
 if ( oldLeft != NULL ) {
   newLeft = (TANGO_LOC) ASSOCretrieve("LCHILD",IDVAL,to);
   ANIMMoveSubTreeUp( oldLeft , newLeft);
 }


 oldRight = childLoc ( "RCHILD", from );
 if ( oldRight != NULL ) {
   newRight = (TANGO_LOC) ASSOCretrieve("RCHILD",IDVAL,to);
   ANIMMoveSubTreeUp( oldRight , newRight);
 }


}



static void 
ANIMMoveSubTreeDown ( from , to )
     TANGO_LOC from, to;
/* 
 * ANIMMoveSubTree -- move a whole subtree from one location to another.
 *        The from/to locations are the location of the old and new root.
 * 
 * Algorithm:
 *     ( essentially the same as move up only do root last )
 *            1.   find out if there is a TWIST-defined left subtree node 
 *                    of this node (not down too far)
 *                       if so retrieve it. Then find out is there is anything
 *                       done there ( either nothing ever entered. or nothing
 *                       there now ).
 *                       if so move it.
 *            2. ditto right child.
 *            3. move the root image.
 * 
 * NOTE:  Assumes that given a "Real" Root to start with.
 *
 */
{
 TANGO_IMAGE root;
 TANGO_LOC   oldLeft, oldRight;
 TANGO_LOC   newLeft, newRight;



 oldLeft = childLoc ( "LCHILD", from );
 if ( oldLeft != NULL ) {
   if ( ASSOCmember ("LCHILD",IDVAL,to) ) {
     newLeft = (TANGO_LOC) ASSOCretrieve("LCHILD",IDVAL,to);
     ANIMMoveSubTreeDown( oldLeft , newLeft);
   } else {
     fprintf(stderr,"MoveSubTreeDown: Attempt to move down too far!!\n");
     fprintf(stderr,"MoveSubTreeDown: skipping move Node \"biffed\" now!!\n");
   }
 }


 oldRight = childLoc ( "RCHILD", from );
 if ( oldRight != NULL ) {
   if ( ASSOCmember ("RCHILD",IDVAL,to) ) {
     newRight = (TANGO_LOC) ASSOCretrieve("RCHILD",IDVAL,to);
     ANIMMoveSubTreeDown( oldRight , newRight);
   } else {
     fprintf(stderr,"MoveSubTreeDown: Attempt to move down too far!!\n");
     fprintf(stderr,"MoveSubTreeDown: skipping move Node \"biffed\" now!!\n");
   }
 }

 
 root = (TANGO_IMAGE) ASSOCretrieve( "IMAGE_AT", IDVAL, from );
 ANIMMoveMe( root , from , to );

}








/********************* section 2 *************************/
/**  These routines are visible to the "outside" world. **/
 
void
ANIMInit( edges)
     int edges;
{
   TANGO_IMAGE loz;

   ASSOCinit();
   ASSOCmake("TEXT",0);
   ASSOCmake("CURRLOC",0);
   ASSOCmake("LOZENGE",0);
   loz = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.1, 0.1, 1,
			   TANGO_COLOR_BLUE, 0.05, 0.0);
   ASSOCstore("LOZENGE",loz);
   TWISTcreate_bintree(IDVAL,0.1,0.9,0.9,0.1,edges);

   nullImage = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.1,  0.1,  0,
				 TANGO_COLOR_RED, 0.05,  0.0);

}


void
ANIMNewVal( val ,  hkey )
     int val ;
     int hkey ;

{
   TANGO_LOC   from,to;
   TANGO_IMAGE text,loz;
   TANGO_PATH  path;
   TANGO_TRANS move;
   TANGO_IMAGE_COMPONENT components[3];
   
   /* add a new value's image */

   /*      */
   fill_component( val , hkey, components );

   text = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, 0.1, 0.1,
			    1, components );

   ASSOCstore("TEXT",text);

   /* move the lozenge to the root and make it visible */
   loz = (TANGO_IMAGE) ASSOCretrieve("LOZENGE");
   from = TANGOimage_loc(loz,TANGO_PART_TYPE_C);
   to = (TANGO_LOC) ASSOCretrieve("NUMBER",IDVAL,0);  /* root from TWISTcreatebintree */
   path = TANGOpath_distance(from,to,1.0);  /* make it in 1 frame */
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,loz,path);
   
   TANGOtrans_perform(move);


   /* save where we're at */
   ASSOCstore("CURRLOC",to);
 }
   



void
ANIMAddNode(  )
{

  TANGO_LOC	  from,to;
   TANGO_IMAGE	  text;
   TANGO_PATH	  movepath;
   TANGO_TRANS	  move;
   /* double	  sx,sy,nx,ny; */
   /* TANGO_IMAGE    loz;      */
   /* TANGO_LOC       parloc; */
   /* TANGO_IMAGE  parnode;   */
   /* TANGO_PATH   onepath;   */
   /* TANGO_TRANS  vis;       */

   /* move the value to its proper spot in the treap */
   text = (TANGO_IMAGE) ASSOCretrieve("TEXT");
   from = TANGOimage_loc(text,TANGO_PART_TYPE_C);
   to = (TANGO_LOC) ASSOCretrieve("CURRLOC");
   movepath = TANGOpath_distance(from,to,SPEED);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,movepath);
   TANGOtrans_perform(move);
   /* save the text image via this particular LOCATION */
   ASSOCstore("IMAGE_AT",IDVAL,to,text);

   /* DEBUG OUT  [ Going to go without lines ]
    * draw an edge to its parent value 
    *parloc = (TANGO_LOC) ASSOCretrieve("PARENT",IDVAL,to);
    *if (parloc)	* not the root *
    * { parnode = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL,parloc);
    *   TANGOloc_inquire(TANGOimage_loc(parnode,TANGO_PART_TYPE_S),&sx,&sy);
    *   TANGOloc_inquire(TANGOimage_loc(text,TANGO_PART_TYPE_N),&nx,&ny);
    * TANGOimage_create(TANGO_IMAGE_TYPE_LINE,sx,sy,1,TANGO_COLOR_BLACK,
    *			    nx-sx,ny-sy,1.0,1.0,0);
    * }
    *
    */ 

   /* Keep Loz Visible so walk back up */

}





void
ANIMCompAndMove( goleft)
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

   /* move the lozenge to the next spot in the treap */
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
ANIMMoveUpAndComp( fromleft)
     int fromleft;
/*
 *      Animate checking of Heap Keys on way back UP the treap.
 */
{
   TANGO_LOC	currloc,to;
   TANGO_IMAGE	loz;
   TANGO_PATH	movepath;
   TANGO_TRANS	move;

   /* should also be lozenge's loc at this point, but need to keep updating */
   currloc = (TANGO_LOC) ASSOCretrieve("CURRLOC");
      
   /* move the lozenge to the next spot in the treap */
   loz = (TANGO_IMAGE) ASSOCretrieve("LOZENGE");

   /* definitely redundant but leave for now */
   if (fromleft)
      to = (TANGO_LOC) ASSOCretrieve("PARENT",IDVAL,currloc);
   else
      to = (TANGO_LOC) ASSOCretrieve("PARENT",IDVAL,currloc);
   movepath = TANGOpath_distance(currloc,to,SLOWMO);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,loz,movepath);
   TANGOtrans_perform(move);
   /* update our current position */
   ASSOCstore("CURRLOC",to);
}



void
ANIMDelBiffNode(  )
/*
 * ANIMDelBiffNode -- remove a node which couldn't be inserted due to
 *        height limitation from view and make Loz disappear too.
 *
*/
{
    TANGO_IMAGE	  text;
    TANGO_TRANS   text_out;

    /* find the value to remove for scene */
    text = (TANGO_IMAGE) ASSOCretrieve("TEXT");
    text_out = TAPvis_toggle( text );
    TANGOtrans_perform(text_out);

    /* used to  make the lozenge invisible, not anymore  */

}


void
ANIMRLRotate ()
/*
 * ANIMRLRotate -- pivot subtree at the current node from right to 
 *           left.
 *
 * NOTE: if rotating right to left then there must be a root and right child.
 *       Remeber the childLoc routine only returns a Loc if there a treap node
 *       there.
 * 
 *  Alogrithm:
 *          1.  Gather the locations of the first three levels of locations.
 *                if some of the lower ones are not there then exit on error.
 *          2.  Move left subtree down.
 *          3.  Move old root down to left subtree root position.
 *          4.  Move new root into position.
 *          5.  Move new root's "old" left subtree into postion as the "new"
 *                 old roots right subtree.
 *          6.  Move new root's "old right subtree up into its new position.
 * 
*/
{
  TANGO_LOC  oldRoot ;
  TANGO_LOC  oldLeft, newRoot  ;
  TANGO_LOC  tempLoc;
  TANGO_LOC  oldLLeft, oldLRight;
  TANGO_LOC  oldRLeft, oldRRight;
  TANGO_IMAGE oldRootImage, newRootImage;
  TANGO_TRANS highlight;

  oldRoot = (TANGO_LOC) ASSOCretrieve("CURRLOC");
  oldRootImage = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL, oldRoot);

  /* Tap 5  this image now */
  highlight = TAPflash(oldRootImage,5 );
  TANGOtrans_perform(highlight );

  newRoot = childLoc("RCHILD" , oldRoot);
  if ( newRoot == NULL ) {
    fprintf(stderr,"RLRotate ERROR: new root is NOT a real image!!!\n");
    exit(1);
  } else {
    newRootImage = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL, newRoot);
    TANGOtrans_perform(TAPflash(newRootImage,5 ));
  }


  oldLeft = childLoc("LCHILD" , oldRoot);

  if ( oldLeft != NULL ) {  /* then have left subtree to move */
    tempLoc = oldLeft;
  } else {
    if ( ASSOCmember("LCHILD", IDVAL, oldRoot) ){
      tempLoc = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL, oldRoot);
    } else {
      fprintf(stderr,"RLRotate: No left child to rotate into!!");
      exit(1);
    }
  }

  if (ASSOCmember("LCHILD", IDVAL , tempLoc)) { /* there is a child of lchild*/
    oldLLeft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL , tempLoc);
  } else {
    fprintf(stderr,"RLRotate: No left left child to rotate into!!");
    exit(1);    
  }

  if (ASSOCmember("RCHILD", IDVAL , tempLoc)) { /* there is a child of lchild*/
    oldLRight = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL , tempLoc);
  } else {
    fprintf(stderr,"RLRotate: No left right child to rotate into!!");
    exit(1);    
  }

  oldRLeft = childLoc ("LCHILD", newRoot );
  oldRRight = childLoc ("RCHILD", newRoot );

  /* now start moving junk */ 

  /* Step 2. */
  if ( oldLeft != NULL ) { /* there IS a left subtree already there */
    ANIMMoveSubTreeDown( oldLeft , oldLLeft );
  }

  /* Step 3. */
  ANIMMoveMe( oldRootImage , oldRoot , tempLoc );
             /* there might not have been a previous left child image so use
              * tempLoc which holds the location of the "old" root's lchild
              */

  /* Step 4. */
  ANIMMoveMe( newRootImage , newRoot , oldRoot );   


  /* Step 5.*/
  if ( oldRLeft != NULL ) { /* there IS a right-left subtree already there */
    ANIMMoveSubTreeUp( oldRLeft , oldLRight ); 
               /* could be UP or Down since lateral move */
  }

  /* Step 6.*/
  if ( oldRRight != NULL ) { /* there IS a right-right subtree already there */
    ANIMMoveSubTreeUp( oldRRight , newRoot );
  }


  return;
}






void
ANIMLRRotate ()
/*
 * ANIMLRRotate -- pivot subtree at the current node from left to 
 *           right.
 *
 * NOTE: if rotating right to left then there must be a root and right child.
 *       Remeber the childLoc routine only returns a Loc if there a treap node
 *       there.
 * 
 *  Alogrithm:
 *          1.  Gather the locations of the first three levels of locations.
 *                if some of the lower ones are not there then exit on error.
 *          2.  Move right subtree down.
 *          3.  Move old root down to right subtree root position.
 *          4.  Move new root into position.
 *          5.  Move new root's "old" right subtree into postion as the "new"
 *                 old roots left substree.
 *          6.  Move new root's "old left subtree up into its new position.
 * 
*/
{
  TANGO_LOC  oldRoot ;
  TANGO_LOC  oldRight, newRoot  ;
  TANGO_LOC  tempLoc;
  TANGO_LOC  oldLLeft, oldLRight;
  TANGO_LOC  oldRLeft, oldRRight;
  TANGO_IMAGE oldRootImage, newRootImage;
  TANGO_TRANS highlight;

  oldRoot = (TANGO_LOC) ASSOCretrieve("CURRLOC");
  oldRootImage = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL, oldRoot);

  /* Tap 5  this image now */
  highlight = TAPflash(oldRootImage,5 );
  TANGOtrans_perform(highlight );

  newRoot = childLoc("LCHILD" , oldRoot);
  if ( newRoot == NULL ) {
    fprintf(stderr,"LRRotate ERROR: new root is NOT a real image!!!\n");
    exit(1);
  } else {
    newRootImage = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",IDVAL, newRoot);
    TANGOtrans_perform(TAPflash(newRootImage,5 ));
  }


  oldRight = childLoc("RCHILD" , oldRoot);

  if ( oldRight != NULL ) {  /* then have left subtree to move */
    tempLoc = oldRight;
  } else {
    if ( ASSOCmember("RCHILD", IDVAL, oldRoot) ){
      tempLoc = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL, oldRoot);
    } else {
      fprintf(stderr,"LRRotate: No right child to rotate into!!");
      exit(1);
    }
  }

  if (ASSOCmember("LCHILD", IDVAL , tempLoc)) { /* there is a child of rchild*/
    oldRLeft = (TANGO_LOC) ASSOCretrieve("LCHILD", IDVAL , tempLoc);
  } else {
    fprintf(stderr,"LRRotate: No right left child to rotate into!!");
    exit(1);    
  }

  if (ASSOCmember("RCHILD", IDVAL , tempLoc)) { /* there is a child of rchild*/
    oldRRight = (TANGO_LOC) ASSOCretrieve("RCHILD", IDVAL , tempLoc);
  } else {
    fprintf(stderr,"LRRotate: No right right child to rotate into!!");
    exit(1);    
  }

  oldLLeft = childLoc ("LCHILD", newRoot );
  oldLRight = childLoc ("RCHILD", newRoot );

  /* now start moving junk */ 

  /* Step 2. */
  if ( oldRight != NULL ) { /* there IS a right subtree already there */
    ANIMMoveSubTreeDown( oldRight , oldRRight );
  }

  /* Step 3. */
  ANIMMoveMe( oldRootImage , oldRoot , tempLoc );
             /* there might not have been a previous right child image so use
              * tempLoc which holds the location of the "old" root's rchild
              */

  /* Step 4. */
  ANIMMoveMe( newRootImage , newRoot , oldRoot );   


  /* Step 5.*/
  if ( oldLRight != NULL ) { /* there IS a left-right subtree already there */
    ANIMMoveSubTreeUp( oldLRight , oldRLeft ); 
               /* could be UP or Down since lateral move */
  }

  /* Step 6.*/
  if ( oldLLeft != NULL ) { /* there IS a left-left subtree already there */
    ANIMMoveSubTreeUp( oldLLeft , newRoot );
  }


  return;
}

    
/******************** debugging ***********************/

#define XOFFSET 0.00
#define YOFFSET 0.03



static void 
fill_component(skey,hkey,temp)
     int skey,hkey;
     TANGO_IMAGE_COMPONENT *temp;
/*
 * fill_componet -- fill a component structure to form image of the combo of
 *   BST key and hash key.  the hash key appears below and is marked with an
 *   h
 *
 *  Credits: essentially borrowed an modified from fftscenes.c 
 */
{
       
  /*  ****  Binary Search Key Text    **** */
  temp[0].type=TANGO_IMAGE_TYPE_TEXT;
  temp[0].args=(char *)calloc(80,sizeof(char));
  sprintf(temp[0].args,"%f %f TANGO_COLOR_BLACK variable %d 1",
	    0.0,0.0,skey);


  
  /*  ****  Heap Key Text    **** */
  temp[1].type=TANGO_IMAGE_TYPE_TEXT;
  temp[1].args=(char *)calloc(80,sizeof(char));
  sprintf(temp[1].args,"%f %f TANGO_COLOR_BLACK variable h%d 1",
	  XOFFSET,YOFFSET,hkey);

  
  temp[2].type=TANGO_IMAGE_TYPE_COMPOSITE;
  temp[2].args=NULL;

}  /*  fill_component()  */


