#include "xtango.h"

typedef struct node *pNode;

struct node 
{
  pNode p;  /* parent */
  pNode child;
  pNode sibling;
  pNode right;

  int key;
  int mark;
  int degree;

  TANGO_LOC   loc;
  TANGO_LOC   tree_loc;
  TANGO_IMAGE tree_im;
};


typedef struct heap *pHeap;

struct heap 
{
  pNode	    head;
  TANGO_LOC loc;
};

