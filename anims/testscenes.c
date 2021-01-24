#include <stdio.h>
#include "xtango.h"


static TANGO_IMAGE_COMPONENT face[8] = {
   { TANGO_IMAGE_TYPE_CIRCLE, "0.0000 0.0000 TANGO_COLOR_BLACK 0.0570 0.0000"},
   { TANGO_IMAGE_TYPE_CIRCLE, "-0.0252 -0.0172 TANGO_COLOR_BLACK 0.0093 0.0000"},
   { TANGO_IMAGE_TYPE_CIRCLE, "0.0146 -0.0159 TANGO_COLOR_BLACK 0.0093 0.0000"},
   { TANGO_IMAGE_TYPE_LINE, "-0.0278 0.0278 TANGO_COLOR_BLACK 0.0119 0.0013 0.0000 1.0000 0"},
   { TANGO_IMAGE_TYPE_POLYLINE, "-0.0040 0.0013 TANGO_COLOR_BLACK 0.0000 1.0000 0 3 0.0119 0.0159 -0.0106 0.0159"},
   { TANGO_IMAGE_TYPE_LINE, "-0.0291 0.0384 TANGO_COLOR_BLACK 0.0543 -0.0026 0.0000 1.0000 0"},
   { TANGO_IMAGE_TYPE_LINE, "0.0000 0.0305 TANGO_COLOR_BLACK 0.0199 -0.0053 0.0000 1.0000 0"},
   { TANGO_IMAGE_TYPE_COMPOSITE, NULL}
};


void
scene1()
{
   static double path_x[28] = { -0.0066, 0.0079, 0.0106, 0.0252, 0.0146, 0.0132, 0.0079, 0.0026, 0.0000, -0.0053, -0.0093, -0.0106, -0.0132, -0.0093, -0.0132, -0.0146, -0.0119, -0.0026, -0.0040, 0.0066, 0.0119, 0.0172, 0.0132, 0.0000, -0.0026, -0.0053
     , -0.0106, -0.0066 };
   static double path_y[28] = { -0.0199, -0.0265, -0.0040, 0.0000, 0.0040, 0.0106, 0.0119, 0.0185, 0.0079, 0.0146, 0.0119, 0.0079, 0.0053, -0.0026, -0.0053, -0.0053, -0.0079, -0.0079, -0.0119, -0.0146, -0.0079, -0.0013, 0.0146, 0.0079, 0.0053, 0.0053
     , -0.0013, -0.0040 };
   static double ppolyline_x[4] = { 0.1245, 0.2503, 0.1695, 0.0901 };
   static double ppolyline_y[4] = { -0.1152, -0.0278, 0.0914, 0.0265 };
   static double polygon_x[4] = { 0.0967, 0.1987, 0.1325, -0.0026 };
   static double polygon_y[4] = { 0.0675, -0.0291, -0.1430, -0.0795 };
   static double spline_x[4] = { 0.1272, 0.1086, -0.0464, -0.1086 };
   static double spline_y[4] = { 0.0464, 0.1603, 0.1894, 0.1351 };
   TANGO_IMAGE line, rect, circle, ellipse, ppolyline, polygon, spline, text, compos;
   TANGO_PATH path;
   TANGO_TRANS m1, m2, m3, m4, m5, m6, m7, m8, m9;

   path = TANGOpath_create(28, path_x, path_y);
   line = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.1377, 0.0954, 1, TANGO_COLOR_BLACK, 0.1086, 0.0689, 0.5000, 0.5000, 0);
   rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.1007, 0.2503, 1, TANGO_COLOR_GREEN, 0.1126, 0.0609, 0.5000);
   circle = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 0.1298, 0.4556, 1, TANGO_COLOR_BLUE, 0.0477, 0.8333);
   ellipse = TANGOimage_create(TANGO_IMAGE_TYPE_ELLIPSE, 0.2609, 0.7046, 1, TANGO_COLOR_MAROON, 0.1682, 0.1662, 0.2777);
   ppolyline = TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE, 0.4291, 0.8212, 1, TANGO_COLOR_BLACK, 5, ppolyline_x, ppolyline_y, 1.0000, 1.0000, 3);
   polygon = TANGOimage_create(TANGO_IMAGE_TYPE_POLYGON, 0.6954, 0.7642, 1, TANGO_COLOR_RED, 5, polygon_x, polygon_y, 0.7222);
   spline = TANGOimage_create(TANGO_IMAGE_TYPE_SPLINE, 0.8358, 0.3285, 1, TANGO_COLOR_BLACK, 5, spline_x, spline_y, 0.0000, 1.0000, 0);
   text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.6411, 0.1748, 1, TANGO_COLOR_BLACK, NULL, "tstring");
   compos = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, 0.3682, 0.1642, 1, face);
   m1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, line, path);
   m2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect, path);
   m3 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, circle, path);
   m4 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ellipse, path);
   m5 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, ppolyline, path);
   m6 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, polygon, path);
   m7 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, spline, path);
   m8 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, text, path);
   m9 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, compos, path);
   TANGOtrans_perform(m1);
   TANGOtrans_perform(m2);
   TANGOtrans_perform(m3);
   TANGOtrans_perform(m4);
   TANGOtrans_perform(m5);
   TANGOtrans_perform(m6);
   TANGOtrans_perform(m7);
   TANGOtrans_perform(m8);
   TANGOtrans_perform(m9);

   m1 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, line, path);
   m2 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, rect, path);
   m3 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, circle, path);
   m4 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, ellipse, path);
   m5 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, ppolyline, path);
   m6 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, polygon, path);
   m7 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, spline, path);
   m8 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, text, path);
   m9 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, compos, path);
   TANGOtrans_perform(m1);
   TANGOtrans_perform(m2);
   TANGOtrans_perform(m3);
   TANGOtrans_perform(m4);
   TANGOtrans_perform(m5);
   TANGOtrans_perform(m6);
   TANGOtrans_perform(m7);
   TANGOtrans_perform(m8);
   TANGOtrans_perform(m9);
}




