#include <stdio.h>
#include <math.h>
#include "xtango.h"

#define INFINITY  1000.0
#define UNSEEN	  0
#define FRINGE	  1
#define INTREE	  2

#define TOPS 25

#define SQUARE(A)  ((A) * (A))
#define MIN(A,B)   ((A) < (B) ? (A) : (B))


void ANIMInit(), ANIMVertex(), ANIMEdge(), ANIMGetNode(), ANIMExamine(),
     ANIMNewMin(), ANIMShorter(), ANIMAddFringe(), ANIMAdd();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID,     (FPTR)ANIMInit}},
			    {"Vertex",	  1, {CHAR_PTR, (FPTR)ANIMVertex}},
			    {"Edge",	  1, {CHAR_PTR, (FPTR)ANIMEdge}},
			    {"GetStart",  1, {CHAR_PTR, (FPTR)ANIMGetNode}},
			    {"GetEnd",	  1, {CHAR_PTR, (FPTR)ANIMGetNode}},
			    {"Examine",	  1, {VOID,     (FPTR)ANIMExamine}},
			    {"NewMin",	  1, {VOID,     (FPTR)ANIMNewMin}},
			    {"Shorter",	  1, {VOID,     (FPTR)ANIMShorter}},
			    {"AddFringe", 1, {VOID,     (FPTR)ANIMAddFringe}},
			    {"Add",	  1, {VOID,     (FPTR)ANIMAdd}},
			    {NULL,     NULL,  NULL,           NULL} };
main()
{
   int i,j,n, status[TOPS], from[TOPS], numbeento, next, start,end;
   double loc[TOPS][2],dist[TOPS],edge[TOPS][TOPS],min;
   char *reply;

   TANGOalgoOp(fcn, "BEGIN");
   TANGOalgoOp(fcn, "Init");

   printf("SHORTEST PATH \n");
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

   for (i=0; i<n; ++i)
      for (j=0; j<n; ++j)
	 edge[i][j] = INFINITY;

   printf("Select pairs of vertices for edges.  (Click STOP to stop.)\n");
   for (;;)
      { reply = TANGOalgoOp(fcn, "Edge");
	if (reply && *reply)
	   { sscanf(reply,"%d %d",&i,&j);
	     if (i == -1) break;
	     edge[i][j] = sqrt( SQUARE(loc[i][0]-loc[j][0]) + SQUARE(loc[i][1]-loc[j][1]) );
	     edge[j][i] = edge[i][j];	/* undirected for now */
	   }
      }

   for (i=0; i<n; ++i)
      { status[i] = UNSEEN;
	dist[i] = INFINITY;
	from[i] = 0;
      }

   printf("Select the start vertex\n");
   reply = TANGOalgoOp(fcn, "GetStart");
   sscanf(reply,"%d",&start);

   printf("Select the end vertex\n");
   reply = TANGOalgoOp(fcn, "GetEnd");
   sscanf(reply,"%d",&end);

   status[start] = INTREE;
   dist[start] = 0.0;
   from[start] = start;
   numbeento = 1;
   TANGOalgoOp(fcn, "AddFringe",start,start);
   TANGOalgoOp(fcn, "Add",start);

   for (i=0; i<n; ++i)
      if ((status[i] == UNSEEN) && (edge[start][i] != INFINITY))
	 { status[i] = FRINGE;
	   from[i] = start;
	   dist[i] = edge[start][i];
	   TANGOalgoOp(fcn, "AddFringe",start,i);
	 }

   while (numbeento < n)
      { min = INFINITY;
	for (i=0; i<n; ++i)
	   if (status[i] == FRINGE)
	      { TANGOalgoOp(fcn, "Examine",from[i],i);
		if (dist[i] < min)
		   { TANGOalgoOp(fcn, "NewMin",from[i],i);
		     min  = dist[i];
		     next = i;
		   }
	      }
	TANGOalgoOp(fcn, "Add",next);
	status[next] = INTREE;
	numbeento++;
	if (next == end)
	   break;

	for (i=0; i<n; ++i)
	   if ((status[i] == UNSEEN) && (edge[next][i] != INFINITY))
	      { TANGOalgoOp(fcn, "AddFringe",next,i);
		status[i] = FRINGE;
		from[i] = next;
		dist[i] = dist[next] + edge[next][i];
	      }
	   else if (status[i] == FRINGE)
	      { if (dist[next]+edge[next][i] < dist[i])  /* better way of */
		   { dist[i] = dist[next]+edge[next][i];  /* getting there */
		     from[i] = next;
		     TANGOalgoOp(fcn, "Shorter",next,i);
		   }
	      }
      }
   TANGOalgoOp(fcn, "END");
}
