/* Combsort, implemented by Kenneth Moorman */


#include <stdio.h>
#include "xtango.h"

#define SHRINKFACTOR 1.3

void ANIMInit(), 
     ANIMInput(), ANIMInput2(),
     ANIMDraw(), ANIMDraw2(), 
     ANIMCompare(), ANIMCompare2(),
     ANIMExchange(), ANIMExchange2(),
     ANIMIn_place();

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Input",    1, {VOID, (FPTR)ANIMInput}},
			    {"Input2",   1, {VOID, (FPTR)ANIMInput2}},
			    {"Draw",     1, {VOID, (FPTR)ANIMDraw}},
			    {"Draw2",    1, {VOID, (FPTR)ANIMDraw2}},
                            {"Compare",  1, {VOID, (FPTR)ANIMCompare}},
                            {"Compare2", 1, {VOID, (FPTR)ANIMCompare2}},
			    {"Exchange", 1, {VOID, (FPTR)ANIMExchange}},
			    {"Exchange2", 1, {VOID, (FPTR)ANIMExchange2}}, 
                            {"In place", 1, {VOID, (FPTR)ANIMIn_place}},
			    {NULL,	 0, NULL, NULL} };


int list1[1000], list2[1000];

main()
{
	int i, number;
	extern int list1[], list2[];
	
	TANGOalgoOp(fnc, "BEGIN");

        printf("Combsort\n");
	printf("How many elements? ");
	scanf("%d", &number);
	
	TANGOalgoOp(fnc, "Init");
	
	printf("Enter the elements\n");
   	for (i=0; i<number; i++) {
		scanf("%d",&list1[i]);
		list2[i] = list1[i];	
		TANGOalgoOp(fnc, "Input", list1, i, list1[i]);
      	}

	both_sort(number);
}


both_sort(size)
int size;
{
	int switches, hold, i, j, top, gap, flag = 0;
	int comb_switches, bubble_switches;
	int j1, j2;
	extern int list1[], list2[];
	
	TANGOalgoOp(fnc, "Draw", list1, size);
	gap=size;
	
	do {
		gap = gap * 8 / 11;
		switch (gap) {
			case 0:
				gap = 1;
				break;
			case 9: 
			case 10:
			case 11:
				break;
			default:
				break;
		}
		
		comb_switches = 0;
		
		top = size - gap;
		
		for (i=0;i<top;++i) {
			j1 = i + gap;
			
			/* check for need to swap in comb */
			if (i < top)
				TANGOalgoOp(fnc, "Compare", list1, i, list1, j1);	
			if ((i < top) && (list1[i] > list1[j1])) {  
				hold = list1[i];
				list1[i] = list1[j1];
				list1[j1] = hold;
				++comb_switches;
				TANGOalgoOp(fnc, "Exchange", list1, i, list1, j1);	
			}
			
		}
	} while (comb_switches ||  (gap > 1));

	TANGOalgoOp(fnc, "END");
}
		

