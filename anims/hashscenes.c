/***********************  hashscenes.c  **********************/
/***  This module uses XTANGO routines to animate cyclic   ***/
/***  linear probing in a hash table.  Operations which    ***/
/***  are supported include add, update, in table?, and    ***/
/***  delete elements with given keys.  The keys are ints  ***/
/***  and the data are strings, for this implementation.   ***/
/***  See hash.README for more details                     ***/
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <xtango.h>

int tableTopIndex, tableBottomIndex;  /* for conversion to locaton indices */

/************/

void Anim_Init(lowIndex, highIndex)
  int lowIndex, highIndex;
{
  int i;
  double x, y, incr; 
  double width, height, startx, starty, space;
  char str[4];
  char empty_str[2];
  char action_str[8];
  char index_str[6];
  char key_str[4];
  char data_str[5];
  char status_str[7];
  char newKey_str[16];
  char newData_str[16];
  TANGO_IMAGE action, index, key, data, status, element, text[100], index_ptr;
  TANGO_IMAGE newKey, newData, dummy;
  TANGO_PATH path;
  TANGO_TRANS trans;
  TANGO_LOC action_loc, newKey_loc, newData_loc, loc;

/*** initialize strings ***/
  strcpy(empty_str, "");
  strcpy(action_str, "ACTION:");
  strcpy(index_str, "Index");
  strcpy(key_str, "Key");
  strcpy(data_str, "Data");
  strcpy(status_str, "Status");
  strcpy(newKey_str, "Current Key:");
  strcpy(newData_str, "Current Data:");

/*** set location array index bounds ***/ 
  tableTopIndex = 0;
  tableBottomIndex = highIndex - lowIndex;

/*** create associations ***/
  ASSOCmake("ACTION_LOC", 0);
  ASSOCmake("ACTION", 0);
  ASSOCmake("KEY", 0);
  ASSOCmake("KEY_LOC", 0);
  ASSOCmake("DATA", 0);
  ASSOCmake("DATA_LOC", 0);
  ASSOCmake("INDEX_PTR", 0);
  ASSOCmake("TABLE", 2);
  ASSOCmake("TABLE_LOC", 2);
  ASSOCmake("TABLE_TEXT", 2);
  ASSOCmake("INDEX_LOC", 2);

/*** Draw hash table and record locations within it     ***/
/*** slot #0 is at the bottom of the table              ***/
/*** dummy empty string is used for intial ASSOC stores ***/ 

/*** first column in table is for keys ***/
  startx = .5;
  starty = .2; /* ends at .8 */
  width = .1;
  height = .6 / (highIndex - lowIndex + 1); 
  TWISTcreate_image_array("TABLE", 0, highIndex - lowIndex + 1,
                          TANGO_IMAGE_TYPE_RECTANGLE, 0, 0, startx, starty, 
                          NULL, width,
                          NULL, height, 0.0, 1, TANGO_COLOR_BLUE, 0.0);
  TWISTcreate_loc_array("TABLE_LOC", 0, highIndex - lowIndex + 1,
                          0, startx + width/5.0, starty + height/1.5, height);
  for(i = 0, y = starty + height/1.5; i <= (highIndex - lowIndex); i++)  {
    dummy = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, startx, y, 0,
                             TANGO_COLOR_BLACK, NULL, empty_str, 0);
    ASSOCstore("TABLE_TEXT", 0, i, dummy);
    y += height;
  } 
/*** next is data element ***/
  startx = .6;
  width = .2;
  TWISTcreate_image_array("TABLE", 1, highIndex - lowIndex + 1,
                          TANGO_IMAGE_TYPE_RECTANGLE, 0, 0, startx, starty, 
                          NULL, width,
                          NULL, height, 0.0, 1, TANGO_COLOR_BLUE, 0.0);
  TWISTcreate_loc_array("TABLE_LOC", 1, highIndex - lowIndex + 1,
                          0, startx + width/5.0, starty + height/1.5, height);
  for(i = 0, y = starty + height/1.5; i <= (highIndex - lowIndex); i++)  {
    dummy = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, startx, y, 0,
                             TANGO_COLOR_BLACK, NULL, empty_str, 0);
    ASSOCstore("TABLE_TEXT", 1, i, dummy);
    y += height;
  } 
/*** last is status indicator ***/
  startx = .8;
  width = .1;
  TWISTcreate_image_array("TABLE", 2, highIndex - lowIndex + 1,
                          TANGO_IMAGE_TYPE_RECTANGLE, 0, 0, startx, starty, 
                          NULL, width,
                          NULL, height, 0.0, 1, TANGO_COLOR_BLUE, 0.0);
  TWISTcreate_loc_array("TABLE_LOC", 2, highIndex - lowIndex + 1,
                          0, startx + width/5.0, starty + height/1.5, height);
  for(i = 0, y = starty + height/1.5; i <= (highIndex - lowIndex); i++)  {
    dummy = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, startx, y, 0,
                             TANGO_COLOR_BLACK, NULL, empty_str, 0);
    ASSOCstore("TABLE_TEXT", 2, i, dummy);
    y += height;
  } 

/*** draw index labels and record locations next to them ***/
  incr = .6 / (highIndex - lowIndex + 1);  
  x = .45;
  y = starty + (incr / 2.0);
  for(i = highIndex; i >= lowIndex; i--)  {
    sprintf(str, "%d", i);
    text[i] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1,
                             TANGO_COLOR_BLACK, NULL, str, 1);
    y += incr;    
  }
  x = .35;
  y = starty + (incr / 2.0);
  TWISTcreate_loc_array("INDEX_LOC", 0, highIndex - lowIndex + 1,
                        0, x, y, incr);
 
/*** draw text labels and create any corresponding locations ***/
/*** again, dummy empty strings are stored                   ***/
  action = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .25, .1, 1,
                             TANGO_COLOR_BLACK, NULL, action_str, 0);
  action_loc = TANGOloc_create(.4, .1);
  ASSOCstore("ACTION_LOC", action_loc);
  dummy = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .4, .1, 1,
                             TANGO_COLOR_BLACK, NULL, empty_str, 0);
  ASSOCstore("ACTION", dummy);
  index = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .45, .18, 1,
                             TANGO_COLOR_BLACK, NULL, index_str, 1);
  key = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .55, .18, 1,
                             TANGO_COLOR_BLACK, NULL, key_str, 1);
  data = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .7, .18, 1,
                             TANGO_COLOR_BLACK, NULL, data_str, 1);
  status = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .85, .18, 1,
                             TANGO_COLOR_BLACK, NULL, status_str, 1);
  newKey = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .01, .4, 1,
                             TANGO_COLOR_BLACK, NULL, newKey_str, 0);
  newKey_loc = TANGOloc_create(.05, .45);   
  ASSOCstore("KEY_LOC", newKey_loc);
  dummy = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .05, .45, 1,
                             TANGO_COLOR_BLACK, NULL, empty_str, 0);
  ASSOCstore("KEY", dummy);
  newData = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .01, .55, 1,
                             TANGO_COLOR_BLACK, NULL, newData_str, 0);
  newData_loc = TANGOloc_create(.05, .6);   
  ASSOCstore("DATA_LOC", newData_loc);
  dummy = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, .05, .6, 1,
                             TANGO_COLOR_BLACK, NULL, empty_str, 0);
  ASSOCstore("DATA", dummy);

/*** draw index arrow ***/
  loc = (TANGO_LOC)ASSOCretrieve("INDEX_LOC", 0, tableBottomIndex);
  TANGOloc_inquire(loc, &x, &y);
  index_ptr = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, x, y, 1, 
                                TANGO_COLOR_RED, .05, 0.0, .4, 1.0, 1);
  ASSOCstore("INDEX_PTR", index_ptr);


/*** create null path to display stuff ***/
  path = TANGOpath_null(1);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
  TANGOtrans_perform(trans);

  TANGOpath_free(1,path);
  TANGOtrans_free(1, trans);
}

/************/

void Anim_Blink(image)
  TANGO_IMAGE image;
{
  TANGO_PATH path;
  TANGO_TRANS t1, t2, t3, t4, trans;

/*** make image blink ***/
  path = TANGOpath_null(5);
  t1 = TAPvis_toggle(image);
  t2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
  t3 = TAPvis_toggle(image);
  t4 = TANGOtrans_concatenate(4, t1, t2, t3, t2);
  trans = TANGOtrans_iterate(t4, 3);
  TANGOtrans_perform(trans);

  TANGOpath_free(1,path);
  TANGOtrans_free(5, t1, t2, t3, t4, trans);
}

/************/

void Anim_Action(string)
  char *string;
{
  double x, y;
  TANGO_LOC loc;
  TANGO_IMAGE text, image;
  TANGO_PATH path;
  TANGO_TRANS trans;

/*** display string describing the current action ***/
/*** first delete old action string ***/
  image = (TANGO_IMAGE)ASSOCretrieve("ACTION");
  path = TANGOpath_null(1);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, image, path); 
  loc = (TANGO_LOC)ASSOCretrieve("ACTION_LOC");
  TANGOloc_inquire(loc, &x, &y);
  text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1,
                           TANGO_COLOR_RED, NULL, string, 0);
  ASSOCstore("ACTION", text);
  TANGOtrans_perform(trans);

  TANGOpath_free(1, path);
  TANGOtrans_free(1, trans);
}

/************/
 
void Anim_NewElement(key, string)
  int key;
  char *string;
{
  double x, y;
  char str[10];
  TANGO_LOC loc, loc2;
  TANGO_PATH rm_path;
  TANGO_TRANS rm_trans;
  TANGO_IMAGE text, image;

/*** draw new key and data in appropriate location ***/
/*** first delete old key, data, if still there    ***/
  text = (TANGO_IMAGE)ASSOCretrieve("KEY");
  loc = (TANGO_LOC)ASSOCretrieve("KEY_LOC");
  TANGOloc_inquire(loc, &x, &y);
  loc2 = (TANGO_LOC)TANGOimage_loc(text, TANGO_PART_TYPE_SW);
  if ((TANGOloc_X(loc2) - x) <= .1)   {
    rm_path = TANGOpath_null(1);
    rm_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, text, rm_path); 
    TANGOtrans_perform(rm_trans);
    TANGOpath_free(1, rm_path);
    TANGOtrans_free(1, rm_trans);
  }

  sprintf(str, "%d", key);
  text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1,
                           TANGO_COLOR_MAROON, NULL, str, 0);
  ASSOCstore("KEY", text);

  text = (TANGO_IMAGE)ASSOCretrieve("DATA");
  loc = (TANGO_LOC)ASSOCretrieve("DATA_LOC");
  TANGOloc_inquire(loc, &x, &y);
  loc2 = (TANGO_LOC)TANGOimage_loc(text, TANGO_PART_TYPE_SW);
  if ((TANGOloc_X(loc2) - x) <= .1)   {
    rm_path = TANGOpath_null(1);
    rm_trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, text, rm_path); 
    TANGOtrans_perform(rm_trans);
    TANGOpath_free(1, rm_path);
    TANGOtrans_free(1, rm_trans);
  }

  text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1,
                           TANGO_COLOR_MAROON, NULL, string, 0);
  ASSOCstore("DATA", text);

/*** blink index arrow to simulate hash function calculation ***/
  image = (TANGO_IMAGE)ASSOCretrieve("INDEX_PTR");
  Anim_Blink(image);
}

/************/

void Anim_ElementAtIndex(index)
  int index;
{
  TANGO_IMAGE image;
  TANGO_LOC from_loc, to_loc;
  TANGO_PATH path;
  TANGO_TRANS trans;

/*** move index arrow to current location to simulate     ***/
/*** element will go to current hash location if possible ***/
  image = (TANGO_IMAGE)ASSOCretrieve("INDEX_PTR");
  from_loc = TANGOimage_loc(image, TANGO_PART_TYPE_W);
  to_loc = (TANGO_LOC)ASSOCretrieve("INDEX_LOC", 0, tableBottomIndex - index);
  path = TANGOpath_motion(from_loc, to_loc, TANGO_PATH_TYPE_STRAIGHT);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, path);
  TANGOtrans_perform(trans); 

  TANGOpath_free(1, path);
  TANGOtrans_free(1, trans);
}

/************/

void Anim_AddElement(index)
  int index;
{
  TANGO_IMAGE key, data, image;
  TANGO_LOC from_loc, to_loc;
  TANGO_PATH key_path, data_path, path;
  TANGO_TRANS key_trans, data_trans, trans;

/*** move current key and data into hash table at particular index ***/
  key = (TANGO_IMAGE)ASSOCretrieve("KEY");
  from_loc = TANGOimage_loc(key, TANGO_PART_TYPE_SW);
  to_loc = (TANGO_LOC)ASSOCretrieve("TABLE_LOC", 0, tableBottomIndex - index);
  key_path = TANGOpath_motion(from_loc, to_loc, TANGO_PATH_TYPE_CLOCKWISE);
  key_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, key, key_path);
  data = (TANGO_IMAGE)ASSOCretrieve("DATA");
  from_loc = TANGOimage_loc(data, TANGO_PART_TYPE_SW);
  to_loc = (TANGO_LOC)ASSOCretrieve("TABLE_LOC", 1, tableBottomIndex - index);
  data_path = TANGOpath_motion(from_loc, to_loc, TANGO_PATH_TYPE_CLOCKWISE);
  data_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, data, data_path);
  trans = TANGOtrans_compose(2, key_trans, data_trans);
  TANGOtrans_perform(trans);
  
/*** delete old table contents ***/
  image = (TANGO_IMAGE)ASSOCretrieve("TABLE_TEXT", 0, tableBottomIndex - index);
  path = TANGOpath_null(1);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, image, path); 
  TANGOtrans_perform(trans);
  image = (TANGO_IMAGE)ASSOCretrieve("TABLE_TEXT", 1, tableBottomIndex - index);
  path = TANGOpath_null(1);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, image, path); 
  TANGOtrans_perform(trans);

/*** store new contents ***/
  ASSOCstore("TABLE_TEXT", 0, tableBottomIndex - index, key);
  ASSOCstore("TABLE_TEXT", 1, tableBottomIndex - index, data);
  TANGOpath_free(3, key_path, data_path, path);
  TANGOtrans_free(2, key_trans, data_trans);
}

/************/

void Anim_UpdateElement(index)
  int index;
{
  TANGO_IMAGE data, image;
  TANGO_LOC from_loc, to_loc;
  TANGO_PATH data_path, path;
  TANGO_TRANS trans;

/*** key is at current index, so update data element ***/
  data = (TANGO_IMAGE)ASSOCretrieve("DATA");
  from_loc = TANGOimage_loc(data, TANGO_PART_TYPE_SW);
  to_loc = (TANGO_LOC)ASSOCretrieve("TABLE_LOC", 1, tableBottomIndex - index);
  data_path = TANGOpath_motion(from_loc, to_loc, TANGO_PATH_TYPE_CLOCKWISE);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, data, data_path);
  TANGOtrans_perform(trans);
  
/*** delete old data in table ***/
  image = (TANGO_IMAGE)ASSOCretrieve("TABLE_TEXT", 1, tableBottomIndex - index);
  path = TANGOpath_null(1);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, image, path); 
  TANGOtrans_perform(trans);

/*** store new data ***/
  ASSOCstore("TABLE_TEXT", 1, tableBottomIndex - index, data);
  TANGOpath_free(2, data_path, path);
  TANGOtrans_free(1, trans);
}

/**********/

void Anim_KeyMatch(index)
  int index;
{
  TANGO_IMAGE key;
  TANGO_TRANS trans;

/*** blink key at index to indicate a match with current key ***/
  key = (TANGO_IMAGE)ASSOCretrieve("TABLE_TEXT", 0, tableBottomIndex - index);
  Anim_Blink(key);
}

/************/

void Anim_CompareKeys(index)
  int index;
{
  static double out_x[1] = { -1.0 };
  static double out_y[1] = { 0.0 };
  static double fill_x[1] = { .2 };
  static double fill_y[1] = { 0.0 };
  TANGO_IMAGE key;
  TANGO_PATH outline, fill, path;
  TANGO_TRANS t1, t2, t3, trans;

/*** highlight key at index to simulate comparison of ***/
/*** key and current key                              ***/
  key = (TANGO_IMAGE)ASSOCretrieve("TABLE", 0, tableBottomIndex - index);
  outline = TANGOpath_create(1, out_x, out_y);
  fill = TANGOpath_create(1, fill_x, fill_y);
  path = TANGOpath_null(5);
  t1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, key, fill);
  t2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
  t3 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, key, outline);
  trans = TANGOtrans_concatenate(3, t1, t2, t3);
  TANGOtrans_perform(trans);

  TANGOpath_free(3, outline, fill, path);
  TANGOtrans_free(4, t1, t2, t3, trans);
}

/************/

void Anim_CheckStatus(index)
  int index;
{
  static double out_x[1] = { -1.0 };
  static double out_y[1] = { 0.0 };
  static double fill_x[1] = { .2 };
  static double fill_y[1] = { 0.0 };
  TANGO_IMAGE image;
  TANGO_PATH outline, fill, path;
  TANGO_TRANS t1, t2, t3, trans;

/*** highlight status of hash location at index to simulate ***/
/*** a lookup of the status                                 ***/
  image = (TANGO_IMAGE)ASSOCretrieve("TABLE", 2, tableBottomIndex - index);
  outline = TANGOpath_create(1, out_x, out_y);
  fill = TANGOpath_create(1, fill_x, fill_y);
  path = TANGOpath_null(5);
  t1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, image, fill);
  t2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
  t3 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, image, outline);
  trans = TANGOtrans_concatenate(3, t1, t2, t3);
  TANGOtrans_perform(trans);

  TANGOpath_free(3, outline, fill, path);
  TANGOtrans_free(4, t1, t2, t3, trans);
}

/************/

void Anim_UpdateStatus(index, string)
  int index;
  char *string;
{
  double x, y;
  TANGO_LOC loc;
  TANGO_IMAGE text, image;
  TANGO_PATH path;
  TANGO_TRANS trans;

/*** highlight status at index and update it to new value ***/
  Anim_CheckStatus(index);

  image = (TANGO_IMAGE)ASSOCretrieve("TABLE_TEXT", 2, tableBottomIndex - index);
  path = TANGOpath_null(1);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, image, path); 
  loc = (TANGO_LOC)ASSOCretrieve("TABLE_LOC", 2, tableBottomIndex - index);
  TANGOloc_inquire(loc, &x, &y);
  text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1,
                           TANGO_COLOR_BLACK, NULL, string, 0);
  ASSOCstore("TABLE_TEXT", 2, tableBottomIndex - index, text);
  TANGOtrans_perform(trans);

  TANGOpath_free(1, path);
  TANGOtrans_free(1, trans);
}


