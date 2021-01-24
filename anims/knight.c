/* Chess Knight's tour by Cristina Fernandes */

#include <stdio.h>
#include "xtango.h"

#define Max 10
#define Max2 100
#define FAIL 0

void ANIMInit(), ANIMMove(), ANIMBack(), ANIMTwinkle(), ANIMTour();

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)ANIMInit}},
			    {"Move", 1, {VOID, (FPTR)ANIMMove}},
			    {"Back", 1, {VOID, (FPTR)ANIMBack}},
			    {"Try",  1, {VOID, (FPTR)ANIMTwinkle}},
			    {"Tour", 1, {VOID, (FPTR)ANIMTour}},
			    {NULL, NULL, NULL, NULL} };
     
void main ()
{
  int A[Max][Max];      /* A[i,j] = 0 if the i,j position of the board     */
                        /* is free, otherwise it contains the number of    */
                        /* the step in which the i,j position was visited. */
  int dx[8];            /* The horizontal offsets of each possible knight  */
                        /* movement.                                       */
  int dy[8];            /* The same as dx, but to vertical offsets.        */
  int Move[Max2];       /* Move[i] indicates the movement executed by the  */
                        /* knight in the i step.                           */
  int n,                /* The size of the side of the board.              */
      i, j,             /* Auxiliars for traversing the board.             */
      m,                /* Indicates the step.                             */
      t,                /* Indicates the movement that is being tried.     */
      all;              /* Indicates if the first part of the animation    */
                        /* is being executed or not.                       */ 
  
  printf("Illustrating the tour of a knight on a chessboard\n\n");
  TANGOalgoOp(fcn, "BEGIN");
  
  printf("Input n: ");
  scanf("%d", &n);

  printf("Only the final result (0) or all the process (1)? ");
  scanf("%d", &all);
  
  /* In the begining all the positions are free. */

  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      A[i][j] = 0;

  /* Initialization of the offset arrays. */

  dx[0] = 1;    dy[0] = 2;
  dx[1] = 2;    dy[1] = 1;
  dx[2] = 2;    dy[2] = -1;
  dx[3] = 1;    dy[3] = -2;
  dx[4] = -1;   dy[4] = -2;
  dx[5] = -2;   dy[5] = -1; 
  dx[6] = -2;   dy[6] = 1;
  dx[7] = -1;   dy[7] = 2;

  /* Positioning the knight in the first position of the board. */
  
  m = 1;
  t = 0;
  i = 0;
  j = 0;
  A[i][j] = m;
  Move[0] = 0;

  TANGOalgoOp(fcn, "Init", n);

  while (m>0 && m<n*n) 
    {
      if (t == 8)
      /* All possible movements were tried. */
      /* So we will move back the knight.   */
          {
            m--;
            t = Move[m];
            A[i][j] = 0;
            i = i-dx[t];
            j = j-dy[t];
	    t = t+1;
	    if (all) TANGOalgoOp(fcn, "Back", i, j, m);
          }
      else 
        if (i+dx[t]>=0 && i+dx[t]<n && j+dy[t]>=0 && j+dy[t]<n)
        /* The movement t is a valid movement. */
          if (A[i+dx[t]][j+dy[t]] == 0)
          /* The position is free so we will move the knight. */
            {
              Move[m] = t;
              m++;
              i = i+dx[t];
              j = j+dy[t];
              A[i][j] = m;
              t = 0;
	      if (all) TANGOalgoOp(fcn, "Move", i, j, m-1);
	    }
          else 
	    {
              /* The position was alredy visited. */
	      if (all) TANGOalgoOp(fcn, "Try", i+dx[t], j+dy[t], FAIL);
	      t++;
	    }
        else t++;
    }

  if (m==0) printf("No solution! \n");
  else
    { 
      printf("There is a solution! \n");
      TANGOalgoOp(fcn, "Tour", n, Move, dx, dy); 
    }

  TANGOalgoOp(fcn, "END");
}

