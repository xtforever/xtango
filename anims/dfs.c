#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMInput_adj(), ANIMDraw(), ANIMNew_comp(), ANIMEnd_comp(),
     ANIMVisit(), ANIMBacktrack(), ANIMDone(), ANIMInput_loc();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"Input_adj", 1, {VOID, (FPTR)ANIMInput_adj}},
			    {"Input_loc",   1, {VOID, (FPTR)ANIMInput_loc}},
			    {"Display",   1, {VOID, (FPTR)ANIMDraw}},
			    {"New_comp",   1, {VOID, (FPTR)ANIMNew_comp}},
			    {"End_comp",   1, {VOID, (FPTR)ANIMEnd_comp}},
			    {"Visit",   1, {VOID, (FPTR)ANIMVisit}},
			    {"Backtrack",   1, {VOID, (FPTR)ANIMBacktrack}},
			    {"Done",   1, {VOID, (FPTR)ANIMDone}},
			    {NULL,	  0,  NULL, NULL} };


void visit();

int	n,now,val[50],adj[50][50];
double	loc[50][2];

main()
{
   int i,j;

   TANGOalgoOp(fcn, "BEGIN");
   printf("You should use the input file dfs.in for this anim\n");
   printf("How many vertices?\n");
   scanf("%d",&n);

   TANGOalgoOp(fcn, "Init");

   printf("Enter the locations\n");
   for (i=0; i<n; ++i)
      { scanf("%lf %lf",&loc[i][0],&loc[i][1]);
	TANGOalgoOp(fcn, "Input_loc",loc,i,loc[i][0],loc[i][1]);
      }

   printf("Enter the adjacency matrix\n");
   for (i=0; i<n; ++i)
      for (j=0; j<n; ++j)
	 { scanf("%d",&adj[i][j]);
	   TANGOalgoOp(fcn, "Input_adj",adj,i,j,adj[i][j]);
	 }

   TANGOalgoOp(fcn, "Display",loc,n,adj);

   now = 0;
   for (j=0; j<n; ++j)
      val[j] = 0;

   for (j=0; j<n; ++j)
      if (val[j] == 0)
	 { TANGOalgoOp(fcn, "New_comp",loc,j,now);
	   visit(j);
	   TANGOalgoOp(fcn, "End_comp");
	 }
   TANGOalgoOp(fcn, "END");
}

void
visit(k)
   int k;
{
   int t;

   now += 1;
   val[k] = now;
   for (t=0; t<n; ++t)
      if ((adj[k][t] != 0) && (val[t] == 0))
	 { TANGOalgoOp(fcn, "Visit",loc,t,now);
	   visit(t);
	   TANGOalgoOp(fcn, "Backtrack",loc,k);
	 }
   TANGOalgoOp(fcn, "Done",loc,k);
}




