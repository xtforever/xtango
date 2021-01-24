/* this program implements the graph coloring algorithm */

#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMVertex(), ANIMEdge(), ANIMCG_Color();
static NAME_FUNCT fcn[] = { {"Init",     1, {VOID,     (FPTR)ANIMInit}}, 
			    {"Vertex",   1, {CHAR_PTR, (FPTR)ANIMVertex}},
			    {"Edge",     1, {CHAR_PTR, (FPTR)ANIMEdge}},
			    {"CG_Color", 1, {VOID,     (FPTR)ANIMCG_Color}},
			    {NULL,    NULL, NULL,            NULL} };

#define N 20 	      /* max number of nodes in the graph */

int adj[N][N];   /* need to initialize the adjacency matrix */
int color[N];        /* initially none of them  colored */
int n;            /* number of vertices in graph */
int numcolors=6;   /* number of colors available for coloring */

void
nextvalue(node)
   int node;
{
   int j;

   do {
      color[node] = (color[node] + 1) % (numcolors + 1);
      TANGOalgoOp(fcn, "CG_Color",node,color[node]);
      if (color[node] == 0)
         return;
      for (j=0; j<n; ++j)
         if (adj[node][j] && (color[node] == color[j]))
            break;
      if (j==n)
         return;
    } while (1==1);
 }


int
coloring(node)
   int node;
{
   int i;

   do {
      nextvalue(node);
      if (color[node] == 0)
         return(0);
      else
        {
          if (node == (n-1))
	    { printf("A coloring is possible!\nNode: \n");
              for (i=0; i<n; ++i)
                 printf("%d  ",i);
              printf("\nColor:\n");
              for (i=0; i<n; ++i)
                 printf("%d  ",color[i]);
              printf("\n");
              return(1);
            }
         else
            if (coloring(node+1))
               return(1);
	}
    } while (node != 0);
   return(0);
 }





main()
{
   int i,j;
   double loc[N][N];
   char *reply;

   for (i=0; i<N; ++i)
     color[i] = 0;
   TANGOalgoOp(fcn, "BEGIN");
   TANGOalgoOp(fcn, "Init");

   printf("Graph colorability \n");

   printf("How many colors should be used? (2-6 allowable)\n");
   scanf("%d",&numcolors);

   printf("Interactively draw the graph with the mouse.\n");
   printf("Click to select a vertex position (click in STOP to stop)\n");
   n=0;
   for (;;)
      { reply = TANGOalgoOp(fcn, "Vertex",n);
	if (!reply || (*reply=='\0')) break;
	sscanf(reply,"%lf %lf",&(loc[n][0]),&(loc[n][1]));
	n++;
	free(reply);
      }

   for (i=0; i<N; ++i)
      for (j=0; j<N; ++j)
	 adj[i][j] = 0;

   printf("Select pairs of vertices for edges.  (Click STOP to stop.)\n");
   for (;;)
      { reply = TANGOalgoOp(fcn, "Edge");
	if (reply && *reply)
	   { sscanf(reply,"%d %d",&i,&j);
	     if (i == -1) break;
	     adj[j][i] = adj[i][j] = 1;	/* there's an edge there */
	   }
      }
  if(coloring(0) == 0)
    { TANGOalgoOp(fcn, "CG_Color",0,0);
      printf("coloring of the graph not possible\n");
    }
 TANGOalgoOp(fcn, "END");
 }
