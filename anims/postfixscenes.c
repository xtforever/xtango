/* PROGRAM TO CONVERT FROM INFIX TO POSTFIX (Reverse Polish Notation) */
/* AUTHOR: Sougata Mukherjea */

#include <stdio.h>
#include "xtango.h"

#define MYFONT  "variable" /*defining my font*/

int stackloc,outloc;
TANGO_IMAGE point;/* input pointer defined globally */

void AnimInit(expr,len)
/* procedure for initialisation */
char expr[];
int len;
{

#define STACKLIMIT 10;

static double stack_x[3] = {0.0000,0.1500,0.1500};
static double stack_y[3] = {0.3000,0.3000,0.0000};
int i;
char ch;
double x,y;

TANGO_LOC loc;
TANGO_IMAGE title,in_text, stack_text, out_text,temp_text;
TANGO_PATH path;
TANGO_TRANS trans;

ASSOCinit();
ASSOCmake("TEXT",2);
ASSOCmake("INLOC",2);
ASSOCmake("INPOINTER",2);
ASSOCmake("OUTLOC",2);
ASSOCmake("STACK",1);

title = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.1750, 0.0750, 1, TANGO_COLOR_RED, MYFONT, "CONVERSION FROM INFIX TO POSTFIX", 0);
in_text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.0450, 0.1500, 1, TANGO_COLOR_BLUE, MYFONT, "INPUT", 0);
stack_text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.4000, 0.4000, 1, TANGO_COLOR_BLUE, MYFONT, "STACK", 0);
out_text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.7500, 0.1500, 1, TANGO_COLOR_BLUE, MYFONT, "OUTPUT", 0);

stackloc = STACKLIMIT;
TWISTcreate_image_array(NULL,expr,stackloc,TANGO_IMAGE_TYPE_RECTANGLE,0,0,0.4000,0.4300,NULL,0.1000,NULL,0.0500,0.0000,1,TANGO_COLOR_BLACK,0.0);
TWISTcreate_loc_array("INLOC",expr,len,1,0.0450,0.2000,0.0200);
TWISTcreate_loc_array("INPOINTER",expr,len,1,0.0450,0.2100,0.0200);
TWISTcreate_loc_array("OUTLOC",expr,len,1,0.7500,0.2000,0.0200);
point = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,0.0450,0.2100,1,TANGO_COLOR_BLACK,0.0020,0.0400,0.0000,1.0000,2);
outloc=0;

/* Print the input characters */
for (i=0;i<len;i++)
  { loc = (TANGO_LOC) ASSOCretrieve("INLOC",expr,i);
    TANGOloc_inquire(loc,&x,&y);
    strncpy(&ch,&expr[i],1);
    temp_text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1, TANGO_COLOR_MAROON, MYFONT, &ch, 0);
   ASSOCstore("TEXT",expr,i,temp_text);
}

path = TANGOpath_null(1);
trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(trans);
TANGOpath_free(1,path);
TANGOtrans_free(1,trans);
}

void AnimOut(expr,index)
/* procedure for outputting a character of input */
char expr[];
int index;
{
TANGO_IMAGE token;
TANGO_LOC from_loc,to_loc;
TANGO_PATH path;
TANGO_TRANS move;

token = (TANGO_IMAGE) ASSOCretrieve("TEXT",expr,index);
from_loc = (TANGO_LOC) ASSOCretrieve("INLOC",expr,index);
to_loc = (TANGO_LOC) ASSOCretrieve("OUTLOC",expr,outloc);
outloc++;
path = TANGOpath_motion(from_loc,to_loc,TANGO_PATH_TYPE_CLOCKWISE);
move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,token,path);
TANGOtrans_perform(move);

TANGOpath_free(1,path);
TANGOtrans_free(1,move);
}

void AnimPointer(expr,index,len)
/* procedure for moving the input pointer */
char expr[];
int index;
{
TANGO_LOC from_loc,to_loc;
TANGO_PATH path;
TANGO_TRANS trans;;

if (index < len)/* end of input not reached */
  { from_loc = (TANGO_LOC) ASSOCretrieve("INPOINTER",expr,(index - 1));
    to_loc = (TANGO_LOC) ASSOCretrieve("INPOINTER",expr,index);
    path = TANGOpath_motion(from_loc,to_loc,TANGO_PATH_TYPE_STRAIGHT);
    trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,point,path);}
else { path = TANGOpath_null(1);/* end of input reached */
       trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,point,path);}

TANGOtrans_perform(trans);
TANGOpath_free(1,path);
TANGOtrans_free(1,trans);
}

void AnimInvisible(expr,index)
/* procedure for making an input character invisible */
char expr[];
int index;
{
TANGO_IMAGE token;
TANGO_PATH path;
TANGO_TRANS trans;

token = (TANGO_IMAGE) ASSOCretrieve("TEXT",expr,index);
path = TANGOpath_null(1);
trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,token,path);
TANGOtrans_perform(trans);

TANGOpath_free(1,path);
TANGOtrans_free(1,trans);
}

void AnimPush(expr,index)
/* procedure for pushing a character to the stack */
char expr[];
int index;
{
TANGO_IMAGE token,rect;
TANGO_LOC from_loc,to_loc;
TANGO_PATH path;
TANGO_TRANS move;

stackloc--;
token = (TANGO_IMAGE) ASSOCretrieve("TEXT",expr,index);
from_loc = (TANGO_LOC) ASSOCretrieve("INLOC",expr,index);
rect = (TANGO_IMAGE) ASSOCretrieve("ID",expr,stackloc);
to_loc = TANGOimage_loc(rect,TANGO_PART_TYPE_C); 
path = TANGOpath_motion(from_loc,to_loc,TANGO_PATH_TYPE_STRAIGHT);
move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,token,path);
TANGOtrans_perform(move);
ASSOCstore("STACK",rect,token);

TANGOpath_free(1,path);
TANGOtrans_free(1,move);
}

void AnimBlink(expr)
/* procedure for blinking the top stack location*/
char expr[];
{
TANGO_IMAGE rect;
TANGO_PATH path,fillpath;
TANGO_TRANS fill;
double flash[4];

rect = (TANGO_IMAGE) ASSOCretrieve("ID",expr,stackloc);
flash[0] = 0.5; /* halffill */
flash[2] = -1.0; /* outline */
flash[1] = flash[3] = 0.0;
path = TANGOpath_create(4,flash,flash);
fillpath = TANGOpath_iterate(path,4.0);
fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,rect,fillpath);
TANGOtrans_perform(fill);

TANGOpath_free(2,path,fillpath);
TANGOtrans_free(1,fill);
}

void AnimPopOut(expr)
/* procedure for popping a character from the stack and outputting it */
char expr[];
{
TANGO_IMAGE token,rect;
TANGO_LOC from_loc,to_loc;
TANGO_PATH path;
TANGO_TRANS move;

rect = (TANGO_IMAGE) ASSOCretrieve("ID",expr,stackloc);
from_loc = TANGOimage_loc(rect,TANGO_PART_TYPE_C); 
token = (TANGO_IMAGE) ASSOCretrieve("STACK",rect);
to_loc = (TANGO_LOC) ASSOCretrieve("OUTLOC",expr,outloc);
outloc++;
stackloc++;
path = TANGOpath_motion(from_loc,to_loc,TANGO_PATH_TYPE_STRAIGHT);
move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,token,path);
TANGOtrans_perform(move);

TANGOpath_free(1,path);
TANGOtrans_free(1,move);
}

void AnimPop(expr)
/* procedure for popping a character from the stack and making it invisible*/
char expr[];
{
TANGO_IMAGE token,rect;
TANGO_PATH path;
TANGO_TRANS trans;

rect = (TANGO_IMAGE) ASSOCretrieve("ID",expr,stackloc);
token = (TANGO_IMAGE) ASSOCretrieve("STACK",rect);
stackloc++;
path = TANGOpath_null(1);
trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,token,path);
TANGOtrans_perform(trans);

TANGOpath_free(1,path);
TANGOtrans_free(1,trans);
}

void AnimError()
/* procedure for error reporting */
{

TANGO_IMAGE error_text;
TANGO_PATH path;
TANGO_TRANS trans;

error_text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.4000, 0.2500, 1, TANGO_COLOR_ORANGE, MYFONT, "ERROR IN THE INFIX EXPRESSION", 0);

path = TANGOpath_null(1);
trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(trans);
TANGOpath_free(1,path);
TANGOtrans_free(1,trans);
}
