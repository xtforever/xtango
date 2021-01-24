/*************************

   Reverse Polish Calculator
           by
      Brad Topol

***************************/

#include <stdio.h>
#include "xtango.h"

void 
ANIMInit ()
{
	TANGO_IMAGE title,title2;
	TANGO_PATH path;
	TANGO_TRANS trans,trans2,alltrans;
	ASSOCinit();
	ASSOCmake("YVAL",0);
	ASSOCmake("STACKBOX",1);
	ASSOCmake("STACKVAL",1);
	ASSOCstore("YVAL",0);
        /* create title */
        title = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.01, 0.05, 1,
        	TANGO_COLOR_BLACK, NULL,"Reverse Polish Calculator",0);
        title2 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.01, 0.11, 1,
        TANGO_COLOR_BLACK, NULL,"Currently Scanned Char ==>",0);

        path = TANGOpath_null(2);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,title,path); 
	trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,title2,path); 
        alltrans = TANGOtrans_compose(2,trans,trans2);
        /* display the title */
        TANGOtrans_perform(alltrans);

}

void 
ANIMCurrentChar (s)
char *s;
{
	TANGO_IMAGE title,title2;
	TANGO_PATH path,wait_path;
	TANGO_TRANS trans,trans2,alltrans;
        /* display the currently read character */
        title2 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.50, 0.11, 1,
        TANGO_COLOR_BLACK, NULL,s,0);

        path = TANGOpath_null(2);
        wait_path = TANGOpath_null(100);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,title2,path); 
	trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,wait_path);
        alltrans = TANGOtrans_compose(2,trans,trans2);
        TANGOtrans_perform(alltrans);
        /* Now make character dissappear */
        path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,title2,path); 
        alltrans = TANGOtrans_compose(1,trans);
        TANGOtrans_perform(alltrans);

}
void 
ANIMPushStack(f)
float f;
{

double cur_y_position;
TANGO_IMAGE stack_box, stack_val;
TANGO_PATH path, wait_path;
TANGO_TRANS alltrans, trans1, trans2, trans3;
int temp_sp;
char temp[10];

path = TANGOpath_null(2);
wait_path = TANGOpath_null(500);
temp_sp = ASSOCretrieve("YVAL");
cur_y_position = 0.9  - (((float) temp_sp) * 0.10);
/* create stack rectangle and value that goes in it */
stack_box = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.65,   
   cur_y_position, 1, TANGO_COLOR_BLACK, 0.27, 0.05, 0.0);
sprintf(temp,"%f",f);
stack_val = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.70,   
    cur_y_position + 0.03, 1, TANGO_COLOR_BLACK, NULL, temp, 0);

/* store the images, so they can be retrieved later  */

ASSOCstore ("STACKBOX",temp_sp,stack_box);
ASSOCstore ("STACKVAL",temp_sp,stack_val);
ASSOCstore("YVAL",temp_sp+1);

trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_box,path);
trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_val,path);
trans3 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,wait_path);
/* Compose into one atomic transition */
alltrans = TANGOtrans_compose(2,trans1,trans2);
/* display all this hard work */

TANGOtrans_perform(alltrans);


}

void 
ANIMClearStack()
{
/* this function clears the final result off of the stack */

double cur_y_position;
TANGO_IMAGE stack_box, stack_val;
TANGO_PATH path, wait_path;
TANGO_TRANS alltrans, trans1, trans2, trans3;
char temp[10];
int temp_sp;
/* retrieve info about image on top of stack */
temp_sp = ASSOCretrieve("YVAL");
if (temp_sp >= 1) {
   stack_box = (TANGO_IMAGE) ASSOCretrieve ("STACKBOX",temp_sp - 1);
   stack_val = (TANGO_IMAGE) ASSOCretrieve ("STACKVAL",temp_sp - 1);

   path = TANGOpath_null(1);
   wait_path = TANGOpath_null(150);
   /* make image disappear */
   trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_box,path);
   trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_val,path);
   trans3 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,wait_path);

   /* Compose into one atomic transition */
   alltrans = TANGOtrans_compose(3,trans1,trans2,trans3);
   /* store a new stack value as a stack item has be "popped" */

   ASSOCstore("YVAL",temp_sp-1);
   TANGOtrans_perform(alltrans);
   TANGOtrans_free(4,trans1,trans2,trans3,alltrans);
   }
}

void 
ANIMMerge(operator)
char operator;
{

double cur_y_position;
TANGO_IMAGE stack_box1, stack_val1,operand_box,operand_val;
TANGO_IMAGE stack_box2, stack_val2;
TANGO_PATH path, wait_path,path_box1, path_box2, path_val1, path_val2;
TANGO_PATH op_pathbox, op_pathval;
TANGO_TRANS alltrans, trans1, trans2, trans3;
TANGO_TRANS move_box1, move_box2, move_val1, move_val2, smoosh,move_opbox;
TANGO_TRANS move_opval;
TANGO_LOC loc_box1, loc_box2,loc_opbox;
TANGO_LOC loc_val1, loc_val2,loc_opval;
char temp[3];
int temp_sp;

temp_sp = ASSOCretrieve("YVAL");
cur_y_position = 0.9  - (((float) temp_sp) * 0.10) + 0.22;

/* create display for the operand */
operand_box = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.20,   
   cur_y_position, 1, TANGO_COLOR_BLACK, 0.025, 0.0);
sprintf(temp,"%c",operator);
operand_val = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.195,   
    cur_y_position + 0.01, 1, TANGO_COLOR_BLACK, NULL, temp, 0);

path = TANGOpath_null(2);
trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,operand_box,path);
trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,operand_val,path);

/* Compose into one atomic transition */

alltrans = TANGOtrans_compose(2,trans1,trans2);
/* display the operand  */
/* retrieve information about top two stack elements */
TANGOtrans_perform(alltrans);
stack_box1 = (TANGO_IMAGE) ASSOCretrieve ("STACKBOX",temp_sp - 1);
stack_val1 = (TANGO_IMAGE) ASSOCretrieve ("STACKVAL",temp_sp - 1);
if (temp_sp == 1)  {
    printf("ERROR: TOO MANY OPERRANDS\n");
    path = TANGOpath_null(1);
    move_box1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_box1,path);
    move_val1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_val1,path);
    smoosh    = TANGOtrans_compose(2, move_box1,move_val1);
    TANGOtrans_perform(smoosh);
    /* stack needs to be readjusted by taking off top element */
    ASSOCstore("YVAL",temp_sp-1);
    return;
}
  stack_box2 = (TANGO_IMAGE) ASSOCretrieve ("STACKBOX",temp_sp - 2);
  stack_val2 = (TANGO_IMAGE) ASSOCretrieve ("STACKVAL",temp_sp - 2);

/* do all work necessary to make stack elements and operand merge */
loc_box1 = TANGOimage_loc(stack_box1, TANGO_PART_TYPE_S );
loc_val1 = TANGOimage_loc(stack_val1, TANGO_PART_TYPE_S );

loc_box2 = TANGOimage_loc(stack_box2, TANGO_PART_TYPE_S );
loc_val2 = TANGOimage_loc(stack_val2, TANGO_PART_TYPE_S );
loc_opbox = TANGOimage_loc(operand_box, TANGO_PART_TYPE_C );
loc_opval = TANGOimage_loc(operand_val, TANGO_PART_TYPE_C );
wait_path = TANGOpath_null(600);

path_box1 = TANGOpath_example(loc_box1, loc_box2, wait_path);
path_box2 = TANGOpath_example(loc_box2, loc_box1, wait_path);
path_val1 = TANGOpath_example(loc_val1, loc_val2, wait_path);
path_val2 = TANGOpath_example(loc_val2, loc_val1, wait_path);
op_pathbox = TANGOpath_example(loc_opbox, loc_val2, wait_path);
op_pathval = TANGOpath_example(loc_opval, loc_val2, wait_path);

move_box1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, stack_box1, path_box1);
move_val1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, stack_val1, path_val1);
move_opbox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, operand_box, op_pathbox);
move_opval = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, operand_val, op_pathval);
smoosh    = TANGOtrans_compose(4, move_box1, move_val1, move_opbox, move_opval);
/* smoosh it all together */
TANGOtrans_perform(smoosh);

/* make it all disappear */
path = TANGOpath_null(1);
move_box1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_box1,path);
move_val1 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_val1,path);
move_box2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_box2,path);
move_val2 = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,stack_val2,path);
move_opbox = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,operand_box,path);
move_opval = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,operand_val,path);
smoosh    = TANGOtrans_compose(6, move_box1, move_box2, move_val1, move_val2,
                   move_opbox,move_opval);
TANGOtrans_perform(smoosh);
/* stack needs to be readjusted by taking off top two elements */
ASSOCstore("YVAL",temp_sp-2);

}
