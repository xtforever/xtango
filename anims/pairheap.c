#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include "xtango.h"
#define MAXLEVELS 100
#define DELETE 1
#define REDUCE 0

/* FILE *infile; */    /* used in dos to read in a transaction file */

struct Node {
	int key, l, r, level;
	struct Node *dad, *left, *right;
	int x;
	TANGO_LOC pos;
	} *Heap;

struct Node* sibling_war();
void comp_link();   /* struct Node **, struct Node *, struct Node *); */
void del_node();
void reduce_node();
void add_key();
struct Node * find_key();
void print_heap();    /* struct Node *); */

struct pole {
	int level;
	int sum, pos;
	} LO[MAXLEVELS];        /* Location tally used to keep level info */

void AInquire(),APop(), AInit(), AAdd(), ABounds(), ADelete(); 
struct Node * AReduce();

static NAME_FUNCT fnc[] = { {"Init",        1, {VOID, (FPTR)AInit}},
					 {"Add",         1, {VOID, (FPTR)AAdd}},
					 {"Delete",      1, {VOID, (FPTR)ADelete}},
					 {"Pop",         1, {VOID, (FPTR)APop}},
					 {"Inquire",     1, {VOID, (FPTR)AInquire}},
					 {"Reduce",      1, {VOID, (FPTR)AReduce}},
					 {"Bounds",      1, {VOID, (FPTR)ABounds}},
					 { NULL,         0, NULL, NULL} };


struct OpToken {
	char op;
	int  val, val2;
	struct OpToken *Next;
	} *OpList;

/* ******************************************************* */
/* this array will point to nodes as needed for operations */
struct Node *NDARRY[30*MAXLEVELS];
/* ******************************************************* */

extern struct TNODE *THeap;
extern TANGO_LOC root_loc;



main(argc, argv)
int argc;
char *argv[];
{
int stupid; 
TANGOalgoOp(fnc, "BEGIN");
TANGOalgoOp(fnc, "Init");

/*   if (argc >= 2) input_file(argv[1]);   */

help();

for (;;) {
	if (OpList == NULL)
		if (!get_token()) exit(0);
	if (stupid = process_OpList() == 0 ) break;
	TANGOalgoOp(fnc, "Bounds");
	}
TANGOalgoOp(fnc, "END");
exit(0);
}


help()
{
printf("\nPAIRING HEAP PRIORITY QUEUE\nTwo Pass Variation\nBinary Tree Representation\n\n");
printf("i - insert key\n");
printf("m - extract minimum key\n");
printf("d - delete key\n");
printf("k - decrease key\n");
printf("l - look at a node's contents\n");
printf("h - print this menu\n");
printf("q - quit\n\n");
}


void reduce_node( val )     /* no explicit call to TANGOalgoOp */
int val;
{
struct Node *target;

if ( Heap == NULL) return;
target = (struct Node *)  TANGOalgoOp(fnc, "Reduce", val, REDUCE );
if (target == NULL) return;
Heap->key -= val;
if (target != Heap) {
        if( target->right != NULL )
                target->right->dad = target->dad;
	if (target->dad->left == target)
		target->dad->left = target->right;
	else
		target->dad->right = target->right;

	target->right = NULL;
	comp_link( &Heap, Heap, target );
	}	
}

/*
	POP_MIN()  takes the returns the minimum key and removes the root
	sibling_war(Heap->left) is called to reorganize the forrest
*/

int pop_min()
{
int ret;
if (Heap == NULL) return (-1);    /* trying to pop a null heap */
ret = Heap->key;

TANGOalgoOp(fnc, "Pop", &THeap, THeap, root_loc);

if ( Heap->left == NULL ) {
	Heap = NULL;
	}
else
	Heap = sibling_war( Heap->left );
return(ret);
}

/*
	SIBLING WAR does the two pass merging and makes appropriate calls
	to TANGO..(TWOPASS) to show the action created by the two passes
	The LEFT parameter is a pointer to the leftmost of the siblings
*/

struct Node * sibling_war(LEFT)
struct Node *LEFT;
{
int temp, nodes, i=0;
while (LEFT != NULL) {     /* NDARRY points to siblings */
	NDARRY[i++] = LEFT;
	LEFT->dad->right = NULL;     /* SEVER the link */
	LEFT = LEFT->right;
	}
if (1 == i) return (NDARRY[0]);  /* only 1 sibling so return pointer to it */
nodes = i;
NDARRY[nodes] = NULL;
for (i=0; i+1<nodes; i+=2) {      /* do left to right pass combining pairs*/
	comp_link(&NDARRY[i], NDARRY[i], NDARRY[i+1]);
	temp = i;
	}
if (temp == (nodes-3))      /* if there is a straggler node left over */
	comp_link(&NDARRY[temp], NDARRY[temp], NDARRY[temp+2]);
for (i=temp; i >= 2; i-=2) {
	comp_link(&NDARRY[i-2], NDARRY[i-2], NDARRY[i]);
	}
/* result of the two passes is a pairing heap pointed to by NDARRY[0] */
return (NDARRY[0]);
}


void del_node( )          /* gets a node to delete from animation mouse press */
{
struct Node *target;
if ( Heap == NULL) return;
target = (struct Node *)  TANGOalgoOp(fnc, "Reduce", 0, DELETE );
if (target == NULL) return;
if (target == Heap) 
	Heap = sibling_war( Heap->left);
else {
	if (target->dad->left == target)
		target->dad->left = target->right;
	else
		target->dad->right = target->right;

	target->right = NULL;
	target = sibling_war( target->left);
	comp_link( &Heap, Heap, target );
	}	
}

/*
	 add_key is used to insert a new item into the priority heap.
	 it allocates a node & gives it initial values.

	 TANGO..(ADD) creates a new node image and puts it on the screen
	 comp_link(heap, new) is called to fuse new node into the old tree
*/

void add_key(val)
int val;
{
struct Node *New;
if ( (New = (struct Node *) malloc(sizeof(struct Node))) == NULL ) {
	fprintf(stderr, "Memory allocation error\n");
	exit(0);
	}
New->key = val;
New->dad = NULL;
New->left = NULL;
New->right = NULL;
New->l = New->r = New->level = 0;

TANGOalgoOp(fnc, "Add", &New, val);     /* new node appears in appearance spot */
if (Heap == NULL) {
	Heap = New;
	}
else
	comp_link(&Heap, Heap, New);
}

/*
	COMP_LINK(*PARENT, LEFT, RIGHT)
	Compare and Link two subtrees  PARENT pointer points to lowval node
	ASSUMES:  the LEFT node has no right child !!!!!!
*/

void comp_link(PARENT, LEFT, RIGHT)
struct Node **PARENT, *LEFT, *RIGHT;
{
if ( RIGHT == NULL )  return; 

if (LEFT->key <= RIGHT->key ) {      /* make left node root of new tree */
	RIGHT->dad = LEFT;            /* RIGHT is now L child of LEFT */
	LEFT->right = RIGHT->right;
	if (LEFT->right != NULL)
		LEFT->right->dad = LEFT;       /* new parent */
	RIGHT->right = LEFT->left;
	if (RIGHT->right != NULL)
		RIGHT->right->dad = RIGHT;
	LEFT->left = RIGHT;
	*PARENT = LEFT;                /* don't need to change LEFT->dad */
	}
else {                            /* make the right node root of new tree */
	RIGHT->dad = LEFT->dad;        /* RIGHT node gets LEFT'S parent */
	LEFT->dad = RIGHT;
	LEFT->right = RIGHT->left;     /* L->r assumed not to be used */
	if (LEFT->right != NULL)
		LEFT->right->dad = LEFT;
	RIGHT->left = LEFT;            /* LEFT is left child of RIGHT */
	*PARENT = RIGHT;               /* Point to the winner */
	}
}


struct Node * find_key(key, joe)   /* expects that key is in the heap !!! */ 
int key;
struct Node *joe;
{
struct Node *theNode;
if (joe->key == key)
	return (joe);
else {
  if (joe->left != NULL)
	  return(find_key(key, joe->left));
  else
	  if (joe->right != NULL)
		  return(find_key(joe->right));
	  else
		  return(NULL);
  }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * D U L L   U T I L I T Y   R O U T I N E S * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
input_file, get_token, and process_oplist are uninteresting
utility functions that make life easier but are
not related to the animation
*/

/* 
int input_file(fname)
char *fname;
{
char tempop;
struct OpToken *OLP, *OLPTEMP;
int  tempint, tempint2, i;

	 if( (infile = fopen( fname, "rt" )) == NULL ) {
		 printf( "Can't open input file %s, tough snails\n",fname );
		 exit( 1 );
		 }

if ((OLP = (struct OpToken *)malloc(sizeof(struct OpToken))) == NULL)
	return -1;
OpList = NULL;
OLPTEMP = NULL;

fscanf(infile," %c ", &OLP->op);

for (;;) {
	switch (OLP->op) {
		case 'a' :
		case 'd' : fscanf (infile, "%d ", &OLP->val); break;
		case 'r' : fscanf (infile, "%d %d ", &OLP->val, &OLP->val2); break;
                case 'l' : TANGOalgoOp(fnc, "Layout"); break; 
		default : printf("I don't know any \"%c\" operation\n", OLP->op);
		}
	if ((OLPTEMP = (struct OpToken *)malloc(sizeof(struct OpToken))) == NULL)
		return -1;

	if (OpList == NULL) OpList = OLP;
	if (fscanf(infile," %c ", &tempop) != EOF) {
		OLP->Next = OLPTEMP;
		OLP = OLPTEMP;
		OLP->op = tempop;
		}
	else {
		OLP->Next = NULL;
		break;
		}
	}
fclose(infile);
return (1);
} 

*/ 
int get_token()
{
struct OpToken *OLP;
if ((OpList = (struct OpToken *)malloc(sizeof(struct OpToken))) == NULL)
	return(-1);

printf("What operation? ::");
scanf(" %c", &OpList->op);
getchar();
if ( ( OpList->op == 'i') || (OpList->op == 'a' )) {
	printf("\nValue for new node ::");   scanf("%d", &OpList->val);
	}
if ( ( OpList->op == 'k') || ( OpList->op == 'r' ) ) {
	printf( "Reduce key by what amount? ::");
	scanf("%d", &OpList->val);
	}
OpList->Next = NULL;
return(1);
}



int process_OpList()
{
int ret = 1;
switch (OpList->op) {
	case 'q' : 
		printf("Click on the Quit box in animation window\n");
		return(0);
	case 'l' : 
		TANGOalgoOp(fnc, "Inquire"); break;

	case 'a' :
	case 'i' : 
		add_key(OpList->val); break;
	case 'd' : 
		del_node(); break;
	case 'm' : 
		pop_min(); break;

	case 'k' :
	case 'r' : 
		reduce_node( OpList->val ); break;
	case 'x' : 
		TANGOalgoOp(fnc, "Layout"); break;
	case 'o' : 
		print_heap(Heap); break;
	case 'h' : 
		help(); break;
	default : 
		printf("I don't know a(n) \"%c\" operation\n", OpList->op);
	}
OpList = OpList->Next;
return(ret);
}


void print_heap(this)
struct Node *this;
{
if (this != NULL) {
	printf("|| %3d | %3d | %3d | ", this->key, this->l, this->r);
	printf(" %3d | %3d || ME: %x\n", this->level, this->x, this);
	printf("DAD->%x, left->%x, right->%x\n\n", this->dad, this->left, this->right);

	if (this->left != NULL)
		print_heap(this->left);
	if (this->right != NULL)
		print_heap(this->right);
	}
}

