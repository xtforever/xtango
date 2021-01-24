/*   Implementation of Floyd-Warshall Algorithm */
/*    Heather Pritchett */

#define INF 10000
#include <stdio.h>
#include <math.h>
#include "xtango.h"

/*
 * In a truely interactive program, these would be allocated in the init_vert and
 * add_weights section. Currently, they are hardcoded.
 */

static struct {
	TANGO_IMAGE loc;
	double X;
	double Y;
	}vertex[5];

TANGO_IMAGE txt1,txt2;
TANGO_IMAGE edges[5][5];
TANGO_PATH paths[5][5];
int weights[5][5];

void
init_verts() {	/* begin init_verts  */

	TANGO_PATH path0,path1;
	TANGO_TRANS trans1,resize0;

/*
 * these are the locations of the vertices, they need to be changed so that they
 * can be dynamically entered. Right now they are hardcoded in, along with the labels
 */

static TANGO_IMAGE_COMPONENT tmp0[3] = {
     {TANGO_IMAGE_TYPE_ELLIPSE, "0.000000 0.000000 TANGO_COLOR_BLACK 0.050000 0.050000 0"},
     {TANGO_IMAGE_TYPE_TEXT,    "0.000000 0.000000 TANGO_COLOR_BLACK NULL 1 0"},
     {TANGO_IMAGE_TYPE_COMPOSITE, NULL} 
	}; 
static TANGO_IMAGE_COMPONENT tmp1[3] = {
     {TANGO_IMAGE_TYPE_ELLIPSE, "0.000000 0.000000 TANGO_COLOR_BLACK 0.050000 0.050000 0"},
     {TANGO_IMAGE_TYPE_TEXT,    "0.000000 0.000000 TANGO_COLOR_BLACK NULL 2 0"},
     {TANGO_IMAGE_TYPE_COMPOSITE, NULL} 
	}; 
static TANGO_IMAGE_COMPONENT tmp2[3] = {
     {TANGO_IMAGE_TYPE_ELLIPSE, "0.000000 0.000000 TANGO_COLOR_BLACK 0.050000 0.050000 0"},
     {TANGO_IMAGE_TYPE_TEXT,    "0.000000 0.000000 TANGO_COLOR_BLACK NULL 3 0"},
     {TANGO_IMAGE_TYPE_COMPOSITE, NULL} 
	}; 
static TANGO_IMAGE_COMPONENT tmp3[3] = {
     {TANGO_IMAGE_TYPE_ELLIPSE, "0.000000 0.000000 TANGO_COLOR_BLACK 0.050000 0.050000 0"},
     {TANGO_IMAGE_TYPE_TEXT,    "0.000000 0.000000 TANGO_COLOR_BLACK NULL 4 0"},
     {TANGO_IMAGE_TYPE_COMPOSITE, NULL} 
	}; 
static TANGO_IMAGE_COMPONENT tmp4[3] = {
     {TANGO_IMAGE_TYPE_ELLIPSE, "0.000000 0.000000 TANGO_COLOR_BLACK 0.050000 0.050000 0"},
     {TANGO_IMAGE_TYPE_TEXT,    "0.000000 0.000000 TANGO_COLOR_BLACK NULL 5 0"},
     {TANGO_IMAGE_TYPE_COMPOSITE, NULL} 
	}; 

vertex[0].X = 0.150000; vertex[0].Y = 0.5000000;
vertex[1].X = 0.350000; vertex[1].Y = 0.2500000;
vertex[2].X = 0.750000; vertex[2].Y = 0.2500000;
vertex[3].X = 0.750000; vertex[3].Y = 0.7500000;
vertex[4].X = 0.350000; vertex[4].Y = 0.7500000;
vertex[0].loc = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			vertex[0].X,vertex[0].Y,1,tmp0);
vertex[1].loc = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			vertex[1].X,vertex[1].Y,1,tmp1);
vertex[2].loc = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			vertex[2].X,vertex[2].Y,1,tmp2);
vertex[3].loc = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			vertex[3].X,vertex[3].Y,1,tmp3);
vertex[4].loc = TANGOimage_create(TANGO_IMAGE_TYPE_COMPOSITE,
			vertex[4].X,vertex[4].Y,1,tmp4);

TAPshow(vertex[0].loc);
TAPshow(vertex[1].loc);
TAPshow(vertex[2].loc);
TAPshow(vertex[3].loc);
TAPshow(vertex[4].loc);

txt1 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.1,0.1,1,TANGO_COLOR_GREEN,NULL,"Old Route",0);
txt2 = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,0.3,0.1,1,TANGO_COLOR_MAROON,NULL,"New Route",0);

TAPshow(txt1);
TAPshow(txt2);

	}	/* end init_verts */

void
add_weights(from,to,data) 
	int from,to,data; {	/* begin addweights  */
	

	char str[2];
	double textX,textY;
	double offsetX, offsetY,hyp;
	TANGO_IMAGE wait;
	TANGO_PATH short_path;
	TANGO_TRANS resize,move,all;

/*
 * These are the arrows and their weights. I'm not sure where to add the
 * weights to the picture, so for right now, i'll center them on the line.
 *
 */
	/* offset from vertex = length of x|y * radius of vertex / length of hyponus */
	hyp = sqrt( ((vertex[from].X - vertex[to].X) * (vertex[from].X - vertex[to].X))
                  + ((vertex[from].Y - vertex[to].Y) * (vertex[from].Y - vertex[to].Y)) );
	offsetX= (vertex[from].X - vertex[to].X) * (0.05 / hyp);
	offsetY= (vertex[from].Y - vertex[to].Y) * (0.05 / hyp);

	edges[from][to] = TANGOimage_create(TANGO_IMAGE_TYPE_LINE,
		vertex[from].X-offsetX, vertex[from].Y-offsetY,
		1,TANGO_COLOR_ORANGE,0.0,0.0,0.4,1.0,1);
/*
 * Save a copy of the path as the "base" path (soon to change)
 */
	paths[from][to] = TANGOpath_distance(TANGOimage_loc(vertex[from].loc,
					     TANGO_PART_TYPE_C),
					     TANGOimage_loc(vertex[to].loc,
					     TANGO_PART_TYPE_C),0.05);
/*
 * short_path is the vertex without the ends inside the circle.
 */
	short_path = TANGOpath_distance(TANGOloc_create(vertex[from].X-offsetX,
                                                  vertex[from].Y-offsetY),
				  TANGOloc_create(vertex[to].X+offsetX,
                                                  vertex[to].Y+offsetY), 0.05);
	resize =TANGOtrans_create(TANGO_TRANS_TYPE_RESIZE,edges[from][to],short_path);
	TANGOtrans_perform(resize);

	textX=TANGOloc_X(TANGOimage_loc(edges[from][to],TANGO_PART_TYPE_C));
	textY=TANGOloc_Y(TANGOimage_loc(edges[from][to],TANGO_PART_TYPE_C));
	str[0] = data + '0';
	str[1] = '\0';
		
	wait = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,textX,textY,1,
			TANGO_COLOR_BLUE, NULL, str, 1.0);

	TAPshow(wait);

	}	/* end addweights */

void
new_route(from,to,data,middle,newdata) 
	int from,to,data,middle,newdata; {	/* begin new_route  */

	TANGO_IMAGE dot1,dot2;
	TANGO_PATH path,newpath,rpath;
	TANGO_TRANS move1,move2;
/*
 * Make two dots in the same location on the "from" vertes
 */

	printf("we are trying to go from %d to %d, is %d shorter?\n",from,to,middle);
	printf(" old = %d, new = %d \n",data,newdata);

	dot1 = TANGOimage_create(TANGO_IMAGE_TYPE_ELLIPSE,vertex[from].X,
		vertex[from].Y,1,TANGO_COLOR_GREEN,
		0.02,0.02,1.0);
	dot2 = TANGOimage_create(TANGO_IMAGE_TYPE_ELLIPSE,vertex[from].X,
		vertex[from].Y,1,TANGO_COLOR_MAROON,
		0.02,0.02,1.0);
/*
 * make two paths, one to the "to" vertex and one through the "middle" vertex
 */
	if (data == INF)
		path = TANGOpath_null(0);
/*
 * sometimes there might not be a current path, so it's null
 */
	else
		path = TANGOpath_copy(paths[from][to]);

	newpath = TANGOpath_concatenate(2,paths[from][middle],paths[middle][to]);

	move1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,dot1,path);
	move2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,dot2,newpath);

/*
 * draw the first choice path.
 */
	TANGOtrans_perform(move1);
	TAPflash(dot1,5);
/*
 * draw the second choice path with the new middle vertex
 */
	TANGOtrans_perform(move2);
	TAPflash(dot2,55);

	if (newdata < data) {
/*
 * blow away the old one, save the new one
 */
		printf("	The new route is shorter! %d < %d\n",newdata,data);
		TAPshow(dot1);
		TAPcolor(dot2,TANGO_COLOR_GREEN);
		TAPflash(dot2,20);
		rpath = TANGOpath_reverse(newpath);
		move2 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,
                        dot2,rpath);
        	TANGOtrans_perform(move2);
		paths[from][to] = TANGOpath_copy(newpath);
		TAPshow(dot2);
		}
	else {
/*
 * blow away the new one and leave the old one alone
 */
		printf("	The new route saves us no time. %d >= %d\n",newdata,data);
		TAPshow(dot2);
		TAPflash(dot1,20);
		rpath = TANGOpath_reverse(path);
		move1 = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,
                        dot1,rpath);
                TANGOtrans_perform(move1);
		TAPshow(dot1);
                }

	}   /* end new_route */


void
travel(from,to) 
	int from,to; {	/* begin travel  */


	}   /* end travel */
