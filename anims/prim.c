#include <stdio.h>
#include "stdlib.h"
#include <math.h>
#include "xtango.h"

/*###############################################################################
+
+			MINIMUM SPANNING TREE:  PRIM'S ALGORITHM
+
+ by cheryl krupczak
+
+ This program executes and animates Prim's algorithm for finding a graph's minimum
+ spanning tree.  There are 2 main data structures.  Keys, an array of pointers
+ to keytype structures, provides the heap.  It is heapsorted by the structure
+ field, keywt, which represents the distance to a node not in the tree.  The
+ array qnodes contains the node numbers of those nodes not in the tree.  It
+ shadows the actions of the "keys" heap so that we know which node the keywt
+ refers to (they'll have the same position in the array. 
/###############################################################################*/


#define MAXV 25
#define INQUE 1
#define NOTINQUE 0
#define INFINITY	100
#define DATAFILE	1
#define MOUSE	2

#define SQUARE(A)  ((A) * (A))


typedef struct {
	double	keywt;
	int 	vert1;
	int 	vert2;
} keytype;


void ANIMInit(), ANIMVertex(), ANIMEdge(), ANIMGetNode(), ANIMExamine(),
     ANIMShorter(), ANIMInput_Edge(), ANIMInput_vert();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID,     (FPTR)ANIMInit}},
			    {"Vertex",	  1, {CHAR_PTR, (FPTR)ANIMVertex}},
			    {"Edge",	  1, {CHAR_PTR, (FPTR)ANIMEdge}},
			    {"GetStart",  1, {CHAR_PTR, (FPTR)ANIMGetNode}},
			    {"Examine",	  1, {VOID,     (FPTR)ANIMExamine}},
			    {"Shorter",	  1, {VOID,     (FPTR)ANIMShorter}},
			    {"Input_Edge", 1, {VOID,     (FPTR)ANIMInput_Edge}},
			    {"Input_vert", 1, {VOID,     (FPTR)ANIMInput_vert}},
			    {NULL,     NULL,  NULL,           NULL} };


/* GLOBAL VARIABLES */
double	edge[MAXV][MAXV];
int 	adj[MAXV][MAXV];
int 	status[MAXV];
keytype	*keys[MAXV];		/* the heap.  sorted small to large by "keywt" */
int 	qnodes[MAXV];		/* nodes still in the queue.  Its arrangement
							   shadows the heap so the same array pos tells
							   both the key weight and node */


main()
{
	int 	i, v1, v2, pos, root, numverts;
	int 	node2, minnode;	
	int 	Q;		  /* current # elems in the heap */


	TANGOalgoOp(fcn, "BEGIN");
	TANGOalgoOp(fcn, "Init");

	printf("MINIMUM SPANNING TREE: Prim's algorithm\n");
	numverts = GetInput(&root);

	/* heap ordered small to large. */
	keys[0] = (keytype *)malloc(sizeof(keytype));
	keys[0]->keywt = -1;

	/*----------------------------------------------------------------
	 * Initially, all the nodes are in the queue (i.e. they're NOT in the
	 * spanning tree) and the edge weights (keywt) are infinity.
	 *----------------------------------------------------------------*/
	for ( i=1; i <= numverts; i++) {
		status[i] = INQUE;
		qnodes[i] = i;
		if ( (keys[i] = (keytype *)malloc(sizeof(keytype)) )  == NULL ) {
			printf("ERROR: malloc failed.  Not enough space!\n");
			goto ERREXIT;
		}
		keys[i]->keywt = INFINITY;
	}

	/* put the root at the top of the queue and make its weight 0 */
	qnodes[1] = root;
	qnodes[root] = 1;
	keys[1]->keywt = 0;
	keys[1]->vert1 = root;
	keys[1]->vert2 = root;

	Q = numverts;

	/*---- Here's the big step!  ----------*/
	while ( Q > 0 ) {

		/* add the next minimum edge */
		minnode = delmin(&Q,&v1,&v2);
		if (v1 != v2 )
			TANGOalgoOp(fcn,"Shorter",v1,v2);
		for ( node2 = 1; node2 <= numverts; node2++) {
			if ( (adj[minnode][node2]) && (status[node2] == INQUE) ) {
				pos = findinheap(node2,Q);
				if ( edge[minnode][node2] < keys[pos]->keywt ) {
					reducekey(minnode,node2,pos);
					TANGOalgoOp(fcn,"Examine",minnode,node2);
				}
			}
		}
	}
	printf("\n\n** That's all folks! **\n");
ERREXIT:
	TANGOalgoOp(fcn, "END");
	exit(0);
}


/**********************************************************************************
+ NAME:			GetInput
+ RETURNS:		numvertices
+ DESCRIPTION
+	This routine reads in the information for building the graph.  The info
+ may be read in from a pre-existing data file, or it may be entered interactively
+ using the mouse to draw the graph on the XTANGO screen.
+
+**********************************************************************************/
GetInput(start)
int 	*start;
{
	double	loc[MAXV][2];
	int 	i, j, nverts, method = 0;
	char	*reply;
	int 	v1, v2;

	printf("Select the method for inputting data:\n");
	printf("\tprepared data file (enter 1)\n");
	printf("\tinteractive using mouse (enter 2)\n");
	while ( (method != DATAFILE) && (method != MOUSE) )
		scanf("%d",&method);

	if ( method == DATAFILE ) {
		printf("How many vertices?\n");
		scanf("%d",&nverts);
		printf("Enter the locations\n");
		for (i=1; i<= nverts; ++i) {
			scanf("%lf %lf",&loc[i][0],&loc[i][1]);
			TANGOalgoOp(fcn, "Input_vert",i,loc[i][0],loc[i][1]);
		}

		printf("Enter the adjacency matrix\n");
		for (i=1; i<=nverts; ++i)
			for (j=1; j<=nverts; ++j) {
				scanf("%d",&adj[i][j]);
				if ( (adj[i][j]) && (i<j) ) {
					TANGOalgoOp(fcn, "Input_Edge",i,j);
					edge[i][j] = sqrt( SQUARE(loc[i][0]-loc[j][0]) + SQUARE(loc[i][1]-loc[j][1]) );
					edge[j][i] = edge[i][j];	/*undirected graph */
				}
			 }
/*
TANGOalgoOp(fcn, "Display",loc,nverts,adj);
*/

		printf("Select the START node\n");
		scanf("%d",start);
		return(nverts);
	}

	/*--------------------------------------------------------------
	 * Inputs the vertices using the mouse on the screen
	 *--------------------------------------------------------------*/
	printf("Interactively draw the graph with the mouse.\n");
	printf("Click to select a vertex position (click in STOP to stop)\n");

	nverts = 1;		/* number nodes starting with 1 */
	for (;;) {
		reply = TANGOalgoOp(fcn, "Vertex",nverts);
		if (!reply || (*reply=='\0')) {
			nverts--;
			break;
		}
		sscanf(reply,"%lf %lf",&(loc[nverts][0]),&(loc[nverts][1]));
		nverts++;
		free(reply);
	}

	printf("Select pairs of vertices for edges.  (Click STOP to stop.)\n");
	/*--------------------------------------------------------------
	 * Inputs the edges using the mouse on the screen
	 *--------------------------------------------------------------*/
	for (;;) {
 		reply = TANGOalgoOp(fcn, "Edge");
		if (reply && *reply) {
			sscanf(reply,"%d %d",&v1,&v2);
			if (v1 == -1) break;
			adj[v1][v2] = 1;
			adj[v2][v1] = 1;	/* undirected graph */
			edge[v1][v2] = sqrt( SQUARE(loc[v1][0]-loc[v2][0]) 
									 + SQUARE(loc[v1][1]-loc[v2][1]) );
			edge[v2][v1] = edge[v1][v2];
		}
    }

	printf("Select the start vertex\n");
	reply = TANGOalgoOp(fcn, "GetStart");
	sscanf(reply,"%d",start);
	return(nverts);

}

/************************************************************************
+ NAME:	downheap(pos,heaplen)
+ DESCRIPTION
+	changes the heap by filtering the value at position "pos" in the
+   heap down to its proper location in the heap.  This heap is ordered
+	low to high.
+	NOTE: the qnodes array must shadow the heap
+
+************************************************************************/

downheap(pos,heaplen)
int 	pos, heaplen;
{
	int 	j, smaller;
    int 	nval;	    /* val for node array (shadow array) */
	keytype *kt;

	kt = keys[pos];
	nval = qnodes[pos];
	while ( pos <= heaplen/2 ) {
		j = pos*2;
		if (j > heaplen) break;
		if (j < heaplen) 
			smaller = ( keys[j]->keywt <= keys[j+1]->keywt ? j : j+1 );

		else
			smaller = j;
		if ( kt->keywt <= keys[smaller]->keywt ) 
			break;
		keys[pos] = keys[smaller];
		qnodes[pos] = qnodes[smaller];	/* shadow! */
		pos = smaller;
	}
	keys[pos] = kt;
	qnodes[pos] = nval;

}

/************************************************************************
+ NAME:	upheap(heaplen)
+ DESCRIPTION
+	Checks if a child value is greater than his parent.  If so, they
+   exchange places and the check continues up the tree 
+	NOTE: the qnodes array must shadow the heap
+
+************************************************************************/
upheap(k)
int 	k;
{
	int 	nval;
	keytype *kt;


	kt = keys[k];
	nval = qnodes[k];

	while ( keys[k/2]->keywt > kt->keywt ) {
		keys[k] = keys[k/2];
		qnodes[k] = qnodes[k/2];
		k = k/2;
	}
	keys[k] = kt;
	qnodes[k] = nval;

}

/************************************************************************
+ NAME:			findinheap(node,heaplen)
+ DESCRIPTION
+	Finds the location in the heap that corresponds to node "node"	
+
+************************************************************************/
findinheap(node,heaplen)
int 	node, heaplen;
{
	int 	j;

	for ( j=1; j<=heaplen; j++ ) {
		if ( qnodes[j] == node ) 
			return(j);
	}
}

/************************************************************************
+ NAME:			reducekey(n1,n2,pos)
+ DESCRIPTION
+	This routine reduces the keywt to its new value and then re-heaps
+   the heap.
+************************************************************************/
reducekey(n1,n2,pos)
int 	n1,n2,pos;
{

	keys[pos]->keywt = edge[n1][n2];
	/* record the vertices so we can draw the edge later */
	keys[pos]->vert1 = n1;
	keys[pos]->vert2 = n2;
	/* re-heap the heap */
	upheap(pos);
}


/************************************************************************
+ NAME:			delmin(elems)
+ DESCRIPTION
+  This routine takes off the top element in the heap (it will always be
+  the smallest value) and then moves the last element in the heap to 
+  the top of the heap, and then re-heaps the tree.  Notice that the
+  number of elements in the heap is decreased by one!
+  Since we need to draw the edge, also return its endpoints.
+
+************************************************************************/
delmin(elems,pt1,pt2)
int	*elems, *pt1, *pt2;
{
	int 	minnode;

	minnode = qnodes[1];
	status[minnode] = NOTINQUE;
	*pt1 = keys[1]->vert1;
	*pt2 = keys[1]->vert2;
	keys[1] = keys[*elems];
	qnodes[1] = qnodes[*elems]; /* must shadow heaps movement */
keys[*elems] = keys[0];
	*elems = *elems - 1;
	/* re-heap the tree starting at the top */
	downheap(1,*elems);
	return(minnode);
}
