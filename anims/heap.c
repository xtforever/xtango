#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMAddNode(), ANIMElevate(), ANIMCompare1(), ANIMCompare2(),
            ANIMExchange();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"AddNode", 1, {VOID, (FPTR)ANIMAddNode}},
			    {"Elevate",   1, {VOID, (FPTR)ANIMElevate}},
			    {"Compare1",   1, {VOID, (FPTR)ANIMCompare1}},
			    {"Compare2",   1, {VOID, (FPTR)ANIMCompare2}},
			    {"Exchange",   1, {VOID, (FPTR)ANIMExchange}},
			    {NULL,	  0,  NULL, NULL} };


void upheap();
void downheap();
int Heap[129];

main()
{
   int i,n,m;
   int a[129];

   printf("Input number of elts to be heap-sorted\n");
   scanf("%d",&n);

   TANGOalgoOp(fcn, "BEGIN");
   TANGOalgoOp(fcn, "Init",Heap,n);

   printf("Enter the elements\n");
   for (i=1; i<=n; ++i)
      { scanf("%d",&a[i]);
      }

   m = 0;
   Heap[0] = 10000;
   for (i=1; i<=n; ++i) {
      m++;
      Heap[m] = a[i];
      TANGOalgoOp(fcn, "AddNode",Heap,m,Heap[m]);
      upheap(m);
      }

   for (i=n; i>=1; --i) {
      a[i] = Heap[1];
      TANGOalgoOp(fcn, "Elevate",Heap,m);
      Heap[1] = Heap[m];
      m--;
      downheap(1,m);
      }

   TANGOalgoOp(fcn, "END");
   printf("Sorted array is\n");
   for (i=1; i<=n; ++i)
      printf("%d ",a[i]);
   printf("\n");
}


void
upheap(k)
   int k;
{
   int val;

   val = Heap[k];
   while (Heap[k/2] < val) {
      Heap[k] = Heap[k/2];
      TANGOalgoOp(fcn, "Exchange",Heap,k,k/2);
      k = k/2;
      }
   Heap[k] = val;
}


void
downheap(k,m)
   int k,m;
{
   int val,j,bigger;

   val = Heap[k];
   while (k <= m/2) {
      j = k*2;
      if (j > m) break;
      if (j < m) {
	 bigger = (Heap[j] >= Heap[j+1] ? j : j+1);
	 TANGOalgoOp(fcn, "Compare2",Heap,k);
	 }
      else {
	 bigger = j;
	 TANGOalgoOp(fcn, "Compare1",Heap,k);
	 }
      if (val >= Heap[bigger])
	 break;
      Heap[k] = Heap[bigger];
      TANGOalgoOp(fcn, "Exchange",Heap,bigger,k);
      k = bigger;
      }
   Heap[k] = val;
}





