/* PROGRAM TO CONVERT FROM INFIX TO POSTFIX (Reverse Polish Notation) */
/* AUTHOR: Sougata Mukherjea */

#include <stdio.h>
#include "xtango.h"

#define MAXEXP 80 /* maximum expression length */
#define STACKLIMIT 10 /*maximum stack size */
#define TRUE 1
#define FALSE 0

/* The animation routines */
void AnimInit(),AnimOut(),AnimPush(),AnimBlink(),AnimInvisible();
void AnimPointer(),AnimPop(),AnimPopOut(),AnimError();

static NAME_FUNCT fcn[] = { {"Init", 1, {VOID, (FPTR)AnimInit}},
                            {"Out", 1, {VOID, (FPTR)AnimOut}},
                            {"Push", 1, {VOID, (FPTR)AnimPush}},
                            {"Blink", 1, {VOID, (FPTR)AnimBlink}},
                            {"Invisible", 1, {VOID, (FPTR)AnimInvisible}},
                            {"Pointer", 1, {VOID, (FPTR)AnimPointer}},
                            {"Pop", 1, {VOID, (FPTR)AnimPop}},
                            {"PopOut", 1, {VOID, (FPTR)AnimPopOut}},
                            {"Error", 1, {VOID, (FPTR)AnimError}},
                            {NULL,   0,  NULL, NULL} };

char stack[STACKLIMIT]; /* the stack */
int error,top,stackfull,stackempty; /* stack variables */

int priority (op)
/*function to find the priority of an operator*/
char*  op;
{
 if (!(strncmp(op,"*",1)) || !(strncmp(op,"/",1)))
   return(2);
 else if (!(strncmp(op,"+",1)) || !(strncmp(op,"-",1)))
        return(1);
      else if (!(strncmp(op,"(",1)))
            return(0);
}

void push(item)
/*procedure to push an item into the stack*/
char item;
{ if (top == STACKLIMIT)
    { stackfull = TRUE;
      error = TRUE; }
 else 
     {  stackempty = FALSE;
        top++;
        stack[top] = item;
      }}

char pop()
/*procedure to pop an item from the stack*/
{
char item;
if (top == 0)
   stackempty = TRUE;
else {
   stackfull = FALSE;
   item = stack[top];
   top--;
   return(item);
   }}
 

main()
{
int in,out,donepop;
char expr[MAXEXP]; /* the expression*/
int len; 
char token,temptoken;  


/* get the input */
printf("CONVERTING INFIX TO POSTFIX\n");
printf("ENTER THE INFIX EXPRESSION (without blanks): ");
scanf("%s",expr); 
len = strlen(expr);

TANGOalgoOp(fcn, "BEGIN");
TANGOalgoOp(fcn, "Init",expr,len);

/* initialise */
top = 0;
stackempty = TRUE;
error = FALSE;
in=0;

/* begin conversion to postfix */
while ((in < len) && !(error))
  { token = expr[in];
    if (!(strncmp(&token,"(",1))) /* token = ( */
        { TANGOalgoOp(fcn, "Push",expr,in);
          push(token);}
    else if (!(strncmp(&token,")",1))) /* token = ) */
            { do  
               { temptoken = pop();
                 if (stackempty)
                    error = TRUE; 
                 else { TANGOalgoOp(fcn, "Blink",expr);
                        if (strncmp(&temptoken,"(",1))
                            TANGOalgoOp(fcn, "PopOut",expr);/*output the temptoken*/
                        else TANGOalgoOp(fcn, "Pop",expr);}}/*don't output the temptoken*/
               while (strncmp(&temptoken,"(",1) && !error);
               TANGOalgoOp(fcn, "Invisible",expr,in);}
            else if (!(strncmp(&token,"+",1)) ||
                     !(strncmp(&token,"-",1)) ||
                     !(strncmp(&token,"*",1)) ||
                     !(strncmp(&token,"/",1))) /* token = an operator */
                     { donepop = FALSE;
                       do 
                         { temptoken = pop();
                           if (stackempty)
                              donepop = TRUE;
                           else { TANGOalgoOp(fcn, "Blink",expr);
                                  if (priority(&token) <= priority(&temptoken))
                                     TANGOalgoOp(fcn, "PopOut",expr);/*output the temptoken*/
                                 else { push(temptoken);/*push the temptoken*/
                                        donepop = TRUE; }}}
                           while (!donepop);
                        TANGOalgoOp(fcn, "Push",expr,in);/*push the token*/
                        push(token);  }
                   else TANGOalgoOp(fcn, "Out",expr,in);/* token = an operand, output it */
     in++;/* get next input character */
     TANGOalgoOp(fcn, "Pointer",expr,in,len);}

if (!error)
   while (!stackempty)
     { /* output remaining stack characters */
       token = pop();
       if (!stackempty)
         { TANGOalgoOp(fcn, "Blink",expr);
           if (strncmp(&token,"(",1))
            TANGOalgoOp(fcn, "PopOut",expr);/*output the token*/
           else { error = TRUE;/*ERROR!*/
                  TANGOalgoOp(fcn, "Pop",expr);
                  break;}}}

if (error)
  TANGOalgoOp(fcn, "Error");/* print error message */

TANGOalgoOp(fcn, "END");
}
 



