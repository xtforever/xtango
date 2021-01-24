/***************************************
   Virtual Memory Page Replacement
        by
   Jennifer Lautenschlager
****************************************/


/******************************************************************
*  This file handles the animation for the algorithms             *
******************************************************************/
#include <stdio.h>
#include <string.h>
#include "vmem.h"
#include "xtango.h"

static int num_left_faults=0,   /* Number of faults in left buffer  */
           num_right_faults=0;  /* Number of faults in right buffer */
static int leftframes, rightframes;
static double Lyoffset;   /* How far down?     */
static double Ryoffset;   /* How far down?     */

extern int sound;   /* Whether or not sound */
extern int color;   /* Whether or not color */

/*************************************************************************
*  Sets up the initial animation window: draws the frame buffers on      *
*  right and left, and sets up some associations for later use           *
*************************************************************************/
void AnimInit(left_frames,left_method,right_frames,right_method)    
    int left_frames;        /* How many frames in left buffer  */
    char *left_method;      /* Name of the left method         */
    int right_frames;       /* How many frames in right buffer */
    char *right_method;     /* Name of the right method        */     
{
    double Lbuffer_size = FRAME_HEIGHT*left_frames;   /* How big is buffer */
    double Rbuffer_size = FRAME_HEIGHT*right_frames;  /* How big is buffer */
    TANGO_PATH path;     /* Temp path                */
    TANGO_TRANS trans;   /* Temp transition          */
    TANGO_LOC start_pt,  /* Starting point for block */
	      end_pt;    /* Ending point for block   */
    TANGO_IMAGE image;
    int i;

    /* Copy over pertinent info */
    leftframes  = left_frames;
    rightframes = right_frames;
    Lyoffset = MARGIN+MAX_SIZE-Lbuffer_size;   /* How far down?     */
    Ryoffset = MARGIN+MAX_SIZE-Rbuffer_size;   /* How far down?     */

    /* Define the frame buffers */
    TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,MARGIN,Lyoffset,1,
                      TANGO_COLOR_BLACK,FRAME_WIDTH,Lbuffer_size,0.0);
    TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,MARGIN+FRAME_WIDTH/2,
          SCREEN_SIZE-MARGIN/2,1,TANGO_COLOR_BLACK,NULL,left_method,CENTERED);
    TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,SCREEN_SIZE-MARGIN-FRAME_WIDTH,
                   Ryoffset,1,TANGO_COLOR_BLACK,FRAME_WIDTH,Rbuffer_size,0.0);
    TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,SCREEN_SIZE-MARGIN-FRAME_WIDTH/2,
          SCREEN_SIZE-MARGIN/2,1,TANGO_COLOR_BLACK,NULL,right_method,CENTERED);

    /* Draw in the individual frames */
    for( i = 1; i < left_frames; i++)
        TANGOimage_create(TANGO_IMAGE_TYPE_LINE,MARGIN,Lyoffset+i*FRAME_HEIGHT,
                         1,TANGO_COLOR_BLACK,FRAME_WIDTH,0.0,0.0,1.0,0);

    for( i = 1; i < right_frames; i++ )
        TANGOimage_create(TANGO_IMAGE_TYPE_LINE,SCREEN_SIZE-MARGIN-FRAME_WIDTH,
                         Ryoffset+i*FRAME_HEIGHT,1,TANGO_COLOR_BLACK,
                         FRAME_WIDTH,0.0,0.0,1.0,0);

    /* Make an association for the nice little frames */
    ASSOCinit();
    TWISTcreate_loc_array(NULL,LEFT,left_frames,FALSE,MARGIN,
                          Lyoffset+FRAME_HEIGHT,FRAME_HEIGHT);
    TWISTcreate_loc_array(NULL,RIGHT,right_frames,FALSE,
                          SCREEN_SIZE-MARGIN-FRAME_WIDTH,
                          Ryoffset+FRAME_HEIGHT,FRAME_HEIGHT);

    /* Decide where we want the "blocks" to appear */
    start_pt=TANGOloc_create(SCREEN_SIZE/2-FRAME_WIDTH/2,SCREEN_SIZE-MARGIN/2);
    ASSOCmake("START_PT",0);
    ASSOCstore("START_PT",start_pt);

    /* Decide where we want the "blocks" to disappear */
    end_pt = TANGOloc_create(SCREEN_SIZE/2-FRAME_WIDTH/2,MARGIN/2);
    ASSOCmake("END_PT",0);
    ASSOCstore("END_PT",end_pt);

    /* More associations to worry about later */
    ASSOCmake("LEFT_FRAME",1);
    ASSOCmake("RIGHT_FRAME",1);
    ASSOCmake("LEFT_TEXT",1);
    ASSOCmake("RIGHT_TEXT",1);
    ASSOCmake("LEFT_FAULTS",0);
    ASSOCmake("RIGHT_FAULTS",0);

    /* Keep track of memory faults */
    image = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,MARGIN+FRAME_WIDTH/2,
	    MARGIN/2,1,TANGO_COLOR_BLACK,NULL,"Faults: 0",CENTERED);
    ASSOCstore("LEFT_FAULTS",image);
    image = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,SCREEN_SIZE-MARGIN-
	    FRAME_WIDTH/2,MARGIN/2,1,TANGO_COLOR_BLACK,NULL,"Faults: 0",
	    CENTERED);
    ASSOCstore("RIGHT_FAULTS",image);

    /* To make it magically appear, we need to define a path of some */
    /* type and perform a transition                                 */
    path = TANGOpath_null(1); 
    trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
    TANGOtrans_perform(trans);

    /* Clean up our memory */
    TANGOpath_free(1,path);
    TANGOtrans_free(1,trans);
}

/********************************************************************
*  Swaps a frame out of the buffer                                  *
*    1) Move the frame specified to the END_PT                      *
*    2) Delete said frame                                           *
********************************************************************/
void AnimSwapOut( side, frame_num, frame_size )
          int side;        /* LEFT or RIGHT, which side to swap out */
          int frame_num;   /* Number of frame to kick out           */
	  int frame_size;  /* Number of frames in buffer            */
{
    TANGO_LOC   frompt, topt;
    TANGO_IMAGE TANGO_frame, TANGO_text;
    TANGO_PATH  path;
    TANGO_TRANS trans[2], comp;
    char        frame[20], text[20];
    int         i;

    /* Which frame buffer, right or left? */
    if( side == LEFT )
    {
	strcpy(frame,"LEFT_FRAME");
	strcpy(text,"LEFT_TEXT");
    }
    else
    {
	strcpy(frame,"RIGHT_FRAME");
	strcpy(text,"RIGHT_TEXT");   
    }


    /**** STEP ONE: MOVE THE FRAME SPECIFIED TO THE END_PT ****/


    /* Retrive frame to swap out */
    TANGO_frame = (TANGO_IMAGE)ASSOCretrieve(frame,frame_num);
    TANGO_text  = (TANGO_IMAGE)ASSOCretrieve(text,frame_num);

    /* Get the from and to point */
    frompt = TANGOimage_loc(TANGO_frame,TANGO_PART_TYPE_SW);
    topt = (TANGO_LOC)ASSOCretrieve("END_PT");

    /* Make the path */
    path = TANGOpath_motion(frompt,topt,TANGO_PATH_TYPE_STRAIGHT);

    /* Move text and frame */
    trans[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,TANGO_frame,path);
    trans[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,TANGO_text,path);

    /* Make them occur simultaneously */
    comp = TANGOtrans_compose(2,trans[0],trans[1]);

    /* And perform */
    TANGOtrans_perform(comp);


    /**** STEP TWO: DELETE SAID FRAME ****/


    /* Now that it's there, delete it */
    path = TANGOpath_null(1);
    trans[0] = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,TANGO_text,path);
    trans[1] = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,TANGO_frame,path);

    /* Mix them into animation */
    comp = TANGOtrans_compose(2,trans[0],trans[1]);

    /* And they disappear! */
    TANGOtrans_perform(comp);

    /* A brilliant attempt at sound! */
    if( sound )
    {
        printf("Splat!!!!\n");
        Splat();
    }

    /* Release all memory */
    TANGOpath_free(1,path);
    TANGOtrans_free(3,trans[0],trans[1],comp);
}


/********************************************************************
*  Slides up frames to fill in holes                                *
********************************************************************/
void AnimSlideUp( side, frame_num, frame_size )
          int side;        /* LEFT or RIGHT, which side to swap out */
          int frame_num;   /* Number to start with                  */
	  int frame_size;  /* Number of frames in buffer            */
{
    TANGO_LOC   frompt, topt;
    TANGO_IMAGE TANGO_frame, TANGO_text;
    TANGO_PATH  path;
    TANGO_TRANS trans[2], comp;
    char        frame[20], text[20];
    int         i;

    /* Which frame buffer we're in, right or left? */
    if( side == LEFT )
    {
	strcpy(frame,"LEFT_FRAME");
	strcpy(text,"LEFT_TEXT");
    }
    else
    {
	strcpy(frame,"RIGHT_FRAME");
	strcpy(text,"RIGHT_TEXT");   
    }

    /* slide everything up in the buffers */
    for( i = frame_num; i < frame_size; i++ )
    {
        /* Retrive frame to swap out */
        TANGO_frame = (TANGO_IMAGE)ASSOCretrieve(frame,i);
        TANGO_text  = (TANGO_IMAGE)ASSOCretrieve(text,i);

	/* Store in new location */
        ASSOCstore(frame,i-1,TANGO_frame);
	ASSOCstore(text,i-1,TANGO_text);

	/* Get the from and to point */
	frompt = TANGOimage_loc(TANGO_frame,TANGO_PART_TYPE_SW);
	topt = TANGOimage_loc(TANGO_frame,TANGO_PART_TYPE_NW);

        /* Make the path */
        path = TANGOpath_motion(frompt,topt,TANGO_PATH_TYPE_STRAIGHT);

        trans[0]=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,TANGO_frame,path);
        trans[1]=TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,TANGO_text,path); 

	/* comp them */
	comp = TANGOtrans_compose(2,trans[0],trans[1]);

	/* And shift */
	TANGOtrans_perform(comp);
    }

    /* Release all memory */
    TANGOpath_free(1,path);
    TANGOtrans_free(3,trans[0],trans[1],comp);
}

/*****************************************************************
*  Moves the new frame into whatever block it goes to            *
*  Performs right and left buffers simultaneously                *
*****************************************************************/
void AnimMove( Lstatus, Lblock, Rstatus, Rblock, value )
         int Lstatus; /* SWAP, NEW, or REPEAT                     */
         int Lblock;  /* Where we're putting block on left side   */
         int Rstatus; /* SWAP, NEW, or REPEAT                     */
         int Rblock;  /* Where we're putting block on right side  */
         int value;   /* The reference value for the string       */
{
    double x,y, text_x, text_y;
    TANGO_LOC   loc, center, frompt, Ltopt, Rtopt;
    TANGO_IMAGE Lframe, Ltext, Rframe, Rtext, image;
    TANGO_PATH  Lpath,Rpath;
    TANGO_TRANS trans[10], move[10], comp;
    char        str[25];
    int         i;

    /* Create frame block and number in starting place */
    loc = (TANGO_LOC)ASSOCretrieve("START_PT");
    TANGOloc_inquire(loc,&x,&y);
    if( !color )
        Lframe=TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x,y,1,
                TANGO_COLOR_BLACK,FRAME_WIDTH,FRAME_HEIGHT,0.0);
    else
        Lframe=TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,x,y,1,
                TANGO_COLOR_RED,FRAME_WIDTH,FRAME_HEIGHT,0.5);
    center = TANGOimage_loc(Lframe,TANGO_PART_TYPE_C);
    TANGOloc_inquire(center,&text_x,&text_y);
    sprintf(str,"%d",value);
    Ltext = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,text_x, text_y,
           1,TANGO_COLOR_BLACK,NULL,str,1);

    /* Copy another set -- they'll be going into 2 different buffers */
    Rframe = TANGOimage_copy(Lframe);
    Rtext  = TANGOimage_copy(Ltext);

    /* Figure out the starting and ending points */
    frompt = TANGOimage_loc(Lframe,TANGO_PART_TYPE_SW);
    Ltopt = (TANGO_LOC)ASSOCretrieve("ID",LEFT,Lblock);
    Rtopt = (TANGO_LOC)ASSOCretrieve("ID",RIGHT,Rblock); 

    /* Make the paths arc */
    Lpath = TANGOpath_motion(frompt,Ltopt,TANGO_PATH_TYPE_COUNTERCLOCKWISE);
    Rpath = TANGOpath_motion(frompt,Rtopt,TANGO_PATH_TYPE_CLOCKWISE);

    /* 4 different paths, text and frames for both sides */
    trans[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,Lframe,Lpath);
    trans[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,Ltext,Lpath);
    trans[2] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,Rframe,Rpath);
    trans[3] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,Rtext,Rpath);

    /* Mix them into one massive simultaneous animation */
    comp = TANGOtrans_compose(4,trans[0],trans[1],trans[2],trans[3]); 

    /* And then perform the animation */
    TANGOtrans_perform(comp);

    /* If it's color mode, here's a kludge to make the colors not */
    /* All run together -- there's probably a more elegant way    */
    if( color )
    {
        /* Define the frame buffers */
        TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,MARGIN,Lyoffset,1,
                    TANGO_COLOR_BLACK,FRAME_WIDTH,leftframes*FRAME_HEIGHT,0.0);
        TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE,
                    SCREEN_SIZE-MARGIN-FRAME_WIDTH,Ryoffset,1,
                    TANGO_COLOR_BLACK,FRAME_WIDTH,rightframes*FRAME_HEIGHT,0.0);

        /* Draw in the individual frames */
        for( i = 1; i < leftframes; i++)
            TANGOimage_create(TANGO_IMAGE_TYPE_LINE,MARGIN,
                    Lyoffset+i*FRAME_HEIGHT,
                    1,TANGO_COLOR_BLACK,FRAME_WIDTH,0.0,0.0,1.0,0);

        for( i = 1; i < rightframes; i++ )
            TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
                         SCREEN_SIZE-MARGIN-FRAME_WIDTH,
                         Ryoffset+i*FRAME_HEIGHT,1,TANGO_COLOR_BLACK,
                         FRAME_WIDTH,0.0,0.0,1.0,0); 
    }

    /* Keep track of what we're putting where */
    /* Only show the new blocks               */
    if( Lstatus == NEW )
    {
        ASSOCstore("LEFT_FRAME",Lblock,Lframe);
        ASSOCstore("LEFT_TEXT",Lblock,Ltext);
        num_left_faults++;

	/* Delete the old message */
	image = (TANGO_IMAGE)ASSOCretrieve("LEFT_FAULTS");
        Lpath = TANGOpath_null(1);
	comp = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,image,Lpath);
	TANGOtrans_perform(comp);

	/* Print out the new message */
	sprintf(str,"Faults: %d",num_left_faults);
	image = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,MARGIN+FRAME_WIDTH/2,
		 MARGIN/2,1,TANGO_COLOR_BLACK,NULL,str,CENTERED);
        ASSOCstore("LEFT_FAULTS",image);

	/* Make it visible */
	comp = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,image,Lpath);
	TANGOtrans_perform(comp);
    }
    else
    {
	/* Make invisible */
	Lpath = TANGOpath_null(1);
	trans[0] = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,Lframe,Lpath);
	trans[1] = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,Ltext,Lpath);
	comp = TANGOtrans_compose(2,trans[0],trans[1]);
	TANGOtrans_perform(comp);
    }

    if( Rstatus == NEW )
    {
        ASSOCstore("RIGHT_FRAME",Rblock,Rframe);
        ASSOCstore("RIGHT_TEXT",Rblock,Rtext);
        num_right_faults++;

        /* Delete the old message */
	image = (TANGO_IMAGE)ASSOCretrieve("RIGHT_FAULTS");
	Rpath = TANGOpath_null(1);
	comp = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,image,Rpath);
	TANGOtrans_perform(comp);

	/* Print out the new message */
	sprintf(str,"Faults: %d",num_right_faults);
	image = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,SCREEN_SIZE-MARGIN-
		FRAME_WIDTH/2,MARGIN/2,1,TANGO_COLOR_BLACK,NULL,str,
		CENTERED);
        ASSOCstore("RIGHT_FAULTS",image); 

	/* Make it visible */
	comp = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,image,Rpath);
	TANGOtrans_perform(comp);
    }
    else
    {
	/* Make invisible */
	Rpath = TANGOpath_null(1); 
	trans[0] = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,Rframe,Rpath);
	trans[1] = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,Rtext,Rpath);
	comp = TANGOtrans_compose(2,trans[0],trans[1]);
	TANGOtrans_perform(comp);
    }

    /* Free the memory */
    TANGOpath_free(2,Lpath,Rpath);
    TANGOtrans_free(5,trans[0],trans[1],trans[2],trans[3],comp);
}
