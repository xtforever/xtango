/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (xtango.h)	      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*		John T. Stasko and Doug Hayes		      */

/**************************************************************/
/*****************	types & constants    ******************/
/**************************************************************/

typedef enum {
   TANGO_IMAGE_TYPE_LINE,
   TANGO_IMAGE_TYPE_RECTANGLE,
   TANGO_IMAGE_TYPE_CIRCLE,
   TANGO_IMAGE_TYPE_POLYLINE,
   TANGO_IMAGE_TYPE_POLYGON,
   TANGO_IMAGE_TYPE_ELLIPSE,
   TANGO_IMAGE_TYPE_SPLINE,
   TANGO_IMAGE_TYPE_TEXT,
   TANGO_IMAGE_TYPE_BITMAP,
   TANGO_IMAGE_TYPE_COMPOSITE
   } TANGO_IMAGE_TYPE;

typedef enum {
   TANGO_TRANS_TYPE_VISIBLE,
   TANGO_TRANS_TYPE_FILL,
   TANGO_TRANS_TYPE_MOVE,
   TANGO_TRANS_TYPE_RESIZE,
   TANGO_TRANS_TYPE_RESIZE1,
   TANGO_TRANS_TYPE_RESIZE2,
   TANGO_TRANS_TYPE_RESIZE3,
   TANGO_TRANS_TYPE_RESIZE4,
   TANGO_TRANS_TYPE_RESIZE5,
   TANGO_TRANS_TYPE_RESIZE6,
   TANGO_TRANS_TYPE_RESIZE7,
   TANGO_TRANS_TYPE_GRAB1,
   TANGO_TRANS_TYPE_GRAB2,
   TANGO_TRANS_TYPE_GRAB3,
   TANGO_TRANS_TYPE_GRAB4,
   TANGO_TRANS_TYPE_GRAB5,
   TANGO_TRANS_TYPE_GRAB6,
   TANGO_TRANS_TYPE_GRAB7,
   TANGO_TRANS_TYPE_COLOR,
   TANGO_TRANS_TYPE_RAISE,
   TANGO_TRANS_TYPE_LOWER,
   TANGO_TRANS_TYPE_DELAY,
   TANGO_TRANS_TYPE_REFRESH,
   TANGO_TRANS_TYPE_DELETE,
   TANGO_TRANS_TYPE_SHUFFLE,
   TANGO_TRANS_TYPE_ZOOM
   }  TANGO_TRANS_TYPE;

#define  TANGO_COLOR_WHITE			0
#define  TANGO_COLOR_YELLOW			1
#define  TANGO_COLOR_GREEN			2
#define  TANGO_COLOR_BLUE			3
#define  TANGO_COLOR_ORANGE			4
#define  TANGO_COLOR_RED			5
#define  TANGO_COLOR_MAROON			6
#define  TANGO_COLOR_BLACK			7

#define  TANGO_PATH_TYPE_STRAIGHT		0
#define  TANGO_PATH_TYPE_CLOCKWISE		1
#define  TANGO_PATH_TYPE_COUNTERCLOCKWISE	2

#define  TANGO_PART_TYPE_C			0
#define  TANGO_PART_TYPE_NW			1
#define  TANGO_PART_TYPE_N			2
#define  TANGO_PART_TYPE_NE			3
#define  TANGO_PART_TYPE_E			4
#define  TANGO_PART_TYPE_SE			5
#define  TANGO_PART_TYPE_S			6
#define  TANGO_PART_TYPE_SW			7
#define  TANGO_PART_TYPE_W			8

typedef  int		TANGO_COLOR;
typedef  int		TANGO_LINE_STYLE;
typedef  int		TANGO_FILL_STYLE;
typedef  int		TANGO_PATH_TYPE;
typedef  int		TANGO_PATH_MOTION;
typedef  int		TANGO_PART_TYPE;

typedef  struct _LOC	*TANGO_LOC;

typedef  struct _IMAGE	*TANGO_IMAGE;

typedef  struct _PATH	*TANGO_PATH;

typedef  struct _TRANS	*TANGO_TRANS;

typedef struct _IMAGE_COMP {
   TANGO_IMAGE_TYPE type;
   char 	    *args;
   } TANGO_IMAGE_COMPONENT;

#define VOID     0
#define CHAR_PTR 1

typedef void *(*FPTR)();
typedef struct {	/* List of function name/function    */
   char functname[20];	/*    pointers  for TANGOalgoOp()    */
   int  numfunctions;	/* Number of functions in list	     */
   struct {		/* List of functions		     */
      int  functype;	/*    Function return type	     */
      FPTR function;	/*    Function to call		     */
      } funct[5];	/* (max funct name/funct pairs == 5) */
   } NAME_FUNCT;

/**************************************************************/
/*****************	  entry points       ******************/
/**************************************************************/

extern	void		ASSOCinit();
extern	void		ASSOCmake();
extern	void		ASSOCstore();
extern  int             ASSOCmember();
extern	int		ASSOCretrieve();
extern  int             ASSOCdelete();

extern  char *		TANGOalgoOp();

extern	void		TANGOinit();
extern  void		TANGOend();

extern	int		TANGOinput_coord();
extern	int		TANGOinput_image();

extern  int             TANGOset_coord();
extern  void            TANGOinq_coord();
extern  void            TANGOset_bgcolor();

extern  TANGO_COLOR	TANGOload_color();

extern	TANGO_LOC	TANGOloc_create();
extern	double		TANGOloc_X();
extern	double		TANGOloc_Y();
extern	void		TANGOloc_inquire();
extern	TANGO_LOC	TANGOloc_modify();
extern	int		TANGOloc_equal();

extern	TANGO_IMAGE	TANGOimage_create();
extern  TANGO_IMAGE     TANGOimage_copy();
extern	TANGO_LOC	TANGOimage_loc();

extern	TANGO_PATH	TANGOpath_create();
extern	TANGO_PATH	TANGOpath_load();
extern	TANGO_PATH	TANGOpath_store();
extern	int		TANGOpath_length();
extern	double		TANGOpath_dx();
extern	double		TANGOpath_dy();
extern	TANGO_PATH	TANGOpath_rotate();
extern	TANGO_PATH	TANGOpath_interpolate();
extern  TANGO_PATH      TANGOpath_reverse();
extern	TANGO_PATH	TANGOpath_scale();
extern	TANGO_PATH	TANGOpath_extend();
extern	TANGO_PATH	TANGOpath_null();
extern	TANGO_PATH	TANGOpath_type();
extern	TANGO_PATH	TANGOpath_color();
extern	TANGO_PATH	TANGOpath_add_head();
extern	TANGO_PATH	TANGOpath_add_tail();
extern	TANGO_PATH	TANGOpath_delete_head();
extern	TANGO_PATH	TANGOpath_delete_tail();
extern	TANGO_PATH	TANGOpath_smooth();
extern	TANGO_PATH	TANGOpath_copy();
extern	TANGO_PATH	TANGOpath_example();
extern	TANGO_PATH	TANGOpath_motion();
extern	TANGO_PATH	TANGOpath_distance();
extern	TANGO_PATH	TANGOpath_concatenate();
extern	TANGO_PATH	TANGOpath_iterate();
extern	TANGO_PATH	TANGOpath_compose();
extern	void		TANGOpath_free();

extern	TANGO_TRANS	TANGOtrans_create();
extern	TANGO_TRANS	TANGOtrans_concatenate();
extern	TANGO_TRANS	TANGOtrans_iterate();
extern	TANGO_TRANS	TANGOtrans_compose();
extern	void		TANGOtrans_perform();
extern	void		TANGOtrans_free();

extern	void		TWISTcreate_loc_array();
extern	void		TWISTcreate_2d_loc_array();
extern	void		TWISTcreate_image_array();
extern	void		TWISTcreate_graph();
extern	void		TWISTcreate_bintree();

extern  void            TAPshow();
extern  TANGO_TRANS     TAPfill();
extern  TANGO_TRANS     TAPcolor();
extern  TANGO_TRANS     TAPvis_toggle();
extern  TANGO_TRANS     TAPjump();
extern  TANGO_TRANS     TAPmove();
extern  TANGO_TRANS     TAPtraverse();
extern  TANGO_TRANS     TAPswitch();
extern  TANGO_TRANS     TAPexchange();
extern  TANGO_TRANS     TAPflash();


/**************************************************************/
/*****************	end of xtango.h      ******************/
/**************************************************************/
