#include <stdio.h>
#include "xtango.h"

int queenpos[8];

void ANIMInit(), ANIMMove(), ANIMShow(), ANIMFound(), ANIMRemv(), ANIMAnchor(),
     ANIMDeanchor(), ANIMShowprob();

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
                            {"Place", 1, {VOID, (FPTR)ANIMShow}},
                            {"Remv", 1, {VOID, (FPTR)ANIMRemv}},
                            {"Found", 1, {VOID, (FPTR)ANIMFound}},
                            {"Move", 1, {VOID, (FPTR)ANIMMove}},
                            {"Anchor", 1, {VOID, (FPTR)ANIMAnchor}},
                            {"Deanchor", 1, {VOID, (FPTR)ANIMDeanchor}},
                            {"Showproblem", 1, {VOID, (FPTR)ANIMShowprob}},
                                 {NULL, NULL, NULL, NULL}};
                                 



/* Check for good positioning */

int good(queenpos, row, column)
   int queenpos[];
   int row;
   int column;

{
/* Rows for testing */
int testrow, testcol;
int retval = 1;

testrow = row - 1;
testcol = column - 1;

while (testrow >= 0 && testcol >= 0)
   {
   if (queenpos[testcol] == testrow)
      {
      TANGOalgoOp(fcn, "Showproblem", testrow, testcol, row, column);
      return(0);
      }
   testrow--;
   testcol--;
   }

testrow = row + 1;
testcol = column - 1;

while (testrow < 8 && testcol >= 0 && retval)
   {
   if (queenpos[testcol] == testrow)
      {
      TANGOalgoOp(fcn, "Showproblem", testrow, testcol, row, column);
      return(0);
      }
   testrow++;
   testcol--;
   }
   
testcol = column - 1;
while (testcol >= 0 && retval)
   {
   if (queenpos[testcol] == row)
      {
      TANGOalgoOp(fcn, "Showproblem", row, testcol, row, column);
      return(0);
      }
   testcol--;
   }

return(retval);
}

void queens(queenpos, column)
   int queenpos[];
   int column;
{
int row;

if (column > 7)
   {
   TANGOalgoOp(fcn, "Found");
   return;
   }

for (row = 0; row < 8; row++)
   {
   queenpos[column] = row;
   if (row == 0)
      TANGOalgoOp(fcn, "Place", column);
   else
      TANGOalgoOp(fcn, "Move", row, column);

   /* Check for good placement */
   if (good(queenpos, row, column))
      {
      TANGOalgoOp(fcn, "Anchor", column);
      queens(queenpos, column + 1);
      TANGOalgoOp(fcn, "Deanchor", column);
      }
   }

TANGOalgoOp(fcn, "Remv", column);
queenpos[column] = -1;
}

main()
{
int i;

TANGOalgoOp(fcn, "BEGIN");

for (i = 0; i < 8; i++)
   queenpos[i] = -1;

TANGOalgoOp(fcn, "Init");

queens(queenpos, 0);
TANGOalgoOp(fcn, "END");
}
