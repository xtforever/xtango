/***********************************************
*  Animation Package for Allocating resources  *
*                David Carlson                 *
*                                              *
*              allocatorscenes.c               *
***********************************************/


#include <stdio.h>
#include "xtango.h"

static TANGO_IMAGE_COMPONENT arrow[] =
       {{TANGO_IMAGE_TYPE_LINE,"0.0 0.0 TANGO_COLOR_BLACK 0.0 .001 0.5 1.0 1"},
	{TANGO_IMAGE_TYPE_COMPOSITE, NULL}};

struct arrows{
	TANGO_IMAGE AllArrow[5][5];
	int granted[5];};

static struct Processes{
	TANGO_IMAGE Image;
	TANGO_IMAGE ReqArrow;
	struct arrows Allocated;
	}P_Array[5];

static struct Resources{
	TANGO_IMAGE Image;
	}R_Array[5];

void init(Proc,Res)
int Proc;
int Res;
{
	int i,j;

	TANGO_PATH   path;
	TANGO_TRANS  trans;
	TANGO_IMAGE  imag;

	for(i=0;i<Proc;i++)
	{
		P_Array[i].Image=TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 
			(0.2*i)+0.01, 0.01, 1,TANGO_COLOR_BLACK, 0.1, 0.1, 0);
		for(j=0;j<5;j++)
		{
			P_Array[i].Allocated.granted[j]=0;
		}
	}
	for(i=0;i<Res;i++)
	{
		R_Array[i].Image=TANGOimage_create(TANGO_IMAGE_TYPE_CIRCLE, 
			(0.2*i)+0.06, 0.31, 1,TANGO_COLOR_BLACK, 0.05, 0);}

	path = TANGOpath_null(1);
	trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
	TANGOtrans_perform(trans);
	TANGOpath_free(1,path);
	TANGOtrans_free(1,trans);
}

void request(Proc,Res)
int Proc;
int Res;
{
	TANGO_LOC   Start, Finish;
	TANGO_PATH  path;
	TANGO_TRANS trans;
	
	Start = TANGOloc_create(0,0);
	Finish = TANGOloc_create((Res-Proc)*0.2,0.15);
	
	P_Array[Proc].ReqArrow=TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
		(0.2*Proc)+.06, 0.11, 1, arrow);
	path = TANGOpath_distance(Start, Finish, 0.01);
	trans=TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,P_Array[Proc].ReqArrow,
		path);
	TANGOtrans_perform(trans);
	TANGOpath_free(1,path);
	TANGOtrans_free(1,trans);
}

void allocate(Proc,Res)
int Proc;
int Res;
{
	int         current;
	double      temp;
	TANGO_LOC   Start, Finish;
	TANGO_IMAGE imag;
	TANGO_PATH  path;
	TANGO_TRANS trans, transa, transb, transc;
	
	current = P_Array[Proc].Allocated.granted[Res];
	if(current >= 5)
	{
		temp = 0.08;
		imag = P_Array[Proc].Allocated.AllArrow[Res][4];
		current = 5;
	}
	else
	{
		temp = current * 0.02;
		imag = NULL;
	}
	temp+=(((Proc-Res)*0.2) - 0.04);
	P_Array[Proc].Allocated.granted[Res]++;

	path = TANGOpath_null(1);
	transa=TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,
		P_Array[Proc].ReqArrow,path);

	Start = TANGOloc_create(0,0);
	Finish = TANGOloc_create(temp,-0.15);
	P_Array[Proc].Allocated.AllArrow[Res][current] = 
		TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
		(0.2*Res)+.06, 0.26, 1, arrow);
	path = TANGOpath_distance(Start, Finish, 0.01);
	transb=TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,
		P_Array[Proc].Allocated.AllArrow[Res][current],path); 

	path = TANGOpath_null(1);
	if(imag == NULL)
		transc=TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
	else
		transc=TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);

	trans =TANGOtrans_compose(3,transb,transa,transc);
	TANGOtrans_perform(trans);

	TANGOpath_free(1,path);
	TANGOtrans_free(1,transa);
	TANGOtrans_free(1,transb);
	TANGOtrans_free(1,transc);
	TANGOtrans_free(1,trans);
}

void release(Proc,Res)
int Proc;
int Res;
{
	int current;
	TANGO_IMAGE imag;
	TANGO_PATH  path;
	TANGO_TRANS trans;

	P_Array[Proc].Allocated.granted[Res]--;
	current = P_Array[Proc].Allocated.granted[Res];

	if(current<5)
	{
		imag = P_Array[Proc].Allocated.AllArrow[Res][current];
		path = TANGOpath_null(1);
		trans=TANGOtrans_create(TANGO_TRANS_TYPE_DELETE,imag,path);
		
		TANGOtrans_perform(trans);   
		TANGOpath_free(1,path);
		TANGOtrans_free(1,trans);
	}
}

void block(Proc)
int Proc;
{
	double      xoff[1],yoff[1];
	TANGO_PATH  path;
	TANGO_TRANS trans;
	
	xoff[0] = 1;
	path = TANGOpath_create(1,xoff,yoff);
	trans=TANGOtrans_create(TANGO_TRANS_TYPE_FILL,P_Array[Proc].Image,path);
	TANGOtrans_perform(trans);
	TANGOpath_free(1,path);
	TANGOtrans_free(1,trans);
}

void unblock(Proc)
int Proc;
{
	double      xoff[1],yoff[1];
	TANGO_PATH  path;
	TANGO_TRANS trans;

	xoff[0]= -1;
	path = TANGOpath_create(1,xoff,yoff);
	trans=TANGOtrans_create(TANGO_TRANS_TYPE_FILL,P_Array[Proc].Image,path);
	TANGOtrans_perform(trans);
	TANGOpath_free(1,path);
	TANGOtrans_free(1,trans);
}
