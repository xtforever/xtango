#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMSet_time(), ANIMShow_times(), ANIMEnter(), ANIMExit();

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
			    {"Set_time", 1, {VOID, (FPTR)ANIMSet_time}},
			    {"Show_times", 1, {VOID, (FPTR)ANIMShow_times}},
			    {"Enter", 1, {VOID, (FPTR)ANIMEnter}},
			    {"Exit", 1, {VOID, (FPTR)ANIMExit}},
			    {NULL, 0, NULL, NULL} };

int min_line();

int line[10],n;

main() {
   int time[10];
   int clock,arrival;
   int i,smallest,seed;

   TANGOalgoOp(fcn, "BEGIN");

   printf("Try the input file post.in for this one\n");
   printf("What is the random # seed?\n");
   scanf("%d",&seed);
   srandom(seed);
   clock = 1;
   printf("Enter number of lines in post office\n");
   scanf("%d",&n);
   TANGOalgoOp(fcn, "Init",line,time,n);

   for (i=0; i<n; ++i)
      { line[i] =  time[i] = 0;
	TANGOalgoOp(fcn, "Set_time",i,0);
      }
   TANGOalgoOp(fcn, "Show_times",time,n);

   printf("Type a sequence of arrival times (ints, 1->n, in order, -1 to quit)\n");
   scanf("%d",&arrival);
   do
      { if (arrival == clock)
	   { smallest = min_line();
	     line[smallest]++;
	     TANGOalgoOp(fcn, "Enter",line, smallest, line[smallest]);
	     if (line[smallest] == 1)
		{ time[smallest] = 1 + (random() & 0xf);   /* 1 -> 16 */
		  TANGOalgoOp(fcn, "Set_time",smallest,time[smallest]);
		}
	     TANGOalgoOp(fcn, "Show_times",time,n);

	     scanf("%d",&arrival);
	   }
	else
	   { sleep(2);
	     for (i=0; i<n; ++i)
		if (line[i])
		   { time[i]--;
		     TANGOalgoOp(fcn, "Set_time",i,time[i]);
		   }
	     TANGOalgoOp(fcn, "Show_times",time,n);
	     for (i=0; i<n; ++i)
		if (line[i] && !time[i])
		   { line[i]--;
		     TANGOalgoOp(fcn, "Exit",line,i,line[i]);
		     if (line[i])
			{ time[i] = 1 + (random() & 0xf);   /* 1 -> 16 */
			  TANGOalgoOp(fcn, "Set_time",i,time[i]);
			}
		     TANGOalgoOp(fcn, "Show_times",time,n);
		   }
	     ++clock;
	   }
      } while (arrival != -1);
   TANGOalgoOp(fcn, "END");
}


int
min_line()
{
   int min,i,min_num;

   min = 100;
   for (i=0; i<n; ++i)
      { if (line[i] < min)
	   { min = line[i];
	     min_num = i;
	   }
	if (min == 0) break;
      }
   return(min_num);
}
