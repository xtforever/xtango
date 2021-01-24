/* Game of life program. Adapted from An Introduction to Programming and
   Problem Solving With Pascal, Second Edition, Schieder G.M. et. al. */
/* Author: Kiran Panesar */

#include <stdio.h>
#define and &&
#define or ||
#define false 0
#define true 1
#define maxsize 50

#include "xtango.h"

void init(), animinit(), make_boxes(), compose(),
     perform();

static NAME_FUNCT fnc[] = {{"Init", 1, {VOID, (FPTR) animinit}},
                           {"Compose", 1, {VOID, (FPTR) compose}},
                           {"PerformCompose", 1,{VOID, (FPTR) perform}},
			   {NULL,   0, NULL, NULL}};


void readboard(), changeboard(), writeboard(), swap();

enum cellstate {empty, occupied};
enum growthstate {dead, stable, growing};

int board[2][maxsize][maxsize]; /* the actual board, empty/occupied */
int generation;			/* generation number of the current board */
int new, old;			/* index to new and old boards */
int growthstate;		/* dead, stable, growing */
int size;			/* actual dimensions of the board */
int maxgeneration;		/* maximum number of generations */
int alivecount;			/* number of living organisms */

int main()
{
printf("THE GAME OF LIFE\n");
generation = 1;
alivecount = 0;

readboard();
init();

growthstate = growing;
while(growthstate == growing and generation < maxgeneration)
  {
    changeboard();
    generation++;
    swap(&old, &new);
    sleep(1);
  }
/*
switch(growthstate)
  {
  case stable: printf("stable\n"); break;
  case dead: printf("dead\n");break;
  case growing: printf("exceeded max number of generations \n"); break;
  }
*/
TANGOalgoOp(fnc, "END");
return 1;
}


void init()
{
TANGOalgoOp(fnc, "BEGIN");
TANGOalgoOp(fnc, "Init");	/* call animinit */
}

void readboard()
{
int x, y, i, j;

printf("Enter the max number of generations\n");
scanf("%d", &maxgeneration);
printf("Enter the board size ( <50 )\n");
scanf("%d", &size);
if(size > maxsize) 
  {
    printf("The board is too big. The size has been reset to %d\n", maxsize);
    size = maxsize;
  }

old = 0;
new = 1;

for(i = 0; i < size; i++)
  for(j = 0; j < size; j++)
    board[old][i][j] = empty;

printf("Enter a living spot ( 0 0  to complete it)\n");
scanf("%d", &x); scanf("%d", &y);

while(x and y)
  {
    if(x < 0 or x > size or y < 0 or y > size) 
      printf("Illegal input value %d %d, will be ignored\n", x, y);
    else 
      {
	board[old][x][y] = occupied;
	/* TANGOalgoOp(fnc, "Living", i, j); */
      }
    scanf("%d", &x); scanf("%d", &y);
  }
}

/* count the number of neighbours, keeping in mind that the cell might be a 
   boundary cell */
int countneighbour(x, y)
int x, y;
{
int down, left, right, up;	/* the four directions */
int i,j;			/* loop indices */
int count;			/* count of neighbours */

if(x > 0) left = -1; else left = 0;
if(x < (size-1)) right = 1; else right = 0;
if(y > 0) up = -1; else up = 0;
if(y < (size - 1)) down = 1; else down = 0;

count = 0;
for(i = left; i <= right; i++)
  for(j = up; j <= down; j++)
      if(board[old][x+i][y+j] == occupied and
	 !((i == 0) and (j == 0)))
	count ++;		/* the last condition makes sure that 
			 we don't count the cell itself as a neighbour*/
return count;
}
  
void changeboard(i, j)
int i, j;
{
int change;			/* any change in previous iteration */
int number;			/* number of neighbours */

change = false;
for(i = 0; i < size; i++)
  for(j = 0; j < size; j++)
    {
      number = countneighbour(i, j);
      board[new][i][j] = empty;

      /* unoccupied with 3 neighbours */

      if(board[old][i][j] == empty and number == 3) /* birth */
	{
	  board[new][i][j] = occupied;
	  change = true;
	  alivecount++;
	  TANGOalgoOp(fnc, "Compose", i, j, occupied);
	}
      /* occupied with 0,1,4,5,6,7,8 or more neighbours, die */
      /* 0-1 die of isolation; 4-8 die of crowding */
      if(board[old][i][j] == occupied
	 and number != 2 and number != 3)
	{
	  board[new][i][j] = empty;
	  change = true;
	  alivecount--;
	  TANGOalgoOp(fnc, "Compose", i, j, empty);
	}
      /* occupied with 2 or 3 neighbours */
      if(board[old][i][j] == occupied and (number == 2 or number == 3))
	board[new][i][j] = occupied;
    }
growthstate = growing;
TANGOalgoOp(fnc, "PerformCompose", generation);
if(alivecount == 0) growthstate = dead; 
else if(!change) growthstate = stable;
}

void swap(x, y)
int *x, *y;
{
int temp;

temp = *x;
*x = *y;
*y = temp;
}
