/* Chess Knight's tour by Cristina Fernandes */

#include <stdio.h>
#include "xtango.h" 

#define OFFSET 0.15

#define VISIBLE 1 
#define NOT_VISIBLE 0
#define EPS 0.01
#define MAX 10
#define MAX2 100
#define GREEN 1
#define RED 0 

TANGO_IMAGE bigsquare,
            knight,
            board[MAX][MAX],
            num[MAX2];

double vx[7], vy[7];


int black(row,col)
     int row, col;
/* Verify if (row,col) is a white or a black position. */
{
  return ( (row % 2) == (col % 2) );
}


void ANIMInit(dim)
     int dim;
/* Board construction, numbers construction */
/* and positioning of the knigth.           */
{
  int i, j, xint, yint;
  double x, y, side, unit;
  char str[3];
  
  TANGO_LOC loc;
  TANGO_PATH null_path;
  TANGO_TRANS knight_trans;
  
  /* Board construction */

  side = 1.0 - (2.0 * OFFSET);
  bigsquare = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,
				OFFSET-EPS, OFFSET-EPS,
				VISIBLE, TANGO_COLOR_BLACK,
				side+2*EPS, side+2*EPS, 1.0);
  TAPshow(bigsquare);
  
  side = side / dim;

  for (y = OFFSET, yint = 0; yint < dim; y+=side, yint++)
    for (x = OFFSET, xint = 0; xint < dim; x+=side, xint++) 
      {
	if (black(xint,yint))
           board[xint][yint] =
           TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,
			     x, y,
			     VISIBLE, TANGO_COLOR_BLACK,
			     side, side, 1.0);
	else board[xint][yint] =
	   TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,
			     x, y,
			     VISIBLE, TANGO_COLOR_WHITE,
			     side, side, 1.0);
      }
  
  for (i=0; i<dim; i++)
    for (j=0; j<dim;  j++)
      TAPshow(board[i][j]);

  /* Knight construction and positioning */

  loc = TANGOimage_loc(board[0][0], TANGO_PART_TYPE_C);
  x = TANGOloc_X(loc);
  y = TANGOloc_Y(loc);
  
  unit = side / 4.0;
  vx[0] = 0;       vy[0] = -2*unit;
  vx[1] = unit;    vy[1] = -2*unit;
  vx[2] = 2*unit;  vy[2] = -unit;
  vx[3] = unit;    vy[3] = -unit;
  vx[4] = unit;    vy[4] = 0;
  vx[5] = 2*unit;  vy[5] = unit;
  vx[6] = -unit;   vy[6] = unit;
    
  knight = TANGOimage_create(TANGO_IMAGE_TYPE_POLYGON, x, y,
			     NOT_VISIBLE, TANGO_COLOR_ORANGE,
			     8, vx, vy, 1.0);
  TAPshow(knight);

  knight_trans = TAPjump(knight, TANGO_PART_TYPE_C, loc);
  TANGOtrans_perform(knight_trans);
  knight_trans = TAPvis_toggle(knight);
  TANGOtrans_perform(knight_trans);

  /* Numbers construction */

  for (i=0; i<dim*dim; i++) 
    {
      sprintf(str,"%d",i+1);
      num[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 
				 NOT_VISIBLE, TANGO_COLOR_ORANGE,
				 "variable", str, 1.0);
      TAPshow(num[i]);
    }

  /* Moving knight to be in front of the others images. */

  null_path = TANGOpath_null(1);
  knight_trans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, knight, null_path);
  TANGOtrans_perform(knight_trans);  
}


void ANIMTwinkle(row, col, success)
     int row, col, success;
/* This routine makes a board position twinkle   */
/* in green color if success = 1, red otherwise. */
{
  int i;
    
  TANGO_LOC loc;
  TANGO_PATH bright_color, original_color;
  TANGO_TRANS bright, original;
  
  loc = TANGOimage_loc(board[col][row], TANGO_PART_TYPE_C);
  
  if (success) bright_color = TANGOpath_color(TANGO_COLOR_GREEN);
  else bright_color = TANGOpath_color(TANGO_COLOR_RED);
  
  /* Discovering the original color of this position. */
  if (black(row, col)) original_color = TANGOpath_color(TANGO_COLOR_BLACK);
  else original_color = TANGOpath_color(TANGO_COLOR_WHITE);
  
  bright = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,
			     board[col][row], bright_color);
  original = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,
			       board[col][row], original_color);
  /* Twinkle three times. */
  for (i=0; i<3; i++)
    {
      TANGOtrans_perform(bright);
      TANGOtrans_perform(original);
    }
}



void ANIMMove(row, col, m)
     int row, col, m;
/* Knight moving */
{
  TANGO_LOC to;
  TANGO_TRANS num_trans, knight_trans;

  ANIMTwinkle(row,col,GREEN);

  /* Marking the position with the number of the step. */

  to = TANGOimage_loc(knight, TANGO_PART_TYPE_C);

  num_trans = TAPjump(num[m-1],TANGO_PART_TYPE_C, to);
  TANGOtrans_perform(num_trans);

  num_trans = TAPvis_toggle(num[m-1]);
  TANGOtrans_perform(num_trans);

  /* Moving the knight. */

  to = TANGOimage_loc(board[col][row], TANGO_PART_TYPE_C);

  knight_trans = TAPtraverse(knight, TANGO_PART_TYPE_C, to, TANGO_PATH_TYPE_CLOCKWISE);
  TANGOtrans_perform(knight_trans);
}


void ANIMBack(row, col, m)
     int row, col, m;
/* Makes the knight going back the last movement. */
{
  TANGO_LOC to;
  TANGO_TRANS knight_trans, color_trans, flash_trans, trans, num_trans;
  
  to = TANGOimage_loc(board[col][row], TANGO_PART_TYPE_C);
  
  knight_trans = TAPtraverse(knight, TANGO_PART_TYPE_C, to,
			     TANGO_PATH_TYPE_COUNTERCLOCKWISE);
  color_trans = TAPcolor(knight, TANGO_COLOR_YELLOW);
  flash_trans = TAPflash(knight, 10);
  
  trans = TANGOtrans_compose(3, knight_trans, color_trans, flash_trans); 
  
  TANGOtrans_perform(trans);
  
  color_trans = TAPcolor(knight, TANGO_COLOR_ORANGE);
  TANGOtrans_perform(color_trans);

  /* Erase the number of the step of this position. */
  num_trans = TAPvis_toggle(num[m-1]);
  TANGOtrans_perform(num_trans);
}


void ANIMShrink(dim)
     int dim;
/* Reduce the size of the board. */
{
  int i, j, t;
  double newside, xx[1], yy[1];
    
  TANGO_LOC loc_NW, loc_SE, loc_C, new_loc_NW, new_loc_SE, new_loc_C;
  TANGO_PATH resize_path;
  TANGO_TRANS trans, num_trans, move_trans, resize_trans;

  /* Resizing the bigsquare */

  loc_NW = TANGOimage_loc(bigsquare, TANGO_PART_TYPE_NW);
  new_loc_NW = TANGOloc_create(TANGOloc_X(loc_NW)/2.0,
			       TANGOloc_Y(loc_NW)/2.0);
  move_trans = TAPjump(bigsquare, TANGO_PART_TYPE_NW, new_loc_NW);  

  newside = (1.0 - (2.0 * OFFSET)) / 2.0;
  xx[0] = -(newside+EPS);
  yy[0] = -(newside+EPS);
  
  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,
				   bigsquare, resize_path);
  trans = TANGOtrans_compose(2, move_trans, resize_trans);

  /* Resizing the knight */
  
  xx[0] = -vx[0]/2.1;
  yy[0] = -vy[0]/2.1;

  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1,
				   knight, resize_path);
  trans = TANGOtrans_compose(2, trans, resize_trans);

  xx[0] = -vx[1]/2.1;
  yy[0] = -vy[1]/2.1;
    
  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB2,
				   knight, resize_path);
  trans = TANGOtrans_compose(2, trans, resize_trans);

  xx[0] = -vx[2]/2.1;
  yy[0] = -vy[2]/2.1;
    
  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB3,
				   knight, resize_path);
  trans = TANGOtrans_compose(2, trans, resize_trans);

  xx[0] = -vx[3]/2.1;
  yy[0] = -vy[3]/2.1;
    
  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB4,
				   knight, resize_path);
  trans = TANGOtrans_compose(2, trans, resize_trans);

  xx[0] = -vx[4]/2.1;
  yy[0] = -vy[4]/2.1;
    
  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB5,
				   knight, resize_path);
  trans = TANGOtrans_compose(2, trans, resize_trans);

  xx[0] = -vx[5]/2.1;
  yy[0] = -vy[5]/2.1;
    
  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB6,
				   knight, resize_path);
  trans = TANGOtrans_compose(2, trans, resize_trans);

  xx[0] = -vx[6]/2.1;
  yy[0] = -vy[6]/2.1;
    
  resize_path = TANGOpath_create(1, xx, yy);
  resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB7,
				   knight, resize_path);
  trans = TANGOtrans_compose(2, trans, resize_trans);

  loc_C = TANGOimage_loc(knight, TANGO_PART_TYPE_C);
  new_loc_C = TANGOloc_create(TANGOloc_X(loc_C)/2.0+EPS,
			      TANGOloc_Y(loc_C)/2.0-EPS);
  move_trans = TAPjump(knight, TANGO_PART_TYPE_C, new_loc_C);  
  trans = TANGOtrans_compose(2, trans, move_trans);

  /* Resizing the small squares */

  newside = newside / dim;
  
  for (i=0, t=0; i<dim; i++)
    for (j=0; j<dim; j++, t++)
      {
	loc_C = TANGOimage_loc(num[t], TANGO_PART_TYPE_C);
	new_loc_C = TANGOloc_create(TANGOloc_X(loc_C)/2.0,
				    TANGOloc_Y(loc_C)/2.0);
	num_trans = TAPjump(num[t], TANGO_PART_TYPE_C, new_loc_C);

	loc_NW = TANGOimage_loc(board[i][j], TANGO_PART_TYPE_NW);
	new_loc_NW = TANGOloc_create(TANGOloc_X(loc_NW)/2.0,
				     TANGOloc_Y(loc_NW)/2.0);
	move_trans = TAPjump(board[i][j], TANGO_PART_TYPE_NW, new_loc_NW);

	loc_SE = TANGOimage_loc(board[i][j], TANGO_PART_TYPE_SE);
	xx[0] = -newside;
	yy[0] = -newside;

	resize_path = TANGOpath_create(1, xx, yy);
	resize_trans = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,
					 board[i][j], resize_path);
	trans = TANGOtrans_compose(4, trans, num_trans, move_trans, resize_trans);
      }
  TANGOtrans_perform(trans);  
}  
  

void ANIMTour(dim, Move, dx, dy)
     int dim, Move[MAX2], dx[8], dy[8];
/* Traverse a knight tour given by the arrays Move, dx and dy. */
{
  int i, j, t;
  double side, x, y;
  char str[3];
 
  TANGO_LOC to;
  TANGO_PATH null_path;
  TANGO_TRANS trans, move_trans, num_trans, view_trans;
  TANGO_IMAGE newnum, newbigsquare, newboard[MAX][MAX];
  
  ANIMShrink(dim);
  
  /* Copying board */

  newbigsquare = TANGOimage_copy(bigsquare);
  side = (1.0-(2.0*OFFSET))/2.0;
  side = side + OFFSET;
  
  to = TANGOloc_modify(TANGOimage_loc(bigsquare, TANGO_PART_TYPE_C), side, side);
  trans = TAPjump(newbigsquare, TANGO_PART_TYPE_C, to);
  
  for (i=0; i<dim; i++)
    for (j=0; j<dim; j++)
      {
	newboard[i][j] = TANGOimage_copy(board[i][j]);
	to = TANGOloc_modify(TANGOimage_loc(board[i][j],
		             TANGO_PART_TYPE_C), side, side);
	move_trans = TAPjump(newboard[i][j], TANGO_PART_TYPE_C, to);
	trans = TANGOtrans_compose(2, trans, move_trans);
      }

  null_path = TANGOpath_null(1);
  view_trans = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER, newbigsquare, null_path);
  trans = TANGOtrans_compose(2, trans, view_trans);

  TANGOtrans_perform(trans);

  /* Moving the knight. */
  
  to = TANGOimage_loc(newboard[0][0], TANGO_PART_TYPE_C);
  move_trans = TAPjump(knight, TANGO_PART_TYPE_C, to);

  view_trans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, knight, null_path);
  trans = TANGOtrans_compose(2, move_trans, view_trans);

  TANGOtrans_perform(trans);
  
  /* Performing the tour. */

  for (t=1, i=0, j=0; t<dim*dim; t++)
    {
      i = i+dx[Move[t]];
      j = j+dy[Move[t]];
      
      sprintf(str,"%d",t);
      TANGOloc_inquire(to, &x, &y);
      
      newnum = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 
				 VISIBLE, TANGO_COLOR_ORANGE,
				 "variable", str, 1.0);
      TAPshow(newnum);

      to = TANGOimage_loc(newboard[i][j], TANGO_PART_TYPE_C);
      move_trans = TAPtraverse(knight, TANGO_PART_TYPE_C,
			       to, TANGO_PATH_TYPE_CLOCKWISE);
      TANGOtrans_perform(move_trans);
    }
}

