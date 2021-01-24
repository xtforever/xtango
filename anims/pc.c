#include <stdio.h>
#include "xtango.h"

#define TRUE 1
#define FALSE 0

void ANIMInit(), ANIMP_wait(), ANIMP_active(), ANIMC_wait(), ANIMC_active(),
     ANIMNew_item(), ANIMFull(), ANIMIn(), ANIMOut();

static NAME_FUNCT fcn[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
		      	    {"P wait",   1, {VOID, (FPTR)ANIMP_wait}},
		       	    {"P active", 1, {VOID, (FPTR)ANIMP_active}},
		       	    {"C wait",   1, {VOID, (FPTR)ANIMC_wait}},
		       	    {"C active", 1, {VOID, (FPTR)ANIMC_active}},
		       	    {"New item", 1, {VOID, (FPTR)ANIMNew_item}},
		       	    {"Full",     1, {VOID, (FPTR)ANIMFull}},
		       	    {"In",       1, {VOID, (FPTR)ANIMIn}},
		       	    {"Out",      1, {VOID, (FPTR)ANIMOut}},
			    {NULL,	 0, NULL, NULL} };
main()
{
   int item,in,out,n;
   int p_waiting,c_waiting;
   int buffer[10];
   char input[3];
   char cmd;

   TANGOalgoOp(fcn, "BEGIN");

   in = out = 0;
   item = 0;
   p_waiting = c_waiting = FALSE;
   printf("Enter the number of queue slots\n");
   scanf("%d",&n);
   TANGOalgoOp(fcn, "Init", buffer,n);

   do
      { printf("Enter next command\n");
	scanf("%1s",input);
	cmd = input[0];

	if (cmd == 'p')
	   { if (!p_waiting)
		{ item++;
		  TANGOalgoOp(fcn, "New item",&item,item);
		  if ((in+1)%n == out)	/* queue full */
		     { p_waiting = TRUE;
		       TANGOalgoOp(fcn, "Full",&item,item);
		       TANGOalgoOp(fcn, "P wait");
		     }
		  else
		     { buffer[in] = item;
		       TANGOalgoOp(fcn, "In",&item,item,buffer,in,(in+1)%n);
		       in = (in+1)%n;
		     }
		  if (c_waiting)
		     { buffer[out] = 0;
		       TANGOalgoOp(fcn, "Out",&item,item,buffer,(out+1)%n,out);
		       TANGOalgoOp(fcn, "C active");
		       out = (out+1)%n;
		       c_waiting = FALSE;
		     }
		}
	   }
	else if (cmd == 'c')
	   { if (!c_waiting)
		{ if (in == out)  /* queue empty */
		     { c_waiting = TRUE;
		       TANGOalgoOp(fcn, "C wait");
		     }
		  else
		     { TANGOalgoOp(fcn,"Out",&item,buffer[out],
					     buffer,(out+1)%n,out);
		       buffer[out] = 0;
		       out = (out+1)%n;
		     }
		  if (p_waiting)
		     { buffer[in] = item;
		       TANGOalgoOp(fcn, "P active");
		       TANGOalgoOp(fcn, "In", &item,item,buffer,in,(in+1)%n);
		       in = (in+1)%n;
		       p_waiting = FALSE;
		     }
		}
	   }
	else if (cmd == 'q')
	   break;
      }
   while (1==1);
   TANGOalgoOp(fcn, "END");
}
