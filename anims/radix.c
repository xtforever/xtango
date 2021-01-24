/*   RADIX SORT  developed by Mary Ann Frogge   */

#include <malloc.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "xtango.h"
#include "radix.h"

void ANIMInit(), ANIMDraw(), ANIMToQueue(), ANIMMoveBack();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Display",     1, {VOID, (FPTR)ANIMDraw}},
                            {"ToQueue", 1, {VOID, (FPTR)ANIMToQueue}},
                            {"MoveBack", 1, {VOID, (FPTR)ANIMMoveBack}},
			    {NULL,	 0, NULL, NULL} };




int factor = 10, largest = 0, total;

/* Read in list size and contents */
void get_numbers()
{
  int i;
  struct element *tmp;

  printf("Radix Sort\n");
  printf("\nThis animation looks best with less than 15 numbers.");
  printf("\nHow many numbers in the array? ");
  scanf("%d", &total );
  if( total > 25 )
  {
    printf("\nNo more than 25 elements are allowed.  Total changed to 25.");
    total = 25;
  }
  printf("\nPlease input each number and hit return. \n> ");

  if( (tmp = malloc(sizeof(struct element))) == NULL )
  {
    printf("\nMemory cannot be allocated - exiting");
    exit(0);
  }
  scanf("%d", &tmp->num);
  largest = tmp->num;
  tmp->tag = 0;
  tmp->next = NULL;
  head = tail = tmp;

  for( i=1; i<total; i++ )
  {
    if ( (tmp = malloc( sizeof(struct element))) == NULL ) 
    {
      printf("\nMemory cannot be allocated - exiting");
      exit(0);
    } 
    tmp->next = NULL;
    printf("\n> ");
    scanf("%d", &tmp->num);
    tmp->tag = i;
    if( tmp->num > largest )
      largest = tmp->num;
    tail->next = tmp;
    tail = tmp;
  }
}


/* This function implements one pass of the sort.  It is called for each
   digit by sort().  */
void sort_list()
{
  int cnt, rem;

  tail = NULL;
  cnt = 0;
  /*  for each element, find the remainder index and put the element in
      the correct queue.  Call TANGO routine to move element to queue
      on screen.  */
  while( head != NULL )
  {
    if( head->num < (factor/10) )
      rem = 0;
    else
      rem = (head->num%factor)/(factor/10);
    if( queue[rem] == NULL )
    {
      queue[rem] = head;
    }
    else
    {
      queue[rem+10]->next = head;
    }
    queue[rem+10] = head;
    head = head->next;
    queue[rem+10]->next = NULL;
    TANGOalgoOp( fnc, "ToQueue", cnt, rem );
    cnt++;
  }
}

/* This function calls sorting passes for each factor of 10 in the list. */
void sort()
{
  int i;
  struct element *cur;

  /*  For each iteration on list, make a sorting pass, move the elements 
      from the remainder queues back to the main queue in order, and call
      TANGO routine to animate moving elements back. */
  while( factor <= largest*10 )
  {
    sort_list();
    for( i=0; i<10; i++ )
    {
      if( queue[i] != NULL )
      {
	if( head == NULL )
	{
	  head = queue[i];
	}
	else
	{
	  tail->next = queue[i];
	}
	tail = queue[i+10];
	queue[i] = queue[i+10] = NULL;
      }
    }
    factor *= 10;
    TANGOalgoOp( fnc, "MoveBack" );
  }

}



void main()
{
  int i;
  struct element  *tmp;

  TANGOalgoOp(fnc, "BEGIN");
  TANGOalgoOp(fnc, "Init"); 

  head = tail = NULL;
  for( i=0; i<20; i++ )
    queue[i] = NULL;

  get_numbers();
  TANGOalgoOp( fnc, "Display" );
  sort();


  TANGOalgoOp(fnc, "END");
}



