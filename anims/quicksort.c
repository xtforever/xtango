#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMInput(), ANIMDraw(), 
     ANIMExchange();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Display",  1, {VOID, (FPTR)ANIMDraw}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
			    {NULL,	 0, NULL, NULL} };




void quicksort();
int a[200],n;

main()
{
   int count;

   printf("Quicksort\n");

   TANGOalgoOp(fnc, "BEGIN");
   TANGOalgoOp(fnc, "Init");

   printf("Input number of elts in array\n");
   scanf("%d",&n);

   printf("Enter the elements (must be ints from 0 -> n-1)\n");
   for (count=0; count<n; ++count)
      { scanf("%d",&a[count]);
	TANGOalgoOp(fnc, "Input",a,count,a[count]);
      }

   printf("This may take a sec...\n");
   TANGOalgoOp(fnc, "Display",a,n);
   quicksort(0,n-1);
   TANGOalgoOp(fnc, "END");

   for (count=0; count<n; ++count)
       printf("%d ",a[count]);
   printf("\n");
}


void
quicksort(lo,hi)
   int lo,hi;
{
   int i,j,part,temp;

   if (hi > lo)
      { if (lo == hi-1)
	   { if (a[lo] > a[hi])
		{ temp = a[lo];
		  a[lo] = a[hi];
		  a[hi] = temp;
                  TANGOalgoOp(fnc, "Exchange",a,lo,a,hi);
		}
             return;
	   }

	part = a[hi];
	i = lo-1;
	j = hi;

	do
	   { do
		{ i++;
		}
	     while (a[i] < part);
	     do
		{ j--;
		}
	     while ((j>i) && (a[j] > part));
	     if (j <= i) break;
		 { TANGOalgoOp(fnc, "Exchange",a,i,a,j);
                   temp = a[i];
                 }
	     a[i] = a[j];
	     a[j] = temp;
	   }
	while (j>i);

        TANGOalgoOp(fnc, "Exchange",a,i,a,hi);
	temp = a[i];
	a[i] = a[hi];
	a[hi] = temp;

	quicksort(lo, i-1);
	quicksort(i+1, hi);
      }
}
