#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMInput(), ANIMDraw(), ANIMCompare(),
     ANIMSmaller(), ANIMFix();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Display",     1, {VOID, (FPTR)ANIMDraw}},
			    {"Compare",  1, {VOID, (FPTR)ANIMCompare}},
			    {"Smaller",  1, {VOID, (FPTR)ANIMSmaller}},
			    {"Fix",      1, {VOID, (FPTR)ANIMFix}},
			    {NULL,	 0, NULL, NULL} };


main()
{
   int i,j,k;
   int a[50],b[50],c[100];
   int n,m;
   int count;

   TANGOalgoOp(fnc, "BEGIN");
   TANGOalgoOp(fnc, "Init");

   printf("Enter size of first array\n");
   scanf("%d",&n);
   printf("Enter first array\n");
   for (count=0; count<n; ++count)
      { scanf("%d",&a[count]);
	TANGOalgoOp(fnc, "Input",a,count,a[count]);
      }

   printf("Enter size of second array\n");
   scanf("%d",&m);
   printf("Enter second array\n");
   for (count=0; count<m; ++count)
      { scanf("%d",&b[count]);
	TANGOalgoOp(fnc, "Input",b,count,b[count]);
      }

   TANGOalgoOp(fnc, "Display",a,n,b,m,c,n+m);
   sleep(2);

   i = j = k = 0;
   while ((i<n) && (j<m))
      { TANGOalgoOp(fnc, "Compare",a,i,b,j);
	sleep(1);
	if (a[i] < b[j])
	   { c[k] = a[i];
	     TANGOalgoOp(fnc, "Smaller",a,i,b,j,c,k);
	     i++;
	   }
	else
	   { c[k] = b[j];
	     TANGOalgoOp(fnc, "Smaller",b,j,a,i,c,k);
	     j++;
	   }
	k++;
	sleep(1);
      }

   if (i>=n)
      for ( ; j<m; ++j)
	 { c[k] = b[j];
	   TANGOalgoOp(fnc, "Fix",b,j,c,k);
	   ++k;
	 }
   else
      for ( ; i<n; ++i)
	 { c[k] = a[i];
	   TANGOalgoOp(fnc, "Fix",a,i,c,k);
	   ++k;
	 }

   TANGOalgoOp(fnc, "END");
}



