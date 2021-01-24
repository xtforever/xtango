/*****************************************************************************/
/*  Fast Fourier Transform Algorithm Animation-by Tim Dean                   */
/*  For use with the XTANGO algorithm animation system.                      */
/*                                                                           */
/*  This file contains the animation routines used to animate the Fast       */
/*  Fourier Transform algorithm in the file "fft.c"                          */
/*****************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "xtango.h"

#define ELEMENT_HEIGHT 0.05
#define ELEMENT_WIDTH  0.2
#define PI (4*atan(1.0))


/*****************************************************************************/
/*  The create_component() routine is used to create the image parameter to  */
/*  be used as a argument to the TANGOimage_create() function for a image of */
/*  the type TANGO_IMAGE_TYPE_COMPOSITE.  Since creation of an image to show */
/*  a complex valued data point will be needed repeatedly, this was placed   */
/*  into a subroutine to facilitate quicker creation. The input values are   */
/*  the real and imaginary parts of the complex value to be visualized. Also */
/*  required is a properly allocated array of type TANGO_IMAGE_COMPONENT.    */
/*  This array should have a size of 4 to properly hold the information.     */
/*****************************************************************************/
void create_component(real,imag,temp)
     float real,imag;
     TANGO_IMAGE_COMPONENT *temp;
{
       
  /*  ****  Outer Rectangle Definition   **** */
  temp[0].type=TANGO_IMAGE_TYPE_RECTANGLE;
  temp[0].args=(char *)calloc(80,sizeof(char));
  sprintf(temp[0].args,"%f %f TANGO_COLOR_BLUE %f %f 1.0",
	  0.0,0.0,ELEMENT_WIDTH,ELEMENT_HEIGHT);
  
  /*  ****  Border Rectangle Definition   **** */
  temp[1].type=TANGO_IMAGE_TYPE_RECTANGLE;
  temp[1].args=(char *)calloc(80,sizeof(char));
  sprintf(temp[1].args,"%f %f TANGO_COLOR_BLACK %f %f 0.0",
	  0.0,0.0,ELEMENT_WIDTH,ELEMENT_HEIGHT);
  
  /* ****  Value Definition  **** */
  temp[2].type=TANGO_IMAGE_TYPE_TEXT;
  temp[2].args=(char *)calloc(80,sizeof(char));
  if(imag >= 0)  
    sprintf(temp[2].args,"%f %f TANGO_COLOR_YELLOW variable %3.1f+j%3.1f 1",
	    0.5*ELEMENT_WIDTH,0.5*ELEMENT_HEIGHT,real,imag);
  else
    sprintf(temp[2].args,"%f %f TANGO_COLOR_YELLOW variable %3.1f-j%3.1f 1",
	    0.5*ELEMENT_WIDTH,0.5*ELEMENT_HEIGHT,real,-imag);

  temp[3].type=TANGO_IMAGE_TYPE_COMPOSITE;
  temp[3].args=NULL;

}  /*  CREATE_COMPONENT()  */


/*****************************************************************************/
/*  The purpose of the delete_component() routine is to free up the memory   */
/*  that was allocated during creation of the component object.              */
/*****************************************************************************/
void delete_component(comp)
     TANGO_IMAGE_COMPONENT *comp;
{
  int i;

  for(i=0;i<4;i++)
    free(comp[i].args);
}  /*  DELETE_COMPONENT()  */


/*****************************************************************************/
/*  The ANIMInit() routine is used to initialize the display for the fft     */
/*  algorithm animation.  Its input parameters are the size of the data list */
/*  and the array of data list values.  Note that the size of the list is    */
/*  twice the number of complex values, since the data is stored as an alt-  */
/*  -ternating list of real and complex values.                              */
/*****************************************************************************/
void ANIMInit(size,data)
     int size;
     float *data;
{
  int i;
  TANGO_IMAGE value;
  TANGO_IMAGE_COMPONENT components[4];
  TANGO_PATH path;
  TANGO_TRANS trans;
  TANGO_LOC loc;
  float startx,starty;

  ASSOCinit();
  
  ASSOCmake("NUM_ELEMENTS",0);              /*  Number of Complex Elements  */
  ASSOCstore("NUM_ELEMENTS",size/2);        /*  in input sequence           */

  ASSOCmake("VALUE_AT",1);                  /*  Location of Values          */
  ASSOCmake("VALUE",1);                     /*  Actual Value Box Images     */

  ASSOCmake("NEW_VALUES",1);                /*  Location and Images for New */
  
  ASSOCmake("TWIDDLE_TRANS",0);             /*  Twiddle Factor Transitions  */ 
  ASSOCmake("BUTTERFLIES",0);               /*  Butterfly Transitions       */

  startx=0.5-(ELEMENT_WIDTH/2);
  starty=0.5-size*ELEMENT_HEIGHT/2;

  for(i=1;i<size;i+=2){
    create_component(data[i],data[i+1],components);
    value=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,startx,
			    starty+((float)(i-1))*ELEMENT_HEIGHT,1,components);
    ASSOCstore("VALUE",(i-1)/2,value);
    loc=TANGOimage_loc(value,TANGO_PART_TYPE_C);
    ASSOCstore("VALUE_AT",(i-1)/2,loc);
  }  /*  FOR - Each Complex Array Element  */

  path=TANGOpath_null(3);
  trans=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
  TANGOtrans_perform(trans);
  TANGOpath_free(1,path);
  TANGOtrans_free(1,trans);

}  /*  ANIMInit()  */


/*****************************************************************************/
/*  The ANIMFlip() routine animates the bit reversal method that is used to  */
/*  reorder the input data list.  Since the FFT algorithm uses a 'clever'    */
/*  implementation, this routine takes some extra work to compute the data   */
/*  that is needed to show how the algorithm works.                          */
/*****************************************************************************/
void ANIMFlip(){
  int i,j,temp,numbits,size;
  unsigned bitsize;
  int **bit_patterns;
  char buffer[3];
  TANGO_LOC *spots1,*spots2;
  TANGO_IMAGE *images,*copies;
  TANGO_PATH *paths;
  TANGO_TRANS *movers,move_all,line,null_trans;  

  ASSOCmake("BIT_IMAGES",2);
  ASSOCmake("BIT_COPIES",2);
  ASSOCmake("NUM_BITS",0);

  size=(int)ASSOCretrieve("NUM_ELEMENTS");
  frexp((double)size,&numbits);
  numbits--;
  bitsize=(unsigned)numbits;

  ASSOCstore("NUM_BITS",numbits);

  /*********************************************************/
  /*  Since the actual FFT algorithm uses roundabout ways  */
  /*  to reverse bits, we must seperately figure out the   */
  /*  bit patterns in order to visualize the bit reversal  */
  /*  process. This is done below.                         */
  /*********************************************************/
  bit_patterns=(int **)calloc((unsigned)size,sizeof(int*));
  for(i=0;i<size;i++){
    bit_patterns[i]=(int *)calloc(bitsize,sizeof(int));
    temp=i;
    for(j=0;j<numbits;j++){
      bit_patterns[i][j]=(int)(temp/(int)pow(2.0,(double)(numbits-j-1)));
      temp=temp%((int)(pow(2.0,(double)(numbits-j-1))));
    }  /*  FOR - Each Binary Place  */
  }  /*  FOR - Each Index Number  */

  /*  Allocate space for locations,images,image copies,and transitions. */
  spots1=(TANGO_LOC *)calloc(bitsize,sizeof(TANGO_LOC));
  spots2=(TANGO_LOC *)calloc(bitsize,sizeof(TANGO_LOC));
  images=(TANGO_IMAGE *)calloc(bitsize,sizeof(TANGO_IMAGE));
  copies=(TANGO_IMAGE *)calloc(bitsize,sizeof(TANGO_IMAGE));
  paths =(TANGO_PATH *)calloc(bitsize,sizeof(TANGO_PATH));
  movers=(TANGO_TRANS *)calloc(bitsize,sizeof(TANGO_TRANS));
  move_all=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(10));
 

  /*  For each array entry, figure location of bit images, create the    */
  /*  images and copies, figure motion paths, and move copies along path */
  for(i=0;i<size;i++){
    TANGO_LOC spot;
      
    line=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(1));
    spot=(TANGO_LOC)ASSOCretrieve("VALUE_AT",i);

    for(j=0;j<numbits;j++){
      sprintf(buffer,"%d",bit_patterns[i][numbits-1-j]);
      spots1[j]=TANGOloc_modify(spot,-ELEMENT_WIDTH/2.0-(j+1)*0.1,0.0);
      spots2[j]=TANGOloc_modify(spot,ELEMENT_WIDTH/2.0+(j+1)*0.1,0.0);
      images[j]=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,TANGOloc_X(spots1[j]),
				  TANGOloc_Y(spots1[j]),1,TANGO_COLOR_MAROON,
				  "variable",buffer,1);
      copies[j]=TANGOimage_copy(images[j]);
      paths[j]=TANGOpath_motion(spots1[j],spots2[j],TANGO_PATH_TYPE_STRAIGHT);
      movers[j]=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,copies[j],paths[j]);
      line=TANGOtrans_compose(2,line,movers[j]);
      
      /*  Free unneeded memory locations  */
      TANGOpath_free(1,paths[j]);
      TANGOtrans_free(1,movers[j]);

      /*  Store Images so they can be removed later  */
      ASSOCstore("BIT_IMAGES",i,j,images[j]);
      ASSOCstore("BIT_COPIES",i,j,copies[j]);
    }  /*  FOR - Each Bit In This Element  */

    move_all=TANGOtrans_concatenate(2,move_all,line);
    TANGOtrans_free(1,line);
  }  /*  FOR - Each Element in Array  */

  /*  Display - First Perform NULL transition(to show screen) then perform  */
  /*  the move_all transition, which shows flipping of the bits.            */
  null_trans=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(5));
  TANGOtrans_perform(null_trans);
  TANGOtrans_perform(move_all);

  /*  Closing Procedures for this function  */
  free(spots1); free(spots2);
  free(images); free(movers); free(paths);
  TANGOtrans_free(2,null_trans,move_all);
  for(i=0;i<size;i++)
    free(bit_patterns[i]);
  free(bit_patterns);
    
}  /*  ANIMFlip()  */


/*****************************************************************************/
/*  The ANIMSwap() routine is responsible for displaying a swapping of two   */
/*  data array locations during the bit reversal phase of the algorithm. The */
/*  input arguments are the indices of the data locations.  Note that the    */
/*  indices used are the TANGO_ASSOC "VALUE AT" indices, not the data array  */
/*  index values.  This must be reflected in the call to ANIMSwap().         */
/*****************************************************************************/
void  ANIMSwap(index1,index2)
     int index1,index2;
{
  TANGO_LOC loc1,loc2;
  TANGO_IMAGE image1,image2;
  TANGO_PATH proto_path,rot_path1,rot_path2,path1,path2;
  TANGO_TRANS trans1,trans2,trans3,trans4,trans,trans_flash;
  double dx,dy;


  loc1=(TANGO_LOC)ASSOCretrieve("VALUE_AT",index1);
  loc2=(TANGO_LOC)ASSOCretrieve("VALUE_AT",index2);

  dx=TANGOloc_X(loc1)+ELEMENT_WIDTH;
  dy=(TANGOloc_Y(loc1)+TANGOloc_Y(loc2))/2.0;

  image1=(TANGO_IMAGE)ASSOCretrieve("VALUE",index1);
  image2=(TANGO_IMAGE)ASSOCretrieve("VALUE",index2);

  proto_path=TANGOpath_type(TANGO_PATH_TYPE_CLOCKWISE);
  rot_path1=TANGOpath_rotate(proto_path,270);
  rot_path2=TANGOpath_rotate(proto_path,90);

  path1=TANGOpath_example(loc1,loc2,rot_path1);
  path2=TANGOpath_example(loc2,loc1,rot_path2);

  trans1=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,image1,path1);
  trans2=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,image2,path2);
  
  trans=TANGOtrans_compose(2,trans1,trans2);

  trans3=TAPflash(image1,3);
  trans4=TAPflash(image2,3);
  trans_flash=TANGOtrans_compose(2,trans3,trans4);

  TANGOtrans_perform(trans_flash);
  TANGOtrans_perform(trans);

  ASSOCstore("VALUE",index1,image2);
  ASSOCstore("VALUE",index2,image1);
  
  TANGOpath_free(5,path1,path2,proto_path,rot_path1,rot_path2);
  TANGOtrans_free(6,trans1,trans2,trans3,trans4,trans_flash,trans);
}  /*  ANIMSwap()  */


/*****************************************************************************/
/*  The ANIMBitErase() routine is responsible for erasing the bit images     */
/*  from the display once bit reversal swapping has been completed.          */
/*****************************************************************************/
void ANIMBitErase(){
  TANGO_IMAGE image;
  TANGO_TRANS trans;
  int i,j,size,numbits;
  
  size=(int)ASSOCretrieve("NUM_ELEMENTS");
  numbits=(int)ASSOCretrieve("NUM_BITS");

  trans=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(1));
  for(i=0;i<size;i++)
    for(j=0;j<numbits;j++){
      image=(TANGO_IMAGE)ASSOCretrieve("BIT_IMAGES",i,j);
      trans=TANGOtrans_compose(2,trans,TAPvis_toggle(image));
      image=(TANGO_IMAGE)ASSOCretrieve("BIT_COPIES",i,j);
      trans=TANGOtrans_compose(2,trans,TAPvis_toggle(image));
    }  /* FOR - Each Bit Location  */
  
  TANGOtrans_perform(trans);
  TANGOtrans_free(1,trans);

}  /*  ANIMNitErase()  */


/*****************************************************************************/
/*  The ANIMShift() routine causes the data value images to shift from their */
/*  current x location to the left, so that they will be in a vertical line  */
/*  on the left side of the screen.  This is used after each stage so that   */
/*  the "current action" will always be on the visible screen.               */
/*****************************************************************************/
void ANIMShift(){
  int i,size;
  TANGO_TRANS *movers,move_all;
  TANGO_PATH  *paths;
  TANGO_LOC old_loc,new_loc;
  TANGO_IMAGE image;

  size=(int)ASSOCretrieve("NUM_ELEMENTS");
  movers=(TANGO_TRANS *)calloc(size,sizeof(TANGO_TRANS));
  paths=(TANGO_PATH *)calloc(size,sizeof(TANGO_PATH));
  move_all=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(10));

  for(i=0;i<size;i++){

    image=(TANGO_IMAGE)ASSOCretrieve("VALUE",i);
    old_loc=(TANGO_LOC)ASSOCretrieve("VALUE_AT",i);
    new_loc=TANGOloc_modify(old_loc,-0.35,0.0);

    paths[i]=TANGOpath_motion(old_loc,new_loc,TANGO_PATH_TYPE_STRAIGHT);
    movers[i]=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,image,paths[i]);

    move_all=TANGOtrans_compose(2,move_all,movers[i]);

    ASSOCstore("VALUE_AT",i,new_loc);
    TANGOpath_free(1,paths[i]);
    TANGOtrans_free(1,movers[i]);
  }  /*  FOR - Each Array Value Image  */

  free(movers);
  free(paths);
  TANGOtrans_perform(move_all);
  TANGOtrans_free(1,move_all);
  
}  /*  ANIMShift()  */


/*****************************************************************************/
/*  The ANIMUnitCircle() routine creates and displays an image showing the   */
/*  unit circle in the complex plain, with markers distributed evenly around */
/*  it.  The input argument is an integer specifying how many divisions are  */
/*  desired for the markers along the circle. This is used at each stage of  */
/*  the FFT algorithm to show where the twiddle factors come from.           */
/*****************************************************************************/
void ANIMUnitCircle(divs)
     int divs;
{
  TANGO_IMAGE circle,point,*markers;
  TANGO_TRANS trans;
  static TANGO_IMAGE_COMPONENT im[6] = {
    {TANGO_IMAGE_TYPE_CIRCLE, "0.0 0.0 TANGO_COLOR_BLACK 0.075 0.0"},
    {TANGO_IMAGE_TYPE_LINE, "-0.1 0.0 TANGO_COLOR_BLACK 0.2 0.0 0.0 1.0 0"},
    {TANGO_IMAGE_TYPE_LINE, "0.0 -0.1 TANGO_COLOR_BLACK 0.0 0.2 0.0 1.0 0"},
    {TANGO_IMAGE_TYPE_TEXT, "0.075 0.05 TANGO_COLOR_BLACK NULL Re 0"},
    {TANGO_IMAGE_TYPE_TEXT, "-0.05 -0.075 TANGO_COLOR_BLACK NULL Im 0"},
    {TANGO_IMAGE_TYPE_COMPOSITE, NULL}
  };

  double dx,dy,theta,theta_step;
  int i;

  ASSOCmake("CIRCLE",0);
  ASSOCmake("TOGGLE",0);
  ASSOCmake("MARKERS",1);

  markers=(TANGO_IMAGE *)calloc(divs,sizeof(TANGO_IMAGE));

  theta_step=2*PI/divs;

  circle=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,0.85,0.5,0,im);
  ASSOCstore("CIRCLE",circle);
  trans=TAPvis_toggle(circle);

  for(i=0;i<divs;i++){
    theta=i*theta_step;
    dx=0.075*cos(theta);
    dy=0.075*sin(theta);

    markers[i]=TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,0.85+dx,0.5+dy,
				 0,TANGO_COLOR_MAROON,0.01,1.0);
    
    ASSOCstore("MARKERS",i,markers[i]);
    trans=TANGOtrans_compose(2,trans,TAPvis_toggle(markers[i]));
  }  /*  FOR - Each Twiddle Factor Value on Unit Circle  */

  ASSOCstore("TOGGLE",trans);

}  /*  ANIMUnitCircle()  */


/*****************************************************************************/
/*  The ANIMGetTwiddle() does the animation for the twiddle factor creation  */
/*  and scaling.  It was desired for the animation to show the motion of all */
/*  identical twiddle factors in a particaluar algorithm stage as a simul-   */
/*  -taneous action. Since the FFT algorithm does the computations one value */
/*  at a time, the transitions used are created and stored during each call  */
/*  of the routine, but only performed for calls with the appropriate value  */
/*  of the input parameter flag.  The other input parameters are:            */
/*                                                                           */
/*        div_number - A number ID for which twiddle value is needed         */
/*        divs - The number of twiddle factors in this stage of FFT          */
/*        real - The real part of the twiddle factor value                   */
/*        imag - The imaginary part of twiddle factor value                  */
/*        to_value - An index number into the list of data values            */
/*        newr - The real part of the newly scaled value                     */
/*        newim - The imaginary part of newly scaled value                   */
/*        flag - A code to tell if at start or end of stage                  */
/*               1: First and last value of stage                            */
/*               2: First value of stage                                     */
/*               3: Last value of stage                                      */
/*               4: Middle Value of stage                                    */
/*****************************************************************************/
void ANIMGetTwiddle(div_number,divs,real,imag,to_value,newr,newim,flag)
     int div_number,divs,to_value,flag;
     float real,imag,newr,newim;
{
  TANGO_LOC loc_from,loc_to,loc_new;
  TANGO_PATH twiddle_path;
  TANGO_IMAGE image,new_image,marker,mult;
  TANGO_IMAGE marker_temp,old_image;
  static TANGO_IMAGE circle_temp;
  TANGO_IMAGE_COMPONENT comp[4];
  TANGO_TRANS trans,new_trans,old_trans,move_trans,flasher;
  TANGO_TRANS start_trans,end_trans,clear_trans;
  int i;
  
  /*  Identify point on unit circle with a flash  */
  marker=(TANGO_IMAGE)ASSOCretrieve("MARKERS",div_number);
  flasher=TAPflash(marker,5);

  /*  Initialize transition.  This is done so that a simultaneous      */
  /*  animation can be composed using multiple calls of this function  */
  if(flag == 1 || flag == 2){  /*  If first factor of this value  */

    if(div_number == 0){  /*  If first value of stage  */
      old_trans=(TANGO_TRANS)ASSOCretrieve("TOGGLE");
    }  /*  IF - Unit Circle turned on */
    else{  /*  Else if not first value of stage  */
      old_trans=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,
				  TANGOpath_null(1));
    }  /*  ELSE - Unit Circle Was already on  */

    old_trans=TANGOtrans_concatenate(2,old_trans,flasher);
  }  /*  IF - Beginning of new transition */
  else
    old_trans=(TANGO_TRANS)ASSOCretrieve("TWIDDLE_TRANS");
  
  /*  Create Image for multiplication sign  */
  mult=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.5,0.5,1,
			 TANGO_COLOR_BLACK,"variable","X",1);

  /*  Determine starting and ending positions  */
  loc_from=TANGOimage_loc(marker,TANGO_PART_TYPE_C);
  loc_from=TANGOloc_modify(loc_from,-ELEMENT_WIDTH/2,-ELEMENT_HEIGHT/2);
  loc_to=(TANGO_LOC)ASSOCretrieve("VALUE_AT",to_value);

  /*  Create image for twiddle factor  */
  create_component(real,imag,comp);
  image=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			  TANGOloc_X(loc_from),TANGOloc_Y(loc_from),0,comp);
  start_trans=TAPvis_toggle(image);

  /*  Create movement transition for value and mult sign */
  move_trans=TAPmove(image,TANGO_PART_TYPE_C,loc_to);
  move_trans=TANGOtrans_compose(2,move_trans,
				TAPmove(mult,TANGO_PART_TYPE_C,loc_to));

  /*  Remove old value's image and multiplication sign  */
  loc_new=TANGOimage_loc(ASSOCretrieve("VALUE",to_value),
			 TANGO_PART_TYPE_NW);
  clear_trans=TAPvis_toggle(ASSOCretrieve("VALUE",to_value));
  clear_trans=TANGOtrans_compose(3,clear_trans,TAPvis_toggle(image),
				 TAPvis_toggle(mult));
  ASSOCdelete("VALUE",to_value);

  /*  Create Image for new value  */
  delete_component(comp);
  create_component(newr,newim,comp);
  new_image=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,TANGOloc_X(loc_new),
			  TANGOloc_Y(loc_new),0,comp);
  ASSOCstore("VALUE",to_value,new_image);

  /*  Show New Image, and remove mult sign  */
  end_trans=TAPvis_toggle(new_image);
  
  new_trans=TANGOtrans_concatenate(4,start_trans,move_trans,
				   clear_trans,end_trans);
  trans=TANGOtrans_compose(2,old_trans,new_trans);


  TANGOtrans_free(5,new_trans,start_trans,end_trans,move_trans,clear_trans);

  /*  Image Creation and destruction done.  Now must decide if it   */
  /*  is time to perform the transition yet.  Transition only done  */
  /*  after last copy of this twiddle factor is moved.              */
  if(flag == 1 || flag == 3){
    if(div_number == divs/2-1)
      trans=TANGOtrans_concatenate(2,trans,ASSOCretrieve("TOGGLE"));
    TANGOtrans_perform(trans);
    TANGOtrans_free(1,trans);
    if(flag == 3)ASSOCdelete("TWIDDLE_TRANS");
  }  /*  All Done - Perform and free all memory  */
  else{
    if(flag == 4) TANGOtrans_free(1,ASSOCretrieve("TWIDDLE_TRANS"));
    ASSOCstore("TWIDDLE_TRANS",trans);
  }  /*  Not Done - Store for next time through  */

}  /*  ANIMGetTwiddle()  */


/*****************************************************************************/
/*  The ANIMButterfly() does the animation for the butterfly portions of the */
/*  FFT algorithm.  This is done by keeping a cumalative transition stored   */
/*  until the routine is called with the appropriate value of <flag>. The    */
/*  input parameters used by this routine are:                               */
/*                                                                           */
/*    new_value_real(1,2): Real parts of 2 result values to be butterflied   */
/*    new_value_in(1,2): Imaginary parts of the 2 result values              */
/*    old_value_*****  : Real and Imaginary parts of 2 input values          */
/*    i,j: Index numbers into the list of data images                        */
/*    flag: Control flag specifying whether at start or end of stage         */
/*         1) First time calling during present stage                        */
/*         2) Middle of present stage                                        */
/*         3) Last call of stage.  Must perform transitions now              */
/*****************************************************************************/
void ANIMButterfly(new_value_real1,new_value_im1,new_value_real2,new_value_im2,
		   old_value_real1,old_value_im1,old_value_real2,old_value_im2,
		   i,j,flag)
     float new_value_real1,new_value_im1,new_value_real2,new_value_im2;
     float old_value_real1,old_value_im1,old_value_real2,old_value_im2;
     int i,j,flag;
{
  TANGO_LOC old_loc1,old_loc2,new_loc1,new_loc2;
  TANGO_IMAGE old_im1,old_im2,new_im1,new_im2,plus,minus;
  TANGO_IMAGE copy1a,copy1b,copy2a,copy2b;
  TANGO_PATH path11,path22,path12,path21,plus_path,minus_path;
  TANGO_TRANS old_trans,new_trans,trans11,trans22,trans12,trans21;
  TANGO_TRANS plus_trans,minus_trans,trans;
  TANGO_IMAGE_COMPONENT im1[4],im2[4],map1[4],map2[4];
  float x,y1,y2;

  /*  Use flag to determine initialization procedure */
  if(flag == 1)                 /*  First time through during this stage */
    old_trans=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(1));
  else
    old_trans=(TANGO_TRANS)ASSOCretrieve("BUTTERFLIES");


  /*  Get needed locations and images of old data values  */
  old_loc1=(TANGO_LOC)ASSOCretrieve("VALUE_AT",i);
  old_loc2=(TANGO_LOC)ASSOCretrieve("VALUE_AT",j);
  old_im1=(TANGO_IMAGE)ASSOCretrieve("VALUE",i);
  old_im2=(TANGO_IMAGE)ASSOCretrieve("VALUE",j);

  /*  Create locations for new data values and intermediate points */
  new_loc1=TANGOloc_modify(old_loc1,0.7,0.0);
  new_loc2=TANGOloc_modify(old_loc2,0.7,0.0);

  /*  Create images for the plus and minus operators */
  x=TANGOloc_X(new_loc1);
  y1=TANGOloc_Y(new_loc1);
  y2=TANGOloc_Y(new_loc2);
  plus=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y1,0,TANGO_COLOR_BLACK,
			 "variable","+",1);
  minus=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y2,0,TANGO_COLOR_BLACK,
			 "variable","-",1);

  /*  Create images for values and store  */
  create_component(new_value_real1,new_value_im1,map1);
  create_component(new_value_real2,new_value_im2,map2);
  create_component(old_value_real1,old_value_im1,im1);
  create_component(old_value_real2,old_value_im2,im2);
  copy1a=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			   TANGOloc_X(old_loc1)-ELEMENT_WIDTH/2.0,
			   TANGOloc_Y(old_loc1)-ELEMENT_HEIGHT/2,0,im1);
  copy2a=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			   TANGOloc_X(old_loc2)-ELEMENT_WIDTH/2.0,
			   TANGOloc_Y(old_loc2)-ELEMENT_HEIGHT/2.0,0,im2);
  copy1b=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			   TANGOloc_X(old_loc1)-ELEMENT_WIDTH/2.0,
			   TANGOloc_Y(old_loc1)-ELEMENT_HEIGHT/2.0,0,im1);
  copy2b=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			   TANGOloc_X(old_loc2)-ELEMENT_WIDTH/2.0,
			   TANGOloc_Y(old_loc2)-ELEMENT_HEIGHT/2.0,0,im2);
  new_im1=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			    TANGOloc_X(new_loc1)-ELEMENT_WIDTH/2.0,
			    TANGOloc_Y(new_loc1)-ELEMENT_HEIGHT/2.0,0,map1);
  new_im2=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			    TANGOloc_X(new_loc2)-ELEMENT_WIDTH/2.0,
			    TANGOloc_Y(new_loc2)-ELEMENT_HEIGHT/2.0,0,map2);
  delete_component(im1);
  delete_component(im2);
  delete_component(map1);
  delete_component(map2);
  ASSOCstore("NEW_VALUES",i,new_im1);
  ASSOCstore("NEW_VALUES",j,new_im2);
  
  /*  Create paths for image motions  */
  path11=TANGOpath_motion(old_loc1,new_loc1,TANGO_PATH_TYPE_STRAIGHT);
  path22=TANGOpath_motion(old_loc2,new_loc2,TANGO_PATH_TYPE_STRAIGHT);
  path12=TANGOpath_motion(old_loc1,new_loc2,TANGO_PATH_TYPE_STRAIGHT);
  path21=TANGOpath_motion(old_loc2,new_loc1,TANGO_PATH_TYPE_STRAIGHT);
  
  /*  Create transitions for image motions  */
  trans11=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,copy1a,path11);
  trans11=TANGOtrans_concatenate(3,TAPvis_toggle(copy1a),trans11,
				 TAPvis_toggle(copy1a));
  trans12=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,copy1b,path12);
  trans12=TANGOtrans_concatenate(3,TAPvis_toggle(copy1b),trans12,
				 TAPvis_toggle(copy1b));
  trans21=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,copy2a,path21);
  trans21=TANGOtrans_concatenate(3,TAPvis_toggle(copy2a),trans21,
				 TAPvis_toggle(copy2a));
  trans22=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,copy2b,path11);
  trans22=TANGOtrans_concatenate(3,TAPvis_toggle(copy2b),trans22,
				 TAPvis_toggle(copy2b));
  TANGOpath_free(4,path11,path12,path21,path22);
  
  new_trans=TANGOtrans_compose(4,trans11,trans12,trans21,trans22);
  new_trans=TANGOtrans_concatenate(3,new_trans,TAPvis_toggle(new_im1),
				   TAPvis_toggle(new_im2));
  new_trans=TANGOtrans_concatenate(5,TAPvis_toggle(plus),TAPvis_toggle(minus),
				   new_trans,TAPvis_toggle(plus),
				   TAPvis_toggle(minus));
  
  /*  Put together all transitions into one large transition  */
  trans=TANGOtrans_compose(2,old_trans,new_trans);
  TANGOtrans_free(6,trans11,trans12,trans21,trans22,old_trans,new_trans);

  /*  Use flag to determine whether or not to perform transition */
  if(flag == 3){
    TANGOtrans_perform(trans);
    TANGOtrans_free(1,trans);
  }  /*  IF - Last time through during this stage   */
  else{
    ASSOCstore("BUTTERFLIES",trans);
  }  /*  ELSE - Not done: Store for future use  */

}  /*  ANIMButterfly()  */


void ANIMUpdate(){
  TANGO_LOC old;
  TANGO_IMAGE image_old,image_new;
  TANGO_TRANS trans,erase,move,temp;
  int size,i;

  trans=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,TANGOpath_null(1));
  size=(int)ASSOCretrieve("NUM_ELEMENTS");
  
  for(i=0;i<size;i++){
    image_old=(TANGO_IMAGE)ASSOCretrieve("VALUE",i);
    image_new=(TANGO_IMAGE)ASSOCretrieve("NEW_VALUES",i);
    erase=TAPvis_toggle(image_old);

    old=(TANGO_LOC)ASSOCretrieve("VALUE_AT",i);
    move=TAPmove(image_new,TANGO_PART_TYPE_C,old);
    
    temp=TANGOtrans_concatenate(2,erase,move);
    trans=TANGOtrans_compose(2,trans,temp);

    ASSOCstore("VALUE",i,image_new);
  }  /*  FOR - Each Data Value  */

  TANGOtrans_perform(trans);
  TANGOtrans_free(4,trans,erase,move,temp);

}  /*  ANIMUpdate()  */
    
    
