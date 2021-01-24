/*********************************************************/
/* Author:  Gan, Yee Huat                                */
/*                                                       */
/* This program implements the Graham's scan algorithm.  */
/* This algorithm solves the convex-hull problem by      */
/* maintaining a stack S of candidate points.            */
/* Each point of the input set Q is pushed once onto the */
/* stack, and the points that are not vertices of the    */
/* convex hull are eventually popped from the stack.     */
/* when the algorithm terminates, stack S contains 	 */
/* exactly the vertices of the convex hull.              */
/*                                                       */
/* Please refer to page 898 of the "Introduction to      */
/* Algorithms" by Cormen, Leiserson and Rivest for       */
/* a complete description of the algorithm.              */
/*********************************************************/

#include <stdio.h>
#include <math.h>
#include <xtango.h>

#define TRUE      1
#define FALSE     0
#define MAXPOINTS 100

#define COLLINEAR 0
#define RIGHTTURN 1
#define LEFTTURN  2

#define PI 3.14159267

void ANIMInit(), ANIMDraw(), ANIMFindMin(), ANIMSetMin(),
     ANIMUnSetMin(), ANIMSwap(), ANIMSelectPt(), ANIMCompAngle(),
     ANIMUnSelectPt(), ANIMDrawConvHull();

static NAME_FUNCT fnc[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
                            {"Draw", 1, {VOID, (FPTR)ANIMDraw}},
                            {"FindMin", 1, {VOID, (FPTR)ANIMFindMin}},
                            {"SetMin", 1, {VOID, (FPTR)ANIMSetMin}},
                            {"UnSetMin", 1, {VOID, (FPTR)ANIMUnSetMin}},
                            {"Swap", 1, {VOID, (FPTR)ANIMSwap}},
                            {"SelectPt", 1, {VOID, (FPTR)ANIMSelectPt}},
                            {"CompAngle", 1, {VOID, (FPTR)ANIMCompAngle}},
                            {"UnSelectPt", 1, {VOID, (FPTR)ANIMUnSelectPt}},
                            {"DrawConvHull", 1, {VOID, (FPTR)ANIMDrawConvHull}},
                            {NULL,   0, NULL, NULL}
                          };

typedef struct Point{
    int x;
    int y;
    double angle;
} POINT;

int InputPoints(pt)
POINT pt[];
{
    int i;	

    printf("Enter the x and y coordinates of the points (0->1000)\n");
    printf("To exit, enter -1 -1\n\n");
    for (i = 0;;) {
	printf("Enter x and y: ");
        scanf("%d %d", &pt[i].x, &pt[i].y);
        if (pt[i].x == -1 && pt[i].y == -1)
            break;
        TANGOalgoOp(fnc, "Draw", pt[i].x, pt[i].y, i);
        i++; 
    }
    return i;
}

int FindMin(pt, numpt)
POINT pt[];
int numpt;
{
    int i, minpt;

    minpt = 0;
    TANGOalgoOp(fnc, "SetMin", minpt);
    for (i = 1; i < numpt; i++) {
        TANGOalgoOp(fnc, "FindMin", i, minpt);
	if (pt[i].y < pt[minpt].y) {
            TANGOalgoOp(fnc, "UnSetMin", minpt);
            TANGOalgoOp(fnc, "SetMin", i);
	    minpt = i;
	}
	else if (pt[i].y == pt[minpt].y) {
	    if (pt[i].x < pt[minpt].x) {
                TANGOalgoOp(fnc, "UnSetMin", minpt);
                TANGOalgoOp(fnc, "SetMin", i);
		minpt = i;
            }
        }
    }
    return minpt;
}

void Push(index, Stack, top)
int index, Stack[], *top;
{
    Stack[*top] = index;
    (*top)++;
}

void Pop(Stack, top)
int Stack[], *top;
{
    (*top)--;
}
    
void CalculateAngle(pt, numpt)
POINT pt[];
int numpt;
{
    int i;

    for (i = 1; i < numpt; i++) {
        if (pt[i].x - pt[0].x == 0)
            pt[i].angle = PI/2;
        else {
            pt[i].angle = atan((double)(pt[i].y - pt[0].y) / 
                               (double)(pt[i].x - pt[0].x));
            if (pt[i].angle < 0)
                pt[i].angle += PI;
        }
    }
}

int CrossProduct(p0, p1, p2)
POINT p0, p1, p2;
{
    int product;

    product = ((p1.x - p0.x) * (p2.y - p0.y))
	      - ((p2.x - p0.x) * (p1.y - p0.y));
    product = - product;  
    if (product == 0)
	return (COLLINEAR);
    else if (product < 0)
	return (LEFTTURN);
    else 
	return (RIGHTTURN);
}

int CompareAngle(pt, id1, id2, id3)
POINT pt[];
int id1, id2, id3;
{
    TANGOalgoOp(fnc, "CompAngle", id1, id2, id3);
    return (CrossProduct(pt[id1], pt[id2], pt[id3]));
}

main()
{
    POINT pt[MAXPOINTS], temp; 
    int Stack[MAXPOINTS], numpt, minpt, top, i, j;

    TANGOalgoOp(fnc, "BEGIN");
    TANGOalgoOp(fnc, "Init");

    /* Input the points */
    numpt = InputPoints(pt);
	
    /* Find the point with the minimum y-coordinate, or the
       leftmost such point in case of a tie */
    minpt = FindMin(pt, numpt);
	
    /* Let pt[0] be the minimum point by swapping if necessary */
    if (minpt != 0) {
        TANGOalgoOp(fnc, "Swap", 0, minpt);
        TANGOalgoOp(fnc, "UnSetMin", minpt);
        TANGOalgoOp(fnc, "SetMin", 0);
 	temp = pt[0];
	pt[0] = pt[minpt];
        pt[minpt] = temp;
    }

    /* Compute the angle with respect to p[0] */
    CalculateAngle(pt, numpt);

    /* Sort by polar angle around p[0] */

    for (j = numpt-2; j >= 0; --j)
        for (i = 1; i <= j; ++i)
            if (pt[i].angle > pt[i+1].angle) {
		TANGOalgoOp(fnc, "Swap", i, i+1);
                temp = pt[i];
                pt[i] = pt[i+1];
                pt[i+1] = temp;
            }

    /* Initialize the stack top pointer and push the first 3 
       points into the stack */
    top = 0;
    Push(0, Stack, &top);
    Push(1, Stack, &top);
    Push(2, Stack, &top);

    TANGOalgoOp(fnc, "SelectPt", 0);
    TANGOalgoOp(fnc, "SelectPt", 1);
    TANGOalgoOp(fnc, "SelectPt", 2);

    /* Check the angle formed by the first two points on the stack
       and the current point */
    for (i = 3; i < numpt; i++) {
	while (CompareAngle(pt, Stack[top-2], Stack[top-1], i) 
               != LEFTTURN) {
	    TANGOalgoOp(fnc, "UnSelectPt", Stack[top-1]);
	    Pop(Stack, &top);
	}
	Push(i, Stack, &top);
        TANGOalgoOp(fnc, "SelectPt", Stack[top-1]);
    }

    /* Print out the points in the convex hull */
    printf("\n\nThe convex hull is:\n\n");
    for (i = 0; i < top; i++) {
        if (i < top-1)
            TANGOalgoOp(fnc, "DrawConvHull", Stack[i], Stack[i+1]);
        else
            TANGOalgoOp(fnc, "DrawConvHull", Stack[i], Stack[0]);
	printf("%d: %d %d\n", Stack[i], pt[Stack[i]].x, pt[Stack[i]].y);
    }

    TANGOalgoOp(fnc, "END");
}
