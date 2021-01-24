#include <stdio.h>
#include "xtango.h"
#define SPLIT 1
#define JOIN  0

void ADraw(), AInit(), AInput(), APart(), AJoin(), APass(), ARebound();
void AUncolor();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)AInit}},
                            {"Input",    1, {VOID, (FPTR)AInput}},
                            {"Uncolor",  1, {VOID, (FPTR)AUncolor}},
                            {"Draw",     1, {VOID, (FPTR)ADraw}},
                            {"Part",     1, {VOID, (FPTR)APart}},
                            {"Pass",     1, {VOID, (FPTR)APass}},
                            {"Rebound",  1, {VOID, (FPTR)ARebound}},
                            {NULL,       0, NULL, NULL} };

main()
{
   int n,i,j,k,shell,val;
   int a[100];

   int count;
   TANGOalgoOp(fnc, "BEGIN");

   printf("Input number of elements in array\n");
   scanf("%d",&n);
   TANGOalgoOp(fnc, "Init");

   printf("Enter the elements\n");
   for (count=0; count<n; ++count) {
      scanf("%d",&a[count]);
      TANGOalgoOp(fnc, "Input",a,count,a[count]);
      }

   TANGOalgoOp(fnc, "Draw",a, n);
   shell = 1;
   do {shell = shell*3 + 1;} while (shell <= n);
   while (shell) {
        if ((shell < n) && (shell != 1)) TANGOalgoOp(fnc, "Uncolor", a, n);
        shell /= 3;
	for (i=0; i<shell; ++i) {
	   j = i + shell;
           TANGOalgoOp(fnc, "Part", a, shell, i, SPLIT, n); 
           while (j < n) {
              k = j;
              val = a[k];
              while (a[k-shell] > val) {
                 TANGOalgoOp(fnc, "Pass", a, k-shell, k, i);
                 a[k] = a[k-shell];
                 k -= shell;
                 if (k < shell) break;
                 }
	      a[k] = val;
              j += shell;
              if (k>= shell) TANGOalgoOp(fnc, "Rebound", a, k-shell, k);
              }
           TANGOalgoOp(fnc, "Part", a, shell, (i % shell), JOIN, n); 
	   }
      }
   for (i=0; i<n; ++i)
      printf("%d ",a[i]);
   printf("\n");
   TANGOalgoOp(fnc, "END"); 
}



