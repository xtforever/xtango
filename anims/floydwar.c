/*   Implementation of Floyd-Warshall Algorithm */
/*    Heather Pritchett */

#define INF 10000
#include <stdio.h>
#include "xtango.h"

/*
 * Xtango animation functions 
 */

void init_verts(),add_weights(),new_route(),travel();
 
static NAME_FUNCT fnc[] = { {"init_verts",  1, {VOID,     (FPTR)init_verts}},
                            {"add_weights", 1, {VOID,     (FPTR)add_weights}},
                            {"new_route", 1, {VOID,     (FPTR)new_route}},
                            {"travel", 1, {VOID,     (FPTR)travel}},
                            {NULL,     0, NULL,     NULL} };

/*
 *	global variables used by program	
 */

int *NextNode;  /* This is where the algorithm stores the path
		 * NextNode[x][y] contains the number of the next to last 
		 * node along the shortest path from x to y. 
                 */

int *LeastCost; /* this the is cost of the least cost path from 1
		 * to n, it is initialized as the basic cost of the network 
		 * from node to node. If and nodes are not directly touching,
	 	 * the value is infinity, if they do touch, the value is the 
		 * cost of the node. 
                 */
int SIZE;

int indx(X,Y) int X, Y; {      /* allows my to make a single array  
                                * doublely accessable 
                                */
	return((X*SIZE) + Y);  
	}

main() {

	int N,i,j,k,NewRoute,z,from,to;

/* initializes the NextNode and the LeastCost matrices, by copying 
 *  the  RealCost matrix into them 
 */

        printf("Floyd-Warshall algorithm for shortest paths\n\n");
	printf("\n\nI hope you ran this using the input file: floydwar.in \n");
	printf("            floydwar < floydwar.in\n");

/* call to initialize the TANGO package 
 */

	TANGOalgoOp(fnc, "BEGIN");

/*
 * read in the variables (from a file, screen-input doesn't work)
 */
	scanf("%d",&N);

	printf(" Number of vertices: %d\n",N);
	SIZE = N;
/*
 * draw the vertices on the screen 
 */

	TANGOalgoOp(fnc, "init_verts");
/*
 * set aside the space for the two matrices 
 */

	NextNode = (int*) calloc(N*N,sizeof(int));

	LeastCost = (int*) calloc(N*N,sizeof(int));
/*
 * initialize them to 0 for row = col, and infinity elsewhere 
 */

	for ( i = 0 ; i < N ; i++ ) {
		for ( j = 0 ; j < N ; j++ ) {
			z = indx(i,j);
			if (i == j)
				NextNode[z] = 0;
			else
				NextNode[z] = INF;
			LeastCost[z] = NextNode[z];
			}
		}
/*
 * Now we need to get the edges and their values 
 */

	scanf("%d%d",&i,&j);
	while (scanf("%d",&k) != EOF) {
		z = indx(i-1,j-1);
/*
 * add them to the screen graph 
 */

		TANGOalgoOp(fnc, "add_weights",i-1,j-1,k);

		LeastCost[z] = k;
		NextNode[z] = k;
		scanf("%d%d",&i,&j);
		}

	printf("The last value entered was: %d\n",k);
/*
 * Traverse the edges exhaustively (this algorithm has 
 * worst case O(n^3) )
 */

		for ( j = 0 ; j < N ; j++ ) {
	for ( i = 0 ; i < N ; i++ ) {
			if (LeastCost[indx(j,i)] < INF && j != i) {
/*
 * We have an path from j to i. In order to be sure it's the shortest route,
 * we have to compare it with the cheapest cost path from 
 */
				for ( k = 0 ; k < N ; k++ ) {
/*
 * Is it shorter to go through i on the j -> k path?
 */
				if (LeastCost[indx(i,k)] < INF && i!=k&& j!=k) {
					NewRoute = LeastCost[indx(j,i)] 
					   	 + LeastCost[indx(i,k)];

					TANGOalgoOp(fnc, "new_route",
						j,k,LeastCost[indx(j,k)],
						i,NewRoute);
					scanf("");

					if (NewRoute < LeastCost[indx(j,k)]) {
/*
 * We found a shorter path! Lets show it with XTango
 */
				 	   	LeastCost[indx(j,k)] = NewRoute;
				    		NextNode[indx(j,k)] = NextNode[indx(i,k)];
				    	} /* end if */
					} /* end if */
					} /* end for */
				} /* end if */
			} /* end for */
		} /* end for */

/*
 * Print out our results to the text screen 
 */
	for ( i = 0 ; i < N ; i++ )
		for ( j = 0 ; j < N ; j++ )
			if (i < j) 
			printf("From node %d to %d the shortest path is of length %d\n",
			i+1,j+1,LeastCost[indx(i,j)]);
	
/*
 * call to end the TANGO package 
 */

	TANGOalgoOp(fnc,"END");

	} /* end main */
