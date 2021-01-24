/*======================================================================*/
/*----------------------------------------------------------------------*/
/* sssp.c - Single Source Shortest Paths				*/
/* 	    An XTango animation of Dijkstra's SSSP algorithm		*/
/* 	    by Anthony G. Francis, Jr.					*/
/*----------------------------------------------------------------------*/
/* References:	_Data Structures and Algorithms_, Aho, Hopcroft, and	*/
/*			Ullman. Addison-Wesley 1983.			*/
/*		_Computer Algorithms: Introduction to Design and	*/
/*			Analysis_ 2e, Baase. Addison-Wesley 1988.	*/
/*----------------------------------------------------------------------*/
/*======================================================================*/

/*======================================================================*/
/* #include Files							*/
/*======================================================================*/
#include <stdio.h>
#include <math.h>
#include "xtango.h"

/*======================================================================*/
/* #define Statements							*/
/*======================================================================*/
#define INFINITY	1000.0
#define UNSEEN		0
#define FRINGE		1
#define INTREE		2

#define MAX_ARRAY	25

/* Do we need SQUARE and MIN from spath.c? */
#define SQUARE(A)	((A) * (A))
#define DIST(A,B)	sqrt(SQUARE(A.dX-B.dX) + SQUARE(A.dY-B.dY))

/*======================================================================*/
/* XTango Declarations							*/
/*======================================================================*/
void	ANIMInit(),
	ANIMVertex(),
	ANIMEdge(),
	ANIMGetNode(),
	ANIMExamine(),
	ANIMCompare(),
	ANIMDump(),
	ANIMZuck(),
	ANIMNewMin(),
	ANIMShorter(),
	ANIMAddFringe(),
	ANIMAdd();

static NAME_FUNCT fcn[] = {
	{"Init",	1, {VOID,	(FPTR)ANIMInit}},
	{"Vertex",	1, {CHAR_PTR,	(FPTR)ANIMVertex}},
	{"Edge",	1, {CHAR_PTR,	(FPTR)ANIMEdge}},
	{"GetStart",	1, {CHAR_PTR,	(FPTR)ANIMGetNode}},
	{"Examine",	1, {VOID,	(FPTR)ANIMExamine}},
	{"Compare",	1, {VOID,	(FPTR)ANIMCompare}},
	{"Dump",	1, {VOID,	(FPTR)ANIMDump}},
	{"Zuck",	1, {VOID,	(FPTR)ANIMZuck}},
	{"NewMin",	1, {VOID,	(FPTR)ANIMNewMin}},
	{"Shorter",	1, {VOID,	(FPTR)ANIMShorter}},
	{"AddFringe",	1, {VOID,	(FPTR)ANIMAddFringe}},
	{"Add",		1, {VOID,	(FPTR)ANIMAdd}},
	{NULL,		NULL, NULL,	NULL} };

/*======================================================================*/
/* Main Function							*/
/*======================================================================*/
main()
{

/*-Data Structures------------------------------------------------------*/
struct vertex {				/* Data structure for a vertex	*/
	int 		iStatus;
	double 		dX;
	double 		dY;
	double		dDist;
	int		iFrom;
	};

   struct vertex 
	svaVertex[MAX_ARRAY];		/* List of all vertices  	*/

   double 
	daEdge[MAX_ARRAY][MAX_ARRAY], 	/* Adjacency matrix for edges 	*/  
   	dMin, 				/* Current minimum distance 	*/
        dTemp;				/* temporary value		*/

   int	iVisited, 			/* Number of vertices visited 	*/
	iNext,				/* Next Vertex			*/
	iStart,				/* Start Vertex			*/
	iNum,				/* Number of Vertices		*/
	i,j,k;				/* Loop Variables		*/

   char *cpReply; 			/* Ptr for XTango return vals	*/

   /* Set up the XTango System */
   TANGOalgoOp(fcn, "BEGIN");
   TANGOalgoOp(fcn, "Init");

   /* Brag to the User */
   printf("Dijkstra's Single Source Shortest Paths Algorithm\n");
   printf("Adapted for XTango by Anthony G. Francis, Jr.\n");

   /*-Interactively Draw the Graph Vertices-----------------------------*/
   printf ("Draw the graph with the mouse.\n");
   printf ("Click to select a vertex position (click in STOP to stop)\n");
   iNum = 0;				/* Counting Begins at 0 	*/

   for (;;) {				/* Loop until broken		*/
      cpReply = TANGOalgoOp(fcn, "Vertex",iNum);
					/* Get user input from XTango	*/

      if (!cpReply || (*cpReply=='\0')) /* Break if the user hits STOP	*/
	 break;				

      sscanf(cpReply,"%lf %lf", 	/* Get the vertex location	*/
	&(svaVertex[iNum].dX),
	&(svaVertex[iNum].dY));

      free(cpReply);			/* Free the ptr for more input	*/
      iNum++;				/* Increment the Vertex Counter	*/

      if (iNum>MAX_ARRAY) 		/* Break if we hit array's end	*/
	break;
      }

   /*-Clear all daEdge elements-------------------------------------------*/
   for (i=0; i<iNum; ++i)
      for (j=0; j<iNum; ++j)
        daEdge[i][j] = INFINITY;

   /*-Clear all vertex values-------------------------------------------*/
   for (i=0; i<iNum; ++i) { 
      svaVertex[i].iStatus = 	UNSEEN;
      svaVertex[i].dDist = 	INFINITY;
      svaVertex[i].iFrom = 	0;
      }

   /*-Interactively Select the Graph Edges------------------------------*/
   printf("Select pairs of vertices for daEdges.  (Click STOP to stop.)\n");

   for (;;) {				/* Loop until broken		*/
      cpReply = TANGOalgoOp(fcn, "Edge");
					/* Get user input from XTango	*/

      if (cpReply && *cpReply) {	/* If we have a vaild reply ...	*/
         sscanf(cpReply,"%d %d",&i,&j); /* ... then get the vertex #'s	*/
         if (i == -1) break;		/* Break if we're done		*/

         daEdge[i][j] = (daEdge[j][i] = DIST(svaVertex[i],svaVertex[j]));
					/* Label the daEdge with the 	*/
					/* distance between vertices.	*/
         				/* This is an undirected graph.	*/
         }
   }


   /*-Interactively Select the Graph Edges------------------------------*/
   printf("Select the start vertex\n");

   cpReply = TANGOalgoOp(fcn, "GetStart");
   sscanf(cpReply,"%d",&iStart);

   /*-------------------------------------------------------------------*/
   /* Single Source Shortest Paths Algorithm				*/
   /*-------------------------------------------------------------------*/
   /*-Initialize Data Structures----------------------------------------*/
   svaVertex[iStart].iStatus = 	INTREE;
   svaVertex[iStart].dDist =	0.0;
   svaVertex[iStart].iFrom =	iStart;
   iVisited =			1;

   /*-Add the first node and the first fringe---------------------------*/
   TANGOalgoOp(fcn, "AddFringe",iStart,iStart,svaVertex[iStart].dDist);
   TANGOalgoOp(fcn, "Add",	iStart);

   /* add the fringe elements */
   for (i=0; i<iNum; ++i)
      if ((svaVertex[i].iStatus == UNSEEN) && 
	  (daEdge[iStart][i] != INFINITY)) {
	 svaVertex[i].iStatus 	= FRINGE;
         svaVertex[i].iFrom 	= iStart;
         svaVertex[i].dDist 	= daEdge[iStart][i];
         TANGOalgoOp(fcn, "AddFringe",iStart,i,svaVertex[i].dDist);
         }

   /* Loop until you've visited all of them */
   while (iVisited < iNum) { 

      /* Check each element of the fringe - which is the minimum? */
      dMin = INFINITY;
      for (i=0; i<iNum; ++i)
          if (svaVertex[i].iStatus == FRINGE) { 

             /* Is this the first time through the loop?*/
	     if (dMin != INFINITY) {
		/* If not, compare values */
		TANGOalgoOp(fcn, "Compare",
		   iNext, i, (svaVertex[i].dDist < dMin)); }
             else { 
		/* Else, Set up the Compare Box */
                TANGOalgoOp(fcn, "Examine", svaVertex[i].iFrom,i);
 		TANGOalgoOp(fcn, "Dump", i); }

	     /* If the new vertex is closer, make it the current minumum */
             if (svaVertex[i].dDist < dMin) { 
		TANGOalgoOp(fcn, "NewMin",svaVertex[i].iFrom,i);
                dMin  = svaVertex[i].dDist;
                iNext = i;
                }
          }
     
      /* Add the current minimum to the tree */
      TANGOalgoOp(fcn, "Zuck", iNext);
      TANGOalgoOp(fcn, "Add",  iNext);
      svaVertex[iNext].iStatus = INTREE;
      iVisited++;

      /* Loop through unseen and fringed vertices */
      for (i=0; i<iNum; ++i)

	 /* Add new fringe vertices */
         if ((svaVertex[i].iStatus 	== UNSEEN) && 
	     (daEdge[iNext][i] 	!= INFINITY)) { 
            svaVertex[i].iStatus 	= FRINGE;
            svaVertex[i].iFrom 		= iNext;
            svaVertex[i].dDist 		= svaVertex[iNext].dDist + 
				 	  daEdge[iNext][i];
	    TANGOalgoOp(fcn, "AddFringe",iNext,i,svaVertex[i].dDist);
            }

	 /* check old fringes to see if they are closer now */
         else if (svaVertex[i].iStatus == FRINGE) { 
	    if (svaVertex[iNext].dDist+daEdge[iNext][i] < svaVertex[i].dDist) { 
	       dTemp 			= svaVertex[i].dDist;
               svaVertex[i].dDist 	= svaVertex[iNext].dDist + 
					  daEdge[iNext][i];  
  					/* better way of getting there */
	       dTemp 			= svaVertex[i].dDist - dTemp;
               svaVertex[i].iFrom = iNext;
               TANGOalgoOp(fcn, "Shorter",iNext,i,dTemp);
               }
            }
      }
   
   printf("All vertices have been visited.\n");

   /*-End the Program---------------------------------------------------*/
   TANGOalgoOp(fcn, "END");

}
