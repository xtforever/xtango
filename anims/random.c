#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMPrimary(), ANIMSecondary(), ANIMProbe(),
 ANIMFill();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"Primary",   2,{{VOID, (FPTR)ANIMPrimary},
					     {VOID, (FPTR)ANIMProbe}}},
			    {"Secondary", 2,{{VOID, (FPTR)ANIMSecondary},
					     {VOID, (FPTR)ANIMProbe}}},
			    {"NewElement",   1, {VOID, (FPTR)ANIMFill}},
        		    {NULL,	  0,  NULL, NULL} };


main()
{
   int n,i,seed,value,array[200],filled;

   printf("Generate how big of a sequence?\n");
   scanf("%d",&n);

   TANGOalgoOp(fcn, "BEGIN");
   TANGOalgoOp(fcn, "Init",array,n);
   for (i=0; i<n; ++i)
      array[i] = 0;

   printf("What is the random number seed (int)?\n");
   scanf("%d",&seed);
   srandom(seed);

   filled = 0;
   while (filled != n)
      { value = random() % n;
	TANGOalgoOp(fcn, "Primary",array,value,array[value]);
	array[value] += 1;
	while (array[value] != 1)
	   { value++;
	     if (value == n)
		value = 0;
	     TANGOalgoOp(fcn, "Secondary",array,value,array[value]);
	     array[value]++;
	   }
	printf("%d ",value);
	TANGOalgoOp(fcn, "NewElement",array,value);
	filled++;
      };
   printf("\n");
   TANGOalgoOp(fcn, "END");
}

