/*    Viewing Transformations
         by Kim Liu
*/

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <xtango.h>
#include "viewtran.h"

static int STEP_NUM= 1;

TANGO_IMAGE StringImage;
TANGO_IMAGE CaptionImage;
TANGO_TRANS ViewTrans;
TANGO_TRANS ModelTrans;
TANGO_TRANS SquareTrans;
TANGO_IMAGE ViewImage[3];
TANGO_IMAGE ModelImage[3];
TANGO_IMAGE SquareImage[4];
double      ViewX[4];
double      ViewY[4];
double      ModelX[4];
double      ModelY[4];
double      SquareX[4];
double      SquareY[4];

void ANIMInitializeImages()
{
int count;
 for (count=0; count<4; count++) {
  ViewX[count]=0.0; ViewY[count]=0.0;
  ModelX[count]=0.0; ModelY[count]=0.0;
  SquareX[count]=0.0; SquareY[count]=0.0;
 }

 for (count = 0; count < 3; count++) {
  ViewImage[count] =
    TANGOimage_create( TANGO_IMAGE_TYPE_LINE , 
       0.0, 0.0, 1, TANGO_COLOR_BLACK, 
       ViewX[count+1], ViewY[count+1], 1.0, 1.0, (count==2) ? 1 : 0 );
  ModelImage[count] =
    TANGOimage_create( TANGO_IMAGE_TYPE_LINE , 
       0.0, 0.0, 1, TANGO_COLOR_MAROON, 
       ModelX[count+1], ModelY[count+1], 1.0, 1.0, (count==2) ? 1: 0 );
 }
 for (count = 0; count < 4; count++) {
  SquareImage[count] =
    TANGOimage_create( TANGO_IMAGE_TYPE_LINE , 
       0.0, 0.0, 1, TANGO_COLOR_BLUE, 
       SquareX[count], SquareY[count], 1.0, 1.0, 0 );
 }
  CaptionImage =
   TANGOimage_create( TANGO_IMAGE_TYPE_TEXT, 0.1, 0.8, 1, TANGO_COLOR_BLACK,
       NULL, "Black = View Point", 0 );
  TAPshow( CaptionImage );
  CaptionImage =
   TANGOimage_create( TANGO_IMAGE_TYPE_TEXT, 0.1, 0.825, 1, TANGO_COLOR_MAROON,
       NULL, "Maroon = Model origin", 0 );
  TAPshow( CaptionImage );
  CaptionImage =
   TANGOimage_create( TANGO_IMAGE_TYPE_TEXT, 0.1, 0.85, 1, TANGO_COLOR_BLUE,
       NULL, "Blue = Square", 0 );
  TAPshow( CaptionImage );
  CaptionImage =
   TANGOimage_create( TANGO_IMAGE_TYPE_TEXT, 0.1, 0.875, 1, TANGO_COLOR_BLACK,
       NULL, "Arrow heads on Z-axis", 0 );
  TAPshow( CaptionImage );
} /* End of InitializeImages */

void ANIMmakeViewTrans( NewX, NewY )
double *NewX; double *NewY;
{
TANGO_PATH Path;
TANGO_TRANS TS[3];
TANGO_TRANS TM[3];
TANGO_TRANS Step;

double vx, vy;
int Count;

  for (Count = 0; Count<3; Count++) {
    vx = NewX[Count+1] - ViewX[Count+1];
    vy = NewY[Count+1] - ViewY[Count+1];
    vx /= (double) STEP_NUM;
    vy /= (double) STEP_NUM;
    Path = TANGOpath_create( 1, &vx, &vy );
    TS[Count] =
      TANGOtrans_create( TANGO_TRANS_TYPE_RESIZE, ViewImage[Count], Path );
    TANGOpath_free( 1, Path );
    vx = NewX[0] - ViewX[0];
    vy = NewY[0] - ViewY[0];
    vx /= (double) STEP_NUM;
    vy /= (double) STEP_NUM;
    Path = TANGOpath_create( 1, &vx, &vy );
    TM[Count] =
      TANGOtrans_create( TANGO_TRANS_TYPE_MOVE, ViewImage[Count], Path );
    TANGOpath_free( 1, Path );
  }
  for (Count = 0; Count<4; Count++) {
    ViewX[Count] = NewX[Count];
    ViewY[Count] = NewY[Count];
  }
  Step =
    TANGOtrans_compose( 6, TS[0], TS[1], TS[2], 
                           TM[0], TM[1], TM[2] ); 
  ViewTrans = TANGOtrans_iterate( Step, STEP_NUM );
  TANGOtrans_free( 7, TS[0], TS[1], TS[2], Step,
                      TM[0], TM[1], TM[2]); 
} /* End of ANIMmoveView */

void ANIMmakeModelTrans( NewX, NewY )
double *NewX; double *NewY;
{
TANGO_PATH Path;
TANGO_TRANS TS[3];
TANGO_TRANS TM[3];
TANGO_TRANS Step;

double vx, vy;
int Count;

  for (Count = 0; Count<3; Count++) {
    vx = NewX[Count+1] - ModelX[Count+1];
    vy = NewY[Count+1] - ModelY[Count+1];
    vx /= (double) STEP_NUM;
    vy /= (double) STEP_NUM;
    Path = TANGOpath_create( 1, &vx, &vy );
    TS[Count] =
       TANGOtrans_create( TANGO_TRANS_TYPE_RESIZE, ModelImage[Count], Path );
    TANGOpath_free( 1, Path );
    vx = NewX[0] - ModelX[0];
    vy = NewY[0] - ModelY[0];
    vx /= (double) STEP_NUM;
    vy /= (double) STEP_NUM;
    Path = TANGOpath_create( 1, &vx, &vy );
    TM[Count] =
      TANGOtrans_create( TANGO_TRANS_TYPE_MOVE, ModelImage[Count], Path );
    TANGOpath_free( 1, Path );
  }
  for (Count = 0; Count<4; Count++) {
    ModelX[Count] = NewX[Count];
    ModelY[Count] = NewY[Count];
  }
  Step =
    TANGOtrans_compose( 6, TS[0], TS[1], TS[2], 
                           TM[0], TM[1], TM[2] ); 
  ModelTrans = TANGOtrans_iterate( Step, STEP_NUM );
  TANGOtrans_free( 7, TS[0], TS[1], TS[2], Step,
                      TM[0], TM[1], TM[2]); 
} /* End of ANIMmoveModel */

void ANIMmakeSquareTrans( NewX, NewY )
double *NewX; double *NewY;
{
TANGO_PATH Path;
TANGO_TRANS TS[4];
TANGO_TRANS TM[4];
TANGO_TRANS Step;

double vx, vy;
double lx, ly;
int Count;

  for (Count = 0; Count<4; Count++) {
    lx = NewX[(Count + 1) % 4] - NewX[Count];
    ly = NewY[(Count + 1) % 4] - NewY[Count];
    lx = lx -( SquareX[(Count+1) %4] - SquareX[Count] );
    ly = ly -( SquareY[(Count+1) %4] - SquareY[Count] );
    lx /=(double) STEP_NUM;
    ly /=(double) STEP_NUM;
    Path = TANGOpath_create( 1, &lx, &ly );
    TS[Count] =
      TANGOtrans_create( TANGO_TRANS_TYPE_RESIZE, SquareImage[Count], Path );
    TANGOpath_free( 1, Path );
    vx = NewX[Count] - SquareX[Count];
    vy = NewY[Count] - SquareY[Count];
    vx /=(double) STEP_NUM;
    vy /=(double) STEP_NUM;
    Path = TANGOpath_create( 1, &vx, &vy );
    TM[Count] =
      TANGOtrans_create( TANGO_TRANS_TYPE_MOVE, SquareImage[Count], Path );
    TANGOpath_free( 1, Path );
  }
  for (Count = 0; Count<4; Count++) {
    SquareX[Count] = NewX[Count];
    SquareY[Count] = NewY[Count];
  }
  Step =
    TANGOtrans_compose( 8, TS[0], TS[1], TS[2], TS[3],
                           TM[0], TM[1], TM[2], TM[3] ); 
  SquareTrans = TANGOtrans_iterate( Step, STEP_NUM );
  TANGOtrans_free( 9, TS[0], TS[1], TS[2], TS[3], Step,
                      TM[0], TM[1], TM[2], TM[3] ); 
} /* End of ANIMmoveSquare */

void ANIMturnon( )
{
 int c;
 for (c=0;c<3;c++){
  TAPshow(ViewImage[c]);
  TAPshow(ModelImage[c]);
 }
 for (c=0;c<4;c++){
  TAPshow(SquareImage[c]);
 }
 STEP_NUM = 30;
}

void ANIMdoTrans( )
{
  TANGO_TRANS Temp;

  Temp = TANGOtrans_compose( 3, ViewTrans, ModelTrans, SquareTrans );
  TANGOtrans_perform( Temp );
  TANGOtrans_free( 4, Temp, ViewTrans, ModelTrans, SquareTrans );
} /* End of doTrans */

void ANIMcleanup()
{
 return;
} /* End of ANIMcleanup */

void ANIMprint( Str )
char * Str;
{
  StringImage =
   TANGOimage_create( TANGO_IMAGE_TYPE_TEXT, 0.1, 0.9, 1,
                      TANGO_COLOR_BLACK, NULL, Str, 0 );
  TAPshow( StringImage );
}

void ANIMunprint( )
{
TANGO_TRANS Temp;

  Temp = TAPvis_toggle( StringImage );
  TANGOtrans_perform( Temp );
  TANGOtrans_free( 1, Temp );
}
