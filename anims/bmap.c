#include <stdio.h>
#include "xtango.h"

void scene();

static NAME_FUNCT fcn[] = { {"scene", 1, {VOID, (FPTR)scene}},
			    {NULL,     0, NULL, NULL} };

void main() {
   TANGOalgoOp(fcn, "BEGIN");
   TANGOalgoOp(fcn, "scene");
   TANGOalgoOp(fcn, "END");
   }
