/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (sup.h)	      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*			John T. Stasko			      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/10/26 JDH	      Baselined source.                       */
/*							      */
/**************************************************************/

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

typedef enum
{
   LINE,
   RECT,
   FILLRECT,
   CIRCLE,
   FILLCIRCLE,
   ELLIPSE,
   FILLELLIPSE,
   POLYLINE,
   POLYGON,
   FILLPOLYGON,
   SPLINE,
   TEXT,
   BMAP
} ITYPE;

typedef struct	ERASER
{
   ITYPE	  itype;
   TANGO_IMAGE	  image;
/*-----------------------*/
   int thick;
/*-----------------------*/
   int		  x0;
   int		  y0;
   int		  x1;
   int		  y1;
   char 	  text[STRINGLENGTH];
   int		  fontid;
   int		  sides;
   int		  xs[8];
   int		  ys[8];
   int		  arrow;
   int		  arrowx[4];
   int		  arrowy[4];
   struct ERASER *next;
} *ERASER_PTR;

/**************************************************************/
/*****************    end of xtangosup.h     ******************/
/**************************************************************/
