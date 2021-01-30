/*****************************************************************************

	dfa	- Deterministic Finite Automaton simulator

	Scott Robertson

*****************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "xtango.h"
#include "dfa.h"

#define MAXSTATES	10

void ANIMInit(), ANIMVertex(), ANIMEdge(), ANIMFinal(), ANIMShorter(),
	ANIMBeginTraverse(), ANIMTraverse(), ANIMFlashDot();
 
static NAME_FUNCT fcn[] = { {"Init",         1,{VOID,     (FPTR)ANIMInit}},
                            {"Vertex",       1,{CHAR_PTR, (FPTR)ANIMVertex}},
                            {"Edge",         1,{CHAR_PTR, (FPTR)ANIMEdge}},
                            {"Final",        1,{CHAR_PTR, (FPTR)ANIMFinal}},
                            {"BeginTraverse",1,{VOID,   (FPTR)ANIMBeginTraverse}},
                            {"Traverse",     1,{VOID,     (FPTR)ANIMTraverse}},
                            {"FlashDot",     1,{VOID,     (FPTR)ANIMFlashDot}},
                            {NULL,     NULL,  NULL,           NULL} };

void initTransTable (table, start, final, num)
int table[MAXSTATES][2];
int *start, final[MAXSTATES];
int *num;
{
 FILE *in;
 int i;
 int currentState, nextState;
 enum outputType input;
 char *reply;

 /* read in vertices */

 printf ("Deterministic Finite Automaton Builder\n\n");
 printf ("Place DFA states on the work screen by clicking with the leftmost\n");
 printf ("mouse button. The first state shall be the start state.\n"); 
 printf ("Click on 'Stop' when you are finished.\n\n");
 *num = 0;

 while (1) {
   reply = TANGOalgoOp(fcn, "Vertex",*num);
   if (!reply) break;
   (*num)++;
 }
 
 /* hardcoded start state = 0 */

 *start = 0;

 /* read in edges */

 printf ("Create transitions by the following method:\n");
 printf ("    1. Click on the origination state.\n");
 printf ("    2. Click on the destination state.\n");
 printf ("    3. Click on a third intermediate point.\n");
 printf ("       (A transition arrow will be drawn from the origination\n");
 printf ("        state to the destination state, through the intermediate\n");
 printf ("        point. Note that the origination and destination states\n");
 printf ("        can be the same state.)\n");
 printf ("    3. Click on the button representing the input desired for");
 printf ("       this\n");
 printf ("       transition (A, B, or A/B).\n");
 printf ("\nPlease make sure that you create a DFA and not an NFA!\n\n");
 printf ("Click on 'Stop' when you are finished.\n\n");

 while (1) {
   reply = TANGOalgoOp(fcn, "Edge");
   if (reply && *reply) {
     sscanf (reply, "%d %d %d", &currentState, &nextState, &input);
     if (currentState == -1)
       break;
     if (input == ABout) {
       table[currentState][Aout] = nextState;
       table[currentState][Bout] = nextState;
     } 
     else
       table[currentState][input] = nextState;
   }
 }

 printf ("Click on states to tag them as final states.\n");
 printf ("Click on 'Stop' when you are finished.\n\n");

 for (i = 0; i < *num; i++) {
   final[i] = 0;
 }

 /* read in final states */

 while (1) {
   reply = TANGOalgoOp(fcn, "Final");
   if (!reply)
     break;
   sscanf (reply, "%d", &currentState);
   final[currentState] = 1;
 }

/*
 in = fopen ("table.dat", "r");

 fscanf (in, "%d", num);
 fscanf (in, "%d", start);

 for (i = 0; i < *num; i++) {
   fscanf (in, "%d", &currentState);
   fscanf (in, "%d", &(table[currentState][0]));
   fscanf (in, "%d", &(table[currentState][1]));
   fscanf (in, "%d", &(final[currentState]));
 }

 fclose (in);
*/
}

main ()
{
 int transTable[MAXSTATES][2]; /* state transition table */
 int start, final[MAXSTATES];  /* start state and final states */
 int numStates;
 int lastState, currentState;
 int input;
 char cinput;
 
 TANGOalgoOp(fcn, "BEGIN");
 TANGOalgoOp(fcn, "Init");

 initTransTable (transTable, &start, final, &numStates);

 TANGOalgoOp(fcn, "BeginTraverse", 0);

 currentState = start;

 printf ("\nEnter a string: ");

 while ((input = getchar()) != EOF) {
   if (!isspace(cinput = (char) input)) {
     switch (toupper(cinput)) {
	case 'A' :
	  input = 0;
	  break;
	case 'B' :
	  input = 1;
	  break;
	default :
	  printf ("Error: Invalid input character. Must be 'a' or 'b'.\n");
	  exit(0);
	  break;
     }

     lastState = currentState;
     currentState = transTable[currentState][input];
     TANGOalgoOp(fcn, "Traverse",lastState,currentState);
   }
   else if (input == '\n') {
     if (final[currentState]) {
       printf ("\nInput string accepted by DFA.\n");
     }
     else {
       printf ("\nInput string not accepted by DFA.\n");
     }
     TANGOalgoOp(fcn, "FlashDot");
     currentState = 0;
     TANGOalgoOp(fcn, "BeginTraverse", 0);
     printf ("\nEnter a string: ");
   }
 }

}
