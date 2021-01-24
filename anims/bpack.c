#include <stdio.h>
#include "xtango.h"

void AnimInit(), AnimNewWeight(), AnimMoveTo(), AnimInPlace();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)AnimInit}},
			    {"NewWeight", 1, {VOID, (FPTR)AnimNewWeight}},
			    {"Failure",   1, {VOID, (FPTR)AnimMoveTo}},
			    {"Success",   2,{{VOID, (FPTR)AnimMoveTo},
					     {VOID, (FPTR)AnimInPlace}}},
			    {NULL,	  0,  NULL, NULL} };

main()
{
   int n,b,i,wtnum;
   double wt;
   double bin[100];

   TANGOalgoOp(fcn, "BEGIN");

   printf("How many bins?\n");
   scanf("%d",&n);
   TANGOalgoOp(fcn, "Init",bin,n);

   for (i=0; i<n; ++i)
      bin[i] = 0.0;

   wtnum = 0;
   printf("Enter the weights (range 0.0->1.0, with 0.0 to quit)\n");
   for (;;)
      { scanf("%lf",&wt);
	if (wt == 0.0) break;
	TANGOalgoOp(fcn, "NewWeight",&wt,wtnum,wt);
	b = 0;
	while (bin[b] + wt > 1.0)
	   { TANGOalgoOp(fcn, "Failure",&wt,wtnum,bin,b);
	     b++;
	   }
	TANGOalgoOp(fcn, "Success",&wt,wtnum,bin,b);
	bin[b] += wt;
	wtnum++;
      }
   TANGOalgoOp(fcn, "END");
}

