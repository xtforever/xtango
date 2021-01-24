#include <stdio.h>
#include "stdlib.h"
#include <math.h>
#include "xtango.h"

/*-------------------------------------------------------------------------

                      TRAVELLING SALESMAN PROBLEM
                            Gena R. Hillhouse

   This version of the travelling salesman problem determines the minimum
   spanning tree for the graph, using Prim's algorithm.  The salesman then
   does a pre-order walk of the MST nodes.

   Many thanks to Cheryl Krupczak for her implementation of Prim's MST
   algorithm, which I used as the basis for my animation.

----------------------------------------------------------------------------*/

#define MAXV 25
#define INQUE 1
#define NOTINQUE 0
#define INFINITY	100
#define DATAFILE	1
#define MOUSE	2

#define TRUE 1
#define FALSE 0

#define SQUARE(A)  ((A) * (A))


typedef struct {
	double	keywt;
	int 	vert1;
	int 	vert2;
} keytype;


void ANIMInit(), ANIMVertex(), ANIMEdge(), ANIMGetNode(), ANIMExamine(),
     ANIMShorter(), ANIMInput_Edge(), ANIMInput_vert(),
     ANIMShowPath(), ANIMClearScreen(), ANIMSalesWalk();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID,     (FPTR)ANIMInit}},
			 {"Vertex",	  1, {CHAR_PTR, (FPTR)ANIMVertex}},
			 {"Edge",	  1, {CHAR_PTR, (FPTR)ANIMEdge}},
			 {"GetStart",  1, {CHAR_PTR, (FPTR)ANIMGetNode}},
			 {"Examine",	  1, {VOID,     (FPTR)ANIMExamine}},
			 {"Shorter",	  1, {VOID,     (FPTR)ANIMShorter}},
			 {"Input_Edge", 1, {VOID,     (FPTR)ANIMInput_Edge}},
			 {"Input_vert", 1, {VOID,     (FPTR)ANIMInput_vert}},
                        {"ShowPath",   1, {VOID,     (FPTR)ANIMShowPath}},
                        {"ClearScreen",   1, {VOID,     (FPTR)ANIMClearScreen}},
                        {"SalesWalk",   1, {VOID,     (FPTR)ANIMSalesWalk}},
			    {NULL,     NULL,  NULL,           NULL} };

/* minimum spanning tree structure */
typedef struct _mst
{
  int     node;
  double  cost;
  struct _mst *leftnode;
  struct _mst *rightnode;
} mst;



/* GLOBAL VARIABLES */
double	edge[MAXV][MAXV];
int 	adj[MAXV][MAXV];
int 	status[MAXV];
keytype	*keys[MAXV];		/* the heap.  sorted small to large by "keywt" */
int 	qnodes[MAXV];		/* nodes still in the queue.  Its arrangement
	       			  shadows the heap so the same array pos tells
					   both the key weight and node */

mst     *ms_tree = NULL;


main()
{
	int 	i, v1, v2, pos, root, numverts;
	int 	node2, minnode;	
	int 	Q;		  /* current # elems in the heap */


	TANGOalgoOp(fcn, "BEGIN");
	TANGOalgoOp(fcn, "Init");

	printf("\n\nTRAVELLING SALESMOUSE PROBLEM\n\n\n");
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
                {
                   AddMST (&ms_tree, v1, v2);
                   TANGOalgoOp(fcn,"Shorter",v1,v2);
                }
		for ( node2 = 1; node2 <= numverts; node2++) {
                   if ((adj[minnode][node2]) && (status[node2] == INQUE) ) {
                       pos = findinheap(node2,Q);
		       if ( edge[minnode][node2] < keys[pos]->keywt ) {
				reducekey(minnode,node2,pos);
				TANGOalgoOp(fcn,"Examine",minnode,node2);

				}
			}
		}
	}

/*--------------------------------------------------*/
         printf ("\n\nHere's the minimum spanning tree for the graph.\n\n");
         TANGOalgoOp (fcn, "ClearScreen", ms_tree->node);

         DisplayMST (ms_tree);

{ int tmp = 0;
         printf ("\n\n.... Now the salesman makes his rounds\n\n");

         TreeWalk (ms_tree, &tmp);
         TANGOalgoOp(fcn, "SalesWalk", tmp, ms_tree->node);
}

printf ("THE END\n");

/*------------------------------------------------*/
ERREXIT:
	TANGOalgoOp(fcn, "END");
	exit(0);
}

TreeWalk (tree, lastnode)
mst *tree;
int *lastnode;
{
  mst *ptr;

  ptr = tree;
  if (ptr != NULL)
  {
       if (*lastnode != 0)
       {
          TANGOalgoOp(fcn, "SalesWalk", *lastnode, ptr->node);
       }
    *lastnode = ptr->node;

    if (ptr->leftnode != NULL)
      TreeWalk (ptr->leftnode, lastnode);

    if (ptr->rightnode != NULL)
      TreeWalk (ptr->rightnode, lastnode);
  }

}




DisplayMST (tree)
mst *tree;
{ 

  if (tree != NULL)
  {

    if (tree->leftnode != NULL)
    {
       TANGOalgoOp (fcn, "ShowPath", tree->node, tree->leftnode->node);
        DisplayMST(tree->leftnode );
    }


    if (tree->rightnode != NULL)
    {
       TANGOalgoOp (fcn, "ShowPath", tree->node, tree->rightnode->node);
       DisplayMST (tree->rightnode);
    }
  }
}

int SearchTree (root, FindThis, matchnode)
mst *root;
int FindThis;
mst **matchnode;

{
   int search = FALSE; 

  if (root != NULL)
  {
     if (root->node != FindThis)
     {
       if (root->leftnode != NULL)
         search = SearchTree (root->leftnode,FindThis, matchnode);

       if (search == FALSE)
       {
         if (root->rightnode != NULL)
           search = SearchTree (root->rightnode, FindThis, matchnode);
         else 
           return (FALSE);


         if (search != FALSE)
           return (TRUE);
         else return(FALSE);
       }
       else
          return (TRUE);
     }
     else
     {  
        *matchnode = root;
        return (TRUE);
     }
  }
  else 
    return (NULL);
}

AddMST (ms_tree, v1, v2)
mst **ms_tree;
int v1, v2;
{
   mst *treenode;
   mst *ptr;
   int  found;

   ptr = *ms_tree;

   found = SearchTree (*ms_tree, v1, &ptr);

   treenode = (mst *)malloc (sizeof(mst));
   if (treenode != NULL)
   {
/*
      treenode->cost = keys[pos]->keywt;
*/
      treenode->node = v2;
      treenode->leftnode = NULL;
      treenode->rightnode = NULL;
   }
   else printf ("MALLOC FAILED\n");

   if (*ms_tree == NULL)
   {
      *ms_tree  = (mst *)malloc (sizeof(mst));
      if (*ms_tree != NULL)
      {
/*
         (*ms_tree)->cost = keys[pos]->keywt;
*/
         (*ms_tree)->node = v1;
         (*ms_tree)->leftnode = treenode;
         (*ms_tree)->rightnode = NULL;
       }

   }
   else
   if (found == FALSE)
      printf ("DIDN'T FIND PARENT %d IN TREE for node %d\n",  v1, v2);
   else
   {


     if (ptr->leftnode == NULL)
     {
       ptr->leftnode = treenode;
     }
     else
     {
       ptr->rightnode = treenode;
     }
   }
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

	printf("Select the method for entering data:\n");
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
