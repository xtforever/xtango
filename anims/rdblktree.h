/* Red-Black tree declarations for 'rdblktree.c' */
#define maxdepth 4

typedef struct RBnode {
  char color;                    /* 'R' for red or 'B' for black */
  int val;                       /* integer value of variable */
  struct RBnode* left;
  struct RBnode* right;          /* The tree pointers, including a back one */
  struct RBnode* parent;
} rbnode;
typedef rbnode* rbptr;
