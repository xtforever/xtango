/* Binary tree declarations for 'bintree.c' */
#define maxdepth 5
#define TRUE 1
#define FALSE 0

typedef struct Bnode {
  int val;                       /* integer value of variable */
  struct Bnode* left;
  struct Bnode* right;          /* The tree pointers, including a back one */
  struct Bnode* parent;
} bnode;
typedef bnode* bptr;
