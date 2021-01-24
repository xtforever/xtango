/* Polygon Clipping    by Kevin Haulk  */



#include <stdio.h>
#include "xtango.h"
#include "math.h"
#include "clip.h"

#define TOL 1E-20
#define mag(x) ((x) < 0.0) ? -(x) : (x)


void ANIMInit(),ANIMFreeMark(),ANIMHilite(), ANIMUpdatePoly(), ANIMSetCursor(),
     ANIMHideCursor(), ANIMMoveInIn(), ANIMMoveInOut(), ANIMMoveOutOut(),
     ANIMMoveOutIn();
char *ANIMVertex();

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
                            {"Vertex", 1, {CHAR_PTR, (FPTR)ANIMVertex}},
                            {"FreeMark", 1, {VOID, (FPTR)ANIMFreeMark}},
                            {"Hilite", 1, {VOID, (FPTR)ANIMHilite}},
                            {"Update", 1, {VOID, (FPTR)ANIMUpdatePoly}},
                            {"SetCursor", 1, {VOID, (FPTR)ANIMSetCursor}},
                            {"HideCursor", 1, {VOID, (FPTR)ANIMHideCursor}},
                            {"MoveInIn", 1, {VOID, (FPTR)ANIMMoveInIn}},
                            {"MoveInOut", 1, {VOID, (FPTR)ANIMMoveInOut}},
                            {"MoveOutOut", 1, {VOID, (FPTR)ANIMMoveOutOut}},
                            {"MoveOutIn", 1, {VOID, (FPTR)ANIMMoveOutIn}},
                            {NULL, NULL, NULL, NULL} };

NODE *polygon;     /* The pointer to the polygon list (circular queue) */

double top = .25;        /* The top of the window */
double bottom = .75;     /* The bottom of the window */
double left = .25;       /* The left side of the window */
double right = .75;      /* The right side of the window */





/* ============ MakeQ ============ */

void MakeQ(head)
NODE **head;

/* Makes a queue */

{
(*head) = NULL;
}





/* ============ IsEmtQ ============ */

int IsEmtQ(head)
NODE *head;

/* Checks to see if a queue is empty */

{
return(head == NULL);
}





/* ============ AddQ ============ */

void AddQ(head, thevert)
NODE **head;
VERTEX thevert;

/* Adds a new node to the list (a new vertex to the polygon) */

{
NODE *tempnode;

/* Allocate the new node */
tempnode = (NODE *)malloc(sizeof(NODE));

/* Copy the node information into the node for the list */
tempnode->vert = thevert;

/* Check to see if the list is empty */
if (*head == NULL)
   {
   *head = tempnode;
   tempnode->nextnode = tempnode;
   }
/* Otherwise, do a normal insert */
else
   {
   tempnode->nextnode = (*head)->nextnode;
   (*head)->nextnode = tempnode;
   (*head) = tempnode;
   }
}





/* ============ DeleteQ ============ */

int DeleteQ(head, thevert)
NODE **head;
VERTEX *thevert;
/* Deletes the first node from the queue and returns the values contained
   in the node.  It returns a true (non-zero value) as long as there are
   nodes in the queue.                                                    */
   
{
NODE *tempptr;

/* Checks to make sure the list isn't empty */
if (*head == NULL)
   return(0);
   
/* Set up this temporary pointer */
tempptr = (*head)->nextnode;

/* Get the values of the first node */
(*thevert) = tempptr->vert;

/* Check to see how to delete the node from the list */
if (tempptr->nextnode == tempptr)
   (*head) = NULL;
else
   (*head)->nextnode = tempptr->nextnode;
   
/* Free up the node */
free(tempptr);

return(1);
}





/* ============ LookNode ============ */

void LookNode(anode, thevert)
NODE *anode;
VERTEX *thevert;

/* Gets the value from a node without deleting it */

{
(*thevert) = anode->vert;
}





/* ============ NextNode ============ */

int NextNode(head, current, next)
NODE *head;
NODE *current;
NODE **next;

/* Gives the pointer to the next node in a list when the current pointer
   is given.  0 is returned if the current pointer is at the end of the 
   list, otherwise, a non-zero value is returned.                       */

{
(*next) = current->nextnode;
return(current != head);
}





/* ============ MakeVert ============ */

VERTEX MakeVert(x, y)
double x;
double y;

/* Makes a vertex */

{
VERTEX tempvert;

tempvert.xval = x;
tempvert.yval = y;

return(tempvert);
}





/* ============ horiz_inter ============ */

void horiz_inter(vert1, vert2, x, y)
VERTEX *vert1;
VERTEX *vert2;
double *x;
double *y;

/* Finds the intersection of a horizontal line and the line formed by
   two vertices.                                                      */
   
{
double denom;
double m;

denom = vert2->xval - vert1->xval;
if (fabs(denom) < TOL)
   (*x) = vert1->xval;   
else
   {
   m = (vert2->yval - vert1->yval) / denom;
   (*x) = ((*y) - vert1->yval) / m + vert1->xval;
   }
}





/* ============ vertical_inter ============ */

void vertical_inter(vert1, vert2, x, y)
VERTEX *vert1;
VERTEX *vert2;
double *x;
double *y;

/* Computes the intersection of a vertical line and the line defined by the
   vertices.  No error checking is done */
   
{
double m;

m = (vert2->yval - vert1->yval) / (vert2->xval - vert1->xval);
(*y) = m * ((*x) - vert1->xval) + vert1->yval;

}





/* ============ Comparison functions ============ */
int leftcomp(avertex)
VERTEX avertex;
{
return(avertex.xval >= left);
}

int rightcomp(avertex)
VERTEX avertex;
{
return(avertex.xval <= right);
}

int topcomp(avertex)
VERTEX avertex;
{
return(avertex.yval >= top);
}

int bottomcomp(avertex)
VERTEX avertex;
{
return(avertex.yval <= bottom);
}





/* ============ clip side ============ */

void clip_side(polygon, side)
NODE **polygon;     /* The polygon list */
char side;          /* Current side we're working on */

/* Clip polygon against a side */

{
NODE *temppoly;     /* The new polygon list */
NODE *tempnode;     /* A temporary node     */
NODE *currnode;     /* The current node     */
VERTEX lastvert;    /* The previous vertex  */
VERTEX currvert;    /* The current vertex   */
VERTEX tempvert;    /* A temporary vertex   */
int currinside;     /* Checks to see if current vertex is inside the window */
int lastinside;     /*   "    "   "  "  previous   "   "    "     "    "    */
int (*isinside)();     /* Pointer to the function for checking */
void (*intersect)();     /* Pointer to the function for intersection */
double x;              /* The x value for intersections */
double y;              /* The y value for intersections */

TANGOalgoOp(fcn, "Update", (*polygon));
TANGOalgoOp(fcn, "Hilite", side);

/* Start a new queue */
MakeQ(&temppoly);

/* Add an extra copy of the first vertex to the end of the list */
NextNode(*polygon, *polygon, &tempnode);
LookNode(tempnode, &tempvert);
AddQ(polygon, tempvert);

/* Set up the needed functions */
if (side == 'l') 
   {
   isinside = leftcomp;
   intersect = vertical_inter;
   x = left;
   }
if (side == 'r')
   {
   isinside = rightcomp;
   intersect = vertical_inter;
   x = right;
   }
if (side == 't')
   {
   isinside = topcomp;
   intersect = horiz_inter;
   y = top;
   }
if (side == 'b')
   {
   isinside = bottomcomp;
   intersect = horiz_inter;
   y = bottom;
   }
   
   
/* Go through every vertex and clip polygon... */
DeleteQ(polygon, &lastvert);

TANGOalgoOp(fcn, "SetCursor", lastvert, isinside(lastvert));

while (!IsEmtQ(*polygon))
   {
   NextNode(*polygon, *polygon, &currnode);
   LookNode(currnode, &currvert);
   
   currinside = isinside(currvert);
   lastinside = isinside(lastvert);
   
   if (lastinside)
      {
      if (currinside)
         {
         AddQ(&temppoly, currvert);
         TANGOalgoOp(fcn, "MoveInIn", lastvert, currvert);
         }
      else
         {
         intersect(&currvert, &lastvert, &x, &y);
         tempvert = MakeVert(x, y);
         AddQ(&temppoly, tempvert);
         TANGOalgoOp(fcn, "MoveInOut", lastvert, currvert, x, y);
         }
      }
   else
      if (currinside)
         {
         intersect(&currvert, &lastvert, &x, &y);
         tempvert = MakeVert(x, y);
         AddQ(&temppoly, tempvert);
         AddQ(&temppoly, currvert);
         TANGOalgoOp(fcn, "MoveOutIn", lastvert, currvert, x, y);
         }
      else
         TANGOalgoOp(fcn, "MoveOutOut", lastvert, currvert);
   
   DeleteQ(polygon, &lastvert);
   }
(*polygon) = temppoly;
TANGOalgoOp(fcn, "HideCursor");
}


/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

/* Print out list -- for debugging */
void PrintQ(head)
NODE *head;
{
NODE *tempptr;
int count = 1;

if (head == NULL)
   {
   printf("Head = NULL\n");
   return;
   }

tempptr = head;
do
   {
   tempptr = tempptr->nextnode;
   printf("   Node %d -- %f   %f\n", count++,
              tempptr->vert.xval, tempptr->vert.yval);
   }   while (tempptr != head);
   
printf("\n");
}





/* ============ CreatePoly ============ */

void CreatePoly(polygon)
NODE **polygon;

/* Creates the polygon */

{
char *reply;
double x, y;
VERTEX tempvert;

printf("Enter the vertices with the mouse (in the correct order)...\n");
printf("Click on stop when finished...\n");
for (;;)
   {
   reply = TANGOalgoOp(fcn, "Vertex");
   if (!reply || (*reply == '\0')) break;
   sscanf(reply, "%lf %lf", &x, &y);
   tempvert = MakeVert(x, y);
   AddQ(polygon, tempvert);
   free(reply);
   }
TANGOalgoOp(fcn, "FreeMark");
}





main()
{
VERTEX tempvert;
int i;
static char *side_order = "ltrb";

TANGOalgoOp(fcn, "BEGIN");
TANGOalgoOp(fcn, "Init", left, right, top, bottom);

/* Set up and create the polygon */
MakeQ(&polygon);
CreatePoly(&polygon);

for (i = 0; i < 4; i++)
   if (polygon != NULL)
      clip_side(&polygon, side_order[i]);
   
TANGOalgoOp(fcn, "Update", polygon);
TANGOalgoOp(fcn, "Hilite", 'n');
TANGOalgoOp(fcn, "END");
}
