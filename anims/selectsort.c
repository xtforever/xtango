#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMInput(), ANIMDraw(), ANIMCompare(),
     ANIMExchange(), ANIMIn_place();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Draw",     1, {VOID, (FPTR)ANIMDraw}},
			    {"Compare",  1, {VOID, (FPTR)ANIMCompare}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
			    {"In place", 1, {VOID, (FPTR)ANIMIn_place}}, 
			    {NULL,	 0, NULL, NULL} };


main()
{
   int n,i,j,min,count;
   int temp;
   int a[200];

   TANGOalgoOp(fnc, "BEGIN");

   printf("Selection Sort\n");
   printf("Input number of elts in array\n");
   scanf("%d",&n);

   TANGOalgoOp(fnc, "Init");
   printf("Enter the elements\n");
   for (count=0; count<n; ++count)
      { scanf("%d",&a[count]);
	TANGOalgoOp(fnc, "Input", a, count, a[count]);
      }

   TANGOalgoOp(fnc, "Draw", a, n);

   for (i=0; i<n; ++i)  
      { min = i;
	for (j=i+1; j<n; ++j)
	  { TANGOalgoOp(fnc, "Compare", a, j, a, min);
            if (a[j] < a[min])
	       min = j; 
          }
        TANGOalgoOp(fnc, "Exchange", a, i, a, min);
	temp = a[min];
	a[min] = a[i];
	a[i] = temp;
	TANGOalgoOp(fnc, "In place", a, i);
      }         
   TANGOalgoOp(fnc, "END");
}



