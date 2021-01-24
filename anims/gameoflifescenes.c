/* Game of life program. Adapted from An Introduction to Programming and
   Problem Solving With Pascal, Second Edition, Schieder G.M. et. al. */
/* Author: Kiran Panesar */

#include <stdio.h>
#include "xtango.h"
#define usage 0.8		/* side of drawing is 0.8 */

void make_boxes(), compose(), perform(), init_compose(), junk_compose();
char * my_itoa();

TANGO_TRANS composed_trans;
TANGO_IMAGE text1;
enum cellstate {empty, occupied};

void animinit()
{
ASSOCinit();			/* initialize the associations */
make_boxes();			/* make the yellow boxes */
init_compose();			/* init the composed image to be null */
TANGOtrans_perform(composed_trans); /* display the box */
}


/* create squares on the screen, not displayed yet */

void make_boxes()
{
extern int size;		/* number of squares on board */
float width, top;		/* width of board and top position */
int i, j;			/* loop control */
float dx, dy;
float W;			/* width of each square */

TANGO_IMAGE square;

W = usage/size;

width = usage;

top = 0.5 - width/2.0;
dx = dy = 0;
for(i = 0; i < size; i++)
  {
    for(j = 0; j < size; j++)
      {
	square = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, top+dx, top+dy,
				   1,				 
				   TANGO_COLOR_YELLOW, W, W,
				   1.0000);
	ASSOCstore("ID", i, j, square);
	dx += W;
      }
    dy += W;
    dx = 0;
  }
}


void compose(i,j,k)
int i, j, k;
/* i, j are the coordinates and k =0 dead and k = 1 living */
{
TANGO_IMAGE square;
TANGO_PATH yellow_path, blue_path;
TANGO_TRANS change_color;

square = (TANGO_IMAGE)ASSOCretrieve("ID", i, j);
yellow_path = TANGOpath_color(TANGO_COLOR_YELLOW);
blue_path = TANGOpath_color(TANGO_COLOR_BLUE);

if(k==occupied)			/* living */
  change_color = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,
				   square, blue_path);
else				/* dead */
  change_color = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,
				   square, yellow_path);

composed_trans = TANGOtrans_compose(2, change_color, composed_trans);
TANGOtrans_free(1, change_color);
}



void perform(i)
{
TANGOtrans_perform(composed_trans); /* perform the changes */
init_compose();			/* reset composed transition */
}


void junk_perform(i)
int i;
{
TANGO_PATH blue_path;
TANGO_TRANS delay1, visible;
char text[20];

strcpy(text, my_itoa(i));

text1 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.20, 0.92, 1,
			    TANGO_COLOR_RED, NULL, "Generation    ", 0);
blue_path = TANGOpath_color(TANGO_COLOR_BLUE);
visible = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, text1, blue_path);
TANGOtrans_perform(visible);	/* make the previous one invisible */

TANGOtrans_perform(composed_trans);

text1 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.20, 0.92, 1,
			    TANGO_COLOR_RED, NULL, text, 0);

TANGOtrans_perform(visible);	/* display the current generation # */
init_compose();			/* reset composed transition */
}

char * my_itoa(i)
int i;
{
char gen[20];
char no[4];


no[0] = (char)' ';
no[1] = (char)(i / 10) + '0';
no[2] = (char)(i % 10) + '0';
no[3] = NULL;

strcpy(gen, "Generation");
strcat(gen, no);

return(gen);
}





/* initialize the composed path to a transition that does nothing */
/* called at each generation */
void init_compose()
{
TANGO_PATH path1;
TANGO_IMAGE square;

path1 = TANGOpath_color(TANGO_COLOR_RED);
square = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.0626, 0.2642, 0, TANGO_COLOR_RED, 0.0861, 0.0939, 1.0000);
composed_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, square, path1);

}

