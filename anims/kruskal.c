#include <stdio.h>
#include "stdlib.h"
#include <math.h>
#include "xtango.h"

/*###############################################################################/
+ 
+		MINIMUM SPANNING TREE -- KRUSKAL'S ALGORITHM
+
+ by cheryl krupczak 
+
+ This program executes and animates Kruskal's algorithm for finding a graph's
+ minimum spanning tree.  There are 2 main data structures. Vertices, an array of 
+ pointers to vertex structures, provides linked lists of nodes that are in the
+ same tree.  Trees are unioned by moving pointers.  edges, an array of edge 
+ structures, keeps track of the weight and vertices for each edge.  This array
+ will be sorted by increasing edge weight.  The tree will be built by adding
+ smallest edges first.
/###############################################################################*/

#define MAXV 25
#define MAXE 50

#define SQUARE(A)  ((A) * (A))
#define Getrep(x)  vertices[x]->rep->vertnum

struct vertex {
	int	vertnum;
	struct vertex	*rep;
	struct vertex	*next;
};

struct edge {
	double	weight;
	int 	vert1;
	int 	vert2;
};

int  compare();

void ANIMInit(), ANIMVertex(), ANIMEdge(), ANIMShorter(); 

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID,     (FPTR)ANIMInit}},
			    {"Vertex",	  1, {CHAR_PTR, (FPTR)ANIMVertex}},
			    {"Edge",	  1, {CHAR_PTR, (FPTR)ANIMEdge}},
			    {"Shorter",	  1, {VOID,     (FPTR)ANIMShorter}},
			    {NULL,     NULL,  NULL,           NULL} };
main()
{
	double	loc[MAXV][2];
	char	*reply;
	struct vertex	*vertices[MAXV];
	struct vertex	*vertptr, *newrep, *tail;
	struct edge	edges[MAXE];
	int 	i, v1, v2, numverts = 0, numedges = 0;


	TANGOalgoOp(fcn, "BEGIN");
	TANGOalgoOp(fcn, "Init");

	printf("MINIMUM SPANNING TREE: Kruskal's algorithm\n");
	printf("Interactively draw the graph with the mouse.\n");
	printf("Click to select a vertex position (click in STOP to stop)\n");
	for (;;) {
		reply = TANGOalgoOp(fcn, "Vertex",numverts);
		if (!reply || (*reply=='\0')) break;
		sscanf(reply,"%lf %lf",&(loc[numverts][0]),&(loc[numverts][1]));
		numverts++;
		free(reply);
	}

	for (i=0; i<numverts; i++) {
		vertices[i] = (struct vertex *)malloc(sizeof(struct vertex));
		vertptr = vertices[i];
		vertptr->vertnum = i;
		vertptr->rep = vertptr;
		vertptr->next = NULL;
	}

	printf("Select pairs of vertices for edges.  (Click STOP to stop.)\n");
	for (;;) {
 		reply = TANGOalgoOp(fcn, "Edge");
		if (reply && *reply) {
			sscanf(reply,"%d %d",&v1,&v2);
			if (v1 == -1) break;
			edges[numedges].weight = sqrt( SQUARE(loc[v1][0]-loc[v2][0]) 
									 + SQUARE(loc[v1][1]-loc[v2][1]) );
			edges[numedges].vert1 = v1;
			edges[numedges].vert2 = v2;
			numedges++;
		}
    }

	qsort((char *)edges,numedges,sizeof(struct edge),compare);

	for ( i=0; i < numedges; i++) {
		v1 = edges[i].vert1;
		v2 = edges[i].vert2;
		if ( Getrep(v1) != Getrep(v2) ) {
			/* add the edge, union the sets */
			newrep = vertices[v1]->rep;
			tail = vertices[v1];
			while ( tail->next != NULL )
				tail = tail->next;
			
			tail->next = vertices[v2]->rep;
			/* change reps to point to the new rep */
			while ( tail->next != NULL ) {
				tail = tail->next;
				tail->rep = newrep;
			}
			/*** Do graphics to highlight new edge ***/
			TANGOalgoOp(fcn,"Shorter",v1,v2);

		}
	}
printf("thats all folks\n\n");
	TANGOalgoOp(fcn, "END");
return(0);
}


/*-------------------------------------------------------------------------
 * Compare function to be used by the qsort().  The edges will be sorted
 * from lowest weight, to highest weight.
 *-------------------------------------------------------------------------*/
int compare(arg1,arg2)
void	*arg1, *arg2;
{
	double	w1, w2;

	w1 = ((struct edge *)arg1)->weight;
	w2 = ((struct edge *)arg2)->weight;

	if ( w1 < w2 )
		return (-1);
	if ( w1 == w2 )
		return (0);
	if ( w1 > w2 )
		return (1);

}

