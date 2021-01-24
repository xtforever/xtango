#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMInput(), ANIMDraw(),
     ANIMExchange();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Draw",     1, {VOID, (FPTR)ANIMDraw}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
			    {NULL,	 0, NULL, NULL} };

main()
{
   int n,i,j;
   int temp;
   int a[300];

   int count;

   TANGOalgoOp(fnc, "BEGIN");

   printf("Input number of elts in array\n");
   scanf("%d",&n);

   TANGOalgoOp(fnc, "Init");

   printf("Enter the elements\n");
   for (count=0; count<n; ++count)
      { scanf("%d",&a[count]);
	TANGOalgoOp(fnc, "Input", a, count, a[count]);
      }

   TANGOalgoOp(fnc, "Draw", a, n);

   for (j=n-2; j>=0; --j)
      { for (i=0; i<=j; ++i)
	   { if (a[i] > a[i+1])
		{ temp = a[i];
		  a[i] = a[i+1];
		  a[i+1] = temp;
		  TANGOalgoOp(fnc, "Exchange", a, i, a, i+1);
		}
	   }
      }
   TANGOalgoOp(fnc, "END");
}
