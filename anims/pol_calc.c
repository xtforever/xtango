/*************************

   Reverse Polish Calculator
           by
      Brad Topol

***************************/


#include <stdio.h>
#include <math.h>
#include "xtango.h"

#define MAXOP 10
#define NUMBER '0'
#define MAXVAL 9

void ANIMInit (), ANIMPushStack(), ANIMClearStack(), ANIMCurrentChar();
void ANIMMerge ();
double val[MAXVAL];
int getop();
void push();
double pop();
int sp = 0;
static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"PushStack", 1, {VOID, (FPTR)ANIMPushStack}},
			    {"ClearStack", 1, {VOID, (FPTR)ANIMClearStack}},
			    {"Merge", 1, {VOID, (FPTR)ANIMMerge}},
			    {"CurrentChar", 1, {VOID, (FPTR)ANIMCurrentChar}},
			    {NULL,      0, NULL, NULL} };

main ()
{
	int type;
	double op2;
	char s[MAXOP];
	TANGOalgoOp(fnc, "BEGIN");
 	TANGOalgoOp(fnc, "Init");  
 	printf("Please enter an expression to be calculated and hit return.\n");
        printf(" An example is 1 2 + 3 4 * + \n");
	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			TANGOalgoOp(fnc, "CurrentChar", s);
			push(atof(s));
			break;
		case '+':
			TANGOalgoOp(fnc, "CurrentChar", "+");
			TANGOalgoOp(fnc, "Merge", '+');
			push(pop() + pop());
			break;
		case '*':
			TANGOalgoOp(fnc, "CurrentChar", "*");
			TANGOalgoOp(fnc, "Merge", '*');
			push(pop() * pop());
			break;
		case '-':
			TANGOalgoOp(fnc, "CurrentChar", "-");
			TANGOalgoOp(fnc, "Merge", '-');
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			TANGOalgoOp(fnc, "CurrentChar", "/");
			TANGOalgoOp(fnc, "Merge", '/');
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else 
				printf("error: zero divisor Result is UNDEFINED\n");
			break;
		case '\n':
			TANGOalgoOp(fnc, "ClearStack");
			printf("\n**********The result is ==>%.8g\n", pop()); 
 	                printf("\nPlease enter an expression to be calculated and hit return.\n");
                        printf(" An example is 1 2 + 3 4 * + \n");
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
 	}
	TANGOalgoOp(fnc, "END");
	return 0;
}

void push(f)
double f;
{
	if (sp < MAXVAL) {
		val[sp++] = f;
		TANGOalgoOp(fnc, "PushStack", f);
	}
	else
		printf("error: stack full, can't push %g\n", f);
}

double pop()
{
	if (sp > 0) {
		return val[--sp];
	}
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}
#include <ctype.h>

int getch();
void ungetch();

int getop(s)
char s[];
{
	int i, c;
	while ((s[0] = c = getch()) == ' ' || c == '\t');
    s[1] = '\0';
	if (!isdigit(c) && c != '.')
		return c;
	i = 0;
	if (isdigit(c))
           while (isdigit(s[++i] = c = getch()));
        if (c == '.')
           while (isdigit(s[++i] = c = getch()));
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}
	
#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch()
{
	return (bufp > 0)  ? buf[--bufp] : getchar();
}

void ungetch (c) 
int c;
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
	
