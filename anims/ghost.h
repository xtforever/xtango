#include <math.h>
#define MAXPTS 50
#define MAX_XY 50
typedef struct pointtype {
  int x;
  int y;
  char who;
  double angle;
} pointtype;

typedef pointtype pointlist[MAXPTS];
