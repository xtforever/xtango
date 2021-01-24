#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMInput(), ANIMDisplay(), ANIMTarget(), ANIMTry(),
     ANIMMiss(), ANIMHit(), ANIMMovePtr(), ANIMCleanUp();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			      {"Input",   1, {VOID, (FPTR)ANIMInput}},
			      {"Display", 1, {VOID, (FPTR)ANIMDisplay}},
			      {"Target",  1, {VOID, (FPTR)ANIMTarget}},
			      {"Right",   3, {{VOID, (FPTR)ANIMTry},
					      {VOID, (FPTR)ANIMMiss},
					      {VOID, (FPTR)ANIMMovePtr}}},
			      {"Left",    3, {{VOID, (FPTR)ANIMTry},
					      {VOID, (FPTR)ANIMMiss},
					      {VOID, (FPTR)ANIMMovePtr}}},
			      {"GotIt",   2, {{VOID, (FPTR)ANIMTry},
					      {VOID, (FPTR)ANIMHit}}},
			      {"CleanUp", 1, {VOID, (FPTR)ANIMCleanUp}},
  			      {NULL,	  0,  NULL, NULL} };


			      
main()
{
   int a[100],n,i,left,right,check,target,more;

   TANGOalgoOp(fcn, "BEGIN");
   printf("Input number of elements in array\n");
   scanf("%d",&n);

   TANGOalgoOp(fcn, "Init");

   printf("Enter the integers in sorted order\n");
   for (i=0; i<n; ++i)
      { scanf("%d",&a[i]);
	TANGOalgoOp(fcn, "Input",a,i,a[i]);
      }

   TANGOalgoOp(fcn, "Display",a,n);
   do {
      left = 0;
      right = n-1;
      printf("Search for what value?\n");
      scanf("%d",&target);
      TANGOalgoOp(fcn, "Target",target);
      do {
	 check = (left + right) / 2;
	 if (target < a[check]) {
	    right = check - 1;
	    TANGOalgoOp(fcn, "Right",a,check,0);
	    }
	 else if (target > a[check]) {
	    left = check + 1;
	    TANGOalgoOp(fcn, "Left",a,check,1);
	    }
	 else {
	    TANGOalgoOp(fcn, "GotIt",a,check);
	    break;
	    }
	 } while (left <= right);

      printf("Would you like to continue? (0-no 1-yes)\n");
      scanf("%d",&more);
      if (more)
	 TANGOalgoOp(fcn, "CleanUp",a,n);

      } while (more);
   TANGOalgoOp(fcn, "END");
}
