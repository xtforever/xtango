/* Scott Coulter
Notes on TANGO Animation
ICS 8113
March 16, 1990


Notes on "shufflescenes"--a TANGO animation of a card deck being shuffled.

  It would appear that many varieties of sorting algorithms have been
implemented as TANGO animations.  I hope, however, to be the first person
to animate an "unsorting" algorithm.  Specifically, I have designed an
animation for a simple-but-effective playing-card shuffling algorithm.  
I chose this algorithm for two reasons:  it has a clear visual component,
and I hoped to explore uses of color in TANGO animations.  My original intent
was to display the cards in a rainbow spectrum (their initially sorted order),
then show the rainbow being scrambled.  However, since TANGO cannot yet support
that many colors, my current animation scene uses one color for each suit,
and labels each card with one character for the value.  I think that this will
be sufficiently clear to show the mixing of the cards.  
  There are three animation routines.  ANIMInit puts the 52 rectangles in a row,
with their values labeled.  ANIMExchange exchanges the place of two cards,
and ANIMInPlace draws a white highlight around cards which are in place.
/*   TANGO animation scenes for card deck shuffle algorithm  *
 *   Scott Coulter					     *
 *   ICS 8113J, Program Visualization Seminar		     *
 *   March 16, 1990					     */

#include <stdio.h>
#include <xtango.h>

#define WIDTH 0.01
#define HEIGHT 0.1
#define STARTX 0.05
#define STARTY 0.6
#define UPY 0.4
#define SPEED HEIGHT/5.0
#define ID 0

TANGO_IMAGE rect[52];
TANGO_IMAGE value_img[52];
char VALUES[] = {"A23456789TJQK"};

/*  set up array of 52 "cards" in order; one color per suit */
void
ANIMInit()
{
  TANGO_LOC where, cent;
  TANGO_PATH path;
  TANGO_TRANS delay;
  char te[2];

  int i;
  double lx, ly, cx, cy;

  /* horizontal row of locations for the cards */
  ASSOCmake("position",2);
  TWISTcreate_loc_array("position", ID, 52, 1, STARTX, STARTY, WIDTH+0.008);

  te[1] = '\0';
  /* create 13 rectangles for each suit (each suit in different color) */
  for (i = 0; i < 13; i++) {
    where = (TANGO_LOC) ASSOCretrieve("position", ID, i); 
    TANGOloc_inquire(where, &lx, &ly);
    rect[i] = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, lx, ly, 1,
			     TANGO_COLOR_RED, WIDTH, HEIGHT, 1.0);
    cent = TANGOimage_loc(rect[i], TANGO_PART_TYPE_C);
    TANGOloc_inquire(cent, &cx, &cy); 

   /* put text value of card at center of card */
    te[0] = VALUES[i%13];
    value_img[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, cx, cy, 1,
                                  TANGO_COLOR_BLACK, NULL, te, 1);
  }
  for (i = 13; i < 26; i++) {
    where = (TANGO_LOC) ASSOCretrieve("position", ID, i); 
    TANGOloc_inquire(where, &lx, &ly);
    rect[i] = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, lx, ly, 1,
			     TANGO_COLOR_BLUE, WIDTH, HEIGHT, 1.0);
    cent = TANGOimage_loc(rect[i], TANGO_PART_TYPE_C);
    TANGOloc_inquire(cent, &cx, &cy); 

   /* put text value of card at center of card */
    te[0] = VALUES[i%13];
    value_img[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, cx, cy, 1,
                                  TANGO_COLOR_BLACK, NULL, te, 1);
  }
  for (i = 26; i < 39; i++) {
    where = (TANGO_LOC) ASSOCretrieve("position", ID, i); 
    TANGOloc_inquire(where, &lx, &ly);
    rect[i] = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, lx, ly, 1,
			     TANGO_COLOR_ORANGE, WIDTH, HEIGHT, 1.0);
    cent = TANGOimage_loc(rect[i], TANGO_PART_TYPE_C);
    TANGOloc_inquire(cent, &cx, &cy); 

   /* put text value of card at center of card */
    te[0] = VALUES[i%13];
    value_img[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, cx, cy, 1,
                                  TANGO_COLOR_BLACK, NULL, te, 1);
  }
  for (i = 39; i < 52; i++) {
    where = (TANGO_LOC) ASSOCretrieve("position", ID, i); 
    TANGOloc_inquire(where, &lx, &ly);
    rect[i] = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, lx, ly, 1,
			     TANGO_COLOR_GREEN, WIDTH, HEIGHT, 1.0);
    cent = TANGOimage_loc(rect[i], TANGO_PART_TYPE_C);
    TANGOloc_inquire(cent, &cx, &cy); 

 /* put text value of card at center of card */
    te[0] = VALUES[i%13];
    value_img[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, cx, cy, 1,
                                  TANGO_COLOR_BLACK, NULL, te, 1);
  }
/* make everything appear */
  path = TANGOpath_null(5);
  delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
  TANGOtrans_perform(delay);
  TANGOpath_free(1, path);
  TANGOtrans_free(1, delay);
}


/* exchange two cards; card 'a' will be the rightmost card which is not
   "in-place", card 'b' will be somewhere to the left of it.  */
void
ANIMExchange(a, b)
  int a, b;
{
  TANGO_IMAGE temp_rect,temp_value;
  TANGO_LOC  loc_top, loc_a, loc_b;
  TANGO_PATH up_path, down_path, path_a, path_b, as_path, bs_path;
  TANGO_TRANS move_a, move_text_a, move_b, move_text_b, compose;

  loc_a = TANGOimage_loc(rect[a], TANGO_PART_TYPE_SW);
  loc_b = TANGOimage_loc(rect[b], TANGO_PART_TYPE_SW);
  loc_top = TANGOimage_loc(rect[a], TANGO_PART_TYPE_NW);
  up_path = TANGOpath_distance(loc_a, loc_top, SPEED);
  down_path = TANGOpath_distance(loc_top, loc_a, SPEED);
  path_a = TANGOpath_motion(loc_a, loc_b, TANGO_PATH_TYPE_COUNTERCLOCKWISE);
  path_b = TANGOpath_motion(loc_b, loc_a, TANGO_PATH_TYPE_CLOCKWISE);
  as_path = TANGOpath_concatenate(3, up_path, path_a, down_path);
  bs_path = TANGOpath_concatenate(3, up_path, path_b, down_path);
  move_a = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect[a], as_path);
  move_b = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect[b], bs_path);
  move_text_a = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, value_img[a], as_path);
  move_text_b = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, value_img[b], bs_path);
  compose = TANGOtrans_compose(4, move_a, move_b, move_text_a, move_text_b);
  TANGOtrans_perform(compose);
  temp_rect = rect[a];
  rect[a] = rect[b];
  rect[b] = temp_rect;
  temp_value = value_img[a];
  value_img[a] = value_img[b];
  value_img[b] = temp_value;
  TANGOpath_free(6, path_a, path_b, up_path, down_path, as_path, bs_path);
  TANGOtrans_free(5, move_a, move_b, move_text_a, move_text_b, compose);
}

