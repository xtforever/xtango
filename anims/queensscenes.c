#include <stdio.h>
#include "xtango.h" 
#define OFFSET 0.1 
#define VISIBLE 1 
#define NOT_VISIBLE 0
#define NUMBLINKS 14

TANGO_IMAGE board[8][8];
TANGO_IMAGE queen[8];

int isodd(num)
int num;
{
return((num % 2) == 1);
}

int iseven(num)
int num;
{
return((num % 2) == 0);
}

/* Sets up the board */

void ANIMInit()
{
int xint, yint;
int i,j;
double x, y;
double side;

TANGO_LOC cloc;
TANGO_PATH null_path;
TANGO_TRANS plot_squares;
TANGO_TRANS plot_queens;

TANGOinit();

side = (1.0 - (2.0 * OFFSET)) / 8.0;

for (y = OFFSET, yint = 0; yint < 8; y+=side, yint++)
   for (x = OFFSET, xint = 0; xint < 8; x+=side, xint++)
         board[xint][yint] =
         TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,
                        x,
                        y,
                        VISIBLE,
                        TANGO_COLOR_BLACK,
                        side,
                        side,
                        ((isodd(xint) && isodd(yint)) || (iseven(xint) &&
                         iseven(yint))) ? 1.0 : 0.0 );

null_path = TANGOpath_null(1);

for (i = 0; i < 8; i++)
   for (j = 0; j < 8; j++)
      {
      plot_squares = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, 
                                       board[i][j], null_path);
      TANGOtrans_perform(plot_squares);
      }
      

for (i = 0; i < 8; i++)
   {
   cloc = TANGOimage_loc(board[i][0], TANGO_PART_TYPE_C);
   queen[i] = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,
                                TANGOloc_X(cloc),
                                TANGOloc_Y(cloc),
                                NOT_VISIBLE,
                                TANGO_COLOR_YELLOW,
                                side / 3.0,
                                1.0);
   }
   
for (i = 0; i < 8; i++)
   {
   plot_queens = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, 
                                    queen[i], null_path);
   TANGOtrans_perform(plot_queens);
   }
}




void ANIMShow(col)
   int col;
{
TANGO_PATH null_path;
TANGO_TRANS show_trans;

null_path = TANGOpath_null(1);
show_trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,
                               queen[col], null_path);
TANGOtrans_perform(show_trans);
}





void ANIMMove(row, col)
   int row, col;
{
TANGO_LOC from, to;

TANGO_PATH queen_path;
TANGO_TRANS queen_trans;

from = TANGOimage_loc(board[col][row - 1], TANGO_PART_TYPE_C);
to = TANGOimage_loc(board[col][row], TANGO_PART_TYPE_C);
queen_path = TANGOpath_motion(from, to, TANGO_PATH_TYPE_STRAIGHT);
queen_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, queen[col],
                                queen_path);

TANGOtrans_perform(queen_trans);

}





void ANIMRemv(col)
   int col;
{
TANGO_LOC from, to;

TANGO_PATH queen_path;
TANGO_TRANS queen_trans;
TANGO_PATH null_path;
TANGO_TRANS visi_trans;
TANGO_TRANS remove_trans;

double xoffset[2], yoffset[2];

xoffset[0] = xoffset[1] = yoffset[0] = 0.0;
null_path = TANGOpath_null(1);
visi_trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, 
                               queen[col], null_path);
                               
to = TANGOimage_loc(board[col][0], TANGO_PART_TYPE_C);
from = TANGOimage_loc(board[col][7], TANGO_PART_TYPE_C);
yoffset[1] = TANGOloc_Y(to) - TANGOloc_Y(from);
queen_path = TANGOpath_create(2, xoffset, yoffset);
queen_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,
                                queen[col], queen_path);

remove_trans = TANGOtrans_concatenate(2, visi_trans, queen_trans);

TANGOtrans_perform(remove_trans);
}





void ANIMFound()

{
TANGO_IMAGE message;
TANGO_PATH null_path;
TANGO_TRANS show_trans;
TANGO_TRANS blink_trans;

message = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .5, .95, NOT_VISIBLE, 
                  TANGO_COLOR_BLACK,  NULL, "FOUND ONE!!!", 1);

null_path = TANGOpath_null(1);
show_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, message, null_path);

TANGOtrans_perform(show_trans);

null_path = TANGOpath_null(222);
blink_trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, message, null_path);

TANGOtrans_perform(blink_trans);
}




void ANIMAnchor(col)
int col;
{
TANGO_PATH color_path;
TANGO_TRANS color_trans;

color_path = TANGOpath_color(TANGO_COLOR_RED);
color_trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, queen[col], 
                                color_path);

TANGOtrans_perform(color_trans);
}




void ANIMDeanchor(col)
int col;
{
TANGO_PATH color_path;
TANGO_TRANS color_trans;

color_path = TANGOpath_color(TANGO_COLOR_YELLOW);
color_trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, queen[col],
                                color_path);

TANGOtrans_perform(color_trans);
}

void ANIMShowprob(probrow, probcol, row, col)
int probrow;
int probcol;
int row;
int col;
{
TANGO_LOC start, end;
TANGO_IMAGE prob_line;
TANGO_PATH null_path;
TANGO_TRANS put_trans;
TANGO_TRANS blink_trans;

if (probrow > row)
   {
   start = TANGOimage_loc(queen[probcol], TANGO_PART_TYPE_NE);
   end = TANGOimage_loc(queen[col], TANGO_PART_TYPE_SW);
   }
else if (probrow < row)
   {
   start = TANGOimage_loc(queen[probcol], TANGO_PART_TYPE_SE);
   end = TANGOimage_loc(queen[col], TANGO_PART_TYPE_NW);
   }
else
   {
   start = TANGOimage_loc(queen[probcol], TANGO_PART_TYPE_E);
   end = TANGOimage_loc(queen[col], TANGO_PART_TYPE_W);
   }

prob_line = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
                              TANGOloc_X(start),
                              TANGOloc_Y(start),
                              NOT_VISIBLE,
                              TANGO_COLOR_BLUE,
                              (TANGOloc_X(end) - TANGOloc_X(start)),
                              (TANGOloc_Y(end) - TANGOloc_Y(start)),
                              .5,
                              1.0,
                              0);

null_path = TANGOpath_null(1);
put_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, prob_line, null_path);
TANGOtrans_perform(put_trans);

null_path = TANGOpath_null(NUMBLINKS);
blink_trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, prob_line, 
              null_path);
TANGOtrans_perform(blink_trans);
}
  

