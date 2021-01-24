/* Quicksort developed by Charles Hardnett */
/* 7-16-93 modified by S. Rodger so that it doesn't core dump on */
/*         input by numbers in reverse order */


#include <stdio.h>
#include "xtango.h"

#define DOWN	1
#define UP	0


void ANIMInit(), ANIMInput(), ANIMDraw(), ANIMMove(), ANIMSorted(),
     ANIMExchange(), ANIMShuffle();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Display",  1, {VOID, (FPTR)ANIMDraw}},
                            {"Shuffling",1, {VOID, (FPTR)ANIMShuffle}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
			    {"Recursive_call", 1, {VOID, (FPTR)ANIMMove}},
			    {NULL,	 0, NULL, NULL} };




void quicksort();
int a[200],n;

main()
{
   int count;

   printf("Quicksort\n");
   printf("Input number of elts in array\n");
   scanf("%d",&n);

   TANGOalgoOp(fnc, "BEGIN");
   TANGOalgoOp(fnc, "Init");

   printf("Enter the elements\n");
   for (count=0; count<n; ++count)
      { scanf("%d",&a[count]);
	TANGOalgoOp(fnc, "Input",a,count,a[count]);
      }

   printf("This may take a sec...\n");
   TANGOalgoOp(fnc, "Display",a,n);
   
   quicksort(0,n-1,n);
   TANGOalgoOp(fnc, "END"); 

   for (count=0; count<n; ++count)
       printf("%d ",a[count]);
   printf("\n");
}


void
quicksort(lo,hi,n)
   int lo,hi,n;
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
             TANGOalgoOp(fnc, "Recursive_call",a,lo,hi,UP); /*put splitpoint in place*/
             TANGOalgoOp(fnc, "Shuffling",a,lo,2);
             TANGOalgoOp(fnc, "Shuffling",a,hi,2);
              
             return;
	   }

	part = a[hi];
	i = lo-1;
	j = hi;

	do
	   { do
		{ 
                  i++;
                  TANGOalgoOp(fnc, "Shuffling",a,i,0);
                  TANGOalgoOp(fnc, "Shuffling",a,i,1);
		}
	     while (a[i] < part);
	     do
		{ 
                  j--;
                  TANGOalgoOp(fnc, "Shuffling",a,j,0);
                  TANGOalgoOp(fnc, "Shuffling",a,j,1);
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
   

        TANGOalgoOp(fnc, "Recursive_call",a,lo,hi,UP); /*put splitpoint in place*/
        
        TANGOalgoOp(fnc, "Shuffling",a,i,2);
   
        TANGOalgoOp(fnc, "Recursive_call",a,lo,i-1,DOWN);
        TANGOalgoOp(fnc, "Recursive_call",a,i+1,hi,DOWN);

	quicksort(lo, i-1, n);
	quicksort(i+1, hi, n);
      }
      else if (lo < n)
         TANGOalgoOp(fnc, "Shuffling",a,lo,2); 
}

