/* Matrix mutliplication */
/* Developed by Joshua Bleier */


#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include "xtango.h"

#define square(A)  ((A) * (A))
#define min(A,B)   ((A) < (B) ? (A) : (B))
#define odd(x)     ((x) % 2)

#define size 3

void ANIMInit(), ANIMRow(), ANIMEraseRow(), ANIMColumn(), ANIMEraseColumn(),
     ANIMMultiply(), ANIMEraseMultiply(), ANIMSum(),
     ANIMDisplayArray(), ANIMDisplayChar(), ANIMEraseChars();

static NAME_FUNCT fcn[] = {{"Init",	  1, {VOID,     (FPTR)ANIMInit}},
			   {"Row",	  1, {VOID,     (FPTR)ANIMRow}},
			   {"EraseRow",	  1, {VOID,     (FPTR)ANIMEraseRow}},
			   {"Column",	  1, {VOID,     (FPTR)ANIMColumn}},
			   {"EraseCol",	  1, {VOID,     (FPTR)ANIMEraseColumn}},
			   {"Multiply",	  1, {VOID,     (FPTR)ANIMMultiply}},
			   {"EraseMult",  1, {VOID,     (FPTR)ANIMEraseMultiply}},
			   {"Sum",  	  1, {VOID,     (FPTR)ANIMSum}},
			   {"Array",      1, {VOID,     (FPTR)ANIMDisplayArray}},
			   {"Char",       1, {VOID,     (FPTR)ANIMDisplayChar}},
			   {"EraseChars", 1, {VOID,     (FPTR)ANIMEraseChars}},
			   {NULL,      NULL, NULL,      NULL} };

/* v[i,j] = a1[i,1]xa2[1,j] + a1[i,2]xa2[2,j] + ... + a1[i,n]xa2[n,j] */
void 
main ()
{
  int a1[3][3];
  int a2[3][3];
  int value[3][3];
  int i,j,k, trows, tcols, sval, temp, done;
  double x, y, x2, y2, offset, gap, finalx, finaly;

  TANGOalgoOp(fcn, "BEGIN");
  printf("Matrix multiplication\n");
  printf("  (You probably want to run   <matrixmult.in for this one\n");
  TANGOalgoOp(fcn, "Init");
  trows = tcols = 3;
  done = 0;

  for(i = 0; i < trows; i++)
    {
      for(j = 0; j < tcols; j++)
	{
	  printf("Input the value for a1[%d,%d]> \n", i + 1, j + 1);
	  scanf("%d", &sval);
	  a1[i][j] = sval;
	} /* for j */
    } /* for i */

  for(i = 0; i < trows; i++)
    {
      for(j = 0; j < tcols; j++)
	{
	  printf("Input the value for a2[%d,%d]> \n", i + 1, j + 1);
	  scanf("%d", &sval);
	  a2[i][j] = sval;
	} /* for j */
    } /* for i */

  TANGOalgoOp(fcn, "Array", a1, "Array1", .10, .10);
  TANGOalgoOp(fcn, "Char", "X", .30, .160, 0);
  TANGOalgoOp(fcn, "Array", a2, "Array2", .38, .10);
  TANGOalgoOp(fcn, "Char", "=", .560, .160, 0);

  offset = .040;
  gap    = .020;
  finalx = .66;
  finaly = .10;
  y2     = finaly;

  for(i = 0; i < trows; i++)
    {
      x  = .50;
      y  = .50;

      TANGOalgoOp(fcn, "Row", "Array1", i, .10 , .50);
      TANGOalgoOp(fcn, "Char", "X", .30, .50, 1);
      TANGOalgoOp(fcn, "Char", "=", x, y, 1);
      x = x + offset + gap;
      x2 = finalx;

      for(j = 0; j < tcols; j++)
	{
	  x  = .50 + offset + gap;
	  TANGOalgoOp(fcn, "Column", "Array2", j, .38 , y);

	  sval = 0;
	  for(k = 0; k < trows; k++)
	    {
	      temp = (a1[i][k] * a2[k][j]);
	      TANGOalgoOp(fcn, "Multiply", k, x, y, temp);
	      sval = sval + temp;
	      x = x + offset;
	      if ((k < (trows - 1)) && (done == 0))
		{
		  TANGOalgoOp(fcn, "Char", "+", x, y, 1);
		}
	      x = x + offset;
	    } /* for k */
	  done = 1;
	  x = x + offset;
	  TANGOalgoOp(fcn, "Char", "=", x, y, 1);
	  x = x + offset;
	  TANGOalgoOp(fcn, "Sum", sval, x, y, x2, y2);
	  TANGOalgoOp(fcn, "EraseMult");
	  TANGOalgoOp(fcn, "EraseCol");
	  x2 = x2 + offset + gap;
	} /* for j */
      TANGOalgoOp(fcn, "EraseRow");

      y2 = y2 + offset + gap;
    } /* for i */
/* TANGOalgoOp(fcn, "EraseChars"); */
  TANGOalgoOp(fcn, "END");
} /* main */

