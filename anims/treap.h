/*
 * treap.h  Treap ADT for treap anim project
 *
 * AUTHOR:  Lyman S. Taylor    ( lyman@cc.gatech.edu )
 * 
 */

#define MAX_DEPTH  4
#define TREAP_DEPTH  ( MAX_DEPTH + 3)
#define TRUE  1
#define FALSE 0

typedef struct TNODE  tnode;

struct TNODE {
  int value;
  int priority;
  struct TNODE *left;
  struct TNODE *right;
  struct TNODE *parent;
};


/* slot accessors */
#define TREAP_value(x)       x->value
#define TREAP_priority(x)    x->priority
#define TREAP_lchild(x)      x->left
#define TREAP_rchild(x)      x->right
#define TREAP_parent(x)      x->parent




typedef struct TNODE* tptr;

/*
 *  Animation routines
 *
 */


void ANIMInit( );

void ANIMNewVal(  );

void ANIMAddNode( );

void ANIMCompAndMove(  );

void ANIMMoveUpAndComp(  );

void ANIMDelBiffNode(  );

void ANIMRLRotate ( /* void */ );

void ANIMLRRotate ( /* void */ );


