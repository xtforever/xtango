/****************************
 TOPOLOGICAL SORT (topsort.c)

    by Marin Simina
 ****************************/

#include <stdio.h>
#include "xtango.h"

#define XTS   0.1
#define YTS   0.2
#define XSTEP 0.07

void ANIMInit(), ANIMInput_adj(), ANIMDraw(), ANIMNew_comp(), ANIMEnd_comp(),
     ANIMVisit(), ANIMBacktrack(), ANIMDone(), ANIMInput_loc(), 
     ANIMSorted_loc(), ANIMRedraw(), ANIMPsorted_loc();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"Input_adj", 1, {VOID, (FPTR)ANIMInput_adj}},
			    {"Input_loc",   1, {VOID, (FPTR)ANIMInput_loc}},
			    {"Sorted_loc",  1, {VOID, (FPTR)ANIMSorted_loc}},
			    {"Psorted_loc",  1, {VOID, (FPTR)ANIMPsorted_loc}},
			    {"Display",   1, {VOID, (FPTR)ANIMDraw}},
			    {"New_comp",   1, {VOID, (FPTR)ANIMNew_comp}},
			    {"End_comp",   1, {VOID, (FPTR)ANIMEnd_comp}},
			    {"Visit",   1, {VOID, (FPTR)ANIMVisit}},
			    {"Backtrack",   1, {VOID, (FPTR)ANIMBacktrack}},
			    {"Redraw", 1, {VOID, (FPTR)ANIMRedraw}},  
			    {"Done",   1, {VOID, (FPTR)ANIMDone}},
			    {NULL,	  0,  NULL, NULL} };


void visit();

int	n, now, finish, val[50], adj[50][50], tsorted[50];
double	loc[50][2], tsloc[50][2], ptsloc[50][2], xi, yi, xstep;

main()
{
   int i,j;

   TANGOalgoOp(fcn, "BEGIN");
   printf("You should use the input file topsort.in for this anim\n");
   printf("How many vertices?\n");
   scanf("%d",&n);

   xi= XTS; 
   yi= YTS;
   xstep= XSTEP;
   for(i= 0; i< n; ++i)
     { tsloc[i][0]= xi;
       tsloc[i][1]= yi;
       xi= xi+ xstep;
     }

   TANGOalgoOp(fcn, "Init");

   printf("Enter the locations\n");
   for (i=0; i<n; ++i)
      { scanf("%lf %lf",&loc[i][0],&loc[i][1]);
	TANGOalgoOp(fcn, "Input_loc",loc,i,loc[i][0],loc[i][1]+ YTS);
	TANGOalgoOp(fcn, "Sorted_loc", tsloc, i, tsloc[i][0], tsloc[i][1]); 
      }

   printf("Enter the adjacency matrix\n");
   for (i=0; i<n; ++i)
      for (j=0; j<n; ++j)
	 { scanf("%d",&adj[i][j]);
	   TANGOalgoOp(fcn, "Input_adj",adj,i,j,adj[i][j]);
	 }

   TANGOalgoOp(fcn, "Display",loc,n,adj);

   now = 0;
   finish= 0;

   for (j=0; j<n; ++j)
     { val[j] = 0;
       tsorted[j]= 0;
     }

   for (j=0; j<n; ++j)
      if (val[j] == 0)
	 { TANGOalgoOp(fcn, "New_comp",loc,j,j);
	   visit(j, j);
	   TANGOalgoOp(fcn, "End_comp");
	 }
   xi= XTS; 
   yi= YTS;
   xstep= XSTEP;
   for(i= 0; i< n; ++i)
     { ptsloc[tsorted[i]][0]= xi;
       ptsloc[tsorted[i]][1]= yi;
       xi= xi+ xstep;
     }

   for(i=0; i<n; i++)
     TANGOalgoOp(fcn, "Psorted_loc", ptsloc,i, ptsloc[i][0], 
		 ptsloc[i][1]);
   TANGOalgoOp(fcn, "Redraw", ptsloc, n, adj);
   TANGOalgoOp(fcn, "END");
}

void
visit(k, pred)
   int k, pred;
{
   int t;

   now += 1;
   val[k] = now;
   for (t=0; t<n; ++t)
      if ((adj[k][t] != 0) && (val[t] == 0))
	 { TANGOalgoOp(fcn, "Visit",loc,t,t);
	   visit(t, k);
	   TANGOalgoOp(fcn, "Backtrack",loc,k);
	 }
   tsorted[finish]= k;
   TANGOalgoOp(fcn, "Done",loc,k, pred, tsloc, finish);
   finish+= 1;
}








