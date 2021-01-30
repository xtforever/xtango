/*
 * treap.c -- the main routines for the treap XTango projects [ Not a ADT
 *    Module ] ( i.e. there is a main fcn. at the end of this file.
 *
 *  AUTHOR: Lyman S. Taylor  ( lyman@cc.gatech.edu )
 *
 *   
 *  This project produces an limited animation of a treap abstract data type
 *  ( ADT ).  See the file treap.doc for a listing of the associated files,
 *  background info ,  and "user's manual"
 *
 *
 *  This file, treap.c, is broken up into three sections.  The first defines
 *    the "normal" treap ADT routines.  Only these are also the Animation
 *    drivers.  The second section is some debugging routines.  The  third
 *    is the main function.
 *
 *  NOTES:
 *           The program does not delete unlike the "tree" anim.
 *
 *           It also do not display the tree arcs.  [ See treapscenes.c's 
 *              intro for more dicussion on this. ]
 *
 *
 *           [ PNOTE: CodeCenter barfs on missing some library debug later.]
 *
 *  CREDITS:
 *           portions of treap.c and treapscenes.c were derived from the 
 *              files  tree.c and treescenes.c from the xtango/anims directory.
 *              [ Since not marked otherwise I'll attributet these to Prof.
 *                Stasko ]
 *              Also additional insprition came from avl.c avlscenes for the
 *              subtree movement routines. [ Mainly the idea to leave out the
 *              branches.... that would add much more code. These files were
 *              written by Nick Duncan  ]
 *
 *
 *  BUGS:  
 *        1.  Doesn't do garbage collection at the moment.
 *        2.  Doesn't pick "random" seed .... like time of day. ( may be a 
 *                 feature ... at least until debugged ) So demo mode will 
 *                 always repeat same sequence. And user mode with get same
 *                 sequence of keys.
 *        3.  Sometimes the ANIM structure tree is not tall enough. I catch
 *                 The errors associated with this and bother the user with
 *                 it.
 * 
 * 
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include "xtango.h"
#include  "treap.h"



/* Animation Functions */
                          /* alias     # subcompnts     subcompnts */ 
static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"NewVal", 1, {VOID, (FPTR)ANIMNewVal}},
			    {"AddNode",   1, {VOID, (FPTR)ANIMAddNode}},
			    {"CheckNext",   1, {VOID, (FPTR)ANIMCompAndMove}},
			    {"CheckHeapKey", 1,{VOID,(FPTR)ANIMMoveUpAndComp}},
			    {"RepairBiff",   1, {VOID, (FPTR)ANIMDelBiffNode}},
               		    {"RotateRL",   1, {VOID, (FPTR)ANIMRLRotate}},
               		    {"RotateLR",   1, {VOID, (FPTR)ANIMLRRotate}},
			    {NULL,	  0,  NULL, NULL} };



static	tptr	theRoot;


/*
 * this module's  treap routines
 *   patterned after the "normal" treap ADT routines.
 *

 */


static tptr  TREAP_init ( );
static tptr  TREAP_newNode (  /* value */);
static tptr  TREAP_insert ( /* root ,  newNode , depth */ );
static tptr  TREAP_delete ( /*  root , oldData   */ );   /* NOT done */
static tptr  TREAP_LRRotate ( /* tptr root */ );
static tptr  TREAP_RLRotate ( /* tptr root */ );


static void  TREAP_print ( /* tptr t */);


/*************** section 1  *******************************/


static tptr
TREAP_init (  )
/*
 * TREAP_init --  returns a new instance of a Treap.
 *
*/
{

  /* set up random number generator */
  
  return NULL;


}


static tptr 
TREAP_newNode (  value )
     int value ;
/*
 * TREAP_newNode -- return a new treap node holding the correct value and 
 *          properly intialized. Assign a random heap key. set all other
 *          slots to NULL.
 * 
 */
{
  tptr new;

  new  =  ( tptr ) malloc ( sizeof( tnode ) );
  TREAP_value(new)      = value;
  TREAP_priority(new)   = ( random() % 1024 );
  TREAP_lchild(new)  = NULL;
  TREAP_rchild(new) = NULL;
  TREAP_parent(new)  = NULL;

  TANGOalgoOp(fcn, "NewVal",value , TREAP_priority(new) );

  return new;

}







static tptr
TREAP_LRRotate ( root )
     tptr root;
/*
 * TREAP_LRRotate -- rotate the tree right from the root and return the
 *             the new value of the root.
 *
 *
*/
{
  tptr oldRoot;

  oldRoot = root;            
  root = TREAP_lchild(root); 
  TREAP_lchild(oldRoot) = TREAP_rchild(root);
  TREAP_rchild(root) = oldRoot;
  TANGOalgoOp(fcn, "RotateLR"); 
  return root;
} 



static tptr
TREAP_RLRotate ( root )
     tptr root;
/*
 * TREAP_LRRotate -- rotate the tree right from the root and return the
 *             the new value of the root.
 *
 *
*/
{
  tptr oldRoot;

  oldRoot = root;            
  root = TREAP_rchild(root); 
  TREAP_rchild(oldRoot) = TREAP_lchild(root);
  TREAP_lchild(root) = oldRoot;
  TANGOalgoOp(fcn, "RotateRL"); 
  return root;
} 


static tptr
TREAP_insert( root ,  newNode ,  depth )
     tptr root;
     tptr newNode ;
     int depth ;
/*
 * TREAP_insert -- insert a new node into the tree.
 *
 *  Input: a treap and new data
 *  Output: new treap with new node
 * 
 *  Algorithm.
 *            1. Find  place to insert into Binary Search Tree.
 *            2. Start rising up heap till find place to insert self (reheap)
 *                ( After quick check to see if meet the leaf conditions )
 *            3. Return new Treap root node.  ( change from typical insert
 *                  where child in then assigned. )
 *
 *
 */
{

   if (root == NULL) {
      root =   newNode ;
      TANGOalgoOp(fcn, "AddNode");
      return root ;
    }


   if ( depth >= MAX_DEPTH ) {
     printf("Treap limited to %d levels will not add this node %d*%d\n",
	    MAX_DEPTH, TREAP_value(newNode) , TREAP_priority(newNode) );
     TANGOalgoOp(fcn, "RepairBiff");
     return root ;
   }
   depth += 1; /* go down */

   if ( TREAP_value(newNode) == TREAP_value(root) ) {
     printf("Duplicate keys not allowed skipping insertion \n");
     TANGOalgoOp(fcn, "RepairBiff");
   }else  {
     if (TREAP_value(newNode) < TREAP_value(root) ){
       TANGOalgoOp(fcn, "CheckNext",1);
       TREAP_lchild(root) = TREAP_insert(TREAP_lchild(root) , newNode, depth);
       TANGOalgoOp(fcn, "CheckHeapKey",1);
       if ( TREAP_priority(TREAP_lchild(root)) > TREAP_priority(root) ) {
	 root = TREAP_LRRotate( root );
       }
     } else {
       TANGOalgoOp(fcn, "CheckNext",0);
       TREAP_rchild(root) = TREAP_insert(TREAP_rchild(root) , newNode, depth);
       TANGOalgoOp(fcn, "CheckHeapKey",0);
       if ( TREAP_priority(TREAP_rchild(root)) > TREAP_priority(root) ) {
	 root = TREAP_RLRotate( root ); 
       }
     }
   }
   return root;
};




static tptr
TREAP_insertd( root ,  newNode ,  depth )
     tptr root;
     tptr newNode ;
     int depth ;
/*
 * TREAP_insert -- insert a new node into the tree. This one allow duplicates
 *
 *  Input: a treap and new data
 *  Output: new treap with new node
 * 
 *  Algorithm.
 *            1. Find  place to insert into Binary Search Tree.
 *            2. Start rising up heap till find place to insert self (reheap)
 *                ( After quick check to see if meet the leaf conditions )
 *            3. Return new Treap root node.  ( change from typical insert
 *                  where child in then assigned. )
 *
 *
 */
{

   if (root == NULL) {
      root =   newNode ;
      TANGOalgoOp(fcn, "AddNode");
      return root ;
    }


   if ( depth >= MAX_DEPTH ) {
     printf("Treap limited to %d levels will not add this node %d*%d\n",
	    MAX_DEPTH, TREAP_value(newNode) , TREAP_priority(newNode) );
     TANGOalgoOp(fcn, "RepairBiff");
     return root ;
   }
   depth += 1; /* go down */

   
   if (TREAP_value(newNode) <= TREAP_value(root) ){
     TANGOalgoOp(fcn, "CheckNext",1);
     TREAP_lchild(root) = TREAP_insertd(TREAP_lchild(root) , newNode, depth);
     TANGOalgoOp(fcn, "CheckHeapKey",1);
     if ( TREAP_priority(TREAP_lchild(root)) > TREAP_priority(root) ) {
       root = TREAP_LRRotate( root );
     }
   } else {
     TANGOalgoOp(fcn, "CheckNext",0);
     TREAP_rchild(root) = TREAP_insertd(TREAP_rchild(root) , newNode, depth);
     TANGOalgoOp(fcn, "CheckHeapKey",0);
     if ( TREAP_priority(TREAP_rchild(root)) > TREAP_priority(root) ) {
       root = TREAP_RLRotate( root ); 
     }
   }
   
   return root;
};


/*************** section 2 *******************************/
/* DEBUG routines */



static void  
TREAP_print ( t, depth )
     tptr t;
     int  depth;
{
   int i;
   if (t == NULL) return;

   TREAP_print(TREAP_lchild(t), (depth + 1 ) );
   for( i =0; i < depth ; i++ ) printf("\t");
   printf("<TREAP skey  %d  hkey %d>\n",TREAP_value(t), TREAP_priority(t) );
   TREAP_print(TREAP_rchild(t), ( depth + 1 ) );
};



/*************** section 3 *******************************/



demo()
/* 
 * In this mode the program runs until completion.
 * It will try and insert 20 nodes into the treap.
 *
 */
{
   int i,j;
   tptr nextNode;

   TANGOalgoOp(fcn, "BEGIN");

   theRoot = TREAP_init();

   TANGOalgoOp(fcn, "Init",TREAP_DEPTH);
   for (i = 0; i < 20; ++i) {
      j = random() % 1024;
      nextNode = TREAP_newNode( j ); /* goes here so NewVal gets called */
      theRoot = TREAP_insert(theRoot, nextNode , 0);
    };

   TREAP_print(theRoot, 0);
   TANGOalgoOp(fcn, "END");
};







buggy()
/*
 *  In this mode the "Heap" keys are picked randomly.
 *  By the BST keys are entered by the user.
 *
 *  Insert only available at this time.
 *
*/
{
   int i,value;
   tptr nextNode;

   TANGOalgoOp(fcn, "BEGIN");

   theRoot = TREAP_init();


   TANGOalgoOp(fcn, "Init",TREAP_DEPTH);
   for (i = 0; i < 30; ++i) {
      printf("Enter search key value (1-100) (0 to quit)\n");
      scanf("%d",&value);
      if (value == 0) break;
      nextNode = TREAP_newNode( value ); /* goes here so NewVal gets called */
      theRoot = TREAP_insertd(theRoot, nextNode , 0);
    };

   TREAP_print(theRoot, 0);
   TANGOalgoOp(fcn, "END");
};



randHkeys()
/*
 *  In this mode the "Heap" keys are picked randomly.
 *  By the BST keys are entered by the user.
 *
 *  Insert only available at this time.
 *
*/
{
   int i,value;
   tptr nextNode;

   TANGOalgoOp(fcn, "BEGIN");

   theRoot = TREAP_init();


   TANGOalgoOp(fcn, "Init",TREAP_DEPTH);
   for (i = 0; i < 30; ++i) {
      printf("Enter search key value (1-100) (0 to quit)\n");
      scanf("%d",&value);
      if (value == 0) break;
      nextNode = TREAP_newNode( value ); /* goes here so NewVal gets called */
      theRoot = TREAP_insert(theRoot, nextNode , 0);
    };

   TREAP_print(theRoot, 0);
   TANGOalgoOp(fcn, "END");
};



main( argc , argv)
     int argc;
     char* argv;
/*
 *
*/
{
  char userMode[80];
  printf("Treap data Structure animation\n");
  printf("\t( this animation does NOT draw node circles and edges \n");
  printf("\t the outline of the tree should be clear though). \n\n");

  printf("User mode will attempt the insertion of up to 20 entries \n");
  printf("Demo mode will attempt the insertion of 20 entries without\n");
  printf("\tAny user input\n\n");
  printf("Please select [d]emo or [u]ser mode \n");

  /* probably overkill on size, but hey! */
  //  userMode = (char *)calloc( 80 , sizeof(char));

    
  fgets(userMode, sizeof userMode, stdin);

  switch ( *userMode)
    {
    case 'u':  
      randHkeys();
      break;

     case 'd':
      demo();
      break;
      
    case 'b':
      buggy();
      break;

     default:
       fprintf(stderr,"Improper mode: enter a 'd' or a 'u' \n");
       exit(1);
       break;
    }

  exit(0 );
} 


/* EOF  */


















