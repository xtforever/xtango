#include <stdio.h>
#include "xtango.h"

void hanoi();
void move();

void ANIMInit(), ANIMMove();
static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
			    {"Move", 1, {VOID, (FPTR)ANIMMove}},
			    {NULL,   0, NULL, NULL} };

int peg[3];

main()
{
   int n;

   TANGOalgoOp(fcn, "BEGIN");

   printf("How many disks?\n");
   scanf("%d",&n);

   peg[0] = n;
   peg[1] = peg[2] = 0;

   TANGOalgoOp(fcn, "Init", n);
   hanoi(n,0,2);

   TANGOalgoOp(fcn, "END");
}




void
hanoi(n,from,to)
   int n;
   int from,to;
{
   int spare;

   if (n==1)
      move(from,to);
   else
      { spare = (to != 0 && from != 0) ? 0 :
		(to != 1 && from != 1) ? 1 : 2;
	hanoi(n-1,from,spare);
	move(from,to);
	hanoi(n-1,spare,to);
      }
}




void
move(from,to)
   int from,to;
{
   TANGOalgoOp(fcn, "Move",from,peg[from],to,peg[to]);

   peg[from]--;
   peg[to]++;
}
