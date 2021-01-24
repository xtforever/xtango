/*   Drawing Hilbert Curves
     by  Nirupama Mallavarupu
*/

#include <stdio.h>
#include <xtango.h>


void ANIMInit(){
	ASSOCinit();
}

void
ANIMDraw(srcX,srcY,sizX,sizY)
double srcX, srcY,sizX,sizY;
{
   TANGO_IMAGE line;
   TANGO_PATH nullpath;
   TANGO_TRANS linetrans;

   line = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, srcX, srcY, 1, 
                 TANGO_COLOR_BLACK, sizX,sizY,  0.0000, 1.0000, 0);
   nullpath = TANGOpath_null(1);
   linetrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, line, nullpath);
   TANGOtrans_perform(linetrans);
}



