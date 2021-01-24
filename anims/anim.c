#include <stdio.h>
#include "xtango.h"

void scene1(), scene2(), scene3(), scene5(), scene6(), scene7();
char *scene4();

static NAME_FUNCT fnc[] = { {"scene1", 1, {VOID,     (FPTR)scene1}},
		      	    {"scene2", 1, {VOID,     (FPTR)scene2}},
		       	    {"scene3", 1, {VOID,     (FPTR)scene3}},
		       	    {"scene4", 1, {CHAR_PTR, (FPTR)scene4}},
		       	    {"scene5", 1, {VOID,     (FPTR)scene5}},
		       	    {"scene6", 1, {VOID,     (FPTR)scene6}},
		       	    {"scene7", 1, {VOID,     (FPTR)scene7}},
			    {NULL,     0, NULL,     NULL} };

main()
{
   char *str;

   TANGOalgoOp(fnc, "BEGIN");
   TANGOalgoOp(fnc, "scene1");
   TANGOalgoOp(fnc, "scene2");
   TANGOalgoOp(fnc, "scene3");
   str = TANGOalgoOp(fnc, "scene4");
   printf("->%s<-\n", str);
   TANGOalgoOp(fnc, "scene5");
   TANGOalgoOp(fnc, "scene6");
   TANGOalgoOp(fnc, "scene7");
   TANGOalgoOp(fnc, "END");
}
