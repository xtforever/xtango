#include "xtango.h"
#include "bmap.h"

void scene() {
   static double path_x[27] = {  0.0051,  0.0116,  0.0231,  0.0205,  0.0218,
				 0.0257,  0.0257,  0.0257,  0.0167,  0.0077,
				 0.0026,  0.0013, -0.0051, -0.0051, -0.0090,
				-0.0116, -0.0205, -0.0321, -0.0154, -0.0359,
				-0.0128, -0.0154, -0.0193, -0.0103, -0.0051,
				-0.0013,  0.0051 };
   static double path_y[27] = { -0.0180, -0.0167, -0.0141, -0.0077,  0.0013,
				 0.0064,  0.0116,  0.0193,  0.0244,  0.0180,
				 0.0167,  0.0141,  0.0282,  0.0180,  0.0167,
				 0.0064,  0.0013, -0.0013, -0.0039, -0.0064,
				-0.0064, -0.0077, -0.0180, -0.0180, -0.0116,
				-0.0167, -0.0205 };
   static double slider_x[75] = { 0.0077, 0.0051,  0.0051, 0.0051, 0.0051,
				  0.0077, 0.0051,  0.0051, 0.0039, 0.0103,
				  0.0051, 0.0039,  0.0039, 0.0051, 0.0039,
				  0.0051, 0.0128,  0.0180, 0.0180, 0.0128,
				  0.0103, 0.0077,  0.0077, 0.0077, 0.0128,
				  0.0077, 0.0077,  0.0077, 0.0103, 0.0103,
				  0.0077, 0.0077,  0.0051, 0.0039, 0.0077,
				  0.0051, 0.0077,  0.0103, 0.0103, 0.0103,
				  0.0077, 0.0051,  0.0103, 0.0154, 0.0154,
				  0.0128, 0.0180,  0.0154, 0.0180, 0.0154,
				  0.0051, 0.0180,  0.0231, 0.0128, 0.0103,
				  0.0205, 0.0154,  0.0077, 0.0154, 0.0128,
				  0.0128, 0.0128,  0.0128, 0.0128, 0.0103,
				  0.0103, 0.0128,  0.0077, 0.0077, 0.0077,
				  0.0128, 0.0128,  0.0013, 0.0013, 0.0026 };
   static double slider_y[75] = { 0.0013, 0.0013,  0.0013, 0.0013, 0.0013,
				  0.0026, 0.0000,  0.0026, 0.0013, 0.0013,
				  0.0013, 0.0013,  0.0013, 0.0013, 0.0013,
				  0.0013, 0.0026,  0.0026, 0.0026, 0.0013,
				  0.0000, 0.0013,  0.0013, 0.0000, 0.0013,
				  0.0013, 0.0013,  0.0013, 0.0013, 0.0013,
				  0.0013, 0.0026,  0.0013, 0.0013, 0.0013,
				  0.0013, 0.0013,  0.0013, 0.0026, 0.0013,
				  0.0013, 0.0013,  0.0013, 0.0026, 0.0013,
				  0.0013, 0.0026,  0.0013, 0.0026, 0.0013,
				  0.0013, 0.0013,  0.0000, 0.0013, 0.0013,
				  0.0000, 0.0013,  0.0013, 0.0000, 0.0013,
				  0.0013, 0.0026,  0.0013, 0.0013, 0.0000,
				  0.0013, 0.0013, -0.0013, 0.0013, 0.0013,
				  0.0039, 0.0026,  0.0000, 0.0013, 0.0013 };
   TANGO_LOC   pos, pos2;
   TANGO_IMAGE rect, rect2, circ;
   TANGO_PATH  path, movepath, slider;
   TANGO_TRANS mover, checker, sli, slam, checkslam;

   rect = TANGOimage_create(TANGO_IMAGE_TYPE_BITMAP, 0.4172, 0.4365, 1, 
              bmaps, NUM, WIDTH, HEIGHT);
   path = TANGOpath_create(27, path_x, path_y);
   mover = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect, path);
   TANGOtrans_perform(mover);
   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_S);
   rect2 = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.7266, 0.7073, 1,
			     TANGO_COLOR_RED, 0.0616, 0.0526, 0.0000);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   slam = TANGOtrans_create(TANGO_TRANS_TYPE_SHUFFLE, rect, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_SE);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_E);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_NE);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_N);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_NW);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_W);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   pos = TANGOimage_loc(rect, TANGO_PART_TYPE_SW);
   pos2 = TANGOimage_loc(rect2, TANGO_PART_TYPE_NW);
   movepath = TANGOpath_motion(pos2, pos, TANGO_PATH_TYPE_STRAIGHT);
   checker = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect2, movepath);
   checkslam = TANGOtrans_compose(2, checker, slam);
   TANGOtrans_perform(checkslam);

   circ = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.1309, 0.4121, 1,
			    TANGO_COLOR_MAROON, 0.0411, 0.5000);
   slider = TANGOpath_create(75, slider_x, slider_y);
   sli = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, circ, slider);
   slam = TANGOtrans_create(TANGO_TRANS_TYPE_SHUFFLE, rect, slider);
   checkslam = TANGOtrans_compose(2, sli, slam);
   TANGOtrans_perform(checkslam);
}



