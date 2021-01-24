/******************************************************************
*               Animation Package for Linked Lists                *
*                       Sreenivas Gukal                           *
*                                                                 *
*                  File  "listscenes.c.c"                         *
******************************************************************/

#include <stdio.h>
#include "xtango.h"

#define TEXT_Y	0.5
#define NODE_Y	0.4
#define N_INC_X	0.1
#define N_INC_Y 0.2

static TANGO_IMAGE_COMPONENT nnode[3] =
        {{TANGO_IMAGE_TYPE_RECTANGLE, "0.0001 0.0001 TANGO_COLOR_BLACK 0.1 0.2 0"},
         {TANGO_IMAGE_TYPE_RECTANGLE, "0.1 0.0001 TANGO_COLOR_BLACK 0.05 0.2 0"},
         {TANGO_IMAGE_TYPE_COMPOSITE, NULL}};

static TANGO_IMAGE_COMPONENT arrow[] =
        {{TANGO_IMAGE_TYPE_LINE, "0.125 0.1 TANGO_COLOR_BLACK 0.075 0.0001 0.5 1.0 1"},
         {TANGO_IMAGE_TYPE_COMPOSITE, NULL}};

static TANGO_IMAGE_COMPONENT hhead[3] = 
	{{TANGO_IMAGE_TYPE_TEXT,"0.175 0.125 TANGO_COLOR_BLACK 0 HEAD 1"},
	 {TANGO_IMAGE_TYPE_LINE,"0.175 0.15 TANGO_COLOR_BLACK 0.0001 0.25 0.5 1.0 1"},
	 {TANGO_IMAGE_TYPE_COMPOSITE, NULL}};


static TANGO_IMAGE_COMPONENT alt_hhead[3] = 
	{{TANGO_IMAGE_TYPE_TEXT,"0.175 0.125 TANGO_COLOR_BLACK 0 HEAD 1"},
	 {TANGO_IMAGE_TYPE_LINE,"0.175 0.15 TANGO_COLOR_BLACK 0.125 0.35 0.5 1.0 1"},
	 {TANGO_IMAGE_TYPE_COMPOSITE, NULL}};

static TANGO_IMAGE_COMPONENT prev_ptr[3] = 
	{{TANGO_IMAGE_TYPE_TEXT,"0.0 0.0 TANGO_COLOR_BLACK 0 prev 1"},
	 {TANGO_IMAGE_TYPE_LINE,"0.0 -0.05 TANGO_COLOR_BLACK 0.0 -0.1 0.5 1.0 1"},
	 {TANGO_IMAGE_TYPE_COMPOSITE, NULL}};

static TANGO_IMAGE_COMPONENT new_ptr[3] = 
	{{TANGO_IMAGE_TYPE_TEXT,"0.0 -0.025 TANGO_COLOR_BLACK 0 new 1"},
	 {TANGO_IMAGE_TYPE_LINE,"0.0 -0.075 TANGO_COLOR_BLACK 0.0 -0.125 0.5 1.0 1"},
	 {TANGO_IMAGE_TYPE_COMPOSITE, NULL}};

static TANGO_IMAGE_COMPONENT del_ptr[3] = 
	{{TANGO_IMAGE_TYPE_TEXT,"0.0 -0.025 TANGO_COLOR_BLACK 0 to_delete 1"},
	 {TANGO_IMAGE_TYPE_LINE,"0.0 -0.075 TANGO_COLOR_BLACK 0.0 -0.125 0.5 1.0 1"},
	 {TANGO_IMAGE_TYPE_COMPOSITE, NULL}};

int node_ptr = -1;
double last_x_pos;

void init()
{
	TANGO_PATH   path;
	TANGO_TRANS  trans;
	TANGO_IMAGE  imag;

	ASSOCinit();
	ASSOCmake("IMAGE1",1);
	ASSOCmake("IMAGE2",1);
	ASSOCmake("IMAGE3",1);
	imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, 0.0001, 0.0001, 1, hhead);
	ASSOCstore("IMAGE1",0,imag);
	imag = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.175, TEXT_Y, 1, TANGO_COLOR_BLACK, NULL, "NULL", 1);
	ASSOCstore("IMAGE1",1,imag);
	node_ptr = 1;
	last_x_pos = 0.1;

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
	TANGOtrans_perform(trans);
	TANGOpath_free(1,path);
	TANGOtrans_free(1,trans);
}

void ins(str1,prev)
char *str1;
int prev;
{
	TANGO_PATH   path;
	TANGO_TRANS  trans;
	TANGO_TRANS  trans1, trans2, trans3;
	TANGO_IMAGE  imag, imag2, prev_imag, new_imag, ptr_imag, im[3];
	TANGO_IMAGE  text;
	TANGO_LOC    tloc;
	double       x, y, d, xoff[1], yoff[1];
	double	     x1, y1, x1_inc, y1_inc, x2, y2, x2_inc, y2_inc;
	int 	     ptr;

		/* Move the prev pointer */
	if (prev > 0)
	    {
	    path = TANGOpath_null(1);
	    text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "make prev point to the node before",1);
	    trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	    TANGOtrans_perform(trans);
	    sleep(1);
	    ptr = 1;
	    imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",ptr);
	    tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_C);
	    TANGOloc_inquire(tloc,&x,&y);
	    prev_imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,x,y+0.25,1,prev_ptr);
	    trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,prev_imag,path);
	    TANGOtrans_perform(trans);
	    path = TANGOpath_type(TANGO_PATH_TYPE_STRAIGHT);
	    ptr = 2;
 	    while (ptr <= prev)
		{
		trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,prev_imag,path);
		TANGOtrans_perform(trans);
		sleep(1);
		++ptr;
		}
	    }
	else
	    {
	    path = TANGOpath_null(1);
	    text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "node to be inserted at head",1);
	    trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	    TANGOtrans_perform(trans);
	    sleep(1);
	    }

	ptr = node_ptr;
	++node_ptr;

		/*  Move the NULL  field  */
	path = TANGOpath_type(TANGO_PATH_TYPE_STRAIGHT);
	imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",ptr);
	ASSOCstore("IMAGE1",ptr + 1,imag);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	--ptr;

		/* Move the other nodes */
	while (prev < ptr)
		{
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",ptr);
		ASSOCstore("IMAGE1",ptr + 1,imag);
		trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE2",ptr);
		ASSOCstore("IMAGE2",ptr + 1,imag);
		trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE3",ptr);
		ASSOCstore("IMAGE3",ptr + 1,imag);
		trans3 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
		trans = TANGOtrans_compose(4, trans1, trans2, trans3, trans);
		--ptr;
		}
	TANGOtrans_perform(trans);

		/* Create the long pointer or alt_head */
	if (prev > 0)
		{
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE3",prev);
		tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_W);
		TANGOloc_inquire(tloc,&x,&y);
		path = TANGOpath_null(1);
		trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
		ptr_imag = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, x, y, 1, TANGO_COLOR_BLACK, 0.275, 0.0001, 0.5, 1.0, 1);
		TANGOtrans_perform(trans);
		}
	else
		{
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",0);
		path = TANGOpath_null(1);
		trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
		ptr_imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, 0.0001, 0.0001, 1, alt_hhead);
		TANGOtrans_perform(trans);
		}

		/* Get the position for the new node */
	if (prev > 0)
		{
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",prev);
		tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_NW);
		TANGOloc_inquire(tloc,&x,&y);
		x = x + 0.2;
		y = y + 0.2;
		}
	else
		{
		x = 0.1;
		y = 0.6;
		}


	sleep(1);
	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);
	text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "create a new node",1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	TANGOtrans_perform(trans);
	sleep(2);

		/* Create the new node */
	path = TANGOpath_null(1);
	imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, x, y, 1, nnode);
	ASSOCstore("IMAGE1",prev + 1,imag);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	TANGOtrans_perform(trans);
	new_imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, x+0.075, y+0.4, 1, new_ptr);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	TANGOtrans_perform(trans);

	imag = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x+0.05 , TEXT_Y + 0.2,1, TANGO_COLOR_BLACK, NULL, str1, 1);
	ASSOCstore("IMAGE2",prev + 1,imag);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	TANGOtrans_perform(trans);

	im[0] = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, x, y, 1, arrow);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	TANGOtrans_perform(trans);

	im[1] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x + 0.3, 0.7, 1,TANGO_COLOR_BLACK, 0, "NULL", 1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	TANGOtrans_perform(trans);
	sleep(1);

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);
	if (prev > 0)
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "new->next = prev->next",1);
	else
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "new->next = head",1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	TANGOtrans_perform(trans);
	sleep(2);

		/* Make "next" of new node point to next node */
	im[2] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, x+0.125, 0.7,1,TANGO_COLOR_BLACK, 0.075, -0.2, 0.5, 1.0, 1);
	trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[0],path);
	trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[1],path);
	trans3 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,im[2],path);
	trans = TANGOtrans_compose(3, trans1, trans2, trans3);
	TANGOtrans_perform(trans);
	sleep(2);

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);
	if (prev > 0)
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "prev->next = new",1);
	else
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "head = new",1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	TANGOtrans_perform(trans);
	sleep(2);

		/* Remove the pointers before new node */
	trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,ptr_imag,path);
	TANGOtrans_perform(trans2);

		/* Get the locations for the pointer before */
        if (prev > 0)
                {
                imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",prev);
                tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_NW);
                TANGOloc_inquire(tloc,&x1,&y1);
		x1 += 0.125;
		y1 += 0.1;
		x1_inc = 0.075;
		y1_inc = 0.2;
                }
        else
                {
		im[0] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.175, 0.125,1, TANGO_COLOR_BLACK, 0, "HEAD", 1);
		x1 = 0.175;
		y1 = 0.15;
		x1_inc = 0.0001;
		y1_inc = 0.45;
                }

		/* Get the locations for the pointer after */
	x2 = x + 0.125;
	y2 = y + 0.1;
	x2_inc = 0.075;
	y2_inc = -0.2;

		/* Change the pointer before */
	path = TANGOpath_null(1);
	imag = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,TANGO_COLOR_BLACK,x1_inc, y1_inc,0.5,1.0,1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	TANGOtrans_perform(trans);
	sleep(1);
	trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[2],path);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
	trans = TANGOtrans_compose(2,trans,trans1);
/*
	TANGOtrans_perform(trans);
*/

		/* Gradually move the new node and the pointers */
/*
	imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",prev + 1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
*/
	xoff[0] = 0.0001;
	yoff[0] = -0.01;
	path = TANGOpath_create(1,xoff,yoff);
	imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",prev + 1);
	imag2 = (TANGO_IMAGE)ASSOCretrieve("IMAGE2",prev + 1);
	for (d = 0; d < 0.2; d += 0.01)
		{
		im[1] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,TANGO_COLOR_BLACK,x1_inc, y1_inc - d,0.5,1.0,1); 
		im[2] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x2,y2 - d,1,TANGO_COLOR_BLACK,x2_inc, y2_inc + d,0.5,1.0,1); 
		trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
		trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag2,path);
		trans3 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,new_imag,path);
		trans = TANGOtrans_compose(4, trans1, trans2, trans3, trans);
		TANGOtrans_perform(trans);

		trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[1],path);
		trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[2],path);
		trans = TANGOtrans_compose(2, trans1, trans2);
		}
	TANGOtrans_perform(trans);

		/* Make actual pointers before and after the new node */
	if (prev > 0)
		{
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",prev);
		tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_NW);
		TANGOloc_inquire(tloc,&x,&y);
		imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, x, y, 1, arrow);
		ASSOCstore("IMAGE3",prev,imag);
		imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, x+0.2, y, 1, arrow);
		ASSOCstore("IMAGE3",prev+1,imag);

		path = TANGOpath_null(1);
		trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
		}
	else
		{
		path = TANGOpath_null(1);
		trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[0],path);
		imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,  0.0001, 0.0001, 1,hhead);
		ASSOCstore("IMAGE1",0,imag);
		imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, 0.1, 0.4,1,arrow);
		ASSOCstore("IMAGE3",prev+1,imag);
		}
	TANGOtrans_perform(trans);

		/* Remove prev and new pointers */
	if (prev > 0)
		{
		trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,prev_imag,path);
		TANGOtrans_perform(trans);
		}
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,new_imag,path);
	TANGOtrans_perform(trans);

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);

	TANGOpath_free(1,path);
	TANGOtrans_free(1,trans);
	TANGOtrans_free(1,trans1);
	TANGOtrans_free(1,trans2);
	TANGOtrans_free(1,trans3);
}

void del(pres,flag)
int pres;
int flag;
{
        TANGO_PATH   path;
        TANGO_TRANS  trans;
        TANGO_TRANS  trans1, trans2, trans3;
        TANGO_IMAGE  imag, imag2, ptr_imag, prev_imag, del_imag, im[3];
	TANGO_IMAGE  text;
        TANGO_LOC    tloc;
        double       x, y, d, xoff[1], yoff[1];
        double       x1, y1, x1_inc, y1_inc, x2, y2, x2_inc, y2_inc;
        int          ptr;

	path = TANGOpath_null(1);
	if (pres > 1)
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "make prev point to the node before",1);
	else
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "the node at the head is to be deleted",1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	TANGOtrans_perform(trans);
	sleep(2);

                /* Move the prev pointer */
        if (pres > 1)
            {
	    if (flag)
		ptr = flag;
	    else
            	ptr = 1;
            path = TANGOpath_null(1);
            imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",ptr);
            tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_C);
            TANGOloc_inquire(tloc,&x,&y);
            prev_imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,x,y+0.25,1,prev_ptr);
            trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,prev_imag,path);
            TANGOtrans_perform(trans);
            path = TANGOpath_type(TANGO_PATH_TYPE_STRAIGHT);
	    ++ptr;
            while (ptr < pres)
                {
                trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,prev_imag,path);
                TANGOtrans_perform(trans);
                sleep(1);
                ++ptr;
                }
            }

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);
	text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "to_delete points to the node to be deleted",1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	TANGOtrans_perform(trans);
	sleep(1);

	path = TANGOpath_null(1);
	imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",pres);
	tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_C);
	TANGOloc_inquire(tloc,&x,&y);
	del_imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,x,y+0.3,1,del_ptr);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,del_imag,path);
        TANGOtrans_perform(trans);
	sleep(1);

                /*Get the locations for the pointer before and also delete it */
        if (pres > 1)
                {
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE3",pres-1);
		path = TANGOpath_null(1);
		trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
                imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",pres-1);
                tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_NW);
                TANGOloc_inquire(tloc,&x1,&y1);
                x1 += 0.125;
		x2 = x1 + 0.2;
                y1 += 0.1;
                x1_inc = 0.075;
                y1_inc = 0.0001;
                }
        else
                {
		imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",0);
		path = TANGOpath_null(1);
		trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
                ptr_imag = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.175, 0.125,1, TANGO_COLOR_BLACK, 0, "HEAD", 1);
                x1 = 0.175;
		x2 = 0.225;
                y1 = 0.15;
                x1_inc = 0.0001;
                y1_inc = 0.25;
                }

                /* Get the locations for the pointer after */
        y2 = 0.5;
        x2_inc = 0.075;
        y2_inc = 0.0001;

		/* Delete the pointer after */
        imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE3",pres);
        path = TANGOpath_null(1);
        trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
	trans = TANGOtrans_compose(2, trans1, trans2);
	TANGOtrans_perform(trans);
	
                /* Gradually move the new node and the pointers */
        imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",pres);
        trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
        xoff[0] = 0.0001;
        yoff[0] = 0.01;
        path = TANGOpath_create(1,xoff,yoff);
        imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",pres);
        imag2 = (TANGO_IMAGE)ASSOCretrieve("IMAGE2",pres);
        for (d = 0; d < 0.2; d += 0.01)
                {
                im[1] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x1,y1,1,TANGO_COLOR_BLACK,x1_inc, y1_inc + d,0.5,1.0,1);
                im[2] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,x2,y2 + d,1,TANGO_COLOR_BLACK,x2_inc, y2_inc - d,0.5,1.0,1);
                trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
                trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag2,path);
		trans3 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,del_imag,path);
                trans = TANGOtrans_compose(4, trans1, trans2, trans3, trans);
                TANGOtrans_perform(trans);

                trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[1],path);
                trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,im[2],path);
                trans = TANGOtrans_compose(2, trans1, trans2);
                }
	sleep(2);

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);
	if (pres > 1)
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "prev->next = to_delete->next",1);
	else
		text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "head = to_delete->next",1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	TANGOtrans_perform(trans);
	sleep(2);
        TANGOtrans_perform(trans1);

                /* Create the long pointer or alt_head */
	if (pres == 1)
		{
		path = TANGOpath_null(1);
		trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,ptr_imag,path);
		TANGOtrans_perform(trans);
		}
        if (pres > 1)
                {
                imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",pres-1);
                tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_NW);
                TANGOloc_inquire(tloc,&x,&y);
                path = TANGOpath_null(1);
                ptr_imag = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, x+0.125, y+0.1, 1, TANGO_COLOR_BLACK, 0.275, 0.0001, 0.5, 1.0, 1);
                trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,ptr_imag,path);
                TANGOtrans_perform(trans);
                }
        else
                {
                path = TANGOpath_null(1);
                ptr_imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, 0.0001,0.0001, 1, alt_hhead);
                trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,ptr_imag,path);
                TANGOtrans_perform(trans);
                }
	sleep(1);

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);
	text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.5, 0.05,1 ,TANGO_COLOR_BLACK, NULL, "remove the to_delete node",1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,text,path);
	TANGOtrans_perform(trans);
	sleep(2);

		/* Delete present node */
	path = TANGOpath_null(1);
	imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",pres);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
	imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE2",pres);
	trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
        trans = TANGOtrans_compose(3, trans1, trans2, trans);
        TANGOtrans_perform(trans);

	path = TANGOpath_null(1);
        trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,del_imag,path);
        TANGOtrans_perform(trans);
	sleep(1);

                /* Make pointers before the deleted node */
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,ptr_imag,path);
        if (pres > 1)
                {
                imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",pres-1);
                tloc = TANGOimage_loc(imag,TANGO_PART_TYPE_NW);
                TANGOloc_inquire(tloc,&x,&y);
                imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE, x, y, 1, arrow);
                ASSOCstore("IMAGE3",pres-1,imag);

                trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
                }
        else
                {
                imag = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,  0.0001, 0.0001, 1,hhead);
                ASSOCstore("IMAGE1",0,imag);
                trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
                }
	trans = TANGOtrans_compose(2, trans, trans1);
        TANGOtrans_perform(trans);


        ptr = pres + 1;
	trans = TANGOtrans_compose(0);
        path = TANGOpath_type(TANGO_PATH_TYPE_STRAIGHT);
	path = TANGOpath_reverse(path);

                /* Move the nodes */
        while (ptr < node_ptr)
                {
                imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",ptr);
                ASSOCstore("IMAGE1",ptr - 1,imag);
                trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
                imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE2",ptr);
                ASSOCstore("IMAGE2",ptr - 1,imag);
                trans2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
                imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE3",ptr);
                ASSOCstore("IMAGE3",ptr - 1,imag);
                trans3 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
                trans = TANGOtrans_compose(4, trans1, trans2, trans3, trans);
                ++ptr;
                }

                /*  Move the NULL  field  */
        imag = (TANGO_IMAGE)ASSOCretrieve("IMAGE1",ptr);
        ASSOCstore("IMAGE1",ptr - 1,imag);
        trans1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,imag,path);
	trans = TANGOtrans_compose(2, trans, trans1);

        TANGOtrans_perform(trans);
        --node_ptr;

                /* Remove prev pointer */
        if (pres > 1)
                {
		path = TANGOpath_null(1);
                trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,prev_imag,path);
                TANGOtrans_perform(trans);
                }

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,text,path);
	TANGOtrans_perform(trans);

	TANGOpath_free(1,path);
	TANGOtrans_free(1,trans);
	TANGOtrans_free(1,trans1);
	TANGOtrans_free(1,trans2);
}
