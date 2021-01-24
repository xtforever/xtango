#include <stdio.h>
#include "xtango.h"
#include <stdlib.h>
#include <malloc.h>

#define DELETE 1
#define REDUCE 0
#define TRUE 1
#define FALSE 0
#define MAXLEVELS 100
#define TNRADIUS .015
#define ONE 1.0
#define DISTANCE .035
#define HEIGHT  .05
#define WIDTH   .05
#define MAX(A,B) (( (A) > (B)) ? (A) : (B))
#define MIN(A,B) (( (A) < (B)) ? (A) : (B))

TANGO_TRANS DUMMY_TRANS;
TANGO_PATH DUMMY_PATH, DOWN_PATH, UP_PATH, GREEN_PATH, RED_PATH, EXAMPLE_PATH;

struct TNode {
	int key, l, r, level, x, levels_below;
	struct TNode *dad, *left, *right;
	TANGO_IMAGE image;
	TANGO_IMAGE vim;        /* key's text image in box */
	TANGO_IMAGE RLI;        /* right and left link images */
	TANGO_IMAGE LLI;
	TANGO_LOC pos;
	struct Node *analog;
	} *THeap;



void GETEXTREMEXVALS();
void update_node_width();  
void delete_image();  
struct TNode * Tsibling_war();           /* struct TNode *);  */
void Tcomp_link(); /* struct TNode **, struct TNode *, struct TNode *);  */
void reset_pole();           /* void);  */
struct Node * ADelete();        /* int);  */
void AAdd();           /* int);  */
void APop();           /* void);  */
void AInquire();
void AInit();          /* void);  */
void new_pos();
void metrics();        /* void);  */
TANGO_TRANS TRANSSUBTREE();  /* TANGO_TRANS, TANGO_PATH, struct TNode *);  */

TANGO_LOC new_loc, root_loc;    /* set by Ainit */
float  YDX;                 /* set by layout vertical distance between levels */
float  XDX;                 /* set by layout horizontal space unit            */


struct TNode *NDARRY[10*MAXLEVELS];
double    SCREEN_X_LO = 0.0,
	  SCREEN_X_HI = 1.0,
	  SCREEN_Y_LO = 0.0,
	  SCREEN_Y_HI = 1.0;


struct TNode *TRoot;      /* same as THeap pointer to TNode */

struct pole {             /* location/level tally keeps track of */
	int level;             /* the total width of subtrees of nodes */
	int sum, pos;          /* for each level */
	int nodes;
	} POLE[MAXLEVELS];

/* these tango routines are called by the driver program.      */
/* every operation performed on the driver's data structure    */
/* is also performed on the Tango data structure maintained    */
/* by these routines.  In addition to maintaining the Tango    */
/* data structure, these functions also animate the changes    */
/* to the data structure using XTango objects and fn calls.    */

/*
	SIBLING WAR does the two pass merging
	First it fills the NDARRY pointers to point to the siblings
	of its Parameter LEFT, while it does that, it moves each
	subtree up to the level of the root making ready for the
	two passes of Tcomp_link
*/
struct TNode * Tsibling_war(LEFT)
struct TNode *LEFT;
{
int temp, sibs = 0, i=0, j;

TANGO_LOC   target;
TANGO_PATH  path;     /* visibility & movement paths */
TANGO_TRANS trans, trans2;    /* vis & movement transitions  */


while (LEFT != NULL) {     /* NDARRY points to siblings */
	NDARRY[i++] = LEFT;
	NDARRY[i]   = LEFT->right; 
	if (LEFT->right != NULL) {      /* delete the arrow */
		delete_image(LEFT->RLI); 
		LEFT->right = NULL; 
		update_node_width(LEFT); 
		}
		j = ((i-1) > 0); 
	target = TANGOloc_create(
	(TANGOloc_X(TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW)) + j * XDX),
	(TANGOloc_Y(TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW)) - i * YDX));
	path = TANGOpath_distance (
		TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW),
		target,
		DISTANCE);
	trans = TRANSSUBTREE ( DUMMY_TRANS, path, LEFT); 	
	TANGOtrans_perform(trans);
	LEFT = NDARRY[i]; 
	}

if (1 == i) return (NDARRY[0]);  /* only 1 sibling so return pointer to it */
sibs = i;
NDARRY[sibs] = NULL;
for (i=0; i+1<sibs; i+=2) {      /* do left to right pass combining pairs*/
	Tcomp_link(&NDARRY[i], NDARRY[i], NDARRY[i+1]);
	temp = i;
	}
if (temp == (sibs-3)) Tcomp_link(&NDARRY[temp], NDARRY[temp], NDARRY[temp+2]);

for (i=temp; i >= 2; i-=2) {
	Tcomp_link(&NDARRY[i-2], NDARRY[i-2], NDARRY[i]);
	}
return (NDARRY[0]);     /* result is a pairing heap pointed to by NDARRY[0] */
}


void AInit()
{
TANGO_LOC start, one, two, finish;
TANGO_PATH path_one, path_two, path_three;
double x, y;
x = 0.0, y = 0.0;
start = TANGOloc_create( x, y ); 
x = 0.0, y = 0.5;
one   = TANGOloc_create( x, y ); 
x = 0.5, y = 0.5;
two   = TANGOloc_create( x, y ); 
x = 0.5, y = 0.0;
finish = TANGOloc_create( x, y ); 
path_one = TANGOpath_distance( start, one, DISTANCE );
path_two = TANGOpath_distance( one, two, DISTANCE );
path_three = TANGOpath_distance( two, finish, DISTANCE );
EXAMPLE_PATH =	TANGOpath_rotate(TANGOpath_type(TANGO_PATH_TYPE_COUNTERCLOCKWISE), 180);

XDX = WIDTH * 1.5;
YDX = HEIGHT * 1.5;
new_loc   = TANGOloc_create ( .6, .1);   /* create standard positions */
root_loc  = TANGOloc_create ( .4, .1);   /* create standard new node */
GREEN_PATH = TANGOpath_color( TANGO_COLOR_YELLOW );
RED_PATH   = TANGOpath_color( TANGO_COLOR_RED   );
DUMMY_PATH = TANGOpath_distance(new_loc, root_loc, DISTANCE);
DOWN_PATH  = TANGOpath_distance(new_loc, 
				TANGOloc_create( TANGOloc_X(new_loc) - XDX, 
					       ( TANGOloc_Y(new_loc) + YDX) ), 
				YDX / 5 );
UP_PATH    = TANGOpath_distance(new_loc, 
				TANGOloc_create(TANGOloc_X(new_loc), ( TANGOloc_Y(new_loc) - YDX ) ), 
				YDX / 5 );

DUMMY_TRANS = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, DUMMY_PATH);
THeap = NULL;
reset_pole();
} 

void delete_image(image)
TANGO_IMAGE image;
{
TANGO_TRANS trans;
TANGO_PATH  path;
double xoffset[1], yoffset[1];
xoffset[0] = yoffset[0] = 1.0; 

path = TANGOpath_create(1,xoffset,yoffset); 
trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, image, path);
TANGOtrans_perform(trans); 
TANGOpath_free( 1, path);
TANGOtrans_free( 1, trans);
} 




void APop( p_p_root, p_root, popped_root_loc )
struct TNode **p_p_root;
struct TNode  *p_root;
TANGO_LOC popped_root_loc;
{
TANGO_PATH path;
TANGO_TRANS trans;
struct TNode *temp;

delete_image(p_root->image); 
delete_image(p_root->vim); 
if (p_root->left != NULL)  { 
	delete_image(p_root->LLI); 
	temp = p_root;
	*p_p_root = Tsibling_war( p_root->left );
	free(temp);
        path = TANGOpath_distance(TANGOimage_loc( (*p_p_root)->image,TANGO_PART_TYPE_NW),
		popped_root_loc, DISTANCE); 
	trans = TRANSSUBTREE(DUMMY_TRANS, path, *p_p_root ); 
	TANGOtrans_perform(trans); 
	TANGOtrans_free( 1, trans );
	}
else { 
	free(p_root);
	*p_p_root = NULL;
	}
} 


struct TNode * CHECKTREEFORHIT( x, y, this )
double x, y;
struct TNode *this;
{
struct TNode *result = NULL;

if (this == NULL )   return (NULL);

if ( (TANGOloc_X(TANGOimage_loc(this->image, TANGO_PART_TYPE_NW ))  <= x ) &&
     (TANGOloc_X(TANGOimage_loc(this->image, TANGO_PART_TYPE_NE ))  >= x ) &&
     (TANGOloc_Y(TANGOimage_loc(this->image, TANGO_PART_TYPE_NE ))  <= y ) &&
     (TANGOloc_Y(TANGOimage_loc(this->image, TANGO_PART_TYPE_SE ))  >= y ) )
		return (this);

result = CHECKTREEFORHIT( x, y, this->left);
if (result == NULL)
	result = CHECKTREEFORHIT( x, y, this->right);	
return (result);
}

struct Node * AReduce( reduction, mode)
int reduction, mode;
{
struct TNode *chosen_node, *np_temp;
double x,y;
struct Node *return_node = NULL;
TANGO_LOC  target;
TANGO_PATH path;
TANGO_TRANS trans, trans2, killer;
int he_has_right_child = 0, he_is_left_child, spacing;
double newx, newy, sx, sy;
char str[5];

if (mode == DELETE )
	printf("\nClick on node to delete\n");
else
	printf("\nClick on node to reduce its key value by %d \n", reduction );

if ( TANGOinput_coord( &x, &y ) ) {
	chosen_node = CHECKTREEFORHIT( x, y, THeap );
	}
else    {
	printf("Missed everything \n");
	return( NULL );
	}
if ( chosen_node != NULL ) {
	return_node = chosen_node->analog;
	if ( chosen_node == THeap ) {
		if ( mode == DELETE ) {
			APop( &THeap, THeap, root_loc);
			}
		else {
			delete_image( THeap->vim );
			THeap->key = THeap->key - reduction;
			newx = TANGOloc_X(TANGOimage_loc(THeap->image, TANGO_PART_TYPE_C));
			newy = TANGOloc_Y(TANGOimage_loc(THeap->image, TANGO_PART_TYPE_C));
			sprintf(str,"%d",THeap->key);
			THeap->vim= TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,
				newx,newy,1,TANGO_COLOR_BLACK, "variable",str,1);
			}
		}	 
	else {
		delete_image( chosen_node->vim );
		chosen_node->key = chosen_node->key - reduction;
		newx = TANGOloc_X(TANGOimage_loc(chosen_node->image, TANGO_PART_TYPE_C));
		newy = TANGOloc_Y(TANGOimage_loc(chosen_node->image, TANGO_PART_TYPE_C));
		sprintf(str,"%d",chosen_node->key);
		chosen_node->vim= TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,
			newx,newy,1,TANGO_COLOR_BLACK, "variable",str,1);

		if (chosen_node->right != NULL)
			he_has_right_child = 1;

		if (chosen_node->dad->left == chosen_node ) {
			he_is_left_child = TRUE;
			chosen_node->dad->left = NULL;
			update_node_width(chosen_node->dad);
			chosen_node->dad->left = chosen_node->right; 
			delete_image( chosen_node->dad->LLI);
			}
		else {	
			he_is_left_child  = FALSE;
			chosen_node->dad->right = NULL;
			update_node_width(chosen_node->dad);
			chosen_node->dad->right = chosen_node->right;
			delete_image( chosen_node->dad->RLI );
			}
		
		if ( he_has_right_child ) {
			chosen_node->right->dad = chosen_node->dad;
			delete_image( chosen_node->RLI );
			np_temp = chosen_node->right;
			chosen_node->right = NULL;
			update_node_width (chosen_node);

			if ( he_is_left_child ) {
				spacing = np_temp->r + 1 ; /*  chosen_node->dad->l + 1; */
				target = TANGOloc_create(TANGOloc_X(
				TANGOimage_loc(chosen_node->dad->image, TANGO_PART_TYPE_NW)) - spacing * XDX, 
				TANGOloc_Y(TANGOimage_loc(chosen_node->image, TANGO_PART_TYPE_NW)) );

				sx = (WIDTH + TANGOloc_X(target)) - TANGOloc_X(TANGOimage_loc(chosen_node->dad->image,
							TANGO_PART_TYPE_SW)) ;
				sy = TANGOloc_Y(target) - TANGOloc_Y(TANGOimage_loc(chosen_node->dad->image,
							TANGO_PART_TYPE_SW));

				chosen_node->dad->LLI = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
							TANGOloc_X(TANGOimage_loc(chosen_node->dad->image, 
							TANGO_PART_TYPE_SW)), 
							TANGOloc_Y(TANGOimage_loc(chosen_node->dad->image,
							TANGO_PART_TYPE_SW)),
							1, TANGO_COLOR_BLACK, sx, sy, 0.0, ONE, 0);
				}
			else	{
				spacing = np_temp->l + 1; /* chosen_node->dad->r + 1; */
				target = TANGOloc_create(TANGOloc_X(
				TANGOimage_loc(chosen_node->dad->image, TANGO_PART_TYPE_NW)) + spacing * XDX, 
				TANGOloc_Y(TANGOimage_loc(chosen_node->image, TANGO_PART_TYPE_NW)) ); 

				sx = TANGOloc_X(target) - TANGOloc_X(TANGOimage_loc(chosen_node->dad->image,
							TANGO_PART_TYPE_SE));
				sy = TANGOloc_Y(target) - TANGOloc_Y(TANGOimage_loc(chosen_node->dad->image,
							TANGO_PART_TYPE_SE));

				chosen_node->dad->RLI = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
							TANGOloc_X(TANGOimage_loc(chosen_node->dad->image, 
							TANGO_PART_TYPE_SE)), 
							TANGOloc_Y(TANGOimage_loc(chosen_node->dad->image,
							TANGO_PART_TYPE_SE)),
							1, TANGO_COLOR_BLACK, sx, sy, 0.0, ONE, 0);
				}

			path = TANGOpath_distance( 
				TANGOimage_loc(np_temp->image, TANGO_PART_TYPE_NW),
				target, DISTANCE );

			trans = TRANSSUBTREE( DUMMY_TRANS, path, np_temp);
			TANGOpath_free( 1, path );
			}
		chosen_node->right = NULL;
		update_node_width( chosen_node );
		update_node_width( chosen_node->dad );
		path = TANGOpath_distance(TANGOimage_loc(chosen_node->image, TANGO_PART_TYPE_NW),				new_loc, DISTANCE );
		trans2 = TRANSSUBTREE( DUMMY_TRANS, path, chosen_node);
		if ( he_has_right_child ) {
			killer = trans2;
			trans2 = TANGOtrans_compose( 2, trans, trans2 );
			TANGOtrans_free( 1, killer);
			}
		TANGOtrans_perform(trans2);
		TANGOtrans_free( 1, trans2 );
		
		if ( mode == DELETE )
			APop( &chosen_node, chosen_node, new_loc );
		Tcomp_link( &THeap, THeap, chosen_node);
		}
	}
else {
	printf("Poor Marksmanship\n");
	}

return (return_node);
}

void AInquire()
{
struct TNode *chosen_node;
double x,y;
struct Node *return_node = NULL;

printf("\nClick on the node to inquire about \n");
if ( TANGOinput_coord( &x, &y ) ) {
	chosen_node = CHECKTREEFORHIT( x, y, THeap );
	}
else    {
	printf("poor Marksmanship\n");
	}
if ( chosen_node != NULL ) {
	printf(" Address         : %x\n", chosen_node          );
	printf(" Value           : %d\n", chosen_node->key     );
	printf(" NODE->left      : %x\n", chosen_node->left    );
	printf(" NODE->right     : %x\n", chosen_node->right   );
	printf(" NODE->l         : %d\n", chosen_node->l       );
	printf(" NODE->r         : %d\n", chosen_node->r       );
	}
}



struct Node * ADelete()
{
struct TNode *chosen_node;
double x,y;
struct Node *return_node = NULL;
TANGO_PATH path;
TANGO_TRANS trans, trans2, killer;
int he_has_right_child = 0;

printf("\nClick on the node you wish to delete \n");
if ( TANGOinput_coord( &x, &y ) ) {
	chosen_node = CHECKTREEFORHIT( x, y, THeap );
	}
else    {
	printf("poor Marksmanship\n");
	return( NULL );
	}
if ( chosen_node != NULL ) {
	return_node = chosen_node->analog;
	if ( chosen_node == THeap ) 
		APop( &THeap, THeap, root_loc);
	else {
		if (chosen_node->right != NULL)
			he_has_right_child = 1;

		if (chosen_node->dad->left == chosen_node ) {
			chosen_node->dad->left = chosen_node->right; 
			if ( ! he_has_right_child )
				delete_image( chosen_node->dad->LLI);
			}
		else {	
			chosen_node->dad->right = chosen_node->right;
			if ( ! he_has_right_child )
				delete_image( chosen_node->dad->RLI );
			}
		update_node_width( chosen_node->dad );
		if ( he_has_right_child ) {
			chosen_node->right->dad = chosen_node->dad;
			delete_image( chosen_node->RLI );
			path = TANGOpath_distance( 
				TANGOimage_loc(chosen_node->right->image, 
					TANGO_PART_TYPE_NW),
				TANGOimage_loc(chosen_node->image, TANGO_PART_TYPE_NW),
				DISTANCE );
			trans = TRANSSUBTREE( DUMMY_TRANS, path, chosen_node->right);
			TANGOpath_free( 1, path );
			}
		chosen_node->right = NULL;
		update_node_width( chosen_node );
		path = TANGOpath_distance(TANGOimage_loc(chosen_node->image, TANGO_PART_TYPE_NW),				new_loc, DISTANCE );
		trans2 = TRANSSUBTREE( DUMMY_TRANS, path, chosen_node);
		if ( he_has_right_child ) {
			killer = trans2;
			trans2 = TANGOtrans_compose( 2, trans, trans2 );
			TANGOtrans_free( 1, killer);
			}
		TANGOtrans_perform(trans2);
		TANGOtrans_free( 1, trans2 );
		
		APop( &chosen_node, chosen_node, new_loc );
		Tcomp_link( &THeap, THeap, chosen_node);
		}
	}
else {
	printf("You missed all the nodes\n");
	}

return (return_node);
}


void ABounds()
{
	int i;
	double xmin, xmax, incr; 

	GETEXTREMEXVALS( &xmin, &xmax, THeap );

	if ( ( xmin < SCREEN_X_LO ) || ( xmax > SCREEN_X_HI ) ) {
		incr = .4 / 16;
		for ( i = 0; 
				i < 16; 
				i++, SCREEN_X_LO -= incr, SCREEN_X_HI += incr, SCREEN_Y_LO -= incr, SCREEN_Y_HI += incr 
				)
			TANGOset_coord( SCREEN_X_LO, SCREEN_Y_HI, SCREEN_X_HI, SCREEN_Y_LO );
		}
	else {
		if ( ( SCREEN_X_HI > 1.0 ) &&
			  ( SCREEN_X_LO < 0.0 ) &&
			  ( xmin        > 0.0 ) &&
			  ( xmax        < 1.0 )    )  {

			incr = ( SCREEN_X_HI - 1.0 ) /16 ;
			for ( i = 0; 
					i < 16; 
					i++, SCREEN_X_LO += incr, SCREEN_X_HI -= incr, SCREEN_Y_LO += incr, SCREEN_Y_HI -= incr 
					)
				TANGOset_coord( SCREEN_X_LO, SCREEN_Y_HI, SCREEN_X_HI, SCREEN_Y_LO );
			}
		}
}


/*
	 it allocates a node & gives it initial values.
	 comp_link(heap, new) is called to fuse new node into the old tree
*/

void AAdd( other_node_pointer, val)
struct Node *other_node_pointer; 
int val;
{
TANGO_PATH path;
TANGO_TRANS trans;
struct TNode *New;
char str[5]; 
double newx, newy, xmin = .4, xmax = .4;

if ( (New = (struct TNode *) malloc(sizeof(struct TNode))) == NULL ) {
	fprintf(stderr, "Memory allocation error\n");
	exit(0);
	}
New->analog = other_node_pointer; 
New->key = val;
New->dad = NULL;
New->left = NULL;
New->right = NULL;
New->level = New->levels_below = 0;
New->pos =    TANGOloc_modify (new_loc, 0.0, 0.0);  /* appear in new spot */
New->l = New->r = 0; 
New->image = TANGOimage_create (TANGO_IMAGE_TYPE_RECTANGLE,
			 TANGOloc_X (new_loc),
			 TANGOloc_Y (new_loc), 1,
			 TANGO_COLOR_YELLOW, WIDTH, HEIGHT, ONE);
newx = TANGOloc_X(TANGOimage_loc(New->image, TANGO_PART_TYPE_C));
newy = TANGOloc_Y(TANGOimage_loc(New->image, TANGO_PART_TYPE_C));
sprintf(str,"%d",New->key);
New->vim= TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,newx,newy,1,TANGO_COLOR_BLACK,
                                     "variable",str,1);
if (THeap == NULL) {
	THeap = New;
	path = TANGOpath_distance (New->pos, root_loc, DISTANCE);
	trans = TRANSSUBTREE (DUMMY_TRANS, path, New);
        TANGOtrans_perform(trans);
	New->pos = root_loc;
	}
else {
	Tcomp_link(&THeap, THeap, New);
	}
}




/*
do_weighting() assigns to each node its depth, and
how far to the left and right the node's subtree extends
*/

int do_weighting(this, level)
struct TNode *this;
int level;
{
this->level = level;

if (this->left != NULL)  do_weighting(this->left, level + 1);
if (this->right != NULL)  do_weighting(this->right, level + 1);
update_node_width(this);  
POLE[level].sum += this->l + this->r + 1;
}     /* end of do_weighting */




/*
metrics assigns x positions offsets to each node
lorr  means the node is a left (1) or right (0) child

all this routine does is pass through the heap and measure it
measurements are stored in each node and on the POLE
info from the pole is used to calculate XDX and YDX in layout
*/



void new_pos(this, lorr, parentsx, ppos)
struct TNode *this;
int lorr, parentsx;
TANGO_LOC ppos;       /* parent's location object (not image location) */
{
double x,y;
if (this != NULL) {                        /* if we aren't called w/ null */
	if ((this->l == 0) && (this->r == 0)) {  /* if node is a leaf */
		if (lorr) {     /* if parent's left child */
                        x = TANGOloc_X(ppos) -XDX;
                        y = TANGOloc_Y(ppos) +YDX;
			this->pos = TANGOloc_create(x,y);
			}
		else {
                        x = TANGOloc_X(ppos) +XDX;
                        y = TANGOloc_Y(ppos) +YDX;
			this->pos = TANGOloc_create(x,y);
			}
		}
	else {
	     if (lorr) {     /* if parent's left child */
                x = TANGOloc_X(ppos) - this->x * XDX; 
                y = TANGOloc_Y(ppos) + YDX;
		this->pos = TANGOloc_create(x,y);
		}
             else {
                x = this->x * XDX + TANGOloc_X(ppos);
                y = TANGOloc_Y(ppos) + YDX;
                }
            }
            
	if (this->left != NULL)  new_pos(this->left,  1, this->x, this->pos);
	if (this->right != NULL) new_pos(this->right, 0, this->x, this->pos);
	}   /* if we aren't passed in with a null heap */
}   /* end of new_pos */



void reset_pole()
{
int i;
for (i=0; i<MAXLEVELS; i++) {
	POLE[i].sum = POLE[i].nodes = POLE[i].pos = 0;
	POLE[i].level = i;
	}
}



/*
	 Tcomp_link is always going to operate on two subtrees which
	 have roots on the same horizontal line.
	 Unless later changed, this plane should be the same line
	 that new nodes and the root node exist on.  the linked
	 tree will have its root in the location that is the same as
	 the left node's image location.
*/

void Tcomp_link(PARENT, LEFT, RIGHT)
struct TNode **PARENT, *LEFT, *RIGHT;
{
TANGO_PATH path, path2, flash_path, killer_path;
TANGO_LOC  target, toloc;
TANGO_TRANS trans, trans2, killer;
struct TNode *p_tmp_node;
int spacing, i ;
float sx, sy;

if (RIGHT == NULL) return;

flash_path = TANGOpath_concatenate( 5, RED_PATH, GREEN_PATH, RED_PATH, GREEN_PATH, RED_PATH);
trans  = TANGOtrans_create( TANGO_TRANS_TYPE_COLOR,  LEFT->image, flash_path );
trans2 = TANGOtrans_create( TANGO_TRANS_TYPE_COLOR, RIGHT->image, flash_path );
killer = trans;
trans  = TANGOtrans_compose( 2, trans, trans2 );
TANGOtrans_free( 1, killer );
killer = trans;
trans  = TANGOtrans_iterate( trans, 3 );
TANGOtrans_perform( trans );
TANGOtrans_free( 2, killer, trans );

if (LEFT->key <= RIGHT->key ) {      /* make left node root of new tree */
	RIGHT->dad = LEFT;            /* RIGHT is now L child of LEFT */
	if (LEFT->left != NULL) {
		delete_image( LEFT->LLI );
		LEFT->left->dad = RIGHT;

		path = TANGOpath_copy( DOWN_PATH) ;
		for ( i = 0 ; i <= RIGHT->levels_below ; i++ ) {
			killer_path = path;
			path = TANGOpath_compose( 2, path, DOWN_PATH );
			TANGOpath_free( 1, killer_path );
			}

		trans = TRANSSUBTREE(DUMMY_TRANS, path, LEFT->left);
		TANGOtrans_perform(trans);
		TANGOtrans_free(1, trans);
		}
        spacing =  1 ;
	target = TANGOloc_create(
		(TANGOloc_X (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW)) - spacing  * XDX),
		(TANGOloc_Y (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW)) + YDX  ));
	path = TANGOpath_distance (
		TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_NW),
		target,
		DISTANCE);

	trans = TRANSSUBTREE ( DUMMY_TRANS, path, RIGHT);

	sx = TANGOloc_X(target) - TANGOloc_X(TANGOimage_loc(LEFT->image,
							TANGO_PART_TYPE_SW)) + WIDTH;
	sy = TANGOloc_Y(target) - TANGOloc_Y(TANGOimage_loc(LEFT->image,
                                                         TANGO_PART_TYPE_SW));
	LEFT->LLI   = TANGOimage_create (TANGO_IMAGE_TYPE_LINE, 
				TANGOloc_X (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_SW)),
				TANGOloc_Y (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_SW)), 1,
				TANGO_COLOR_BLACK, sx, sy, 0.0, ONE, 0);
	TANGOtrans_perform(trans);
	TANGOtrans_free( 1, trans );
	trans = TANGOtrans_create( TANGO_TRANS_TYPE_COLOR, RIGHT->image, GREEN_PATH);
	TANGOtrans_perform(trans);
	TANGOtrans_free( 1, trans );
	p_tmp_node = LEFT->left;

	if (p_tmp_node != NULL ) {
                spacing = RIGHT->r + 1 + p_tmp_node->l;
		target = TANGOloc_create(
			(TANGOloc_X (TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_NW)) + spacing * XDX),
			(TANGOloc_Y (TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_NW)) + YDX ) );
		sx = TANGOloc_X(target) - TANGOloc_X(TANGOimage_loc(RIGHT->image,
                                                                    TANGO_PART_TYPE_NW)) - WIDTH;
		sy = TANGOloc_Y(target) - TANGOloc_Y(TANGOimage_loc(RIGHT->image,
                                                                    TANGO_PART_TYPE_NW)) - HEIGHT;
		RIGHT->RLI   = TANGOimage_create (TANGO_IMAGE_TYPE_LINE, 
                         TANGOloc_X (TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_SE)),
			 TANGOloc_Y (TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_SE)), 1,
			 TANGO_COLOR_BLACK, sx, sy, 0.0, ONE, 0);
		path = TANGOpath_distance( TANGOimage_loc( LEFT->left->image, TANGO_PART_TYPE_NW ), 
					   target, DISTANCE );
		trans = TRANSSUBTREE( DUMMY_TRANS, path, LEFT->left); 
		TANGOtrans_perform(trans);
		TANGOtrans_free(1, trans);
		}
	RIGHT->right = LEFT->left;
	LEFT->left = NULL;
	update_node_width(RIGHT); 

	LEFT->left = RIGHT;
	LEFT->left->dad = LEFT; 
	update_node_width(LEFT); 

	*PARENT = LEFT;                /* don't need to change LEFT->dad */
	trans = TANGOtrans_create( TANGO_TRANS_TYPE_COLOR, LEFT->image, GREEN_PATH);
	TANGOtrans_perform(trans);
	TANGOtrans_free( 1, trans );
	}  /* primary if which separates to two types of linking */
else {                            /* make the right node root of new tree */
	RIGHT->dad = LEFT->dad;        /* RIGHT node gets LEFT'S parent */
	LEFT->dad = RIGHT;
	if (RIGHT->left != NULL) {
		delete_image(RIGHT->LLI); 
		RIGHT->left->dad = LEFT;
		spacing = LEFT->r + RIGHT->left->l + 1;

		target = TANGOloc_create(
			(TANGOloc_X (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW)) + spacing * XDX),
			(TANGOloc_Y (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW)) + YDX ));
		path = TANGOpath_distance (
			TANGOimage_loc(RIGHT->left->image, TANGO_PART_TYPE_NW),
			target,
			DISTANCE);
		sx = TANGOloc_X(target) - TANGOloc_X(TANGOimage_loc(LEFT->image,
                                                                    TANGO_PART_TYPE_SE));
		sy = TANGOloc_Y(target) - TANGOloc_Y(TANGOimage_loc(LEFT->image,
                                                                    TANGO_PART_TYPE_SE));
		LEFT->RLI   = TANGOimage_create (TANGO_IMAGE_TYPE_LINE, 
		TANGOloc_X (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_SE)),
		TANGOloc_Y (TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_SE)), 1,
		TANGO_COLOR_BLACK, sx, sy, 0.0, ONE, 0);

		trans = TRANSSUBTREE ( DUMMY_TRANS, path, RIGHT->left ); 
		TANGOtrans_perform(trans);
                TANGOpath_free( 1, path );
		TANGOtrans_free( 1, trans );
		}  /* LEFT->right != NULL */  

	LEFT->right = RIGHT->left;     /* L->r assumed not to be used */
	RIGHT->left = NULL; 
	update_node_width(LEFT); 
        spacing = 1;

	target = TANGOloc_create(
		(TANGOloc_X (TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_NW)) - XDX),
		(TANGOloc_Y (TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_NW)) + YDX));
	/* move left subtree in circle to be left child of right */
	path = TANGOpath_example( TANGOimage_loc( LEFT->image, TANGO_PART_TYPE_NW ),
				  target, 
				  EXAMPLE_PATH );

	trans = TRANSSUBTREE(DUMMY_TRANS, path, LEFT);
        
	path2 = TANGOpath_distance(
		TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_NW ), 
		TANGOimage_loc(LEFT->image, TANGO_PART_TYPE_NW ) , DISTANCE );

	TANGOtrans_perform(trans);   /* move LEFT to link into RIGHT */ 
	TANGOtrans_free( 1, trans );
	trans = TANGOtrans_create( TANGO_TRANS_TYPE_COLOR, LEFT->image, GREEN_PATH);
	TANGOtrans_perform(trans);
	TANGOtrans_free( 1, trans );

	RIGHT->left = LEFT;            /* LEFT is left child of RIGHT */

	sx =  WIDTH - XDX ;  /* LEFT is left of RIGHT  x val */  
	sy =  YDX - HEIGHT; 
	RIGHT->LLI   = TANGOimage_create (TANGO_IMAGE_TYPE_LINE, 
		TANGOloc_X(TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_SW)),
		TANGOloc_Y(TANGOimage_loc(RIGHT->image, TANGO_PART_TYPE_SW)), 1,
		TANGO_COLOR_BLACK, sx, sy, 0.0, ONE, 0);

	trans = TRANSSUBTREE( DUMMY_TRANS, path2, RIGHT);
	TANGOtrans_perform(trans);         /* move RIGHT into position */
	update_node_width(RIGHT); 
	TANGOtrans_free( 1, trans );
	TANGOpath_free( 2, path, path2); 
	*PARENT = RIGHT;               /* Point to the winner */
	trans = TANGOtrans_create( TANGO_TRANS_TYPE_COLOR, RIGHT->image, GREEN_PATH);
	TANGOtrans_perform(trans);
	TANGOtrans_free( 1, trans );
	}


}


void GETEXTREMEXVALS( xmin, xmax, node )
double *xmin, *xmax;
struct TNode *node;
{

double susy;

if ( node == NULL ) 
	return;
else {
	if ( node->left )  {
		GETEXTREMEXVALS( xmin, xmax, node->left  );
		}
	else if ( ( susy = TANGOloc_X( TANGOimage_loc ( node->image, TANGO_PART_TYPE_NW ))) < *xmin )
			*xmin = susy;
 
	if ( node->right ) {
		GETEXTREMEXVALS( xmin, xmax, node->right );
		}
	else if ( ( susy = TANGOloc_X( TANGOimage_loc ( node->image, TANGO_PART_TYPE_NE ))) > *xmax )
		*xmax = susy;
	}
}


/*
	Here I want a procedure which will take a TRANS and a node
	and will concatenate the parameter TRANS with a trans which
	will move the node's image to the location specified by the
	node's pos field and return the composition of the transes
	after doing a preorder traversal of the heap
        ALSO DOES LINKS 
*/


void update_node_width(node)
struct TNode * node;
{
if (node->left == NULL) 
	node->l = node->r = 0;
else {
	node->l = node->left->l + 1; 
	node->r = MAX(node->left->r -1, 0); 
	}
if (node->right != NULL) {
	node->l = MAX( node->l, node->right->l -1); 
	node->r = MAX( node->r, node->right->r +1); 
	}
}


/*
	Here I want a procedure call which will apply a move transition
	to every element of a subtree & compose the transitions so that
	the entire subtree moves at the same time ALONG THE SAME PATH.
*/

TANGO_TRANS
TRANSSUBTREE(trans, path, node)
TANGO_TRANS trans;
TANGO_PATH path;
struct TNode *node;
{
TANGO_TRANS local1, local2, killer; 

if (node != NULL) {
        local1 = TANGOtrans_create(
                TANGO_TRANS_TYPE_MOVE, node->image, path);
        local2 = TANGOtrans_create(
                TANGO_TRANS_TYPE_MOVE, node->vim, path);

        trans = TANGOtrans_compose( 3, trans, local1, local2);
        TANGOtrans_free( 2, local1, local2);
	
	if (node->left != NULL) {
		local1  = TANGOtrans_create (
			TANGO_TRANS_TYPE_MOVE, node->LLI, path);  
                killer = trans;
		trans = TANGOtrans_compose( 2, trans, local1); 
                TANGOtrans_free( 2, local1, killer );
		killer = trans;
		trans = TRANSSUBTREE(trans, path, node->left );
		TANGOtrans_free( 1, killer );
		} 
	if (node->right != NULL) {
		local1  = TANGOtrans_create (
			TANGO_TRANS_TYPE_MOVE, node->RLI, path);  
 		killer = trans;
		trans = TANGOtrans_compose( 2, trans, local1); 
                TANGOtrans_free( 2, local1, killer );
		killer = trans;
		trans = TRANSSUBTREE(trans, path, node->right);
		TANGOtrans_free( 1, killer); 
		}
	}
return (trans);
}


