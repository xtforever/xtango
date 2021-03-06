/*************************************************************/
/*                                                           */
/*                    XTANGO TAP ROUTINES                    */
/*                                                           */
/*************************************************************/
/*      Copyright 1991 Georgia Institute of Technology       */
/*              John T. Stasko and Kevin Haulk               */
/*************************************************************/

/*************************************************************/
/********************    include files    ********************/
/*************************************************************/

#include "xtango.h"

/*************************************************************/
/*******************    GLOBAL variables    ******************/
/*************************************************************/

/*************************************************************/
/*******************    LOCAL variables    *******************/
/*************************************************************/

/*************************************************************/
/*******************    LOCAL functions    *******************/
/*************************************************************/

/*************************************************************/
/* TAPfill -- Fills a specified image to a solid color       */
/*                                                           */
/* RETURNS:  The transition required to perform the fill     */
/*************************************************************/
TANGO_TRANS TAPfill(image)
TANGO_IMAGE image;
{
	TANGO_PATH fill_path;
	TANGO_TRANS fill_trans;
	double x[1], y[1];

	x[0] = 1.0;
	y[0] = 0.0;

	fill_path = TANGOpath_create(1, x, y);
	fill_trans = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, image, fill_path);
	TANGOpath_free(1, fill_path);
	return (fill_trans);
}

/************************************************************/
/* TAPcolor -- Changes the specified image to the given     */
/*             color                                        */
/*                                                          */
/* RETURNS:  The transition required to perform the color   */
/*           change                                         */
/************************************************************/
TANGO_TRANS TAPcolor(image, color)
TANGO_IMAGE image;
TANGO_COLOR color;

{
	TANGO_PATH color_path;
	TANGO_TRANS color_trans;

	color_path = TANGOpath_color(color);
	color_trans =
	    TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, image, color_path);
	TANGOpath_free(1, color_path);
	return (color_trans);
}

/*************************************************************/
/* TAPshow -- Displays an image; that is, performs a delay   */
/*            transition                                     */
/*                                                           */
/* RETURNS:  None.                                           */
/*************************************************************/
void TAPshow(image)
TANGO_IMAGE image;

/* Displays an image */

{
	TANGO_PATH null_path;
	TANGO_TRANS disp_trans;

	null_path = TANGOpath_null(1);
	disp_trans =
	    TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, image, null_path);

	TANGOtrans_perform(disp_trans);
	TANGOpath_free(1, null_path);
	TANGOtrans_free(1, disp_trans);
}

/*************************************************************/
/* TAPvis_toggle -- Toggles the visibility of an object      */
/*                                                           */
/* RETURNS:  The required transition                         */
/*************************************************************/
TANGO_TRANS TAPvis_toggle(image)
TANGO_IMAGE image;

/* Toggles the visibility of an image */

{
	TANGO_PATH null_path;
	TANGO_TRANS visi_trans;

	null_path = TANGOpath_null(1);
	visi_trans =
	    TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, image, null_path);
	TANGOpath_free(1, null_path);
	return (visi_trans);
}

/*************************************************************/
/* TAPjump -- Sets up the transition to change and objects   */
/*            location to the location given.                */
/*                                                           */
/* RETURNS:  The required transition                         */
/*************************************************************/
TANGO_TRANS TAPjump(image, part, location)
TANGO_IMAGE image;
TANGO_PART_TYPE part;
TANGO_LOC location;

{
	TANGO_PATH jump_path;
	TANGO_TRANS jump_trans;
	TANGO_LOC image_loc;
	double xoffset[1], yoffset[1];

	image_loc = TANGOimage_loc(image, part);

	xoffset[0] = TANGOloc_X(location) - TANGOloc_X(image_loc);
	yoffset[0] = TANGOloc_Y(location) - TANGOloc_Y(image_loc);

	jump_path = TANGOpath_create(1, xoffset, yoffset);

	jump_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, jump_path);
	TANGOpath_free(1, jump_path);
	free(image_loc);
	return (jump_trans);
}

/*************************************************************/
/* TAPmove -- sets up the transition to move an object to the*/
/*            given location using a straight path and       */
/*            TANGOpath_motion                               */
/*                                                           */
/* RETURNS:  The move transition.                            */
/*************************************************************/
TANGO_TRANS TAPmove(image, part, location)
TANGO_IMAGE image;
TANGO_PART_TYPE part;
TANGO_LOC location;

{
	TANGO_PATH move_path;
	TANGO_TRANS move_trans;
	TANGO_LOC image_loc;

	image_loc = TANGOimage_loc(image, part);

	move_path =
	    TANGOpath_motion(image_loc, location, TANGO_PATH_TYPE_STRAIGHT);
	move_trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, move_path);
	TANGOpath_free(1, move_path);

	free(image_loc);
	return (move_trans);
}

/*************************************************************/
/* TAPtraverse -- cause an image to traverse a given         */
/*                TANGO_PATH_TYPE from the objects current   */
/*                location to the specified location         */
/*                                                           */
/* RETURNS:  The required transition                         */
/*************************************************************/
TANGO_TRANS TAPtraverse(image, part, location, motion)
TANGO_IMAGE image;
TANGO_PART_TYPE part;
TANGO_LOC location;
TANGO_PATH_TYPE motion;

{
	TANGO_PATH traverse_path;
	TANGO_TRANS traverse_trans;
	TANGO_LOC image_loc;

	image_loc = TANGOimage_loc(image, part);

	traverse_path = TANGOpath_motion(image_loc, location, motion);
	traverse_trans =
	    TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image, traverse_path);
	TANGOpath_free(1, traverse_path);

	free(image_loc);
	return (traverse_trans);
}

/*************************************************************/
/* TAPswitch -- sets up the transition necessary to swap the */
/*              position of two images                       */
/*                                                           */
/* RETURNS:  The switch transition                           */
/*************************************************************/
TANGO_TRANS TAPswitch(image1, image2)
TANGO_IMAGE image1;
TANGO_IMAGE image2;

{
	TANGO_LOC image1_loc;
	TANGO_LOC image2_loc;
	TANGO_PATH switch_path1;
	TANGO_PATH switch_path2;
	TANGO_TRANS switch_trans1;
	TANGO_TRANS switch_trans2;
	TANGO_TRANS total_trans;
	double xoffset[2], yoffset[2];

	image1_loc = TANGOimage_loc(image1, TANGO_PART_TYPE_C);
	image2_loc = TANGOimage_loc(image2, TANGO_PART_TYPE_C);

	xoffset[0] = yoffset[0] = (double)0.0;

	xoffset[1] = TANGOloc_X(image2_loc) - TANGOloc_X(image1_loc);
	yoffset[1] = TANGOloc_Y(image2_loc) - TANGOloc_Y(image1_loc);

	switch_path1 = TANGOpath_create(2, xoffset, yoffset);
	switch_trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image1,
					  switch_path1);

	xoffset[1] = TANGOloc_X(image1_loc) - TANGOloc_X(image2_loc);
	yoffset[1] = TANGOloc_Y(image1_loc) - TANGOloc_Y(image2_loc);

	switch_path2 = TANGOpath_create(2, xoffset, yoffset);
	switch_trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image2,
					  switch_path2);
	TANGOpath_free(2, switch_path1, switch_path2);

	total_trans = TANGOtrans_compose(2, switch_trans1, switch_trans2);

	free(image1_loc);
	free(image2_loc);
	TANGOtrans_free(2, switch_trans1, switch_trans2);
	return (total_trans);
}

/*************************************************************/
/* TAPexchange -- sets up the transition necessary to swap   */
/*                the position of two objects following a    */
/*                straight path                              */
/*                                                           */
/* RETURNS: The transition                                   */
/*************************************************************/
TANGO_TRANS TAPexchange(image1, image2)
TANGO_IMAGE image1;
TANGO_IMAGE image2;

{
	TANGO_LOC image1_loc;
	TANGO_LOC image2_loc;
	TANGO_PATH exchange_path1;
	TANGO_PATH exchange_path2;
	TANGO_TRANS exchange_trans1;
	TANGO_TRANS exchange_trans2;
	TANGO_TRANS total_trans;

	image1_loc = TANGOimage_loc(image1, TANGO_PART_TYPE_C);
	image2_loc = TANGOimage_loc(image2, TANGO_PART_TYPE_C);

	exchange_path1 = TANGOpath_motion(image1_loc, image2_loc,
					  TANGO_PATH_TYPE_STRAIGHT);

	exchange_trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image1,
					    exchange_path1);

	exchange_path2 = TANGOpath_motion(image2_loc, image1_loc,
					  TANGO_PATH_TYPE_STRAIGHT);
	exchange_trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, image2,
					    exchange_path2);
	TANGOpath_free(2, exchange_path1, exchange_path2);

	total_trans = TANGOtrans_compose(2, exchange_trans1, exchange_trans2);

	free(image1_loc);
	free(image2_loc);
	TANGOtrans_free(2, exchange_trans1, exchange_trans2);
	return (total_trans);
}

/*************************************************************/
/* TAPflash -- causes an image to flash on and off the       */
/*             specified number of times                     */
/*                                                           */
/* RETURNS: the flash transition                             */
/*************************************************************/
TANGO_TRANS TAPflash(image, num_flash)
TANGO_IMAGE image;
int num_flash;

{
	TANGO_PATH null_path;
	TANGO_TRANS flash_trans;

	null_path = TANGOpath_null(num_flash * 2);
	flash_trans =
	    TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, image, null_path);
	TANGOpath_free(1, null_path);
	return (flash_trans);
}
