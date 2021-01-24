

/********  K-Selection problem by Oliver Wang ****************/


#include <stdio.h>
#include <stdlib.h>
#include "xtango.h"

#define RAND_MAX 40000

void ANIMInit(), ANIMInput(), ANIMDraw(), ANIMCompare(),ANIMRemove(),
    ANIMShiftToLeft(),  ANIMExchange(),ANIMMove(), ANIMFlash(), 
    ANIMIn_place(),  ANIMCompare2(), ANIMPutText(), ANIMRemoveText(),
    ANIMOutline(), ANIMRemoveRect() , ANIMColor(), ANIMFill() ;

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Draw",     1, {VOID, (FPTR)ANIMDraw}},
			    {"Compare",  1, {VOID, (FPTR)ANIMCompare}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
		            {"Move",     1, {VOID, (FPTR)ANIMMove} },
                         {"ShiftToLeft",    1, {VOID, (FPTR)ANIMShiftToLeft} },
			      {"PutText", 1 , {VOID, (FPTR)ANIMPutText} } ,
			    {"Remove",   1, {VOID, (FPTR)ANIMRemove} } ,
	        	    {"Flash",    1, {VOID, (FPTR)ANIMFlash} },
                            {"Color",    1, {VOID, (FPTR)ANIMColor} },
			    {"In place", 1, {VOID, (FPTR)ANIMIn_place}},
			    {"Compare2", 1, {VOID, (FPTR)ANIMCompare2}},
                         {"RemoveText", 1, {VOID, (FPTR)ANIMRemoveText}}, 
                         {"RemoveRect", 1, {VOID, (FPTR)ANIMRemoveRect}}, 
                          {"Fill",    1, {VOID, (FPTR)ANIMFill} },
                        {"Outline", 1, {VOID, (FPTR)ANIMOutline}},
			    {NULL,	 0, NULL, NULL} };

int median[50], d[100] ;

int RandomSelect(p,r,i)
 int p,r,i;
     {  int q, k;
      if(p==r) return median[p];
        q=RandomPartition(p,r) ;
        k = q-p+1;

      if (i<= k) return RandomSelect(p,q,i) ;
       else return RandomSelect(q+1,r,i-k) ;
      }


int RandomPartition(p,r)
int p,r;
     { int x, i, j, temp ;

       x = median[p]; i= p-1; j=r+1;

       for (;;)
         {  while (median[j-1]>x)  j--;
            j--;
            while (median[i+1]<x)  i++;
            i++;
            if (i<j)
                 {  temp = median[i];
                    median[i]=median[j];
                    median[j]= temp ;
                  }
             else  return j ;
          }
     }



main()
{

   int a[100], n, order ;
   int i,j, k, i1, i2 ;
   int temp,iteration;
   double  dir ;
   int count, col, remainder, half, colMedian, Med  ;
   int flag = 0;
   char str1[5], str2[5], str3[5];

   TANGOalgoOp(fnc, "BEGIN");

   printf("Choose k_th largest of a set of unordered elements\n\n");
   printf("Input number of elts in array\n");
   scanf("%d",&n);


   printf("Input the order number you are looking for\n");
   printf(" i.e.  24 stands for the 24th smallest element\n" );
   scanf("%d",&order);
   sprintf(str1,"%d", order) ;
   

   TANGOalgoOp(fnc, "Init");

   printf("Enter the elements\n");
   for (count=0; count<n; ++count)
      { scanf("%d",&a[count]);
	TANGOalgoOp(fnc, "Input", a, count, a[count]);
      }

  TANGOalgoOp(fnc,"PutText", "Iteration number", 0.35,0.245, 30);
iteration = 0;
while ( n > 5 )
{  iteration++;
   sprintf(str3, "%d", iteration);
    TANGOalgoOp(fnc,"PutText", str3, 0.6, 0.245, 31);
   remainder = n%5;
   col = (n-remainder)/5 ;

 
  
   TANGOalgoOp(fnc, "Draw", a, n);  
   TANGOalgoOp(fnc,"PutText","Divide the elements into groups of 5"
 , 0.5, 0.30, 5 );
 TANGOalgoOp(fnc,"PutText","and sort each group of 5 elements" ,0.5, 0.33, 6 );

   for (k=1; k <= (n/5); k++ )
    { for (j=k*5-1; j>=k*5-5; --j)
      { TANGOalgoOp(fnc, "Outline", a, k) ;
         for (i=k*5-5; i<=j; ++i)
	   { TANGOalgoOp(fnc, "Compare", a, i, a, j); 
	     if (a[i] > a[j])
		{ temp = a[i];
		  a[i] = a[j];
		  a[j] = temp;
		  TANGOalgoOp(fnc, "Exchange", a, i, a, j);
		}
	   }
      /*  TANGOalgoOp(fnc, "Outline", a, k) ; */

      TANGOalgoOp(fnc, "RemoveRect", k );  
      }
  }
   TANGOalgoOp(fnc, "RemoveText", 5) ;
   TANGOalgoOp(fnc, "RemoveText", 6) ;

  k=1; dir = 1.0;
  for(i=2; i< n-remainder ; i+=5)
  {  median[k]=a[i];  k++;
    TANGOalgoOp(fnc, "Move", a,i,dir);
   }


  TANGOalgoOp(fnc,"PutText","Apply this selection algorithm recursively to"
 , 0.5, 0.27, 10 );

  TANGOalgoOp(fnc,"PutText","find out the median of the medians of all groups"
 , 0.5, 0.30, 1 );

  TANGOalgoOp(fnc,"PutText","of 5 elements" , 0.5, 0.33, 2 );
     

  half =(int)(col/2) + 1;
   Med = RandomSelect(1,col,half) ;
  i =1 ;
  while (a[i*5-3] != Med ) i++;
  colMedian = i ;

  k = colMedian * 5 - 3; 
  for(i=1;i<20;i++)
  TANGOalgoOp(fnc, "Flash", a, k) ;

  dir = -1.0;
  for(i=2; i< n-remainder ; i+=5)
  { if (i!= k )
    TANGOalgoOp(fnc, "Move", a,i,dir);
   }

    TANGOalgoOp(fnc, "RemoveText", 10) ;
   TANGOalgoOp(fnc, "RemoveText", 1) ;
   TANGOalgoOp(fnc, "RemoveText", 2) ;
  

TANGOalgoOp(fnc,"PutText","Partition the elements around the median of median"
 , 0.5, 0.30, 3 );
TANGOalgoOp(fnc,"PutText","Elements bigger than it are moved to the top"
 , 0.5, 0.33, 4);
 
  TANGOalgoOp(fnc, "Color", a , k , 1);
 dir = 1.8 ; i1=i2=0;
 for (i=0; i< n ; i++ )
  { if (i!=k)
    { TANGOalgoOp(fnc, "Color", a, i, 1);
       TANGOalgoOp(fnc, "Compare",a,i,a,k) ; 

    if (a[i]>a[k]) 
      { d[i]= 1;
        TANGOalgoOp(fnc, "Move", a, i, dir) ;
         TANGOalgoOp(fnc, "Color", a, i, 2) ;
        i2++;
      }
    else { d[i] = 0;
           TANGOalgoOp(fnc, "Color", a, i, 2) ;
           i1++ ;
           
         }
    }
  }  
    TANGOalgoOp(fnc, "Color", a , k , 2);

 TANGOalgoOp(fnc, "RemoveText", 3) ;
 TANGOalgoOp(fnc, "RemoveText", 4) ;
 
 if (i1==order-1)
   { for (i=0; i< n; i++)
       { if (i!=k)   TANGOalgoOp(fnc, "Remove", a, i) ; }
  
  TANGOalgoOp(fnc,"PutText","The element has been found(The solid rectangle)", 0.5,0.27, 11) ;
  TANGOalgoOp(fnc, "PutText", "Its order is", 0.3, 0.30, 12) ;
  TANGOalgoOp(fnc, "PutText", "Its value is", 0.3, 0.33, 13) ;
  sprintf(str2,"%d", a[k]);
  TANGOalgoOp(fnc, "PutText", str1, 0.5, 0.30, 14) ;
  TANGOalgoOp(fnc,"PutText", str2, 0.5, 0.33, 15);
   TANGOalgoOp(fnc, "Fill", a, k);
   flag = 1;  /* set the flag to 1 so if won't execute some of the code below*/
   printf("%d", a[k]);
   break ;
   }
             
  d[k] = 0;
  TANGOalgoOp(fnc, "Remove", a, k) ;

  if (i1>order-1) {  
  TANGOalgoOp(fnc,"PutText","The element we are looking for is in the smaller",
0.5, 0.27, 21);
  TANGOalgoOp(fnc,"PutText","(bottom) group of the partition",0.5, 0.30, 22);
  TANGOalgoOp(fnc,"PutText","Recursively apply the algorithm to this group",
 0.5, 0.33, 23);

  for(i=0; i<n; i++)
    { if (d[i]== 0 && i!=k)
        TANGOalgoOp(fnc,"Fill", a, i) ;
    }

    for (i=0;i < n; i++)
         { if ( i!= k && d[i]== 1 )
	   TANGOalgoOp(fnc,"Remove", a, i );  
         }
    
    j = 0 ;
    for (i=0; i<n; i++)
      { if (d[i] == 0 && i!= k)
         {     a[j] = a[i];
             TANGOalgoOp(fnc,"Input",a,j, a[j]) ;
              j++ ;
          }
      }

     for (i=0;i < n; i++)
         { if ( i!= k && d[i]== 0 )
	   TANGOalgoOp(fnc,"Remove", a, i );  
         }
    

   n = i1 ;
  } /* end of if */


 else { 
  TANGOalgoOp(fnc,"PutText","The element we are looking for is in the larger",
0.5, 0.27, 21);
  TANGOalgoOp(fnc,"PutText","(top) group of the partition",0.5, 0.30, 22);
  TANGOalgoOp(fnc,"PutText","Recursively apply the algorithm to this group",
 0.5, 0.33, 23);

  for(i=0; i<n; i++)
    { if (d[i]== 1 && i!=k)
        TANGOalgoOp(fnc,"Fill", a, i) ;
    }
      for (i=0;i < n; i++)
         { if ( i!= k && d[i]== 0 )
	   TANGOalgoOp(fnc,"Remove", a, i );  
         }
    j = 0;
    for (i=0; i<n; i++)
      { if (d[i] == 1 &&  i!= k)
          { 
              a[j] = a[i];
             TANGOalgoOp(fnc,"Input",a,j, a[j]) ;
             j++ ;
          }
      }

      for (i=0;i < n; i++)
         { if ( i!= k && d[i]== 1 )
	   TANGOalgoOp(fnc,"Remove", a, i );  
         }
 
 order = order -  i1-1 ; 
 n = i2;
	  } /* else  */
 TANGOalgoOp(fnc,"RemoveText", 21);
 TANGOalgoOp(fnc,"RemoveText", 22);
 TANGOalgoOp(fnc,"RemoveText", 23);
 TANGOalgoOp(fnc,"RemoveText", 31);
  
 } /* while n > 5  */
 TANGOalgoOp(fnc,"RemoveText", 30);
if ( flag!=1 )
{
  TANGOalgoOp(fnc, "Draw", a, n);
    for (j = n-1 ; j > 0 ; j--)
        { for (i=0; i<= j ; i++)
           { TANGOalgoOp(fnc, "Compare", a, i, a, j);
             if (a[i] > a[j])
                { temp = a[i];
                  a[i] = a[j];
                  a[j] = temp;
                  TANGOalgoOp(fnc, "Exchange", a, i, a, j);
                }
	   }
        }

  TANGOalgoOp(fnc,"PutText","The element has been found(The solid rectangle)", 0.5,0.27, 11) ;
  TANGOalgoOp(fnc, "PutText", "Its order is", 0.3, 0.30, 12) ;
  TANGOalgoOp(fnc, "PutText", "Its value is", 0.3, 0.33, 13) ;
  sprintf(str2,"%d", a[order-1]);
  TANGOalgoOp(fnc, "PutText", str1, 0.5, 0.30, 14) ;
  TANGOalgoOp(fnc,"PutText", str2, 0.5, 0.33, 15);

 TANGOalgoOp(fnc, "Fill", a, order-1) ;

}

   TANGOalgoOp(fnc, "END");
  
 }

