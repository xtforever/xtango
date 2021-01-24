#ifndef tree_h
#define tree_h

/* select features to include */
/*#define FRAMEADJUST  /* whether to include buttons that adjust number of frames */
/*#define OWNERSHIP    /* whether to include color by owner option (owners -> colors hard coded) */

#define MAXFRAMES     50
#define texttop     0.94
#define CHARWIDTH   0.009
#define CHARHEIGHT  0.0095

typedef struct treenodestruct
{
  char *name;
  int   size;
  int   files;
  int   date;
  char *month;
  char *yeartime;
  char *owner;
  char *permissions;
  double x1,y1,x2,y2;
  int         visible;
  TANGO_IMAGE image;
  TANGO_IMAGE border;
  TANGO_IMAGE label;
  struct treenodestruct *child, *sibling, *parent;
} treenode;

#endif
