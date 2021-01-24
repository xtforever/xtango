#include <stdio.h>
#define  DUMMY {int dummy; dummy=1;}

main()
{
   int i,j,a[200],val,count,n;

   printf("Input number of elts in array\n");
   scanf("%d",&n);

   printf("Enter the elements\n");
   for (count=0; count<n; ++count)
      { scanf("%d",&a[count]);
	DUMMY
      }

   for (i=1; i<n; ++i)
      { val = a[i];
	j = i;
	while (j > 0)
	   { if (a[j-1] > val)
		{ a[j] = a[j-1];
		  j = j-1;
		}  
	     else
		break;
	   }
	a[j] = val;
      }

   for (count=0; count<n; ++count)
       printf("%d ",a[count]);
   printf("\n");
}
