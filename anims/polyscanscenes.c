#include <stdio.h>
#include <xtango.h>

#define IMAGESIZE 50.0
#define BLACK 1
#define RED   2
#define ORANGE 3
#define YELLOW 4
#define GREEN  5
#define BLUE   6
#define MAROON 7


typedef struct
{
        int color;
        int priority;
} displayrecord;                /* contains the info necessary for display*/



static TANGO_IMAGE_COMPONENT axes[3] = {
   { TANGO_IMAGE_TYPE_LINE, "0.03 0.27 TANGO_COLOR_RED  0.0 0.7 0.5 1.0 2"},
   { TANGO_IMAGE_TYPE_LINE, "0.03 0.97 TANGO_COLOR_RED  0.73 0.0 0.5 1.0 1"},
   { TANGO_IMAGE_TYPE_COMPOSITE, NULL }
};

/* create the data association */
void
ANIMInit()
{
TANGO_IMAGE axis;
TANGO_IMAGE txt;
TANGO_PATH path;
TANGO_TRANS show;

axis = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,0.0,0.0,1,axes);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.03,0.25,1,TANGO_COLOR_BLUE,NULL,"Y",1);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.85,0.97,1,TANGO_COLOR_BLUE,NULL,"X",1);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.90,0.08,1,TANGO_COLOR_BLUE,NULL,"AEL",1);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.03,0.05,1,TANGO_COLOR_BLUE,NULL,"X-INTERCEPTS",0);

path = TANGOpath_null(1);
show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(show);
sleep(1);

 }



void
ANIMPoint(num,xval,yval)
int num;
double xval,yval;
{
TANGO_IMAGE pt;
TANGO_IMAGE txt;
TANGO_PATH path;
TANGO_TRANS show;
char numname[3];
int i;

for(i=0; i<3; i++)
   numname[i] = ' ';

num = num + 1;
sprintf(numname,"%d",num);
xval = ((xval / IMAGESIZE) * 0.6) + 0.03;
yval = 0.97 - ((yval / IMAGESIZE) * 0.7);

pt = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,xval,yval,1,TANGO_COLOR_BLACK,0.005,1.0);
xval = xval - 0.02;
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,xval,yval,1,TANGO_COLOR_BLACK,NULL,numname,1);

path = TANGOpath_null(1);
show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(show);
sleep(1);
}


void
ANIMProcess(xval,yval,xold,yold)
double xval,yval;
double xold,yold;
{
TANGO_IMAGE pt;
TANGO_IMAGE pt2;
TANGO_PATH p,path;
TANGO_TRANS show;
TANGO_LOC fromloc;
TANGO_LOC toloc;
double xdiff,ydiff;

xval = ((xval / IMAGESIZE) * 0.6) + 0.03;
yval = 0.97 - ((yval / IMAGESIZE) * 0.7);
xold = ((xold / IMAGESIZE) * 0.6) + 0.03;
yold = 0.97 - ((yold / IMAGESIZE) * 0.7);
xdiff = xval - xold;
ydiff = yval - yold;

pt = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,xold,yold,1,TANGO_COLOR_BLACK,0.005,1.0);
fromloc = TANGOimage_loc(pt,TANGO_PART_TYPE_C);
toloc = TANGOimage_loc(pt,TANGO_PART_TYPE_C);
/*
pt2 = TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE,xval,yval,1,TANGO_COLOR_BLACK,0.005,1.0);
toloc = TANGOimage_loc(pt2,TANGO_PART_TYPE_C);
*/

toloc = TANGOloc_modify(toloc,xdiff,ydiff);
p = TANGOpath_type(TANGO_PATH_TYPE_STRAIGHT);
path = TANGOpath_example(fromloc,toloc,p);
show = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,pt,path);

TANGOtrans_perform(show);
TANGOpath_free(2,path,p);
TANGOtrans_free(1,show);
}

void
ANIMNewAel()
{
TANGO_IMAGE rect;
TANGO_PATH path;
TANGO_TRANS show;

rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.87,0.1,1,TANGO_COLOR_WHITE,0.1,0.6,1.0);

path = TANGOpath_null(1);
show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(show);
}


void
ANIMAel(edge,xval,yval)
int edge;
double xval,yval;
{
TANGO_IMAGE rect;
TANGO_IMAGE txt;
TANGO_PATH path;
TANGO_TRANS show;
double xmid,ymid;
char edgename[3];
int i;

for(i=0; i<3; i++)
   edgename[i] = ' ';

edge = edge;

xmid = xval + ((xval + 0.08) - (xval)) / 2.0;
ymid = yval + ((yval + 0.008) - (yval)) / 2.0;

sprintf(edgename,"%d",edge);

rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_WHITE,0.08,0.08,1.0);

txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,xmid,ymid,1,TANGO_COLOR_BLACK,NULL,edgename,1);

path = TANGOpath_null(1);
show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(show);
}


void
ANIMHighlight(edge,xval,yval)
int edge;
double xval,yval;
{
TANGO_IMAGE rect;
TANGO_IMAGE txt;
TANGO_PATH path;
TANGO_TRANS show;
double xmid,ymid;
char edgename[3];
int i;

for(i=0; i<3; i++)
   edgename[i] = ' ';

edge = edge;

xmid = xval + ((xval + 0.08) - (xval)) / 2.0;
ymid = yval + ((yval + 0.008) - (yval)) / 2.0;

sprintf(edgename,"%d",edge);

/*
rect = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_WHITE,0.08,0.08,1.0);
*/

txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,xmid,ymid,1,TANGO_COLOR_RED,NULL,edgename,1);

path = TANGOpath_null(1);
show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(show);
}


void
ANIMIntercept(x1,x2,sline,color,priority,display)
int x1,x2;
int sline;
int color;
int priority;
displayrecord display[300][300];
{
TANGO_IMAGE txt;
TANGO_IMAGE line;
TANGO_PATH path;
TANGO_TRANS show;
char x1name[5];
char x2name[5];
char scanline[5];
int i;
double xval,xval2,yval;
double xdiff,xdiff2;

for(i=0; i<5; i++)
   {
    x1name[i] = ' ';
    x2name[i] = ' ';
   }

sprintf(x1name,"%d",x1);
sprintf(x2name,"%d",x2);
sprintf(scanline,"%d",sline);

txt = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.13,0.07,1,TANGO_COLOR_WHITE,0.14,0.11,1.0);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,0.18,0.10,1,TANGO_COLOR_WHITE,0.19,0.12,1.0);

txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.03,0.09,1,TANGO_COLOR_BLUE,NULL,"X1 = ",0);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.15,0.09,1,TANGO_COLOR_BLACK,NULL,x1name,0);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.03,0.12,1,TANGO_COLOR_BLUE,NULL,"X2 = ",0);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.15,0.12,1,TANGO_COLOR_BLACK,NULL,x2name,0);

txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.03,0.18,1,TANGO_COLOR_BLUE,NULL,"SCANLINE = ",0);
txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.23,0.18,1,TANGO_COLOR_BLACK,NULL,scanline,0);

xval = (((double)x1 / IMAGESIZE) * 0.6) + 0.03;
xval2 = (((double)x2 / IMAGESIZE) * 0.6) + 0.03;
yval = 0.97 - (((double)sline / IMAGESIZE) * 0.7);
xdiff = xval2 - xval;
xdiff2 = 0.6 - xval2;

for(i=x1; i<=x2; i++)
if(priority > display[i][sline].priority)
{
xval = (((double)i / IMAGESIZE) * 0.6) + 0.03;
if(color == BLACK)
   line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_BLACK,0.009,0.007,1.0);
else if(color == RED)
   line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_RED,0.009,0.007,1.0);
else if(color == ORANGE)
   line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_ORANGE,0.009,0.007,1.0);
else if(color == YELLOW)
   line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_YELLOW,0.009,0.007,1.0);
else if(color == GREEN)
   line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_GREEN,0.009,0.007,1.0);
else if(color == BLUE)
   line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_BLUE,0.009,0.007,1.0);
else if(color == MAROON)
   line = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,xval,yval,1,TANGO_COLOR_MAROON,0.009,0.007,1.0);
}


path = TANGOpath_null(1);
show = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
TANGOtrans_perform(show);
}



