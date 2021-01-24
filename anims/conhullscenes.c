/*                 AUTHOR :-      SRINIVAS K. V.               */
/*  This program contains actual animation routines for the    */ 
/*  algorithm operations specified in the code for the convex  */
/*  hull solution using JARVIS MARCH method.                   */ 



# include <stdio.h>
# include <xtango.h>

extern double x,y;
extern lowest;

void 
ANIMInit()
{

   ASSOCinit();
   ASSOCmake("XVALUES",1);
   ASSOCmake("YVALUES",1);
   ASSOCmake("IMG_ID",1);
   ASSOCmake("IMG_ID1",1);
   ASSOCmake("POINTER",0);
   ASSOCmake("PTRLOC",2);
}


void 
ANIMGetpt()
{

   TANGOinput_coord(&x,&y);
}

void 
ANIMDraw(x,y,index)
int x,y,index;
{
   TANGO_IMAGE circle,text;
   TANGO_TRANS delay,num;
   TANGO_PATH path;
   double px,py;
   char str[4];

   px =(double) x / 1000;
   py = (double) y / 1000;
   ASSOCstore("XVALUES",index,x);
  ASSOCstore("YVALUES",index,y); 
  circle = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,px,py,1,TANGO_COLOR_BLUE,0.02,0.0);
  path = TANGOpath_null(10);
  sprintf(str,"%d",index);
  text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,px,py,1,TANGO_COLOR_BLACK,NULL,str,1);
  num = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,text,path);
  ASSOCstore("IMG_ID",index,circle);
  delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,circle,path);
  delay = TANGOtrans_compose(2,delay,num);
  TANGOtrans_perform(delay);
  TANGOpath_free(1,path);
  TANGOtrans_free(2,delay,num);
}

void
ANIMSho_min(index)
int index;
{ double x,y;

    double px,py;
    TANGO_IMAGE sweep_line;
    TANGO_TRANS delay;
    TANGO_PATH path;

   
  path= TANGOpath_null(0);
  py = (double) ASSOCretrieve("YVALUES",index) / 1000;
  sweep_line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,py,1,TANGO_COLOR_RED,1.0,0.0,0.0);  
  delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,sweep_line,path);
  TANGOtrans_perform(delay);
}

void
ANIMComp(min,pres)
int min,pres;
{

  TANGO_IMAGE circ1,circ2;
  TANGO_PATH path,f_path;
  TANGO_TRANS trns1,trns2,join;
  double blink[4];

  circ1 = (TANGO_IMAGE) ASSOCretrieve("IMG_ID",min);
  circ2 = (TANGO_IMAGE) ASSOCretrieve("IMG_ID",pres);
  blink[0]=1.0;blink[2]= -1.0;blink[1]=blink[3]=0.0;
  path = TANGOpath_create(4,blink,blink);
  f_path = TANGOpath_iterate(path,3.0);
  trns1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,circ1,f_path);
  trns2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,circ2,f_path);
  join = TANGOtrans_compose(2,trns1,trns2);
  TANGOtrans_perform(join);
  TANGOpath_free(2,path,f_path);
  TANGOtrans_free(3,trns1,trns2,join);
}
  
void
ANIMChange_min(min,pres)
int min,pres;   
{
   TANGO_PATH path,path1;
   TANGO_IMAGE line1,line2,circ1,circ2;
   TANGO_TRANS join,shift,comp;
   TANGO_LOC point1,point2;
   double y1,y2;
   char str[4];

  y1 = (double) ASSOCretrieve("YVALUES",min) / 1000;
  y2 = (double) ASSOCretrieve("YVALUES",pres) / 1000;
  line1 = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,y1,1,TANGO_COLOR_RED,1.0,0.0,0.0);
  line2 = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,y2,1,TANGO_COLOR_RED,1.0,0.0,0.0);
  point1 = TANGOimage_loc(line1,TANGO_PART_TYPE_SW);
  point2 = TANGOimage_loc(line2,TANGO_PART_TYPE_SW);
  path = TANGOpath_distance(point1,point2,0.002);
/*  path = TANGOpath_rotate(path,270); */
  shift = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,line1,path);
  TANGOtrans_perform(shift);
  path = TANGOpath_null(1);
  line1 = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.0,y1,1,TANGO_COLOR_WHITE,1.0,0.0,0.0);
  shift= TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,line1,path);
  TANGOtrans_perform(shift);
  path = TANGOpath_null(1); 
  y1 = (double) ASSOCretrieve("XVALUES",min)/1000;
  y2 = (double) ASSOCretrieve("YVALUES",min)/1000;
 circ1 =TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,y1,y2,1,TANGO_COLOR_BLUE,0.02,0.0);
 join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,circ1,path);
 sprintf(str,"%d",min);
 circ2=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,y1,y2,1,TANGO_COLOR_BLACK,NULL,str,1);
  shift = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,circ2,path);
  comp = TANGOtrans_compose(2,join,shift);
  TANGOtrans_perform(comp); 
  TANGOpath_free(1,path);
}

void
ANIMComp_polar(min,p0,p2)
int p0,p2,min;
{
   double min_x,min_y,p0_x,p0_y,p2_x,p2_y,blink[4];
   TANGO_IMAGE linep0_p2,linep0_min,img;
   TANGO_TRANS line1,line2,join;
   TANGO_PATH path,f_path;
   char str[4];
    

 min_x = (double)ASSOCretrieve("XVALUES",min)/1000;
 min_y = (double)ASSOCretrieve("YVALUES",min)/1000;
 p0_x = (double)ASSOCretrieve("XVALUES",p0)/1000;
 p0_y = (double)ASSOCretrieve("YVALUES",p0)/1000;
 p2_x = (double)ASSOCretrieve("XVALUES",p2)/1000;
 p2_y = (double)ASSOCretrieve("YVALUES",p2)/1000;
 path = TANGOpath_null(1);
 img = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,p0_x,p0_y,1,TANGO_COLOR_BLUE,0.02,1.0);
  join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
  sprintf(str,"%d",p0);
 img = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,p0_x,p0_y,1,TANGO_COLOR_WHITE,NULL,str,1);
  line1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
  join = TANGOtrans_compose(2,join,line1);
  TANGOtrans_perform(join);
 path = TANGOpath_null(0);
 linep0_min = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,p0_x,p0_y,1,TANGO_COLOR_MAROON,(min_x-p0_x),(min_y-p0_y),0.0,0.5,0);
 line1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,linep0_min,path);
 linep0_p2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,p0_x,p0_y,1,TANGO_COLOR_MAROON,(p2_x-p0_x),(p2_y-p0_y),0.0,0.5,0);
 line2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,linep0_p2,path);
 blink[0]=1.0;blink[2]= -1.0;blink[1]=blink[3]=0.0;
 path = TANGOpath_create(4,blink,blink);
 f_path = TANGOpath_iterate(path,3.0);
/* line1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,linep0_min,f_path);*/
 line2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,linep0_p2,f_path);
 join = TANGOtrans_compose(2,line1,line2);
 TANGOtrans_perform(join);
  TANGOpath_free(2,path,f_path);
  TANGOtrans_free(3,line1,line2,join);
}

void
ANIMClear(min,p0,p2)
int min,p0,p2;
{   int i;
   double min_x,min_y,p0_x,p0_y,p2_x,p2_y,rx,ry;
   TANGO_IMAGE linep0_p2,linep0_min,img,c1,c2;
   TANGO_TRANS line1,line2,join,main,j1;
   TANGO_PATH path,f_path;
   char str[4];


 min_x = (double)ASSOCretrieve("XVALUES",min)/1000;
 min_y = (double)ASSOCretrieve("YVALUES",min)/1000;
 p0_x = (double)ASSOCretrieve("XVALUES",p0)/1000;
 p0_y = (double)ASSOCretrieve("YVALUES",p0)/1000;
 p2_x = (double)ASSOCretrieve("XVALUES",p2)/1000;
 p2_y = (double)ASSOCretrieve("YVALUES",p2)/1000;
 path = TANGOpath_null(0);
 linep0_min = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,p0_x,p0_y,1,TANGO_COLOR_MAROON,(min_x-p0_x),(min_y-p0_y),0.0,0.5,0);
 line1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,linep0_min,path);
 linep0_p2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,p0_x,p0_y,1,TANGO_COLOR_WHITE,(p2_x-p0_x),(p2_y-p0_y),0.0,1.0,0);
 line2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,linep0_p2,path);
 join = TANGOtrans_compose(2,line1,line2);
  rx = (double) ASSOCretrieve("XVALUES",p0)/1000;
  ry = (double) ASSOCretrieve("YVALUES",p0)/1000;
  img = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,rx,ry,1,TANGO_COLOR_BLUE,0.02,1.0);
  path = TANGOpath_null(1);
  j1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
  main = TANGOtrans_compose(2,j1,join);
  sprintf(str,"%d",p0);
  img = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,rx,ry,1,TANGO_COLOR_WHITE,NULL,str,1);
  join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
  main = TANGOtrans_compose(2,main,join);
  TANGOtrans_perform(main);
  c1 =  TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,min_x,min_y,1,TANGO_COLOR_BLUE,0.02,0.0);
  join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c1,path);
  c2=TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,p2_x,p2_y,1,TANGO_COLOR_BLUE,0.02,0.0);
  main = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c1,path);
  main = TANGOtrans_compose(2,main,join);
  sprintf(str,"%d",p2);
 c1 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,p2_x,p2_y,1,TANGO_COLOR_BLACK,NULL,str,1);
 join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c1,path);
 main = TANGOtrans_compose(2,main,join);
 sprintf(str,"%d",min);
 c2=TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,min_x,min_y,1,TANGO_COLOR_BLACK,NULL,str,1);
 join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c2,path);
 main = TANGOtrans_compose(2,main,join);
  TANGOtrans_perform(main);
 p2_x=(double) ASSOCretrieve("XVALUES",lowest)/1000;
 p2_y=(double) ASSOCretrieve("YVALUES",lowest)/1000;
 c1=TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,p2_x,p2_y,1,TANGO_COLOR_BLUE,0.02,1.0);
 path = TANGOpath_null(1);
 join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c1,path);
 sprintf(str,"%d",lowest);
 c2 =TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,p2_x,p2_y,1,TANGO_COLOR_WHITE,NULL,str,1);
 main= TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c2,path);
 main = TANGOtrans_compose(2,main,join);
 TANGOtrans_perform(main);
  TANGOpath_free(1,path);
  TANGOtrans_free(5,line1,line2,join,j1,main);
}
      
void 
ANIMChange_polarmin(min,p0,p2)
int min,p0,p2;
{
   TANGO_IMAGE linep0_min,linep0_p2,img;
   TANGO_TRANS line1,line2,join,main,j1;
   TANGO_PATH path;
    char str[4];
   double min_x,min_y,p0_x,p0_y,p2_x,p2_y,rx,ry;

   min_x = (double)ASSOCretrieve("XVALUES",min)/1000;
   min_y = (double)ASSOCretrieve("YVALUES",min)/1000;
   p0_x = (double)ASSOCretrieve("XVALUES",p0)/1000;
   p0_y = (double)ASSOCretrieve("YVALUES",p0)/1000;
   p2_x = (double)ASSOCretrieve("XVALUES",p2)/1000;
   p2_y = (double)ASSOCretrieve("YVALUES",p2)/1000;
   path = TANGOpath_null(1);
   
   linep0_min = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,p0_x,p0_y,1,TANGO_COLOR_WHITE,(min_x-p0_x),(min_y-p0_y),0.0,1.0,0);
   linep0_p2 = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,p0_x,p0_y,1,TANGO_COLOR_MAROON,(p2_x-p0_x),(p2_y-p0_y),0.0,0.5,0);
   line1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,linep0_min,path);
   line2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,linep0_p2,path);
   join = TANGOtrans_compose(2,line1,line2);
   rx = (double) ASSOCretrieve("XVALUES",p0)/1000;
   ry = (double) ASSOCretrieve("YVALUES",p0)/1000;
   img = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,rx,ry,1,TANGO_COLOR_BLUE,0.02,1.0);
   j1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
   sprintf(str,"%d",p0);
   img = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,rx,ry,1,TANGO_COLOR_WHITE,NULL,str,1);
   main = TANGOtrans_compose(2,j1,join);
   join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
   main = TANGOtrans_compose(2,main,join);
   TANGOtrans_perform(main);
   rx = (double) ASSOCretrieve("XVALUES",min)/1000;
   ry = (double) ASSOCretrieve("YVALUES",min)/1000;
   img = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,rx,ry,1,TANGO_COLOR_BLUE,0.02,0.0);
   j1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
   sprintf(str,"%d",min);
   img = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,rx,ry,1,TANGO_COLOR_BLACK,NULL,str,1);
   join = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,img,path);
   main = TANGOtrans_compose(2,main,join);
   TANGOtrans_perform(main);
   TANGOpath_free(1,path);
   TANGOtrans_free(5,line1,line2,join,j1,main); 
}

void
ANIMDra_final(i,j)
int i,j;
{
   double x1,y1,x2,y2;
   char str[4];
   TANGO_PATH path;
   TANGO_IMAGE line,c1,c2;
   TANGO_TRANS d_line,main,temp;

   x1 = (double) ASSOCretrieve("XVALUES",i) / 1000;
   x2 = (double) ASSOCretrieve("XVALUES",j) / 1000;
   y1 = (double) ASSOCretrieve("YVALUES",i) / 1000;
   y2 = (double) ASSOCretrieve("YVALUES",j) / 1000;
   line = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,TANGO_COLOR_GREEN,(x2-x1),(y2-y1),0.5,1.0,0);
   path = TANGOpath_null(1);
   d_line = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,line,path);
   c1 = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x1,y1,1,TANGO_COLOR_BLUE,0.02,1.0);
   c2 = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,x2,y2,1,TANGO_COLOR_BLUE,0.02,1.0);
   temp = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c1,path);
   main = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c2,path);
   main = TANGOtrans_compose(3,main,temp,d_line);
   sprintf(str,"%d",i);
   c1 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x1,y1,1,TANGO_COLOR_WHITE,NULL,str,1);
   sprintf(str,"%d",j);
   c2 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x2,y2,1,TANGO_COLOR_WHITE,NULL,str,1);
   temp = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c1,path);
   main = TANGOtrans_compose(2,main,temp);
   temp = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,c2,path);
   main = TANGOtrans_compose(2,main,temp);
   TANGOtrans_perform(main);
}
