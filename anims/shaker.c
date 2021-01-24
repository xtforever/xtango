/* Shakersort program for XTango, 
  written by Andres Gomez de Silva Garza (andres@cc) */

#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMInput(), ANIMDraw(), ANIMCompare(),
     ANIMExchange(), ANIMIn_place(), ANIMCompare2();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Draw",     1, {VOID, (FPTR)ANIMDraw}},
			    {"Compare",  1, {VOID, (FPTR)ANIMCompare}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
			    {"In place", 1, {VOID, (FPTR)ANIMIn_place}},
			    {"Compare2", 1, {VOID, (FPTR)ANIMCompare2}},
			    {NULL,	 0, NULL, NULL} };

main()
{
   int n,j,k,l,r,lower,upper;
   int temp;
   int a[50];

   TANGOalgoOp(fnc, "BEGIN");

   printf("Input number of elts in array\n");
   scanf("%d",&n);

   TANGOalgoOp(fnc, "Init");

   printf("Enter the elements\n");
   for (j=0; j<n; j++)
   {
      scanf("%d",&a[j]);
      TANGOalgoOp(fnc, "Input", a, j, a[j]);
   }

   TANGOalgoOp(fnc, "Draw", a, n);

   k = n-1;
   l = 1;
   r = n-1;
   lower = 0;
   upper = n-1;
 
   while (l<=r)
   {
      for (j=r; j>=l; j--)
      {
	 TANGOalgoOp(fnc, "Compare", a, j-1, a, j);
	 if (a[j-1] > a[j])
	 {
            temp = a[j-1];
	    a[j-1] = a[j];
            a[j] = temp;
            k = j;
            TANGOalgoOp(fnc, "Exchange", a, j-1, a, j);
         }
      }
      l = k+1;
      for (j=lower; j<l; j++)
         TANGOalgoOp(fnc, "In place", a, j);
      lower = l-1;
      for (j=l; j<=r; j++)
      {
         TANGOalgoOp(fnc, "Compare", a, j-1, a, j);
         if (a[j-1] > a[j])
         {
            temp = a[j-1];
            a[j-1] = a[j];
            a[j] = temp;
            k = j;
            TANGOalgoOp(fnc, "Exchange", a, j-1, a, j);
         }
      }
      r = k-1;
      for (j=upper; j>r; j--)
         TANGOalgoOp(fnc, "In place", a, j);
      upper = r+1;
   }
   TANGOalgoOp(fnc, "END");
}
