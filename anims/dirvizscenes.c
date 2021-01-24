#include <stdio.h>
#include "../src/xtangolocal.h"
#include "dirviz.h"

/* By Mark Gray (vatavian@cc.gatech.edu) 1 June 1992
   Written for John Stasko's Visualization in Programming course
   Inspired by TreeViz for the Macintosh by Brian Johnson based on
   the treemap concept originated by Ben Schneiderman
 */

TANGO_IMAGE LabelImage;
TANGO_TRANS totaltrans, nulltrans, refreshtrans;
TANGO_PATH  path1, path2;

enum colorfunctions {Owner, Type, Date, White} colorfunction = White;
extern int thismonth;
extern treenode *curroot;
extern int VERTICALSLICE;
extern int DEPTH, MAXDEPTH;

int frames=15;
float framerate=0.5;
int RAISE=0;
double buttonx[10];
double labely = texttop + (1-texttop)/2;

#ifdef OWNERSHIP
static char *OwnerColor[12][2] =
{"vatavian", "wheat",
 "graaff",   "yellow",
 "srdjan",   "YellowGreen",
 "martin",   "CornflowerBlue",
 "kb",       "CadetBlue",
 "noi",      "LavenderBlush2",
 "rjohnson", "sienna",
 "kilian",   "brown",
 "dennis",   "brown2",
 "daniel",   "brown3",
 "root",     "DimGray",
 "",""
};

TANGO_COLOR TangoOwnerColor[12];
#endif

static char *TypeColor[8][8] =
{"Red",       "core",    "~",    "%",    "#",    "",     "",    "",
 "Brown",     "akefile", "",     "",     "",     "",     "",    "",
 "Green",     ".c",      ".cc",  ".C",   ".h",   ".H",   "",    "",
 "slategray", ".o",      "",     "",     "",     "",     "",    "",
 "Blue",      ".icon",   ".rs",  ".gif", ".jpg", ".ps",  ".eps","",
 "SkyBlue",   ".au",     ".snd", ".voc", ".wav", "",     "",    "",
 "OliveDrab", ".Z",      ".zip", ".zoo", ".lha", ".tar", ".lzh","",
 "",          "",        "",     "",     "",     "",     "",    ""
};

TANGO_COLOR TangoTypeColor[7];

static char *GradientColor[8] =
{
 "gray100",
 "gray93",
 "gray84",
 "gray75",
 "gray66",
 "gray57",
 "gray48",
 "gray39"
};

TANGO_COLOR TangoGradientColor[8];

char *month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int MonthDistance(oldmonth)
     char *oldmonth;
{ int oldnum;
  for(oldnum=0; strcmp(oldmonth, month[oldnum]); oldnum++);
  oldnum++;
  if (thismonth >= oldnum) return (thismonth - oldnum);
  else return(12-oldnum+thismonth);
}

int
setframes(files)
     int files;
{ 
  if     (files < 20) frames=20*framerate;
  else if(files < 40) frames=15*framerate;
  else if(files < 60) frames=10*framerate;
  else if(files < 80) frames=5*framerate;
  else                frames=2*framerate;
  if(frames<1) frames=1;
}

ChangeLabel(label)
     char *label;
{
  TANGO_TRANS trans;
  if(LabelImage)
    {
      trans = TAPcolor(LabelImage, TANGO_COLOR_WHITE);
      TANGOtrans_perform(trans);
      TANGOtrans_free(1,trans);
      trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,LabelImage,path1);
      TANGOtrans_perform(trans);
      TANGOtrans_free(1,trans);
    }
  LabelImage = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.01, 0.96, 1,
				 TANGO_COLOR_BLACK, 0, label, 0);
  trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, LabelImage, path1);
  TANGOtrans_perform(trans);
  TANGOtrans_free(1,trans);
}

TANGO_COLOR
ColorOf(node, fill)
     treenode *node;
     double *fill;
{ int i=0, j, age, found=0;

  if(node->permissions[0] == 'd')
       *fill = 0.05;
  else *fill = 1.0;
  switch(colorfunction)
    {
#ifdef OWNERSHIP
    case Owner:
      for(i=0; OwnerColor[i][0][0] !='\0' && strcmp(node->owner, OwnerColor[i][0]); i++);
      if(OwnerColor[i][0][0] != '\0') 
	{ 
	  if(!TangoOwnerColor[i]) TangoOwnerColor[i]=TANGOload_color(OwnerColor[i][1]);
	  return (TangoOwnerColor[i]);
	}
      else return TANGO_COLOR_WHITE;
#endif
    case Type:
      if(node->permissions[0] == 'd') return TANGO_COLOR_BLACK;
      if(node->permissions[3] == 'x') return TANGO_COLOR_ORANGE;
      while(TypeColor[i][0][0] !='\0' && !found)
	{
	  for(j=1; !found && TypeColor[i][j][0] !='\0'; j++)
	    found = !strcmp((node->name+strlen(node->name)-strlen(TypeColor[i][j])), TypeColor[i][j]);
	  if(!found) i++;
	}
      if(TypeColor[i][0][0] != '\0') 
	{ 
	  if(!TangoTypeColor[i]) TangoTypeColor[i]=TANGOload_color(TypeColor[i][0]);
	  return (TangoTypeColor[i]);
	}
      else return TANGO_COLOR_WHITE;
    case Date:
      if(!strchr(node->yeartime, ':')) age = 7;
      else age = MonthDistance(node->month);
      if(!TangoGradientColor[age]) TangoGradientColor[age]=TANGOload_color(GradientColor[age]);
      return (TangoGradientColor[age]);
    }
  return TANGO_COLOR_WHITE;
}

void
AnimInit(root, x1, y1, x2, y2, verticalslice, level)
     treenode *root;
     double x1, y1, x2, y2;
     int verticalslice, level;
{
  treenode *node=root;
  double x1a=x1, y1a=y1, x2a=x2, y2a=y2;
  double fill=1.0;
  int chars;
  
  root->visible = 1;
  if(level>MAXDEPTH) MAXDEPTH=level;
  if(level>=DEPTH || root->permissions[0] == '-') /* this is a leaf node (a box) */
    {
      root->image  = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, x1, y1,
				       1, ColorOf(root, &fill), x2-x1, y2-y1, fill);
      root->border = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, x1, y1,
				       1, TANGO_COLOR_BLACK,    x2-x1, y2-y1, 0.0);
      if((y2-y1)>CHARHEIGHT)
	  {
	    chars = (x2-x1)/CHARWIDTH;
	    if(chars >= 1)
	      if(chars >= strlen(root->name))
		root->label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x1+(x2-x1)/2, y1+(y2-y1)/2,
						1, TANGO_COLOR_BLACK, 0, root->name, 1);
	  }
      root->x1=x1, root->y1=y1;
      root->x2=x2, root->y2=y2;
    }
  else /* this is a directory node and we recurse */
    for(node=root->child; node; node = node->sibling)
      {
	if(verticalslice) x2a = x1a + node->size*(x2-x1)/root->size;
	else              y2a = y1a + node->size*(y2-y1)/root->size;

	node->x1=x1a, node->y1=y1a;
	node->x2=x2a, node->y2=y2a;
	
	AnimInit(node, x1a, y1a, x2a, y2a, !verticalslice, (level+1));

	if(verticalslice) x1a = x2a;
	else              y1a = y2a;
      }
  if(x1==y1) /* make buttons */
    {
      double buttonmid;
      int    button = 0;
      path1 = TANGOpath_null(1);
      path2 = TANGOpath_null(2);
      nulltrans    = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path1);
      refreshtrans = TANGOtrans_create(TANGO_TRANS_TYPE_REFRESH,NULL,path1);		      
      LabelImage = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.01, 0.96, 1, TANGO_COLOR_BLACK, 0, "", 0);

      buttonx[button] = 1.0-6*CHARWIDTH;
      buttonmid = (1+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 1.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"Hide",1);
      button++;

#ifdef OWNERSHIP
      buttonx[button] = buttonx[button-1]-7*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 0.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"Owner",1);
      button++;
#endif

      buttonx[button] = buttonx[button-1]-6*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 0.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"Date",1);
      button++;

      buttonx[button] = buttonx[button-1]-6*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 1.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"Type",1);
      button++;

      buttonx[button] = buttonx[button-1]-4*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 0.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"..",1);
      button++;

      buttonx[button] = buttonx[button-1]-10*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 0.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"Show All",1);
      button++;

      buttonx[button] = buttonx[button-1]-8*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 0.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"Deeper",1);
      button++;

#ifdef FRAMEADJUST
      buttonx[button] = buttonx[button-1]-3*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 0.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"+",1);
      button++;

      buttonx[button] = buttonx[button-1]-3*CHARWIDTH;
      buttonmid = (buttonx[button-1]+buttonx[button])/2;
      TANGOimage_create(TANGO_IMAGE_TYPE_LINE, buttonx[button], texttop, 1, 
			TANGO_COLOR_BLACK, 0.0, (1-texttop), 0.0, 1.0, 0);
      TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,buttonmid,labely,1,TANGO_COLOR_BLACK,0,"-",1);
      button++;
#endif

      buttonx[button] = buttonx[button-1];
      TANGOtrans_perform(refreshtrans, NULL);
    }
}

void
MakeRescanTrans(root, x1, y1, x2, y2, verticalslice, level)
     treenode *root;
     double x1, y1, x2, y2;
     int verticalslice, level;
{
  treenode *node=root, *prevnode=0;
  double x1a=x1, y1a=y1, x2a=x2, y2a=y2, dx=0, dy=0, movedx=0, movedy=0, midx, midy;
  double vx[MAXFRAMES], vy[MAXFRAMES], fill=1;
  TANGO_PATH  path;
  TANGO_TRANS newtrans, tmptrans;
  int segment, chars;
  
  if((level >= DEPTH && !root->visible)
     ||(level >= DEPTH && root->visible && root->image)
     || root->permissions[0] == '-') /* this is a leaf node (a box) */
    {
      if(!root->image)
	{
	  treenode *neighbor;

	  root->visible = 1;
	  if(root->parent)
	    {
	      for(neighbor=root->parent; neighbor; neighbor = neighbor->parent)
		neighbor->files++;

	      root->x1=root->parent->x1;
	      root->y1=root->parent->y1;
	      root->x2=root->parent->x2;
	      root->y2=root->parent->y2;

	      neighbor = root->parent->child;
	      if(neighbor != root) 
		{
		  while(neighbor->sibling != root) 
		    neighbor = neighbor->sibling;
		  if(verticalslice) root->y1 = neighbor->y2;
		  else              root->x1 = neighbor->x2;
		}

	      if(verticalslice) root->y2 = y2;
	      else              root->x2 = x2;

	      root->image  = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, root->x1, root->y1,
					       1, ColorOf(root, &fill), root->x2-root->x1,
					       root->y2-root->y1, fill);
	      root->border = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, root->x1, root->y1,
					       1, TANGO_COLOR_BLACK, root->x2-root->x1,
					       root->y2-root->y1, 0.0);
	    }
	  else
	    {
	      root->x1=x1; root->y1=y1; root->x2=x2; root->y2=y2;
	      
	      root->image  = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, x1, y1, 1, 
					       ColorOf(root, &fill), x2-x1, y2-y1, fill);
	      root->border = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, x1, y1, 1,
					       TANGO_COLOR_BLACK, x2-x1, y2-y1, 0.0);
	    }
	}
      if(root->x1!=x1 || root->y1!=y1 || root->x2!=x2 || root->y2!=y2)
	{
	  if(root->label){
	    newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,root->label,path1);
	    tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	    TANGOtrans_free(1,totaltrans);
	    TANGOtrans_free(1,newtrans);
	    totaltrans = tmptrans;
	    root->label = 0;
	  }
	  
	  if((y2-y1)>CHARHEIGHT)
	    { midx = root->x1 + (root->x2 - root->x1)/2;
	      midy = root->y1 + (root->y2 - root->y1)/2;
	      chars = (x2-x1)/CHARWIDTH;
	      if(chars >= strlen(root->name))
		{
		  root->label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, midx, midy, 1, 
						  TANGO_COLOR_BLACK, 0, root->name, 1);
		  dx = (x1+(x2-x1)/2 - midx)/frames;
		  dy = (y1+(y2-y1)/2 - midy)/frames;
		  for(segment=0; segment<frames; segment++)
		    { vx[segment] = dx;
		      vy[segment] = dy;
		    }
		  path = TANGOpath_create(frames,vx,vy);
		  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,root->label,path);
		  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
		  TANGOtrans_free(1,totaltrans);
		  TANGOtrans_free(1,newtrans);
		      totaltrans = tmptrans;
		}
	    }
	  movedx = dx = (x1-root->x1)/frames;
	  movedy = dy = (y1-root->y1)/frames;
	  for(segment=0; segment<frames; segment++)
	    { vx[segment] = dx;
	      vy[segment] = dy;
	    }
	  path = TANGOpath_create(frames,vx,vy);
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,root->image,path);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  totaltrans = tmptrans;
	  
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,root->border,path);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  TANGOpath_free(1,path);
	  totaltrans = tmptrans;
	  
	  dx = (x2-root->x2)/frames - movedx;
	  dy = (y2-root->y2)/frames - movedy;
	  for(segment=0; segment<frames; segment++)
	    { vx[segment] = dx;
	      vy[segment] = dy;
	    }
	  path = TANGOpath_create(frames,vx,vy);
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,root->image,path);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  totaltrans = tmptrans;
	  
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,root->border,path);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  TANGOpath_free(1,path);
	  totaltrans = tmptrans;
	}
      if(RAISE)
	{
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,root->image,path1);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  totaltrans = tmptrans;
	  
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,root->border,path2);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  totaltrans = tmptrans;
	}
      if((y2-y1)>CHARHEIGHT)
	{
	  chars = (x2-x1)/CHARWIDTH;
	  if(chars >= strlen(root->name))
	    {
	      if(!root->label)
		root->label = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x1+(x2-x1)/2, y1+(y2-y1)/2,
						1, TANGO_COLOR_BLACK, 0, root->name, 1);
	      if(RAISE)
		{
		  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,root->label,path2);
		  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
		  TANGOtrans_free(1,totaltrans);
		  TANGOtrans_free(1,newtrans);
		  totaltrans = tmptrans;
		}
	    }
	}
      root->x1=x1, root->y1=y1;
      root->x2=x2, root->y2=y2;
    }
  else /* this is a directory node and we recurse */
    {
      root->visible = 1;
      if(root->image)
	{
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,root->image,path1);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  totaltrans = tmptrans;
	  root->image = 0;
	  
	  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,root->border,path1);
	  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	  TANGOtrans_free(1,totaltrans);
	  TANGOtrans_free(1,newtrans);
	  totaltrans = tmptrans;
	  root->border = 0;
	  
	  if(root->label){
	    newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,root->label,path1);
	    tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
	    TANGOtrans_free(1,totaltrans);
	    TANGOtrans_free(1,newtrans);
	    totaltrans = tmptrans;
	    root->label = 0;
	  }
	}
      node=root->child;
      while(node)
	{
	  if(node->size < 0)
	    {
	      if(node->image)
		{
		  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,node->image,path1);
		  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
		  TANGOtrans_free(1,totaltrans);
		  TANGOtrans_free(1,newtrans);
		  totaltrans = tmptrans;
		  node->image = 0;
		  
		  newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,node->border,path1);
		  tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
		  TANGOtrans_free(1,totaltrans);
		  TANGOtrans_free(1,newtrans);
		  totaltrans = tmptrans;
		  node->border = 0;
		  
		  if(node->label){
		    newtrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,node->label,path1);
		    tmptrans = TANGOtrans_compose(2,newtrans,totaltrans);
		    TANGOtrans_free(1,totaltrans);
		    TANGOtrans_free(1,newtrans);
		    totaltrans = tmptrans;
		    node->label = 0;
		  }
		}
	      if(prevnode) prevnode->sibling = node->sibling;
	      else {
		root->child = node->sibling; 
		if(node->sibling) node->sibling->parent = root;
	      }
	      free(node->permissions);
	      free(node->owner);
	      free(node->month);
	      free(node->yeartime);
	      free(node->name);
	      free(node);
	      if(prevnode) node = prevnode->sibling;
	      else node = root->child;
	    }
	  else
	    {
	      if(verticalslice) x2a = x1a + node->size*(x2-x1)/root->size;
	      else              y2a = y1a + node->size*(y2-y1)/root->size;
	      
	      if(node->permissions[0]!='-' && (level<DEPTH-1 || !node->visible))
		{ node->x1=x1a, node->y1=y1a;
		  node->x2=x2a, node->y2=y2a;
		}
	      
	      MakeRescanTrans(node, x1a, y1a, x2a, y2a, !verticalslice, (level+1));
	      
	      if(verticalslice) x1a = x2a;
	      else              y1a = y2a;
	      prevnode = node;
	      node = node->sibling;
	    }
	}
      root->x1=x1, root->y1=y1;
      root->x2=x2, root->y2=y2;
    }
}

void
AnimUpdate(curroot)
     treenode *curroot;
{
  totaltrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path1);
  setframes(curroot->files);
  MakeRescanTrans(curroot, 0.0, 0.0, 1.0, texttop, VERTICALSLICE, 0);
  TANGOtrans_perform(totaltrans, NULL);
  TANGOtrans_free(1,totaltrans);  
}

treenode *
GetFileAt(x,y,root,filepath,level)
     double x,y;
     treenode *root;
     char *filepath;
     int level;
{ treenode *node=root->child;

  while(node && (node->x1 > x || node->y1 > y || node->x2 < x || node->y2 < y))
    node = node->sibling;

  if(!node 
     || node->permissions[0] == '-'
     || (level>=DEPTH && (!node->visible || !node->child || !node->child->visible)))
    return node;

  strcat(filepath, node->name);
  strcat(filepath, "/");
  return GetFileAt(x,y,node,filepath,(level+1));
}

void
MakeColorTrans(root)
     treenode *root;
{
  treenode *node=root;
  double fill;
  TANGO_RECTANGLE_PTR rectangle;

  if(root->visible)
    {
      if(root->image)
	{
	  rectangle = (TANGO_RECTANGLE_PTR) ((struct _IMAGE *)(root->image))->object;
	  rectangle->color = ColorOf(root, &fill);
	}
      for(node=root->child; node; node = node->sibling)
	MakeColorTrans(node);
    }
}

void
ColorUpdate(root)
     treenode *root;
{
  MakeColorTrans(root);
  TANGOtrans_perform(refreshtrans, NULL);
}

void
Updir()
{
  treenode *neighbor;
  TANGO_PATH path;
  double x1=0.0, y1=0.0, x2=1.0, y2=texttop;
  if(curroot->parent)
    {
      neighbor = curroot->parent->child;
      if(neighbor != curroot) 
	{
	  while(neighbor->sibling != curroot) 
	    neighbor = neighbor->sibling;
	  if(VERTICALSLICE) y1 = neighbor->y2;
	  else              x1 = neighbor->x2;
	}
      neighbor = curroot->sibling;
      if(neighbor) 
	{
	  if(VERTICALSLICE) y2 = neighbor->y1;
	  else              x2 = neighbor->x1;
	}
      setframes(curroot->files);
      frames/=2;
      if(frames<1) frames=1;
      path = TANGOpath_null(frames);
      totaltrans = TANGOtrans_create(TANGO_TRANS_TYPE_REFRESH,NULL,path);
      MakeRescanTrans(curroot, x1, y1, x2, y2, VERTICALSLICE, 0);
      VERTICALSLICE=!VERTICALSLICE;
      TANGOtrans_perform(totaltrans, curroot);
      TANGOpath_free(1,path);
      TANGOtrans_free(1,totaltrans); 
      curroot = curroot->parent;
    }
}

void
Downdir() /* assumes curroot already updated to be new root */
{ treenode *child;
  int files=curroot->files;
  if(curroot->permissions[0] != '-')
    {
      totaltrans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path1);
      VERTICALSLICE=!VERTICALSLICE;
      if(files < 2)
	{ files = 1;
	  for(child = curroot->child; child; child = child->sibling)
	    files++;
	}
      setframes(files);
      MakeRescanTrans(curroot, 0.0, 0.0, 1.0, texttop, VERTICALSLICE, 0);
      TANGOtrans_perform(totaltrans, curroot);
      TANGOtrans_free(1,totaltrans);
    }
  else curroot = curroot->parent;
}

void
AnimFollowMouse(root)
     treenode *root;
{
  treenode *target = 0, *node;
  double x,y;
  char filepath[512];
  char label[512];
  char label2[512];
  char size[12];
  int button;

  while(1)
    {
      if (TANGOinput_coord(&x,&y))
	{
	  if(y > texttop)
	    {
	      button=0;
	      if(x > buttonx[button++]) /* hide */
		{
		  if(target)
		    { 
		      sprintf(label, "Hiding %s...", target->name);
		      ChangeLabel(label);
		      sprintf(label2, "Hid %s.", target->name);
		      for(node = target->parent; node; node = node->parent)
			node->size -= target->size;
		      target->size = -1;   
		      AnimUpdate(curroot);
		      ChangeLabel(label2);
		      target = 0;
		    }
		  else
		    {
		      strcpy(label, "No file selected.");
		      ChangeLabel(label);
		    }
		}
	      else 
#ifdef OWNERSHIP
	      if(x > buttonx[button++])
		{ colorfunction = Owner; ColorUpdate(root);
		  strcpy(label, "Colored by owner.");
		  ChangeLabel(label);
		}
	      else 
#endif
	      if(x > buttonx[button++])
		{ colorfunction = Date;  ColorUpdate(root);
		  strcpy(label, "Darker = older.");
		  ChangeLabel(label);
		}
	      else 
	      if(x > buttonx[button++])
		{ colorfunction = Type;  ColorUpdate(root);
		  strcpy(label, "Colored by file type.");
		  ChangeLabel(label);
		}
	      else 
	      if(x > buttonx[button++])
		if(curroot->parent)
		  {
		    RAISE=0;
		    sprintf(label, "Changing up to %s from %s...", curroot->parent->name, curroot->name);
		    sprintf(label2, "Now in %s. Up from %s.", curroot->parent->name, curroot->name);
		    ChangeLabel(label);
		    Updir();
		    ChangeLabel(label2);
		    TANGOtrans_perform(refreshtrans, NULL);
		    target = 0;
		  }
		else
		  {
		    sprintf(label, "Already in top directory %s.", curroot->name);
		    ChangeLabel(label);
		  }
	      else 
	      if(x > buttonx[button++])
		{ 
		  sprintf(label, "Showing all files...");
		  ChangeLabel(label);
		  DEPTH=MAXDEPTH; 
		  RAISE=0;
		  AnimUpdate(curroot);
		  sprintf(label, "Depth = %d.", DEPTH);
		  ChangeLabel(label);
		}
	      else
	      if(x > buttonx[button++])
		{ 
		  if(DEPTH<MAXDEPTH)
		    {
		      sprintf(label, "Going deeper...");
		      ChangeLabel(label);
		      DEPTH++;
		      RAISE=0;
		      AnimUpdate(curroot);
		    }
		  sprintf(label, "Depth = %d.", DEPTH);
		  ChangeLabel(label);
		}
	      else 
#ifdef FRAMEADJUST
	      if(x > buttonx[button++])
		{ 
		  if(framerate<1) framerate+=0.1;
		  sprintf(label, "Framerate = %0.1f.", framerate);
		  ChangeLabel(label);
		}
	      else
	      if(x > buttonx[button++])
		{ 
		  if(framerate>0.01) framerate-=0.1;
		  sprintf(label, "Framerate = %0.1f.", framerate);
		  ChangeLabel(label);
		}
	      else
#endif
		{
		  strcpy(label, "Nothing selected.");
		  ChangeLabel(label);
		}
	    }
	  else if(target && x>=target->x1 && x<=target->x2 && y>=target->y1 && y<=target->y2)
	    { 	
	      for(node = target; node && node->parent!=curroot; node = node->parent);
	      if(node && node->parent==curroot && node->permissions[0] == 'd') 
		{
		  sprintf(label, "Changing into %s...", node->name, curroot->name);
		  sprintf(label2, "Now in %s. Down from %s.", node->name, curroot->name);
		  ChangeLabel(label);
		  curroot = node;
		  RAISE=1;
		  Downdir();
		  ChangeLabel(label2);
		  target = 0;
		}
	    }
	  else
	    {
	      filepath[0] = '\0';
	      if(target = GetFileAt(x,y,curroot,filepath,1))
		{
		  strcat(filepath, target->name);
		  sprintf(label, "%s %d %s %s", filepath, target->size, target->owner, target->permissions);
		  ChangeLabel(label);
		}
	    }
	}
     }
}
