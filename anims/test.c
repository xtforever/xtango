#include <stdio.h>
#include "xtango.h"

void scene1();

static NAME_FUNCT fcn[] = { {"scene1", 1, {VOID, (FPTR)scene1}},
			    {NULL,     0, NULL, NULL} };

void main() {
   TANGOalgoOp(fcn, "BEGIN");
   TANGOalgoOp(fcn, "scene1");
   TANGOalgoOp(fcn, "END");
   }
