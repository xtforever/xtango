/* CS 6490 &&& */

/*****************************************************************************
/* Polygon Scan Conversion
/* Written by Steven A. Cover
/* October 14, 1991
/****************************************************************************/

/*****************************************************************************/
/*
/* FUNCTION
/* 	definitions
/*
/* DESCRIPTION
/*	Includes all of the #include and #define and type declarations necessary
/*		for the polygon scan conversion.`
/*
/* DATE LAST MODIFIED
/*	October 10, 1991
/*
/*******************************************************************sac*******/

#include <stdio.h>
#include "xtango.h"
#include <math.h>


void ANIMInit(), ANIMPoint(), ANIMAel(), ANIMHighlight(), ANIMIntercept(), ANIMProcess(), ANIMNewAel(); 

static NAME_FUNCT fnc[] = { {"Init",     1, {VOID, (FPTR)ANIMInit}},
			    {"Point",      1, {VOID, (FPTR)ANIMPoint}},
			    {"Ael",      1,{VOID, (FPTR)ANIMAel}},
			    {"Highlight",      1,{VOID, (FPTR)ANIMHighlight}},
			    {"Intercept",      1,{VOID, (FPTR)ANIMIntercept}},
			    {"Process",      1,{VOID, (FPTR)ANIMProcess}},
			    {"NewAel",      1,{VOID, (FPTR)ANIMNewAel}},
			    {NULL,	 0, NULL, NULL} };


#define MAXEDGES 40              /* the maximum number of allowed edges */
#define IMAGESIZE 50.0

typedef struct
{
	int color;
	int priority;
} displayrecord;                /* contains the info necessary for display*/

typedef struct
{
	int priority;
	int color;
	int numpts;
	double ptsarray[2][40];
}polyrecord,*polyptr;           /* contains the polygon info read from file*/

typedef struct record
{
	double x1;
	double y1;
	double x2;
	double y2;
	double xinter;          /* x-intercept for the edge */
	int edgenum;
	struct record *link;
}edgerecord,*edgeptr;           /* contains the edge pointss and intercept */

edgerecord edges[40];           /* global declaration of the edge list */

edgerecord ael[40];             /* global declaration of the active edge list */

/****************************************************************************/
/* 
/* FUNCTION
/*	readpoly
/*
/* DESCRIPTION
/*	Readpoly reads in the polygon information from a file and places it into
/*		a polyrecord. The minimum and maximum y values are also calcu-
/*		lated and returned.  This function returns a 1 or a 0 depending
/*		on whether EOF has been reached.
/*
/* DATE LAST MODIFIED
/*	October 10, 1991
/*
/******************************************************************sac*******/

int readpoly(fp,newpoly,maxy,miny)
FILE *fp;                         /* pointer to the "polygons" file  */
polyrecord *newpoly;              /* pointer to return the polygon info  */
double *maxy;                     /* returns the maximum y value     */
double *miny;                     /* returns the minimum y value     */
{
	int i;                    /* loop counter                    */
	int priority;             /* reads in the priority info      */
	int color;                /* reads in the color info         */
	int numpts;               /* reads in the number of points   */
	double point;             /* reads in each x and y point     */


	fscanf(fp,"%d%*c",&priority);
	if(feof(fp))
		return(1);
	newpoly->priority = priority;
	fscanf(fp,"%d%*c",&color);
	newpoly->color = color;
	fscanf(fp,"%d%*c",&numpts);
	newpoly->numpts = numpts;
	for(i=0; i<newpoly->numpts; i++)
	{
		fscanf(fp,"%lf%*c",&point);
		newpoly->ptsarray[0][i] =  point;
		fscanf(fp,"%lf%*c",&point);
		newpoly->ptsarray[1][i] =  point;
		if(point > *maxy)
			*maxy =  point;
		if(point < *miny)
			*miny =  point;
   		TANGOalgoOp(fnc, "Point",i,newpoly->ptsarray[0][i],newpoly->ptsarray[1][i]);
	}
	if(feof(fp))
		return(1);

	else
		return(0);
}

/****************************************************************************/
/*
/* FUNCTION
/*	initael
/*
/* DESCRIPTION
/*	Initael initializes the values in the global active edge list.
/*
/* DATE LAST MODIFIED
/*	October 10, 1991
/*
/*****************************************************************sac*******/

void initael()
{
	int i;                         /* loop counter  */

	for(i=0; i<MAXEDGES; i++)
	{
		ael[i].x1 = 0.0;
		ael[i].y1 = 0.0;
		ael[i].x2 = 0.0;
		ael[i].y2 = 0.0;
		ael[i].xinter = -1.0;
	}
}

/***************************************************************************/
/*
/* FUNCTION
/*	removedge
/*
/* DESCRIPTION
/*	Removes an edge indicated by index from the active edge list.
/*
/* DATE LAST MODIFIED 
/*	October 10, 1991
/*
/******************************************************************sac******/ 

removedge(index,numedges) 
int index;                                  /* index of edge to be removed */ 
int numedges;                               /* number of edges in ael      */
{
	int i;                              /* loop counter */

	for(i=index; i<(numedges-1); i++)
	{
		ael[i].x1 = ael[i+1].x1;
		ael[i].y1 = ael[i+1].y1;
		ael[i].x2 = ael[i+1].x2;
		ael[i].y2 = ael[i+1].y2;
		ael[i].xinter = ael[i+1].xinter;
		ael[i].edgenum = ael[i+1].edgenum;
	}
	ael[i].x1 = 0.0;
	ael[i].y1 = 0.0;
	ael[i].x2 = 0.0;
	ael[i].y2 = 0.0;
	ael[i].xinter = -1.0;
	ael[i].edgenum = 0;
}

/***************************************************************************
/*
/* FUNCTION
/*	removehor
/* DESCRIPTION
/*	Given an index and the number of total edges, removehor removes
/*		the edge positioned at index.  It is assumed that this
/*		edge is horizontal.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

void removehor(index,numedges)
int index;				/* the edge to be removed */
int numedges;				/* the number of edges in edges */
{
	int i;				/* loop variable */

	for(i=index; i<(numedges-1); i++)
	{
		edges[i].x1 = edges[i+1].x1;
		edges[i].y1 = edges[i+1].y1;
		edges[i].x2 = edges[i+1].x2;
		edges[i].y2 = edges[i+1].y2;
		edges[i].xinter = edges[i+1].xinter;
		edges[i].edgenum = edges[i+1].edgenum;
	}
	edges[i].x1 = 0.0;
	edges[i].y1 = 0.0;
	edges[i].x2 = 0.0;
	edges[i].y2 = 0.0;
	edges[i].xinter = -1.0;
	edges[i].edgenum = 0;
}


/***************************************************************************
/*
/* FUNCTION
/*	addael()
/* DESCRIPTION
/*	Given the location of the next edge in the ael, addael adds the 
/*		values in edges pointed to by currentedge.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/
void addael(aelcount,currentedge)
int aelcount;			/* the number of edges in the ael */
int currentedge;		/* the current edge in edges      */
{
	ael[aelcount].x1 = edges[currentedge].x1;
	ael[aelcount].y1 = edges[currentedge].y1;
	ael[aelcount].x2 = edges[currentedge].x2;
	ael[aelcount].y2 = edges[currentedge].y2;
	ael[aelcount].edgenum = edges[currentedge].edgenum;
}



/***************************************************************************
/* FUNCTION
/*	switchedges()
/* DESCRIPTION
/*	Switchedges reverses the edges located in value1 and value2.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

void switchedges(value1,value2)
int value1;
int value2;
{
	edgerecord temp;		/* a temporary edge record */

	temp.x1 = edges[value1].x1;
	temp.y1 = edges[value1].y1;
	temp.x2 = edges[value1].x2;
	temp.y2 = edges[value1].y2;
	temp.edgenum = edges[value1].edgenum;

	edges[value1].x1 = edges[value2].x1;
	edges[value1].y1 = edges[value2].y1;
	edges[value1].x2 = edges[value2].x2;
	edges[value1].y2 = edges[value2].y2;
	edges[value1].edgenum = edges[value2].edgenum;

	edges[value2].x1 = temp.x1;
	edges[value2].y1 = temp.y1;
	edges[value2].x2 = temp.x2;
	edges[value2].y2 = temp.y2;
	edges[value2].edgenum = temp.edgenum;

}

/***************************************************************************
/* FUNCTION
/*	switchael()
/* DESCRIPTION
/*	Switchael reverses the values pointed to by value1 and value2 
/*		in ael.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/
void switchael(value1,value2)
int value1;
int value2;
{
	edgerecord temp;		/* temporary edge record */

	temp.x1 = ael[value1].x1;
	temp.y1 = ael[value1].y1;
	temp.x2 = ael[value1].x2;
	temp.y2 = ael[value1].y2;
	temp.xinter = ael[value1].xinter;
	temp.edgenum = ael[value1].edgenum;

	ael[value1].x1 = ael[value2].x1;
	ael[value1].y1 = ael[value2].y1;
	ael[value1].x2 = ael[value2].x2;
	ael[value1].y2 = ael[value2].y2;
	ael[value1].xinter = ael[value2].xinter;
	ael[value1].edgenum = ael[value2].edgenum;

	ael[value2].x1 = temp.x1;
	ael[value2].y1 = temp.y1;
	ael[value2].x2 = temp.x2;
	ael[value2].y2 = temp.y2;
	ael[value2].xinter = temp.xinter;
	ael[value2].edgenum = temp.edgenum;

}

/***************************************************************************
/* FUNCTION
/*	getmaxy()
/* DESCRIPTION
/*	Determines which of the 2 values is greater and returns it.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

double getmaxy(val1,val2)
double val1,val2;
{
	if(val1 < val2)
		return(val2);
	else
		return(val1);
}

/***************************************************************************
/* FUNCTION
/*	getminy()
/* DESCRIPTION
/*	Returns the minimum value of val1 & valu2.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

double getminy(val1,val2)
double val1,val2;
{
	if(val1 > val2)
		return(val2);
	else
		return(val1);
}

/***************************************************************************
/* FUNCTION
/*	quicksort()
/* DESCRIPTION
/*	Performs a quicksort of the values in the edge list by maxy.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

void quicksort(first,finish)
int first;			/* the first location in edges */
int finish;			/* the last location in edges  */
{
	int left,right;		/* the left & right reference variables */
	int mid;		/* the middle reference variable */
	double leftmaxy,rightmaxy,midmaxy; /* the max values */


	left = first;
	right = finish;
	mid = (int)((first + finish)/2);
	midmaxy = getmaxy(edges[mid].y1,edges[mid].y2);
	do
	{
		leftmaxy = getmaxy(edges[left].y1,edges[left].y2);
		rightmaxy = getmaxy(edges[right].y1,edges[right].y2);

		while(leftmaxy > midmaxy){
			left++;
			leftmaxy = getmaxy(edges[left].y1,edges[left].y2);
		}

		while(rightmaxy < midmaxy){
			right--;
			rightmaxy = getmaxy(edges[right].y1,edges[right].y2);
		}

		if(left <= right)
		{
			switchedges(left,right);
			left++;
			right--;
		}
	}while(right > left);
	if(first < right)
		quicksort(first,right);
	if(left < finish)
		quicksort(left,finish);
}

/***************************************************************************
/* FUNCTION
/*	aelsort()
/* DESCRIPTION
/*	Performs a quicksort on the active edge list by the x-intercepts..
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

void aelsort(first,finish)
int first;			/* the first index position */
int finish;			/* the last index position  */
{
	int left,right;		/* the left & right index values */
	int mid;		/* the middle index values */
	double leftxint,rightxint,midxint;


	left = first;
	right = finish;
	mid = (int)((first + finish)/2);
	midxint = ael[mid].xinter;
	do
	{
		leftxint = ael[left].xinter;
		rightxint = ael[right].xinter;
		while(leftxint < midxint){
			left++;
			leftxint = ael[left].xinter;
		}

		while(rightxint > midxint){
			right--;
			rightxint = ael[right].xinter;
		}

		if(left <= right)
		{
			switchael(left,right);
			left++;
			right--;
		}
	}while(right > left);
	if(first < right)
		aelsort(first,right);
	if(left < finish)
		aelsort(left,finish);

}


/***************************************************************************
/* FUNCTION
/*	aelslope()
/* DESCRIPTION
/*	Calculates the slope of the edge in ael pointed to by index.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/
double aelslope(index)
int index;
{
	double xdiff;		/* the difference in the x direction */
	double ydiff;		/* the difference in the y direction */
	double m;		/* the slope                         */

	xdiff = ael[index].x2 - ael[index].x1;
	ydiff = ael[index].y2 - ael[index].y1;
	if(xdiff == 0)
		return(-999.0);
	m = ydiff / xdiff;
	return(m);
}


/***************************************************************************
/* FUNCTION
/*	slope()
/* DESCRIPTION
/*	Calculates the slope of the edge in edges pointed to by index.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

double slope(index)
int index;
{
	double xdiff;		/* the difference in the x direction */
	double ydiff;		/* the difference in the y direction */
	double m;		/* the slope                         */

	xdiff = edges[index].x2 - edges[index].x1;
	ydiff = edges[index].y2 - edges[index].y1;
	if(xdiff == 0)
		return(-999.0);
	m = ydiff / xdiff;
	return(m);
}

/***************************************************************************
/* FUNCTION
/*	xintercept()
/* DESCRIPTION
/*	Given a point, the slope and the y-value of another point,it 
/*		returns the x-intercept.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/
double xintercept(m,x2,y2,y1)
double m;
double x2;
double y2;
double y1;

{
	double x1;
	
	x1 = (y1 - y2)/m + x2;
	if(x1 <= 0.0)
		x1 = 0.0;
	return(x1);
}

/***************************************************************************
/* FUNCTION
/*	prepreprocess()
/* DESCRIPTION
/*	Removes all of the horizontal edges in the polygon.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

void prepreprocess(polygon)
polyrecord *polygon;
{
	int i;			/* loop variable */
	double m;		/* the slope of the current edge */
	double mod2;		/* indicates the vertex is on a scanline */
	int edgcount;		/* the number of edges           */
	int removed;		/* indicates an edge was removed */

	for(i=0;i<(polygon->numpts - 1); i++)
	{
		edges[i].x1 = polygon->ptsarray[0][i];
		edges[i].y1 = polygon->ptsarray[1][i];
		edges[i].x2 = polygon->ptsarray[0][i+1];
		edges[i].y2 = polygon->ptsarray[1][i+1];
		edges[i].edgenum = i + 1;
	}
	edges[i].x1 = polygon->ptsarray[0][i];
	edges[i].y1 = polygon->ptsarray[1][i];
	edges[i].x2 = polygon->ptsarray[0][0];
	edges[i].y2 = polygon->ptsarray[1][0];
	edges[i].edgenum = i + 1;
	edgcount = polygon->numpts;
	i = 0;
	while(i < edgcount)
	{
		removed = 0;
		m = slope(i);
		mod2 = edges[i].y2 - (int)(edges[i].y2);
		if(mod2 == 0.0)
		{
			if(m == 0)
			{
				removehor(i,edgcount);
				edgcount--;
				removed = 1;
			}
		}
		if(removed != 1)
			i++;
	}
	polygon->numpts = edgcount;
}

/***************************************************************************
/* FUNCTION
/*	preprocess()
/* DESCRIPTION
/*	Preprocesses the polygons edges so that all minimums and maximums
/*		are left alone and so all monotonically increasing and
/*		decreasing lines are altered.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

void preprocess(polygon)
polyrecord *polygon;
{
	int i;		/* loop variable */
	double m;	/* slope of current edge */
	double mod2;	/* slope of the next edge */
	int edgcount;	/* the number of edges  */
	int min,max;	/* indicate if edges form a min or a max */
	int y1greater;	/* next four vars used to calculate mins and maxs */
	int y2greater;
	int y1smaller;
	int y2smaller;
	double y1next;	/* next two provide the y-values of the next edge */
	double y2next;
	double xold,yold;

	edgcount = polygon->numpts;
	i = 0;
	while(i < edgcount)
	{
		m = slope(i);
		y1greater = 0;
		y2greater = 0;
		y1smaller = 0;
		y2smaller = 0;
		min = 0;
		max = 0;

		if(i != (edgcount - 1))
		{
			y1next = edges[i+1].y1;
			y2next = edges[i+1].y2;
		}
		else
		{
			y1next= edges[0].y1;
			y2next = edges[0].y2;
		}


		if(y1next < edges[i].y1)
			y1greater = 1;
		if(y1next < y2next)
			y2greater = 1;
		if(y1next > edges[i].y1)
			y1smaller = 1;
		if(y1next > y2next)
			y2smaller = 1;

		if((y1greater) && (y2greater))
			min = 1;
		if((y1smaller) && (y2smaller))
			max = 1;

		mod2 = edges[i].y2 - (int)(edges[i].y2);
		if(mod2 == 0.0)
		{
			if(min == 1)
			{
				;
			}
			else if(max == 1)
			{
				;
			}
			else if(edges[i].y1 > edges[i].y2)
			{
				xold = edges[i].x2;
                                yold = edges[i].y2;
                                edges[i].y2 = edges[i].y2 + 1;
				edges[i].x2 = xintercept(m,edges[i].x1,edges[i].y1,edges[i].y2);
   				TANGOalgoOp(fnc, "Process",edges[i].x2,edges[i].y2,xold,yold);
			}

			else if(edges[i].y1 < edges[i].y2)
			{
				xold = edges[i].x2;
                                yold = edges[i].y2;
				edges[i].y2 = edges[i].y2 - 1;
				edges[i].x2 = xintercept(m,edges[i].x1,edges[i].y1,edges[i].y2);
   				TANGOalgoOp(fnc, "Process",edges[i].x2,edges[i].y2,xold,yold);

			}
		}
		i++;
	}
	polygon->numpts = edgcount;
}


/***************************************************************************
/* FUNCTION
/*	main()
/* DESCRIPTION
/*	Calls the necessary functions and aids in the polygon scan 
/*		conversion.
/* DATE LAST MODIFIED
/*	October 10, 1991
/******************************************************************sac*****/

main()
{

	int readpoly();
	void addael();

	FILE *fp;			/* file ptr to "polygons" */
	int fptest;		/* checks to see if EOF */
	displayrecord display[300][300]; /* display buffer */
	polyrecord newpoly;	/* holds current polygon values */
	int i,j;		/* loop variables */
	double maxy = 0.0;	/* max y for the polygon */
	int scanliney;		/* the max scanline */
	double miny = 0.0;	/* the min y for the polygon */
	int scanmin;		/* the minimum scan line */
	int sline;		/* the current scan line */
	int currentedge;	/* the current edge counter */
	double edgemaxy;	/* the max y for an edge */
	int aelcount;		/* the number of edges in the ael */
	double edgemin;		/* the minimum edge value */
	double m;		/* the slope of the current edge */
	int x1,x2;		/* the x values for drawing from and to */
	int count = 0;		/* loop counter for display buffer */
	double xval,yval,yval2;       /* TANGO variables */
int input;
int color;
int numpts;
double x,y;


   TANGOalgoOp(fnc, "BEGIN");
   TANGOalgoOp(fnc, "Init");

printf("Would you like to input the coordinates or read in the file 'polyscan.in'?\n");
printf("Enter 1 for user input and 0 for file read-in\n");
scanf("%d",&input);
printf("\n");

if(input == 0)
{
	fptest = 0;
	fp = fopen("polyscan.in","r");
	for(i=0; i<300; i++)
		for(j=0; j<300; j++)
		{
			display[i][j].color = 0;
			display[i][j].priority = -1;
		}
	fptest = readpoly(fp,&newpoly,&maxy,&miny);
}
else
{
fptest = 0;
printf("You have chosen to enter the coordinates. \n");

printf("You may choose a color for the polygon as a value between 1 and 7 as \n");
printf("    indicated:  1. Black \n");
printf("                2. Red   \n");
printf("                3. Orange   \n");
printf("                4. Yellow   \n");
printf("                5. Green   \n");
printf("                6. Blue   \n");
printf("                7. Maroon   \n \n");
printf("What would you like the color to be: (choose an number between 1 and 7)  ");
 
scanf("%d",&color);
printf("\n");
if((color < 1) || (color > 8))
{
printf("YOU HAVE ENTERED AN ILLEGAL COLOR \n");
exit(1);
}

printf("How many points will the polygon have?  You must enter a minimum of 3:  ");
scanf("%d",&numpts);
printf("\n");
if((numpts < 3) || (numpts > MAXEDGES))
{
	printf("YOU HAVE ENTERED AN ILLEGAL NUMBER OF POINTS \n");
	exit(1);
}

printf("In order for the program to operate correctly you must choose your points \n");
printf("    in a counter-clockwise manner.\n");
printf("Please enter the points now by clicking in the window at the location of \n");
printf("    each point \n");


miny = 0.0;
maxy = 0.0;
newpoly.priority = 1;
newpoly.color = color;
newpoly.numpts = numpts;
for(i=0; i<numpts; i++)
{

		TANGOinput_coord(&x,&y);
		x = floor((x * IMAGESIZE) / 0.6 -0.03);
		y = floor(0.0 - ((y - 0.97) / 0.7) * IMAGESIZE);
		if(x > 50.0)
		  x = 50.0;
		if(x < 0.0)
		  x = 0.0;
		if(y > 50.0)
		  y = 50.0;
		if(y < 0.0)
		  y = 0.0;
		
		newpoly.ptsarray[0][i] =  x;
		newpoly.ptsarray[1][i] =  y;
		if(y > maxy)
			maxy =  y;
		if(y < miny)
			miny =  y;
   		TANGOalgoOp(fnc, "Point",i,newpoly.ptsarray[0][i],newpoly.ptsarray[1][i]);


}
}


	while(fptest == 0)
	{
		scanliney = (int)(maxy);
		scanmin = (int)(miny);
		prepreprocess(&newpoly);
		preprocess(&newpoly);
		quicksort(0,(newpoly.numpts - 1));

		sline = scanliney;
		

		for(sline=scanliney; sline>scanmin; sline--)
		{
			currentedge = 0;
			initael();
				
   			TANGOalgoOp(fnc, "NewAel");
			
			edgemaxy = getmaxy(edges[currentedge].y1,edges[currentedge].y2);
			aelcount = 0;
			while((edgemaxy >= sline) && (currentedge != newpoly.numpts))
			{
				addael(aelcount,currentedge);
				aelcount++;
				currentedge++;
				edgemaxy = getmaxy(edges[currentedge].y1,edges[currentedge].y2);
			}

			i = 0;
			while(i < aelcount)
			{
				edgemin = getminy(ael[i].y1,ael[i].y2);
				if(edgemin > sline)
				{
					removedge(i,aelcount);
					aelcount--;
				}

				else
				{
					m = aelslope(i);
					if((m == 0.0) || (m == -999.0))
						ael[i].xinter = ael[i].x1;
					else 
						ael[i].xinter = xintercept(m,ael[i].x2,ael[i].y2,(double)sline);
					i++;
				}

			}
			edgemin = getminy(ael[i-1].y1,ael[i-1].y2);
			if(edgemin > sline)
			{
				removedge(i-1,aelcount);
			}

			else
			{
				m = aelslope(i);
				if((m == 0.0) || (m == -999.0))
					ael[i].xinter = ael[i].x1;
				else
					ael[i].xinter = xintercept(m,ael[i].x2,ael[i].y2,(double)sline);
			}

			if(aelcount != 0)
			{
				aelsort(0,aelcount-1);
			
				xval = 0.85;
				yval = 0.15;
				for(i=0;i<aelcount; i++)
				{
					TANGOalgoOp(fnc,"Ael",ael[i].edgenum,xval,yval);
					yval = yval + 0.10;
				}
		
				for(i=0;i<aelcount;i++)
					count = 0;
				yval = 0.15;
				while(count < aelcount-1)
				{
					x1 = (int)(ael[count].xinter);
					x2 = (int)(ael[count + 1].xinter);
					if(yval != 0.15)
					{
					yval2 = yval - 0.10;
					TANGOalgoOp(fnc,"Ael",ael[count - 1].edgenum,xval,yval2);
					yval2 = yval - 0.20;
					TANGOalgoOp(fnc,"Ael",ael[count - 2].edgenum,xval,yval2);
					}
					TANGOalgoOp(fnc,"Highlight",ael[count].edgenum,xval,yval);
					yval = yval + 0.10;
					TANGOalgoOp(fnc,"Highlight",ael[count + 1].edgenum,xval,yval);
					yval = yval + 0.10;
					TANGOalgoOp(fnc,"Intercept",x1,x2,sline,newpoly.color,newpoly.priority,display);
					for(i=x1; i<=x2; i++)
					{
						if(newpoly.priority > display[i][sline].priority)
						{
							display[i][sline].color = newpoly.color;
							display[i][sline].priority = newpoly.priority;
						}
					}
					count = count + 2;
				}
			}
		}
		if(input == 1)
			fptest = 1;
		else
			fptest = readpoly(fp,&newpoly,&maxy,&miny);
	}
   
TANGOalgoOp(fnc, "END");

/*
	SRGP_begin("steve",300,300,0);
	for(i=0;i<300;i++)
		for(j=0;j<300;j++)
		{
			SRGP_setColor(display[i][j].color);
			SRGP_pointCoord(i,j);
		}
	SRGP_end();
*/
}
