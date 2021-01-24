/* Calculation factorial by Vernard Martin */

#include <stdio.h>
#include <math.h>
#include "xtango.h"


void ANIMInit(), ANIM_SC_Check(), ANIM_SC_Found(), ANIM_Pop_Stack(),
     ANIM_Step_Closer(), ANIM_Recurse();

int factorial();  

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
                            {"StopCondCheck", 1, {VOID, (FPTR)ANIM_SC_Check}},
                            {"StopCondFound", 1, {VOID, (FPTR)ANIM_SC_Found}},
                            {"PopStack", 1, {VOID, (FPTR)ANIM_Pop_Stack}},
                            {"StepCloser", 1, {VOID, (FPTR)ANIM_Step_Closer}},
                            {"Recurse", 1, {VOID, (FPTR)ANIM_Recurse}},
                            {NULL,       0, NULL, NULL} };

 
main()   
{
   int num;           /* global value of int to be factorialized */

   printf("Let's calculate n!\n\n");

   TANGOalgoOp(fnc, "BEGIN");

   printf("Enter an integer in the range of 1 - 10 \n");
   
   scanf("%d", &num);

   TANGOalgoOp(fnc, "Init",num);

   printf("Factorial of %d is %d \n", num ,factorial(num));

   printf("Done. \n");

   TANGOalgoOp(fnc, "END");

}


int factorial ( num )
int num;   
{
 int ans;

TANGOalgoOp(fnc,"StopCondCheck",num);
if (num == 1)
    {
    TANGOalgoOp(fnc,"StopCondFound");
    printf("Reached Stoping condition of Fact(1) \n");
    TANGOalgoOp(fnc,"PopStack",num);
    return(1);
    }
else
    {
    TANGOalgoOp(fnc,"StepCloser");
    TANGOalgoOp(fnc,"Recurse");
    printf("Recursing level %d \n", num);
    ans = num * factorial (num - 1);
    TANGOalgoOp(fnc,"PopStack",num);
    return (ans);
    }
}
