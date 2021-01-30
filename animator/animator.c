#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "xtangolocal.h"

#define SLEN 128


typedef enum {
  Rect,
  Line,
  Circ,
  Tria,
  Ctxt,
  Ltxt
  } ObjType;

typedef struct ObjNode {
   int     id;
   ObjType type;
   struct ObjNode *next;
 } *ObjPtr;


static  void parseLine(char *s);
void bg(char *s);
void coords(char *s);
void delay(char *s);
void line(char *s);
void rectangle(char *s);
void circle(char *s);
void triangle(char *s);
void text(char *s);
void flextext(char *s);
void bigtext(char *s);
void delete(char *s);
void move(char *s);
void moverelative(char *s);
void moveto(char *s);
void jump(char *s);
void jumprelative(char *s);
void jumpto(char *s);
void color(char *s);
void fill(char *s);
void vis(char *s);
void raise(char *s);
void lower(char *s);
void exchangepos(char *s);
void switchpos(char *s);
void swapid(char *s);
void comment(char *s);

static void anim_help();
double getFill();
double getWidth();
double fixY();
int validColor();
void addObject();
ObjPtr findObject();
void removeObject();
TANGO_COLOR getColor();
int stdColor();
int parseText();
int parseFlexText();


ObjPtr  Head = NULL;
ObjPtr  Tail = NULL;
int     debug = 1;



int main(int argc, char *argv[] )
{
   char str[256];
   int c;
   FILE *fp=stdin;
#if 0
   char *s;
   int i;
   for (i = 1; i < argc; ++i) {
      if (argv[i][0] != '-') {
	 anim_help();
       }
      else {
	 for (s = &argv[i][1]; *s != 0; ++s) {
	    switch(*s) {
	       case 'O':
		  debug = 0;
		  break;
	       default:
		  fprintf(stderr,"animator: illegal option %c\n",*s);
		  argc = 0;
		  anim_help();
		  break;
	     }
	  }
       }
    }
#endif

   while(1) {
     c=getopt(argc,argv,"dhf:");
     if (c<0) break;
     switch(c) {
     case 'd':
       debug=0;
       break;

     case 'f':
       fp=fopen(optarg,"r");
       if (!fp)  {
	 fprintf(stderr," can not open %s\n",optarg);
	 break;
       }
     default:
       fprintf(stderr,"animator: unknown  option %c\n",c);
     case 'h':
       anim_help();
     }
   }

   printf("Press RUN ANIMATION button to begin\n");

   TANGOalgoOp(NULL,"BEGIN");

   ASSOCmake("IDS",1);
   while (fgets(str,sizeof str, fp))
       parseLine(str);

   TANGOalgoOp(NULL,"END");
}



static void anim_help ()
{
   fprintf(stderr,"Usage: animator [-dhf<file>]\n");
   fprintf(stderr,"-h   -this help\n");
   fprintf(stderr,"-d   -enable debug\n");
   fprintf(stderr," -f<fname>  -readfile\n");
   exit(1);
}



static void parseLine(char *str)
{
   char cmd[50];
   int i;


   for (i=strlen(str);i>=0;i--)
     if ( iscntrl(str[i]) ) str[i]=0; else break;
   
   if (sscanf(str, "%s", cmd) <= 0)  /* empty line */
      return;
   if (str[0] == '%')
      return;

   printf("line:%s\n",str);
   //usleep(100000);
   
   if (!strcmp(cmd,"line"))
      line(str);
   else if (!strcmp(cmd,"rectangle"))
      rectangle(str);
   else if (!strcmp(cmd,"circle"))
      circle(str);
   else if (!strcmp(cmd,"triangle"))
      triangle(str);
   else if (!strcmp(cmd,"text"))
      text(str);
   else if (!strcmp(cmd,"bigtext"))
      bigtext(str);
   else if (!strcmp(cmd,"flextext"))
      flextext(str);
   else if (!strcmp(cmd, "delete"))
      delete(str);
   else if (!strcmp(cmd,"move"))
      move(str);
   else if (!strcmp(cmd,"moverelative"))
      moverelative(str);
   else if (!strcmp(cmd,"moveto"))
      moveto(str);
   else if (!strcmp(cmd,"jump"))
      jump(str);
   else if (!strcmp(cmd,"jumprelative"))
      jumprelative(str);
   else if (!strcmp(cmd,"jumpto"))
      jumpto(str);
   else if (!strcmp(cmd,"color"))
      color(str);
   else if (!strcmp(cmd,"fill"))
      fill(str);
   else if (!strcmp(cmd,"vis"))
      vis(str);
   else if (!strcmp(cmd,"raise"))
      raise(str);
   else if (!strcmp(cmd,"lower"))
      lower(str);
   else if (!strcmp(cmd,"exchangepos"))
      exchangepos(str);
   else if (!strcmp(cmd,"switchpos"))
      switchpos(str);
   else if (!strcmp(cmd,"swapid"))
      swapid(str);
   else if (!strcmp(cmd,"bg"))
      bg(str);
   else if (!strcmp(cmd,"coords"))
      coords(str);
   else if (!strcmp(cmd,"delay"))
      delay(str);
   else if (!strcmp(cmd,"comment"))
      comment(str);
   else 
      printf("Inavlid command ->%s<- found, ignoring\n",cmd);
}



void bg( char *str)
{
   char cmd[SLEN],color[SLEN];

   sscanf(str, "%s %s",cmd,color);
   TANGOset_bgcolor(color);
 }


void coords( char *str)
{
   char cmd[SLEN];
   double lx,by,rx,ty;

   sscanf(str, "%s %lf %lf %lf %lf",cmd,&lx,&by,&rx,&ty);
   if ((lx >= rx) || (by >= ty)) {
      printf("Invalid coords (%lf,%lf) (%lf,%lf) passed to coords command (Ignoring)\n",lx,by,rx,ty);
      return;
    }
   TANGOset_coord(lx, ty, rx, by);   /* reverse the y's */
}




void delay(char *str)
{
   TANGO_PATH path;
   TANGO_TRANS trans;
   int frames;
   char cmd[SLEN];

   sscanf(str, "%s %d",cmd,&frames);
   path = TANGOpath_null(frames);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
 }


void line(char *str)
{
   char cmd[SLEN],color[SLEN],width[SLEN];
   int id;
   double lx,ly,sx,sy,widthval;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   struct _IMAGE image;
   struct _TANGO_LINE line;

   sscanf(str, "%s %d %lf %lf %lf %lf %s %s",cmd,&id,&lx,&ly,&sx,&sy,color,width);
   if (stdColor(color))
      col = getColor(color);
   else
      col = TANGOload_color(color);
   widthval = getWidth(width);
   if (widthval == -1.0) {
      printf("Invalid width argument ->%s<- given to line command.  (Ignoring)\n",width);
      return;
    }

	     image.type=TANGO_IMAGE_TYPE_LINE;
	     image.loc[0]=lx;
	     image.loc[1]=fixY(ly);
	     image.visible=1;
	     line.color=col;
	     line.size[0]=sx;
	     line.size[1]=0-sy;
	     line.width=widthval;
	     line.style=1;
	     line.arrow=0;

	     image.object=&line;


	     im = TANGOimage_create(&image);

//   im = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, lx, fixY(ly), 1, col, sx, -sy,widthval, 1.0, 0);
                          
   ASSOCstore("IDS",id,im);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   if (findObject(id))
      if (debug) printf("Warning: object with id=%d already exists\n",id);
   addObject(id, Line);
 }




void rectangle(char *str)
{
   char cmd[SLEN],color[SLEN],fill[SLEN];
   int id;
   double lx,ly,sx,sy,fillval;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;
   struct _IMAGE image;
   struct _TANGO_RECTANGLE rec;;


   sscanf(str, "%s %d %lf %lf %lf %lf %s %s",cmd,&id,&lx,&ly,&sx,&sy,color,fill);
   if (stdColor(color))
      col = getColor(color);
   else
      col = TANGOload_color(color);
   fillval = getFill(fill);
   if (fillval == -1.0) {
      printf("Invalid fill argument ->%s<- given to rectangle command.  (Ignoring)\n",fill);
      return;
    }

	     image.type=TANGO_IMAGE_TYPE_RECTANGLE;
	     image.loc[0]=lx;
	     image.loc[1]=fixY(ly)-sy;
	     image.visible=1;
	     rec.color=col;
	     rec.size[0]=sx;
	     rec.size[1]=sy;
	     rec.fill=fillval;
	     image.object=&rec;;

	     im = TANGOimage_create(&image);

 //   im = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, lx, fixY(ly)-sy, 1, col, sx, sy, fillval);
                          
   ASSOCstore("IDS",id,im);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   if (findObject(id))
      if (debug) printf("Warning: object with id=%d already exists\n",id);
   addObject(id, Rect);
 }




void circle(char *str)
{
   char cmd[SLEN],color[SLEN],fill[SLEN];
   int id;
   double lx,ly,rad,fillval;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   struct _IMAGE image;
   struct _TANGO_CIRCLE circ;

   sscanf(str, "%s %d %lf %lf %lf %s %s",cmd,&id,&lx,&ly,&rad,color,fill);
   if (stdColor(color))
      col = getColor(color);
   else
      col = TANGOload_color(color);
   fillval = getFill(fill);
   if (fillval == -1.0) {
      printf("Invalid fill argument ->%s<- given to circle command.  (Ignoring)\n",fill);
      return;
    }

   image.type=TANGO_IMAGE_TYPE_CIRCLE;
	     image.loc[0]=lx;
	     image.loc[1]=fixY(ly);
	     image.visible=1;
	     circ.color=col;
	     circ.radius=rad;
	     circ.fill=fillval;

	     image.object=&circ;;

	     im = TANGOimage_create(&image);
   //   im = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, lx, fixY(ly), 1, col, rad,fillval);
                          
   ASSOCstore("IDS",id,im);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   if (findObject(id))
      if (debug) printf("Warning: object with id=%d already exists\n",id);
   addObject(id, Circ);
 }




void
triangle(char *str)
{
   char cmd[SLEN],color[SLEN],fill[SLEN];
   int id;
   double lx,ly,vx[2],vy[2],fillval;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;
   int i;

   struct _IMAGE image;
   struct _TANGO_POLYGON poly;

   sscanf(str, "%s %d %lf %lf %lf %lf %lf %lf %s %s",cmd,&id,&lx,&ly,
               &vx[0],&vy[0],&vx[1],&vy[1],color,fill);
   vx[0] = vx[0]-lx;    /* make them relative */
   vy[0] = -(vy[0]-ly);   /* y coords are flipped */
   vx[1] = vx[1]-lx;
   vy[1] = -(vy[1]-ly);
   if (stdColor(color))
      col = getColor(color);
   else
      col = TANGOload_color(color);
   fillval = getFill(fill);
   if (fillval == -1.0) {
      printf("Invalid fill argument ->%s<- given to triangle command.  (Ignoring)\n",fill);
      return;
    }

	     image.type=TANGO_IMAGE_TYPE_POLYGON;
	     image.loc[0]=lx;
	     image.loc[1]=fixY(ly);
	     image.visible=1;
	     poly.sides=2;
	     for(i=0;i<poly.sides;i++) {
	       poly.vertex[i][0]=vx[i];
	       poly.vertex[i][1]=vy[i]; 
	     }

	     poly.color=col;
	     poly.fill=fillval;
	     image.object=&poly;

	     im = TANGOimage_create(&image);

   //   im = TANGOimage_create(TANGO_IMAGE_TYPE_POLYGON, lx, fixY(ly), 1,col, 3, vx, vy, fillval);
                         
   ASSOCstore("IDS",id,im);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   if (findObject(id))
      if (debug) printf("Warning: object with id=%d already exists\n",id);
   addObject(id, Tria);
 }

void
text(char *str)
{
   char cmd[SLEN],color[SLEN];
   int id,cen,textindx;
   double lx,ly;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;
   struct _IMAGE image;
   struct _TANGO_TEXT text;

   sscanf(str, "%s %d %lf %lf %d %s",cmd,&id,&lx,&ly,&cen,color);
   textindx = parseText(str);
   if (textindx == -1) return;
   if (stdColor(color))
      col = getColor(color);
   else
      col = TANGOload_color(color);

	     image.type=TANGO_IMAGE_TYPE_TEXT;
	     image.loc[0]=lx;
	     image.loc[1]=fixY(ly);
	     image.visible=1;
	     text.color=col;
	     strncpy(text.font_name,"variable",STRINGLENGTH-1);
	     strncpy(text.text,str+textindx,STRINGLENGTH-1);
	     text.orient=cen;
	     text.xext=0;
	     text.xoff=0;
	     text.yext=0;
	     text.yoff=0;

	     image.object=&text;

	     im = TANGOimage_create(&image);

#if 0
   im = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, lx, fixY(ly), 1, col, 
#ifdef ultrix
                          "fixed",
#else
                          "variable",
#endif
                          str+textindx,cen);
#endif
   ASSOCstore("IDS",id,im);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   if (findObject(id))
      if (debug) printf("Warning: object with id=%d already exists\n",id);
   if (cen)
      addObject(id, Ctxt);
   else
      addObject(id, Ltxt);
 }





void
bigtext(char *str)
{
   char cmd[SLEN],color[SLEN];
   int id,cen,textindx;
   double lx,ly;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;
   struct _IMAGE image;
   struct _TANGO_TEXT text;
  
   sscanf(str, "%s %d %lf %lf %d %s",cmd,&id,&lx,&ly,&cen,color);
   textindx = parseText(str);
   if (textindx == -1) return;
   if (stdColor(color))
      col = getColor(color);
   else
      col = TANGOload_color(color);

	     image.type=TANGO_IMAGE_TYPE_TEXT;
	     image.loc[0]=lx;
	     image.loc[1]=fixY(ly);
	     image.visible=1;
	     text.color=col;
	     strncpy(text.font_name,"12x24",STRINGLENGTH-1);
	     strncpy(text.text,str+textindx,STRINGLENGTH-1);
	     text.orient=cen;
	     text.xext=0;
	     text.xoff=0;
	     text.yext=0;
	     text.yoff=0;
	     image.object=&text;

	     im = TANGOimage_create(&image);

#if 0
   im = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, lx, fixY(ly), 1, col, 
#ifdef ultrix
                          "variable",
#else
                          "12x24",
#endif
                          str+textindx,cen);

#endif

   ASSOCstore("IDS",id,im);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   if (findObject(id))
      if (debug) printf("Warning: object with id=%d already exists\n",id);
   if (cen)
      addObject(id, Ctxt);
   else
      addObject(id, Ltxt);
 }



void
flextext(   char *str)
{
   char cmd[SLEN],color[SLEN],fname[SLEN];
   int id,cen,textindx;
   double lx,ly;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   struct _IMAGE image;
   struct _TANGO_TEXT text;

   sscanf(str, "%s %d %lf %lf %d %s %s",cmd,&id,&lx,&ly,&cen,color,fname);
   textindx = parseFlexText(str);
   if (textindx == -1) return;
   if (stdColor(color))
      col = getColor(color);
   else
      col = TANGOload_color(color);

	     image.type=TANGO_IMAGE_TYPE_TEXT;
	     image.loc[0]=lx;
	     image.loc[1]=fixY(ly);
	     image.visible=1;
	     text.color=col;
	     strncpy(text.font_name,fname,STRINGLENGTH-1);
	     strncpy(text.text,str+textindx,STRINGLENGTH-1);
	     text.orient=cen;
	     text.xext=0;
	     text.xoff=0;
	     text.yext=0;
	     text.yoff=0;
	     image.object=&text;

	     im = TANGOimage_create(&image);


 //   im = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, lx, fixY(ly), 1, col, fname,str+textindx,cen);
                          
   ASSOCstore("IDS",id,im);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   if (findObject(id))
      if (debug) printf("Warning: object with id=%d already exists\n",id);
   if (cen)
      addObject(id, Ctxt);
   else
      addObject(id, Ltxt);
 }





void
delete(char *str)
{
   char cmd[SLEN];
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d",cmd,&id);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to delete object with nonexistant id=%d\n",id);
      return;
    }
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, im, path);
   TANGOtrans_perform(trans);
   ASSOCdelete("IDS",id);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
   removeObject(id);
} 


void
move( char *str)
{
   char cmd[SLEN];
   double tx,ty,fty;
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d %lf %lf",cmd,&id,&tx,&ty);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to move object with nonexistant id=%d\n",id);
      return;
    }
   fty = fixY(ty);
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   if (obj->type == Line)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Rect)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_SW);
   else if (obj->type == Circ)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Tria)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Ctxt)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Ltxt)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_SW);
   loc2 = TANGOloc_create(tx,fty);
   path = TANGOpath_motion(loc1,loc2,TANGO_PATH_TYPE_STRAIGHT);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 



void
moverelative( char *str)
{
   char cmd[SLEN];
   double tx,ty;
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d %lf %lf",cmd,&id,&tx,&ty);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to moverelative object with nonexistant id=%d\n",id);
      return;
    }
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   loc1 = TANGOloc_create(0.0,0.0);
   loc2 = TANGOloc_create(tx,-ty);
   path = TANGOpath_motion(loc1,loc2,TANGO_PATH_TYPE_STRAIGHT);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 



void
moveto(char *str)
{
   char cmd[SLEN];
   int id1,id2;
   ObjPtr obj1,obj2;
   TANGO_IMAGE im1,im2;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d %d",cmd,&id1,&id2);
   if (!(obj1 = findObject(id1))) {
      if (debug) 
         printf("Error: attempt to moveto object with nonexistant id=%d\n",id1);
      return;
    }
   if (!(obj2 = findObject(id2))) {
      if (debug) 
         printf("Error: attempt to moveto object with nonexistant id=%d\n",id2);
      return;
    }
   im1 = (TANGO_IMAGE) ASSOCretrieve("IDS",id1);
   im2 = (TANGO_IMAGE) ASSOCretrieve("IDS",id2);
   if (obj1->type == Line)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Rect)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   else if (obj1->type == Circ)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Tria)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ctxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ltxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   if (obj2->type == Line)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Rect)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   else if (obj2->type == Circ)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Tria)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ctxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ltxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   path = TANGOpath_motion(loc1,loc2,TANGO_PATH_TYPE_STRAIGHT);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im1, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 




void
jump(   char *str)
{
   char cmd[SLEN];
   double tx,ty,fty;
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d %lf %lf",cmd,&id,&tx,&ty);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to jump object with nonexistant id=%d\n",id);
      return;
    }
   fty = fixY(ty);
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   if (obj->type == Line)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Rect)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_SW);
   else if (obj->type == Circ)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Tria)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Ctxt)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_C);
   else if (obj->type == Ltxt)
      loc1 = TANGOimage_loc(im, TANGO_PART_TYPE_SW);
   loc2 = TANGOloc_create(tx,fty);
   path = TANGOpath_distance(loc1,loc2,1000.0);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 



void
jumprelative(   char *str)
{
   char cmd[SLEN];
   double tx,ty;
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d %lf %lf",cmd,&id,&tx,&ty);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to jumprelative object with nonexistant id=%d\n",id);
      return;
    }
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   loc1 = TANGOloc_create(0.0,0.0);
   loc2 = TANGOloc_create(tx,-ty);
   path = TANGOpath_distance(loc1,loc2,1000.0);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 



void
jumpto(   char *str)
{
   char cmd[SLEN];
   int id1,id2;
   ObjPtr obj1,obj2;
   TANGO_IMAGE im1,im2;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d %d",cmd,&id1,&id2);
   if (!(obj1 = findObject(id1))) {
      if (debug) 
         printf("Error: attempt to jumpto object with nonexistant id=%d\n",id1);
      return;
    }
   if (!(obj2 = findObject(id2))) {
      if (debug) 
         printf("Error: attempt to jumpto object with nonexistant id=%d\n",id2);
      return;
    }
   im1 = (TANGO_IMAGE) ASSOCretrieve("IDS",id1);
   im2 = (TANGO_IMAGE) ASSOCretrieve("IDS",id2);
   if (obj1->type == Line)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Rect)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   else if (obj1->type == Circ)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Tria)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ctxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ltxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   if (obj2->type == Line)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Rect)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   else if (obj2->type == Circ)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Tria)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ctxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ltxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   path = TANGOpath_distance(loc1,loc2,1000.0);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im1, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 




void
color(char *str)
{
   char cmd[SLEN],colname[SLEN];
   int id;
   ObjPtr obj;
   TANGO_COLOR col;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d %s",cmd,&id,colname);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to color object with nonexistant id=%d\n",id);
      return;
    }
   if (!validColor(colname)) {
      if (debug) 
         printf("Cannot change object %d to color %s.  (Ignored)\n",id,colname);
      return;
    }
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   col = getColor(colname);
   path = TANGOpath_color(col);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 




void
fill(char *str)
{
   char cmd[SLEN],fill[SLEN];
   int id;
   double fillval,v;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_PATH path1,path2;
   TANGO_TRANS trans1,trans2,both;

   sscanf(str,"%s %d %s",cmd,&id,fill);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to fill object with nonexistant id=%d\n",id);
      return;
    }
   fillval = getFill(fill);
   if (fillval == -1.0) {
      printf("Invalid fill argument ->%s<- given to circle command.  (Ignoring)\n",fill);
      return;
    }
   if (fillval == 0.0)
      v = -1.0;
   else if (fillval == 1.0)
      v = 1.0;
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   if ((fillval < 0.49) || (fillval > 0.51)) {
      path1 = TANGOpath_create(1,&v,&v);
      trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, im, path1);
      TANGOtrans_perform(trans1);
      TANGOpath_free(1,path1);
      TANGOtrans_free(1,trans1);
    }
   else {   /* half-tone */
      v = -1.0;
      path1 = TANGOpath_create(1,&v,&v);
      v = 0.5;
      path2 = TANGOpath_create(1,&v,&v);
      trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, im, path1);
      trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, im, path2);
      both = TANGOtrans_compose(2,trans1,trans2);
      TANGOtrans_perform(both);
      TANGOpath_free(2,path1,path2);
      TANGOtrans_free(3,trans1,trans2,both);
    }
} 




void
vis( char *str)
{
   char cmd[SLEN];
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d",cmd,&id);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to vis object with nonexistant id=%d\n",id);
      return;
    }
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 




void
raise(char *str)
{
   char cmd[SLEN];
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d",cmd,&id);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to raise object with nonexistant id=%d\n",id);
      return;
    }
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 




void
lower(char *str)
{
   char cmd[SLEN];
   int id;
   ObjPtr obj;
   TANGO_IMAGE im;
   TANGO_PATH path;
   TANGO_TRANS trans;

   sscanf(str,"%s %d",cmd,&id);
   if (!(obj = findObject(id))) {
      if (debug) 
         printf("Error: attempt to lower object with nonexistant id=%d\n",id);
      return;
    }
   im = (TANGO_IMAGE) ASSOCretrieve("IDS",id);
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_LOWER, im, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
} 




void
exchangepos(char *str)
{
   char cmd[SLEN];
   int id1,id2;
   ObjPtr obj1,obj2;
   TANGO_IMAGE im1,im2;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path1,path2;
   TANGO_TRANS trans1,trans2,both;

   sscanf(str,"%s %d %d",cmd,&id1,&id2);
   if (!(obj1 = findObject(id1))) {
      if (debug) 
         printf("Error: attempt to exchangepos object with nonexistant id=%d\n",id1);
      return;
    }
   if (!(obj2 = findObject(id2))) {
      if (debug) 
         printf("Error: attempt to exchangepos object with nonexistant id=%d\n",id2);
      return;
    }
   im1 = (TANGO_IMAGE) ASSOCretrieve("IDS",id1);
   im2 = (TANGO_IMAGE) ASSOCretrieve("IDS",id2);
   if (obj1->type == Line)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Rect)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   else if (obj1->type == Circ)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Tria)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ctxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ltxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   if (obj2->type == Line)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Rect)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   else if (obj2->type == Circ)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Tria)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ctxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ltxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   path1 = TANGOpath_motion(loc1,loc2,TANGO_PATH_TYPE_STRAIGHT);
   path2 = TANGOpath_reverse(path1);
   trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im1, path1);
   trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im2, path2);
   both = TANGOtrans_compose(2,trans1,trans2);
   TANGOtrans_perform(both);
   TANGOpath_free(2,path1,path2);
   TANGOtrans_free(3,trans1,trans2,both);
} 




void
switchpos(char *str)
{
   char cmd[SLEN];
   int id1,id2;
   ObjPtr obj1,obj2;
   TANGO_IMAGE im1,im2;
   TANGO_LOC loc1,loc2;
   TANGO_PATH path1,path2;
   TANGO_TRANS trans1,trans2,both;

   sscanf(str,"%s %d %d",cmd,&id1,&id2);
   if (!(obj1 = findObject(id1))) {
      if (debug) 
         printf("Error: attempt to switchpos object with nonexistant id=%d\n",id1);
      return;
    }
   if (!(obj2 = findObject(id2))) {
      if (debug) 
         printf("Error: attempt to switchpos object with nonexistant id=%d\n",id2);
      return;
    }
   im1 = (TANGO_IMAGE) ASSOCretrieve("IDS",id1);
   im2 = (TANGO_IMAGE) ASSOCretrieve("IDS",id2);
   if (obj1->type == Line)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Rect)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   else if (obj1->type == Circ)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Tria)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ctxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_C);
   else if (obj1->type == Ltxt)
      loc1 = TANGOimage_loc(im1, TANGO_PART_TYPE_SW);
   if (obj2->type == Line)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Rect)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   else if (obj2->type == Circ)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Tria)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ctxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_C);
   else if (obj2->type == Ltxt)
      loc2 = TANGOimage_loc(im2, TANGO_PART_TYPE_SW);
   path1 = TANGOpath_distance(loc1,loc2,1000.0);
   path2 = TANGOpath_reverse(path1);
   trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im1, path1);
   trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, im2, path2);
   both = TANGOtrans_compose(2,trans1,trans2);
   TANGOtrans_perform(both);
   TANGOpath_free(2,path1,path2);
   TANGOtrans_free(3,trans1,trans2,both);
} 

void
swapid(char *str)
{
   char cmd[SLEN];
   int id1,id2;
   ObjPtr obj1,obj2;
   TANGO_IMAGE ti;

   sscanf(str,"%s %d %d",cmd,&id1,&id2);
   if (!(obj1 = findObject(id1))) {
      if (debug) 
         printf("Error: attempt to swapid object with nonexistant id=%d\n",id1);
      return;
    }
   if (!(obj2 = findObject(id2))) {
      if (debug) 
         printf("Error: attempt to swapid object with nonexistant id=%d\n",id2);
      return;
    }
   ti = (TANGO_IMAGE) ASSOCretrieve("IDS",id1);
   ASSOCstore("IDS",id1,ASSOCretrieve("IDS",id2));
   ASSOCstore("IDS",id2,ti);
}


   


void
comment(char *str)
{
   printf("%s\n",str+7);
}



double
getWidth(s)
   char *s;
{
   if (!strcmp(s,"thin"))
      return 0.0;
   else if (!strcmp(s, "medthick"))
      return 0.5;
   else if (!strcmp(s, "thick"))
      return 1.0;
   else
      return -1.0;
}


double
getFill(s)
   char *s;
{
   if (!strcmp(s,"outline"))
      return 0.0;
   else if (!strcmp(s, "half"))
      return 0.5;
   else if (!strcmp(s, "solid"))
      return 1.0;
   else
      return -1.0;
}





double
fixY(y)
   double y;
{
   double lx,by,rx,ty;

   TANGOinq_coord(&lx,&by,&rx,&ty);
   return (by-y+ty);
}



int
validColor(name)
   char *name;
{
   if (!strcmp(name,"black"))
      return 1;
   else if (!strcmp(name,"white"))
      return 1;
   else if (!strcmp(name,"red"))
      return 1;
   else if (!strcmp(name,"green"))
      return 1;
   else if (!strcmp(name,"blue"))
      return 1;
   else if (!strcmp(name,"yellow"))
      return 1;
   else if (!strcmp(name,"maroon"))
      return 1;
   else if (!strcmp(name,"orange"))
      return 1;
   else
      return 0;
}



void
addObject(id, type)
   int id;
   ObjType type;
{
   ObjPtr o;

   o = (ObjPtr) malloc(sizeof(struct ObjNode));
   if (!Head)
      Head = o;
   if (Tail)
      Tail->next = o;
   o->id = id;
   o->type = type;
   o->next = NULL;
   Tail = o;
}



ObjPtr
findObject(id)
   int id;
{
   ObjPtr o;

   for (o=Head; o; o=o->next)
       if (o->id == id)
          return(o);
   return(NULL);
}



void
removeObject(id)
   int id;
{
   ObjPtr o,old;

   if (Head->id == id) {
      if (Head == Tail)
         Tail = NULL;
      o = Head;
      Head = Head->next;
      free(o);
      return;
    }
   old = Head;
   for (o=Head->next; o; o=o->next) {
       if (o->id == id) {
          old->next = o->next;
          if (o == Tail)
             Tail = old;
          free(o);
          return;
	}
       old = o;
     }
}



int
getColor(str)
   char *str;
{
   if (strcmp("white",str) == 0)
      return( TANGO_COLOR_WHITE );
   else if (strcmp("black",str) == 0)
      return( TANGO_COLOR_BLACK );
   else if (strcmp("red",str) == 0)
      return( TANGO_COLOR_RED );
   else if (strcmp("orange",str) == 0)
      return( TANGO_COLOR_ORANGE );
   else if (strcmp("yellow",str) == 0)
      return( TANGO_COLOR_YELLOW );
   else if (strcmp("green",str) == 0)
      return( TANGO_COLOR_GREEN );
   else if (strcmp("blue",str) == 0)
      return( TANGO_COLOR_BLUE );
   else if (strcmp("maroon",str) == 0)
      return( TANGO_COLOR_MAROON );
   else 
      return( TANGO_COLOR_BLACK );
}



int
stdColor(str)
   char *str;
{
   if (strcmp("white",str) == 0)
      return( 1 );
   else if (strcmp("black",str) == 0)
      return( 1 );
   else if (strcmp("red",str) == 0)
      return( 1 );
   else if (strcmp("orange",str) == 0)
      return( 1 );
   else if (strcmp("yellow",str) == 0)
      return( 1 );
   else if (strcmp("green",str) == 0)
      return( 1 );
   else if (strcmp("blue",str) == 0)
      return( 1 );
   else if (strcmp("maroon",str) == 0)
      return( 1 );
   else 
      return( 0 );
}



int
parseText(str)
   char *str;
{
   int i,j;

   i = 0;
   while (*(str+i) == ' ') i++;  /* get past start spaces */
   while (*(str+i) != ' ') i++;  /* get past 'text' command */
   for (j=1; j<=5; j++) {    /* get past other params */
      while (*(str+i) == ' ') i++;
      while ((*(str+i) != ' ') && (*(str+i) != '\0')) i++;
    }
   if (*(str+i) == '\0') {
      printf("Error:  No string argument to text, bigtext, or flextext command\n");
      return(-1);
    }
   while (*(str+i) == ' ') i++;
   if (*(str+i) == '\0') {
      printf("Error:  No string argument to text, bigtext, or flextext command\n");
      return(-1);
    }
   return (i);
}



int
parseFlexText(str)
   char *str;
{
   int i,j;

   i = 0;
   while (*(str+i) == ' ') i++;  /* get past start spaces */
   while (*(str+i) != ' ') i++;  /* get past 'text' command */
   for (j=1; j<=6; j++) {    /* get past other params */
      while (*(str+i) == ' ') i++;
      while ((*(str+i) != ' ') && (*(str+i) != '\0')) i++;
    }
   if (*(str+i) == '\0') {
      printf("Error:  No string argument to text, bigtext, or flextext command\n");
      return(-1);
    }
   while (*(str+i) == ' ') i++;
   if (*(str+i) == '\0') {
      printf("Error:  No string argument to text, bigtext, or flextext command\n");
      return(-1);
    }
   return (i);
}



