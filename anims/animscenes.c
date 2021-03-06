#include <stdio.h>
#include "xtangolocal.h"

static TANGO_IMAGE_COMPONENT comppoly[4] = {
   { TANGO_IMAGE_TYPE_CIRCLE, "0.010 0.01 TANGO_COLOR_BLUE 0.006 1.0"},
   { TANGO_IMAGE_TYPE_POLYGON, "0.02 0.02 TANGO_COLOR_MAROON 1.0 4 0.05 0.05 0.01 0.08 -0.07 0.04"},
   { TANGO_IMAGE_TYPE_POLYGON, "0.05 0.05 TANGO_COLOR_BLACK  1.0 4   0.05    0.05 0.01 0.08 -0.07   0.04  "},
   { TANGO_IMAGE_TYPE_COMPOSITE, NULL}
};

static TANGO_IMAGE_COMPONENT pt[2] = {
   { TANGO_IMAGE_TYPE_CIRCLE, "0.0 0.0 TANGO_COLOR_RED 0.0025 1.0"},
   { TANGO_IMAGE_TYPE_COMPOSITE, NULL}
};


void
scene1()
{
  int i;
   static double poly1_x[4] = { -0.099388 ,0.022936 ,0.117737 ,0.181957 };
   static double poly1_y[4] = { 0.145260 ,0.237003 ,0.177370 ,0.284404 };
   static double p2x[4] = { 0.1, 0.1, 0.1, 0.1 };
   static double p2y[4] = { 0.1, 0.1, 0.1, 0.1 };
   static double p3x[4] = { 0.0, 0.0, 0.0, -0.2 };
   static double p3y[4] = { 0.0, 0.0, 0.0, 0.1 };
   static double path1_x[133] = { 0.000000,-0.003058,0.000000,-0.001529,-0.001529,-0.001529,-0.003058,-0.003058,-0.003058,-0.012232,-0.009174,-0.003058,-0.012232,-0.009174,-0.006116,-0.009174,-0.003058,-0.004587,-0.001529,0.000000,0.000000,0.001529,0.003058,0.001529,0.004587,0.006116
     ,0.009174,0.006116,0.004587,0.001529,0.004587,0.004587,0.006116,0.003058,0.001529,0.000000,-0.001529,-0.001529,-0.001529,0.000000,-0.001529,0.000000,0.001529,0.003058,0.006116,0.012232,0.012232,0.015291,0.012232,0.012232,0.012232
     ,0.012232,0.009174,0.009174,0.009174,0.009174,0.006116,0.003058,0.003058,0.000000,0.000000,-0.003058,-0.003058,-0.001529,0.000000,0.000000,0.003058,0.001529,0.004587,0.004587,0.006116,0.015291,0.009174,0.003058,0.001529,0.003058
     ,0.003058,0.000000,0.000000,-0.003058,-0.003058,-0.006116,-0.009174,-0.012232,-0.012232,-0.012232,-0.012232,-0.009174,-0.015291,-0.009174,-0.006116,-0.001529,0.000000,0.000000,0.000000,0.001529,0.001529,0.001529,-0.001529,0.000000,-0.001529
     ,-0.003058,-0.004587,-0.003058,-0.004587,-0.004587,-0.004587,-0.003058,-0.003058,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.001529,-0.001529,-0.001529,-0.006116,-0.006116,-0.001529,0.000000,0.000000,0.001529,0.001529,0.001529
     ,0.001529,0.000000,0.000000,0.000000,0.000000,0.001529,0.000000 };
   static double path1_y[133] = { -0.001529,-0.007645,-0.003058,-0.003058,-0.003058,-0.003058,-0.004587,-0.003058,-0.003058,-0.006116,-0.003058,-0.001529,-0.001529,-0.001529,0.001529,0.001529,0.004587,0.003058,0.004587,0.004587,0.006116,0.006116,0.004587,0.004587,0.004587,0.006116
     ,0.009174,0.006116,0.004587,0.004587,0.006116,0.012232,0.012232,0.012232,0.015291,0.009174,0.004587,0.004587,0.004587,0.001529,0.004587,0.003058,0.006116,0.015291,0.009174,0.012232,0.006116,0.004587,0.003058,0.001529,0.000000
     ,-0.001529,-0.003058,-0.003058,-0.004587,-0.009174,-0.012232,-0.009174,-0.009174,-0.006116,-0.009174,-0.012232,-0.015291,-0.012232,-0.015291,-0.012232,-0.006116,-0.006116,-0.004587,-0.001529,-0.001529,-0.003058,-0.004587,-0.003058,-0.004587,-0.004587
     ,-0.009174,-0.012232,-0.012232,-0.009174,-0.012232,-0.009174,-0.009174,-0.006116,-0.004587,-0.004587,-0.001529,0.000000,0.001529,0.006116,0.009174,0.009174,0.004587,0.004587,0.009174,0.009174,0.009174,0.009174,0.015291,0.012232,0.009174
     ,0.006116,0.004587,0.003058,-0.001529,-0.003058,-0.004587,-0.009174,-0.015291,-0.012232,-0.009174,-0.004587,-0.006116,-0.001529,-0.003058,-0.004587,-0.001529,-0.001529,0.000000,0.003058,0.003058,0.006116,0.006116,0.009174,0.004587,0.006116
     ,0.004587,0.001529,0.003058,0.001529,0.001529,0.001529,0.000000 };
   TANGO_IMAGE poly1,poly2,poly3;
   TANGO_PATH path1,path2,path3,path4;
   TANGO_TRANS resize0,resize1,resize2,resize3,all;

struct _IMAGE image; 
struct _TANGO_POLYLINE poly;
 image.type = TANGO_IMAGE_TYPE_POLYLINE;
 image.loc[0] = 0.475535;
 image.loc[1] = 0.282875;
 image.visible = 1;
 poly.color=TANGO_COLOR_BLACK;
 poly.vertices=5;
 for(i=0;i<poly.vertices;i++)
   {
       poly.vertex[i][0]=poly1_x[i];  //vx
       poly.vertex[i][1]=poly1_y[i]; //vy
   }
 poly.width=0;
 poly.style=1;
 poly.arrow=3;
image.object = &poly;   
poly1=TANGOimage_create(&image); 

 image.type = TANGO_IMAGE_TYPE_POLYLINE;
 image.loc[0] = 0.2;
 image.loc[1] = 0.2;
 image.visible = 1;
 poly.color=TANGO_COLOR_BLACK;
 poly.vertices=5;
 for(i=0;i<poly.vertices;i++)
   {
       poly.vertex[i][0]=p2x[i];  //vx
       poly.vertex[i][1]=p2y[i]; //vy
   }
 poly.width=0;
 poly.style=1;
 poly.arrow=1;
image.object = &poly;   
poly2=TANGOimage_create(&image); 

 image.type = TANGO_IMAGE_TYPE_POLYLINE;
 image.loc[0] = 0.7;
 image.loc[1] = 0.7;
 image.visible = 1;
 poly.color=TANGO_COLOR_BLACK;
 poly.vertices=5;
 for(i=0;i<poly.vertices;i++)
   {
       poly.vertex[i][0]=p3x[i];  //vx
       poly.vertex[i][1]=p3y[i]; //vy
   }
 poly.width=0;
 poly.style=1;
 poly.arrow=2;
image.object = &poly;   
poly3=TANGOimage_create(&image); 

/*
   poly1 = TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE, 0.475535, 0.282875, 1,
		     TANGO_COLOR_BLACK, 5, poly1_x, poly1_y, 0.000000, 1.000000, 3);
   poly2 = TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE, 0.2, 0.2, 1,
		     TANGO_COLOR_BLACK, 5, p2x, p2y, 0.000000, 1.000000, 1);
   poly3 = TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE, 0.7, 0.7, 1,
		     TANGO_COLOR_BLACK, 5, p3x, p3y, 0.000000, 1.000000, 2);
*/


   path1 = TANGOpath_create(133, path1_x, path1_y);
   path2 = TANGOpath_rotate(path1,37);
   path3 = TANGOpath_rotate(path1,125);
   path4 = TANGOpath_rotate(path1,244);
   resize0 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1, poly1, path1);
   resize1 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB2, poly1, path2);
   resize2 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB3, poly1, path3);
   resize3 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB4, poly1, path4);
   all = TANGOtrans_compose(4,resize0,resize1,resize2,resize3);
   TANGOtrans_perform(all);
   TANGOpath_free(4,path1,path2,path3,path4);
   TANGOtrans_free(5,resize0,resize1,resize2,resize3,all);

   path1 = TANGOpath_type(TANGO_PATH_TYPE_STRAIGHT);
   resize0 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE2,poly2,path1);
   TANGOtrans_perform(resize0);
   path2 = TANGOpath_rotate(path1,90);
   resize1 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE2,poly3,path2);
   TANGOtrans_perform(resize1);
}





void
scene2()
{
   static double spl_x[3] = { 0.3 ,0.36 ,0.12 };
   static double spl_y[3] = { 0.1 ,0.4 ,0.31 };
   static double p1_x[87] = { 0.001344,0.009409,-0.001344,0.002688,-0.001344,0.000000,0.000000,0.005376,0.000000,0.000000,0.002688,0.002688,0.008065,0.004032,0.000000,0.000000,0.000000,0.013441,0.000000,0.016129,0.001344,0.005376,0.000000,0.002688,0.002688,0.001344
     ,0.000000,0.000000,-0.002688,-0.005376,-0.004032,-0.005376,-0.005376,-0.005376,-0.005376,-0.005376,0.000000,0.000000,-0.001344,0.001344,0.000000,0.005376,0.005376,0.013441,0.016129,0.001344,0.005376,0.000000,-0.001344,0.001344,0.000000
     ,0.004032,0.005376,0.005376,0.000000,0.001344,0.002688,0.001344,0.000000,0.001344,-0.001344,0.000000,0.000000,-0.001344,-0.008065,-0.008065,-0.008065,-0.005376,-0.013441,-0.026882,-0.008065,-0.002688,-0.002688,0.001344,-0.001344,0.000000
     ,0.001344,-0.001344,0.000000,0.000000,-0.001344,0.001344,-0.005376,-0.005376,0.000000,-0.005376,0.000000 };
   static double p1_y[87] = { -0.013441,-0.064516,-0.008065,-0.008065,-0.008065,-0.005376,-0.008065,-0.008065,-0.008065,-0.004032,-0.002688,-0.002688,-0.001344,0.000000,0.001344,0.001344,0.002688,0.004032,0.005376,0.008065,0.005376,0.005376,0.008065,0.008065,0.010753,0.013441
     ,0.010753,0.005376,0.005376,0.004032,0.004032,0.002688,0.002688,0.000000,0.001344,0.002688,0.002688,0.005376,0.005376,0.010753,0.010753,0.010753,0.005376,0.004032,0.001344,-0.001344,-0.004032,-0.004032,-0.008065,-0.013441,-0.008065
     ,-0.008065,-0.004032,-0.004032,0.001344,0.004032,0.005376,0.010753,0.013441,0.016129,0.010753,0.010753,0.013441,0.013441,0.008065,0.008065,0.002688,0.000000,0.001344,-0.002688,-0.002688,-0.004032,-0.002688,-0.002688,-0.002688,-0.002688
     ,0.000000,-0.002688,-0.002688,-0.005376,-0.005376,-0.005376,-0.008065,-0.008065,-0.002688,-0.001344,0.000000 };
   static double p2_x[91] = { 0.009409,0.008065,0.008065,0.001344,0.002688,0.002688,0.000000,0.000000,0.005376,0.005376,0.001344,0.013441,0.001344,0.002688,0.002688,0.004032,0.001344,-0.001344,-0.004032,-0.001344,-0.001344,0.001344,-0.010753,-0.016129,-0.016129,-0.010753
     ,-0.004032,-0.004032,0.000000,-0.002688,-0.018817,-0.008065,-0.016129,-0.002688,-0.010753,-0.008065,0.001344,0.000000,-0.001344,0.001344,0.001344,0.004032,0.005376,0.005376,0.008065,0.013441,0.005376,0.005376,0.005376,0.016129,0.005376
     ,0.005376,0.005376,0.016129,0.013441,0.002688,0.010753,0.016129,0.001344,0.008065,-0.002688,0.005376,0.001344,0.005376,0.000000,0.018817,0.013441,0.016129,0.000000,0.010753,0.004032,0.000000,-0.002688,-0.002688,-0.001344,-0.005376
     ,-0.013441,-0.024194,-0.026882,-0.013441,-0.004032,-0.001344,-0.001344,-0.005376,-0.005376,-0.010753,0.000000,-0.008065,-0.002688,-0.001344,0.000000 };
   static double p2_y[91] = { -0.072581,-0.010753,-0.010753,-0.004032,-0.002688,-0.001344,-0.001344,-0.001344,0.000000,0.000000,0.002688,0.004032,0.004032,0.005376,0.010753,0.010753,0.008065,0.008065,0.010753,0.010753,0.004032,0.001344,-0.001344,-0.001344,-0.005376,-0.010753
     ,-0.010753,-0.001344,-0.001344,-0.004032,-0.004032,-0.005376,-0.004032,-0.001344,0.004032,0.008065,0.013441,0.010753,0.005376,0.010753,0.018817,0.016129,0.016129,0.013441,0.010753,0.013441,0.013441,0.010753,0.002688,-0.001344,-0.002688
     ,-0.004032,-0.010753,-0.016129,-0.013441,-0.016129,-0.013441,-0.016129,-0.016129,-0.013441,-0.013441,-0.008065,-0.010753,-0.004032,-0.001344,0.000000,0.005376,0.016129,0.024194,0.024194,0.026882,0.021505,0.024194,0.018817,0.016129,0.004032
     ,0.001344,-0.001344,-0.002688,-0.008065,-0.008065,-0.008065,-0.010753,-0.005376,-0.005376,-0.010753,-0.010753,-0.018817,-0.005376,-0.001344,0.000000 };
   static double p3_x[82] = { -0.017473,-0.006720,0.000000,0.000000,0.000000,0.000000,0.000000,-0.001344,0.008065,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.005376,0.021505,0.010753,-0.001344,0.001344,0.005376,0.000000,0.005376,0.000000,-0.005376
     ,0.000000,-0.013441,-0.008065,-0.016129,-0.010753,-0.010753,-0.002688,-0.010753,0.000000,0.000000,0.001344,0.000000,0.000000,0.004032,0.016129,0.000000,0.004032,0.008065,0.001344,0.004032,0.000000,0.000000,-0.008065,-0.040323,-0.037634
     ,-0.032258,-0.026882,-0.010753,0.000000,0.000000,-0.001344,0.004032,0.008065,0.005376,-0.001344,0.013441,0.021505,0.001344,0.004032,0.016129,0.005376,0.008065,0.004032,0.001344,0.004032,0.005376,0.005376,0.016129,0.016129,0.000000
     ,0.002688,0.004032,0.002688,0.001344,0.001344,0.000000 };
   static double p3_y[82] = { 0.014785,0.021505,0.005376,0.004032,0.005376,0.004032,0.004032,0.013441,0.010753,0.010753,0.004032,-0.002688,-0.001344,-0.013441,-0.005376,-0.004032,-0.010753,-0.018817,-0.016129,-0.021505,-0.021505,-0.026882,-0.032258,-0.026882,-0.021505,-0.013441
     ,-0.002688,-0.002688,-0.001344,0.002688,0.008065,0.008065,0.010753,0.010753,0.010753,0.008065,0.005376,0.002688,0.005376,0.021505,0.008065,0.004032,-0.002688,-0.005376,-0.001344,0.002688,0.004032,0.005376,0.004032,-0.001344,-0.004032
     ,-0.002688,0.000000,0.000000,0.002688,0.005376,0.010753,0.016129,0.016129,0.005376,0.001344,0.000000,0.001344,-0.002688,-0.008065,-0.008065,-0.008065,-0.005376,-0.005376,-0.001344,0.000000,0.001344,0.005376,0.005376,0.010753,0.002688
     ,-0.002688,-0.004032,-0.004032,-0.001344,-0.001344,0.000000 };
   TANGO_IMAGE spl;
   TANGO_PATH p1, p2, p3;
   TANGO_TRANS r1, r2, r3, all;

struct _IMAGE ximage; 
struct _TANGO_SPLINE spline;
 int i;
 ximage.type = TANGO_IMAGE_TYPE_SPLINE;
 ximage.loc[0] = 0.383065;
  ximage.loc[1] = 0.346774;
  ximage.visible =1;
  spline.color=TANGO_COLOR_MAROON;
  spline.vertices=4;
  for(i=0;i<spline.vertices;i++) 
    {
      spline.vertex[i][0] = spl_x[i];//vx
      spline.vertex[i][1] = spl_y[i];//vy
    }
  spline.width=0.5;
 spline.style=1;

ximage.object = &spline;   
spl=TANGOimage_create(&ximage); 

   //   spl = TANGOimage_create(TANGO_IMAGE_TYPE_SPLINE, 0.383065, 0.346774, 1, TANGO_COLOR_MAROON, 4, spl_x, spl_y, 0.5, 1.000000);
   p1 = TANGOpath_create(87, p1_x, p1_y);
   p2 = TANGOpath_create(91, p2_x, p2_y);
   p3 = TANGOpath_create(82, p3_x, p3_y);
   r1 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB1, spl, p1);
   r2 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB2, spl, p2);
   r3 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB3, spl, p3);
   all = TANGOtrans_compose(3, r1, r2, r3);
   TANGOtrans_perform(all);
   TANGOtrans_free(4, r1,r2,r3,all);
   TANGOpath_free(3,p1,p2,p3);
}





void
scene3()
{
   static double polygon1_x[5] = { 0.128882 ,0.150621 ,0.091615 ,-0.069876 ,-0.091615 };
   static double polygon1_y[5] = { 0.020186 ,0.156832 ,0.206522 ,0.156832 ,0.074534 };
   static double path_x[72] = { 0.013975,-0.001553,0.003106,0.004658,0.001553,0.004658,0.001553,0.001553,0.001553,0.000000,0.000000,0.000000,0.000000,0.001553,0.006211,0.003106,0.012422,0.012422,0.004658,0.000000,0.003106,0.000000,-0.003106,-0.001553,-0.015528,-0.012422
     ,-0.015528,-0.006211,0.000000,-0.003106,-0.003106,-0.003106,-0.001553,-0.004658,-0.009317,-0.004658,-0.003106,-0.001553,-0.003106,-0.004658,-0.006211,-0.012422,-0.015528,-0.006211,-0.001553,-0.006211,0.000000,0.000000,0.000000,0.000000,0.006211
     ,0.003106,0.003106,0.001553,0.004658,0.000000,0.004658,0.001553,0.004658,0.001553,-0.001553,0.000000,0.003106,0.001553,0.001553,0.006211,0.009317,0.009317,0.006211,0.003106,0.003106,0.000000 };
   static double path_y[72] = { 0.020186,0.001553,0.000000,0.000000,0.004658,0.004658,0.003106,0.006211,0.003106,0.006211,0.006211,0.003106,0.003106,0.006211,0.004658,0.001553,-0.001553,0.000000,0.003106,0.001553,0.012422,0.012422,0.003106,0.001553,-0.001553,0.000000
     ,-0.001553,-0.003106,-0.001553,-0.003106,-0.004658,-0.003106,-0.003106,-0.003106,0.000000,0.003106,0.003106,0.003106,0.003106,0.006211,0.003106,0.001553,-0.001553,-0.001553,-0.003106,-0.009317,-0.004658,-0.003106,-0.004658,-0.004658,-0.006211
     ,-0.004658,0.000000,-0.001553,-0.001553,-0.004658,-0.003106,-0.003106,-0.003106,-0.006211,-0.006211,-0.004658,-0.003106,-0.004658,-0.003106,0.000000,0.003106,0.000000,-0.003106,-0.003106,-0.003106,0.000000 };
   static double px[13] = { .1, .1, .1, .1, .1, .1, .1, .1, .1, .1, .1, .1, .1};
   static double py[13] = { .1, .1, .1, .1, .1, .1, .1, .1, .1, .1, .1, .1, .1};
   TANGO_IMAGE polygon1, polygon2;
   TANGO_PATH path,p;
   TANGO_TRANS trans1, trans2, zoomer;

struct _IMAGE ximage; 
struct _TANGO_POLYGON pgon;
int i;
 ximage.type = TANGO_IMAGE_TYPE_POLYGON;
  ximage.loc[0] = 0.672360;
  ximage.loc[1] = 0.288820;
  ximage.visible =1;
  pgon.color=TANGO_COLOR_GREEN;
  pgon.sides=5;
  for(i=0;i<pgon.sides;i++) 
    {
      pgon.vertex[i][0] =  polygon1_x[i];//vx
      pgon.vertex[i][1] =  polygon1_y[i];//vy
    }
  pgon.fill=1;

  ximage.object = &pgon;   
  polygon1=TANGOimage_create(&ximage); 


//   polygon1 = TANGOimage_create(TANGO_IMAGE_TYPE_POLYGON, 0.672360, 0.288820, 1, TANGO_COLOR_GREEN, 6, polygon1_x, polygon1_y, 1.000000);

   path = TANGOpath_create(72, path_x, path_y);
   trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_GRAB3, polygon1, path);
   TANGOtrans_perform(trans1);
   ximage.type = TANGO_IMAGE_TYPE_POLYGON;
   ximage.loc[0] = 0.3;
   ximage.loc[1] = 0.288820;
   ximage.visible =1;
   pgon.color=TANGO_COLOR_GREEN;
   pgon.sides=5;
   for(i=0;i<pgon.sides;i++) 
     {
       pgon.vertex[i][0] =  polygon1_x[i];//vx
       pgon.vertex[i][1] =  polygon1_y[i];//vy
     }
   pgon.fill=1;

   ximage.object = &pgon;   
   polygon2=TANGOimage_create(&ximage); 
  
//   polygon2 = TANGOimage_create(TANGO_IMAGE_TYPE_POLYGON, 0.3, 0.288820, 1, TANGO_COLOR_GREEN, 6, polygon1_x, polygon1_y, 1.000000);
   trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE3, polygon2, path);
   TANGOtrans_perform(trans2);

/*   p = TANGOpath_create(13, px, py);
   zoomer = TANGOtrans_create(TANGO_TRANS_TYPE_ZOOM, polygon2, p);
   TANGOtrans_perform(zoomer);
*/
}






char *
scene4()
{
  //  double  px[7],py[7];
   TANGO_IMAGE polygon,ptest1,ptest2,ellipse;
   TANGO_PATH  patht,path;
   TANGO_TRANS move,res1,res2,trans;

 struct _IMAGE ximage; 
 struct _TANGO_ELLIPSE elli;
 ximage.type = TANGO_IMAGE_TYPE_ELLIPSE;
 ximage.loc[0] = 0.2;
 ximage.loc[1] = 0.6;
 ximage.visible = 1;
 elli.color=TANGO_COLOR_GREEN;
 elli.size[0]=0.1;
 elli.size[1]=0.05;
 elli.fill=1;
 ximage.object = &elli;   
 ellipse=TANGOimage_create(&ximage); 


   //   ellipse = TANGOimage_create(TANGO_IMAGE_TYPE_ELLIPSE,0.2,0.6,1,
   //				  TANGO_COLOR_GREEN,0.1,0.05,1.0);
   path = TANGOpath_type(TANGO_PATH_TYPE_CLOCKWISE);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,ellipse,path);
   TANGOtrans_perform(move);

   //  px[0] = 0.05;    py[0] = 0.1;
   //   px[1] = -0.05;    py[1] = 0.1;

 struct _TANGO_POLYGON pgon;
   ximage.type = TANGO_IMAGE_TYPE_POLYGON;
   ximage.loc[0] = 0.2;
   ximage.loc[1] = 0.4;
   ximage.visible =1;
   pgon.color=TANGO_COLOR_BLUE;
   pgon.sides=2;

      pgon.vertex[0][0] = 0.05;//vx
      pgon.vertex[0][1] = 0.1;;//vy
      pgon.vertex[1][0] = -0.05;//vx
      pgon.vertex[1][1] = 0.1;//vy

  pgon.fill=0.5;

  ximage.object = &pgon;   
  polygon=TANGOimage_create(&ximage); 

//   polygon = TANGOimage_create(TANGO_IMAGE_TYPE_POLYGON,0.2,0.4,1,
//				  TANGO_COLOR_BLUE,3,px,py,0.5);
   patht = TANGOpath_type(TANGO_PATH_TYPE_CLOCKWISE);
   path = TANGOpath_interpolate(patht,5.0);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,polygon,path);
   res1 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE1,polygon,path);
   res2 = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE2,polygon,path);
   trans = TANGOtrans_compose(3,move,res1,res2);
   TANGOtrans_perform(trans);

   return("here is a string");
}






void
scene5()
{
   static double p1_x[111] = { 0.041494,0.012448,0.016598,0.012448,0.012448,0.008299,0.004149,0.006224,0.004149,0.006224,0.008299,0.012448,0.012448,0.012448,0.016598,0.020747,0.020747,0.020747,0.016598,0.016598,0.012448,0.016598,0.016598,0.016598,0.0124480,.012448
     ,0.016598,0.016598,0.020747,0.012448,0.012448,0.008299,0.012448,0.008299,0.008299,0.012448,0.012448,0.012448,0.016598,0.016598,0.016598,0.020747,0.024896,0.020747,0.016598,0.012448,0.012448,0.016598,0.024896,0.0124480,.016598
     ,0.012448,0.012448,0.004149,0.004149,0.002075,0.000000,-0.002075,-0.006224,-0.008299,-0.012448,-0.020747,-0.020747,-0.020747,-0.024896,-0.029046,-0.033195,-0.037344,-0.037344,-0.020747,-0.016598,-0.016598,-0.012448,-0.012448,-0.012448,-0.012448
     ,-0.016598,-0.012448,-0.016598,-0.016598,-0.058091,-0.016598,-0.012448,-0.020747,-0.016598,-0.020747,-0.020747,-0.016598,-0.020747,-0.012448,-0.012448,-0.016598,-0.008299,-0.006224,-0.008299,-0.006224,-0.012448,-0.008299,-0.008299,-0.012448,-0.008299
     ,-0.006224,-0.012448,-0.004149,-0.004149,-0.004149,-0.006224,-0.006224,-0.006224,0.000000,0.000000 };
   static double p1_y[111] = { 0.109959,0.020747,0.020747,0.020747,0.008299,0.008299,0.004149,0.004149,0.004149,0.004149,0.004149,0.004149,0.004149,0.000000,0.002075,0.000000,-0.002075,-0.004149,-0.002075,-0.006224,-0.004149,-0.006224,-0.008299,-0.012448,-0.012448,-0.016598
     ,-0.016598,-0.020747,-0.020747,-0.020747,-0.020747,-0.024896,-0.016598,-0.020747,-0.024896,-0.024896,-0.024896,-0.024896,-0.024896,-0.020747,-0.020747,-0.016598,-0.008299,-0.008299,-0.002075,0.002075,0.006224,0.016598,0.020747,0.0207470,.020747
     ,0.024896,0.029046,0.020747,0.024896,0.029046,0.033195,0.024896,0.024896,0.020747,0.016598,0.012448,0.012448,0.008299,0.006224,0.002075,0.002075,-0.002075,-0.004149,-0.008299,-0.016598,-0.016598,-0.016598,-0.024896,-0.024896,-0.024896
     ,-0.033195,-0.024896,-0.029046,-0.020747,-0.049793,-0.006224,-0.006224,-0.004149,0.000000,0.000000,0.000000,0.002075,0.002075,0.004149,0.008299,0.008299,0.008299,0.008299,0.008299,0.012448,0.012448,0.012448,0.012448,0.0082990,.008299
     ,0.006224,0.010373,0.004149,0.004149,0.000000,0.004149,0.004149,0.004149,0.002075,0.000000 };
   TANGO_IMAGE rect;
   TANGO_PATH p1, blue, green;
   TANGO_TRANS mover, filler, fly, c1, c2, loop, biggy;


   struct _IMAGE ximage; 
   struct _TANGO_RECTANGLE rec;
   ximage.type = TANGO_IMAGE_TYPE_RECTANGLE;
   ximage.loc[0] = 0.053942;
   ximage.loc[1] = 0.379668;
   ximage.visible = 1;
   rec.color=TANGO_COLOR_BLUE;
   rec.size[0]=0.078838;
   rec.size[1]=0.080913;
   rec.fill = 0.125000;
   ximage.object = &rec;   
   rect=TANGOimage_create(&ximage); 

   //   rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.053942, 0.379668, 1, TANGO_COLOR_BLUE, 0.078838, 0.080913, 0.125000);

   p1 = TANGOpath_create(111, p1_x, p1_y);
   mover = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, rect, p1);
   filler = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, rect, p1);
   fly = TANGOtrans_compose(2, mover, filler);
   blue = TANGOpath_color(TANGO_COLOR_ORANGE);
   green = TANGOpath_color(TANGO_COLOR_BLUE);
   c1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, rect, blue);
   c2 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, rect, green);
   loop = TANGOtrans_concatenate(4, fly, c1, fly, c2);
   biggy = TANGOtrans_iterate(loop, 2);
   TANGOtrans_perform(biggy);
}




void
scene6()
{
   TANGO_IMAGE l1, l2, l3, l4;
   TANGO_IMAGE a1, a2, a3, a4;
   TANGO_IMAGE r1, r2, r3, r4;
   TANGO_PATH path;
   TANGO_TRANS trans;
   struct _IMAGE ximage; 
   struct _TANGO_LINE line;
   ximage.type = TANGO_IMAGE_TYPE_LINE;
   ximage.loc[0] = 0.188873;
   ximage.loc[1] = 0.117130;
   ximage.visible =1;
   line.color=TANGO_COLOR_BLACK;
   line.size[0]=0.000000;
   line.size[1]=0.115666;
   line.width=0.000000;
   line.style=1;
   line.arrow=1;
   ximage.object = &line;   
   l1=TANGOimage_create(&ximage); 
 
//   l1 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.188873, 0.117130, 1, TANGO_COLOR_BLACK, 0.000000, 0.115666, 0.000000, 1.000000, 1);

 ximage.loc[0] = 0.243045;    ximage.loc[1] = 0.231332;
 line.size[0]=0.000000;  line.size[1]=-0.120059;
 line.arrow=1;
 ximage.object = &line;   
 l2=TANGOimage_create(&ximage); 
 
 //   l2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.243045, 0.231332, 1, TANGO_COLOR_BLACK, 0.000000, -0.120059, 0.000000, 1.000000, 1);

 ximage.loc[0] = 0.286969 ;    ximage.loc[1] = 0.168375;
 line.size[0]=0.077599;  line.size[1]=0.0;
 line.arrow=1;
 ximage.object = &line;  
 l3=TANGOimage_create(&ximage); 

 //   l3 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.286969, 0.168375, 1, TANGO_COLOR_BLACK, 0.077599, 0.000000, 0.000000, 1.000000, 1);
 ximage.loc[0] =  0.489019;    ximage.loc[1] =  0.172767;
 line.size[0]= -0.067350;  line.size[1]=  0.000000;
 line.arrow= 1;
 ximage.object = &line;   
 l4=TANGOimage_create(&ximage); 

 //   l4 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.489019, 0.172767, 1, TANGO_COLOR_BLACK, -0.067350, 0.000000, 0.000000, 1.000000, 1);
 
 ximage.loc[0] = 0.188873 ;    ximage.loc[1] =  0.317130;
 line.size[0]= 0.000000 ;  line.size[1]=   0.115666;
 line.arrow= 2;
 ximage.object = &line;   
 a1=TANGOimage_create(&ximage); 

 //  a1 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.188873, 0.317130, 1, TANGO_COLOR_BLACK, 0.000000, 0.115666, 0.000000, 1.000000, 2);

ximage.loc[0] =  0.243045;    ximage.loc[1] = 0.431332 ;
 line.size[0]= 0.000000;  line.size[1]=   -0.120059;
 line.arrow= 2;
 ximage.object = &line;   
 a2=TANGOimage_create(&ximage); 

 //   a2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.243045, 0.431332, 1, TANGO_COLOR_BLACK, 0.000000, -0.120059, 0.000000, 1.000000, 2);
 
ximage.loc[0] =  0.286969;    ximage.loc[1] =  0.368375;
 line.size[0]= 0.077599;  line.size[1]=   0.000000;
 line.arrow=2 ;
 ximage.object = &line;   
 a3=TANGOimage_create(&ximage); 

 //  a3 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.286969, 0.368375, 1, TANGO_COLOR_BLACK, 0.077599, 0.000000, 0.000000, 1.000000, 2);
 ximage.loc[0] =   0.489019;    ximage.loc[1] = 0.372767;
 line.size[0]= -0.067350;  line.size[1]=   0.000000;
 line.arrow=2 ;
 ximage.object = &line;   
 a4=TANGOimage_create(&ximage); 

 // a4 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.489019, 0.372767, 1, TANGO_COLOR_BLACK, -0.067350, 0.000000, 0.000000, 1.000000, 2);

 ximage.loc[0] =   0.188873;    ximage.loc[1] =  0.517130;
 line.size[0]= 0.000000;  line.size[1]=   0.115666 ;
 line.arrow=3;
 ximage.object = &line;   
 r1=TANGOimage_create(&ximage); 

 //   r1 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.188873, 0.517130, 1, TANGO_COLOR_BLACK, 0.000000, 0.115666, 0.000000, 1.000000, 3);
 ximage.loc[0] =0.243045   ;    ximage.loc[1] = 0.631332 ;
 line.size[0]= 0.000000;  line.size[1]=   -0.120059 ;
 line.arrow=3;
 ximage.object = &line;   
 r2=TANGOimage_create(&ximage); 

 //   r2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.243045, 0.631332, 1, TANGO_COLOR_BLACK, 0.000000, -0.120059, 0.000000, 1.000000, 3);
 ximage.loc[0] = 0.286969   ;    ximage.loc[1] = 0.568375;
 line.size[0]= 0.077599;  line.size[1]=  0.000000  ;
 line.arrow=3;
 ximage.object = &line;   
 r3=TANGOimage_create(&ximage); 

 //   r3 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.286969, 0.568375, 1, TANGO_COLOR_BLACK, 0.077599, 0.000000, 0.000000, 1.000000, 3);
 ximage.loc[0] =0.489019   ;    ximage.loc[1] = 0.572767 ;
 line.size[0]=  -0.067350;  line.size[1]=   0.000000 ;
 line.arrow=3;
 ximage.object = &line;   
 r4=TANGOimage_create(&ximage); 

 //   r4 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.489019, 0.572767, 1, TANGO_COLOR_BLACK, -0.067350, 0.000000, 0.000000, 1.000000, 3);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, l4, path);
   TANGOtrans_perform(trans);
}




void
scene7()
{
   static double path_x[61] = { 0.000000,0.000000,-0.025460,-0.002829,-0.005658,-0.029703,-0.005658,-0.005658,0.000000,-0.005658,-0.005658,0.000000,-0.005658,-0.001414,0.007072,0.011315,0.005658,0.005658,0.011315,0.033946,0.000000,0.038190,0.016973,0.050919,0.012730,0.011315
     ,0.031117,0.015559,0.005658,0.011315,0.000000,0.002829,0.014144,0.014144,0.001414,-0.002829,0.004243,0.000000,-0.005658,-0.009901,-0.005658,-0.028289,-0.011315,0.000000,-0.026874,-0.016973,-0.001414,0.002829,0.004243,0.005658,0.000000
     ,-0.005658,-0.012730,-0.009901,-0.022631,-0.011315,-0.012730,-0.004243,-0.011315,-0.005658,-0.012730 };
   static double path_y[61] = { -0.001414,-0.002829,0.008487,-0.004243,0.001414,0.001414,0.005658,0.024045,0.008487,0.028289,0.019802,0.004243,0.029703,0.014144,0.031117,0.012730,0.018388,0.016973,0.012730,0.008487,0.007072,0.018388,0.002829,0.000000,0.000000,0.000000
     ,-0.004243,-0.002829,-0.004243,-0.016973,-0.019802,-0.015559,-0.031117,-0.045262,-0.031117,-0.014144,-0.012730,-0.025460,-0.035361,-0.035361,-0.038190,-0.012730,-0.004243,-0.004243,0.001414,0.016973,0.024045,0.025460,0.019802,0.016973,0.039604
     ,0.016973,0.016973,-0.004243,-0.005658,0.001414,-0.005658,-0.012730,-0.016973,-0.011315,-0.024045 };
   TANGO_IMAGE line;
   TANGO_PATH path;
   TANGO_TRANS trans;

   struct _IMAGE ximage; 
   struct _TANGO_LINE xline;
   ximage.type = TANGO_IMAGE_TYPE_LINE;
   ximage.loc[0] = 0.432815;
   ximage.loc[1] = 0.519095;
   ximage.visible =1;
   xline.color=TANGO_COLOR_BLACK;
   xline.size[0]=0.001414;
   xline.size[1]= -0.189533;
   xline.width=0.000000;
   xline.style=1;
   xline.arrow=3;
   ximage.object = &xline;   
   line=TANGOimage_create(&ximage); 

   //   line = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, 0.432815, 0.519095, 1, TANGO_COLOR_BLACK, 0.001414, -0.189533, 0.000000, 1.000000, 3);
   path = TANGOpath_create(61, path_x, path_y);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE, line, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}





