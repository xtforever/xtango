#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMExchange();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
			    {NULL,	  0,  NULL, NULL} };




main()
{
   int seed,i,j,index,cards[52],C,R,tempcard;

   printf("What is the random number seed (int)?\n");
   scanf("%d",&seed);
   srandom(seed);

   TANGOalgoOp(fcn, "BEGIN");
   for (i=1, index=0; i<=4; ++i)
      for (j=2; j<=14; ++j,++index)
          cards[index] = j;

	TANGOalgoOp(fcn, "Init");
	for (C=51; C>2; --C) {
	  R = (random() % (C-2)) ;
	  tempcard = cards[C];
	  cards[C] = cards[R];
	  cards[R] = tempcard;
	  TANGOalgoOp(fcn, "Exchange",C,R);
        }
   TANGOalgoOp(fcn, "END");
 }

