/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (image)	      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*		John T. Stasko and Doug Hayes		      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/10/26 JDH	      Baselined source.                       */
/* 90/11/25 JDH       Add bitmap image object.                */
/*							      */
/**************************************************************/

/**************************************************************/
/*****************	 include files       ******************/ 
/**************************************************************/

#include "xtangolocal.h"

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

//TANGO_IMAGE	 image_create();
TANGO_IMAGE image_create2( TANGO_IMAGE image, int type);
TANGO_IMAGE image_copy (TANGO_IMAGE image);

void		 compute_arrow_off();
void		 arrow_poly_direction();
TANGO_IMAGE	 read_composite();
void             copy_composite();
int		 decode_trans();
int		 inquire_pathcolor();
int		 get_color();
int		 get_fillstyle();
TANGO_LINE_STYLE get_linestyle();

void	  line_create(), line_draw(), line_trans(), line_bb();
TANGO_LOC line_loc();

void	  rectangle_create(), rectangle_draw(), rectangle_trans(),
	  rectangle_bb();
TANGO_LOC rectangle_loc();

void	  circle_create(), circle_draw(), circle_trans(), circle_bb();
TANGO_LOC circle_loc();

void	  text_create(), text_draw(), text_trans(), text_bb();
TANGO_LOC text_loc();

void	  ellipse_create(), ellipse_draw(), ellipse_trans(), ellipse_bb();
TANGO_LOC ellipse_loc();

void	  polyline_create(), polyline_draw(), polyline_trans(), polyline_bb();
TANGO_LOC polyline_loc();

void	  polygon_create(), polygon_draw(), polygon_trans(), polygon_bb();
TANGO_LOC polygon_loc();

void	  spline_create(), spline_draw(), spline_trans(), spline_bb();
TANGO_LOC spline_loc();

void      bmap_create(), bmap_copy(), bmap_draw(), bmap_trans(), bmap_bb();
TANGO_LOC bmap_loc();

void	  composite_create(), composite_draw(), composite_trans(),
	  composite_bb();
TANGO_LOC composite_loc();

/**************************************************************/
/*****************	  entry points       ******************/
/**************************************************************/

TANGO_IMAGE TANGOimage_create();
TANGO_IMAGE TANGOimage_copy();
TANGO_LOC   TANGOimage_loc();


/**************************************************************/
/* TANGOimage_create -- Create an TANGO_IMAGE with the given  */
/*			starting parameters and return it to  */
/*		        the user.			      */
/* 							      */
/* RETURNS:  Created image.				      */
/**************************************************************/
/*
  FIXME:  broken image_create is used
 */
TANGO_IMAGE
TANGOimage_create(TANGO_IMAGE image) 
{
  TANGO_IMAGE	   new_image;
  IMAGE_PTR	   im;

  new_image=image_create2(image,image->type);

  /* add this image onto the configuration draw list */
  im = (IMAGE_PTR) malloc( sizeof( struct IMAGE) );
  im->image = new_image;
  im->previ = NULL;
  im->nexti = TANGO__data->confighead;

  if (!TANGO__data->confighead)
    TANGO__data->configtail = im;
  else
    TANGO__data->confighead->previ = im;

  TANGO__data->confighead = im;
  new_image->alive = 1;
  new_image->inconfig = im;
  TANGO_image_damage(new_image);

  return(new_image);
      
}  

#if 0
TANGO_IMAGE
TANGOimage_create(type,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,
		     p17,p18,p19,p20)
   TANGO_IMAGE_TYPE  type;

{
   int p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20;
   TANGO_IMAGE	   new_image;
   IMAGE_PTR	   im;

   if (!TANGO__data) TANGOinit();
   DEBUG("TANGOimage_create(%d,%d,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x...)\n",
	 type, p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12);

   new_image = image_create(type,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,
			    p11,p12,p13,p14,p15,p16,p17,p18,p19,p20);
   /* add this image onto the configuration draw list */
   im = (IMAGE_PTR) malloc( sizeof( struct IMAGE) );
   im->image = new_image;
   im->previ = NULL;
   im->nexti = TANGO__data->confighead;
   if (!TANGO__data->confighead)
      TANGO__data->configtail = im;
   else
      TANGO__data->confighead->previ = im;
   TANGO__data->confighead = im;
   new_image->alive = 1;
   new_image->inconfig = im;
   TANGO_image_damage(new_image);

   return(new_image);
}
#endif


/**************************************************************/
/* TANGOimage_copy -- Copy the given TANGO_IMAGE (use the     */
/*		      attributes) to create a new	      */
/*		      TANGO_IMAGE.			      */
/* 							      */
/* RETURNS:  New image.					      */
/**************************************************************/
TANGO_IMAGE 
TANGOimage_copy (TANGO_IMAGE image)
{
   TANGO_IMAGE	   new_image;
   IMAGE_PTR	   im;

   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOimage_copy(0x%p)\n", image);

   if (!image->alive)
     { fprintf(stderr,
	       "Warning: Call to TANGOimage_copy with a deleted image=0x%p\n",
	       image);
       return(NULL);
     }

   new_image = image_copy(image);
   /* add this image onto the configuration draw list */
   im = (IMAGE_PTR) malloc( sizeof( struct IMAGE) );
   im->image = new_image;
   im->previ = NULL;
   im->nexti = TANGO__data->confighead;
   if (!TANGO__data->confighead)
      TANGO__data->configtail = im;
   else
      TANGO__data->confighead->previ = im;
   TANGO__data->confighead = im;
   new_image->alive = 1;
   new_image->inconfig = im;
   TANGO_image_damage(new_image);

   return(new_image);
}



/**************************************************************/
/* TANGOimage_loc -- Return an TANGO_LOC corresponding to the */
/*		     location of the given part of the given  */
/*		     image.				      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
TANGOimage_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   TANGO_LOC loc;

   if (!TANGO__data) TANGOinit();

   DEBUG("TANGOimage_loc(%p)\n", image);

   if (!image->alive)
     { fprintf(stderr,
	       "Warning: Call to TANGOimage_loc with a deleted image=0x%p\n",
	       image);
       return(NULL);
     }

   switch(image->type) {
      case TANGO_IMAGE_TYPE_LINE      : loc = line_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_RECTANGLE : loc = rectangle_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_CIRCLE    : loc = circle_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_POLYLINE  : loc = polyline_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_POLYGON   : loc = polygon_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_ELLIPSE   : loc = ellipse_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_SPLINE    : loc = spline_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_TEXT      : loc = text_loc(image, part);
				        break;
      case TANGO_IMAGE_TYPE_BITMAP    : loc = bmap_loc(image, part);
					break;
      case TANGO_IMAGE_TYPE_COMPOSITE : loc = composite_loc(image, part);
				        break;
      default 			      : loc = (TANGO_LOC)NULL; /* ignore */
      }

   return( loc );
}



/**************************************************************/
/* TANGO_image_init -- (antiquated)			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_image_init (void)
{
}



/**************************************************************/
/* TANGO_image_delete -- Consider the given TANGO_IMAGE to be */
/*		         non-existant.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_image_delete (TANGO_IMAGE image)
{
   if (!image->alive)
     { fprintf(stderr,"Warning: Attempt to delete already deleted image=0x%p\n",
	       image);
       return;
     }

   image->visible = 0;
   image->alive = 0;
   return;
}



/**************************************************************/
/* TANGO_image_trans -- Call appropriate .._trans() function. */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_image_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_image_damage(image);
   switch (image->type) {
      case TANGO_IMAGE_TYPE_LINE      : line_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_RECTANGLE : rectangle_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_CIRCLE    : circle_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_ELLIPSE   : ellipse_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_POLYLINE  : polyline_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_POLYGON   : polygon_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_SPLINE    : spline_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_TEXT      : text_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_BITMAP    : bmap_trans(image,trans_type,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_COMPOSITE : composite_trans(image,trans_type,dx,dy);
				        break;
      default			      : /* ignore */ ;
      }
   TANGO_image_damage(image);
}



/**************************************************************/
/* TANGO_image_draw -- Call appropriate ..._draw() function.  */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_image_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   switch (image->type) {
      case TANGO_IMAGE_TYPE_LINE      : line_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_RECTANGLE : rectangle_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_CIRCLE    : circle_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_ELLIPSE   : ellipse_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_POLYLINE  : polyline_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_POLYGON   : polygon_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_SPLINE    : spline_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_TEXT      : text_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_BITMAP    : bmap_draw(super_im,image,dx,dy);
				        break;
      case TANGO_IMAGE_TYPE_COMPOSITE : composite_draw(super_im,image,dx,dy);
				        break;
      default			      : /* ignore */ ;
      }
}



/**************************************************************/
/* TANGO_image_damage -- Call appropriate ..._damage()        */
/*                                                  function. */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_image_damage (TANGO_IMAGE image)
{
   WIN_COORD lx,by,rx,ty;

   switch (image->type) {
      case TANGO_IMAGE_TYPE_LINE      : line_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_RECTANGLE : rectangle_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_CIRCLE    : circle_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_ELLIPSE   : ellipse_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_POLYLINE  : polyline_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_POLYGON   : polygon_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_SPLINE    : spline_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_TEXT      : text_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_BITMAP    : bmap_bb(image,&lx,&by,&rx,&ty);
				        break;
      case TANGO_IMAGE_TYPE_COMPOSITE : composite_bb(image,&lx,&by,&rx,&ty);
				        break;
      default			      : /* ignore */ ;
      }

   if (lx < TANGO__data->damlx) TANGO__data->damlx = lx;
   if (rx > TANGO__data->damrx) TANGO__data->damrx = rx;
   if (by > TANGO__data->damby) TANGO__data->damby = by;
   if (ty < TANGO__data->damty) TANGO__data->damty = ty;
}



/**************************************************************/
/* TANGO_image_intercepts -- Determine if image intercepts    */
/*      the damage area                                       */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
int 
TANGO_image_intercepts (TANGO_IMAGE image)
{
   WIN_COORD lx,by,rx,ty;

   if (!image->visible)  /* must be visible */
      return (0);

   switch (image->type) {
      case TANGO_IMAGE_TYPE_LINE      : 
               line_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_RECTANGLE : 
               rectangle_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_CIRCLE    : 
               circle_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_ELLIPSE   : 
               ellipse_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_POLYLINE  : 
               polyline_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_POLYGON   : 
               polygon_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_SPLINE    : 
               spline_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_TEXT      : 
               text_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_BITMAP    : 
               bmap_bb(image,&lx,&by,&rx,&ty);
               break;
      case TANGO_IMAGE_TYPE_COMPOSITE : 
               composite_bb(image,&lx,&by,&rx,&ty);
               break;
      default			      : 
               return(0) /* ignore */ ;
      }

   
   /* Check for intersection with damage area */
   if (((int) floor(((lx - TANGO__data->lx) * 
                  TANGO__data->x_WIN_COORD_to_int) + 0.5) - 1)
	 >
	 ((int)  ceil(((TANGO__data->damrx - TANGO__data->lx) * 
                  TANGO__data->x_WIN_COORD_to_int) + 0.5)))
     return(0);
   if (((int)  ceil(((rx - TANGO__data->lx) * 
                  TANGO__data->x_WIN_COORD_to_int) + 0.5) + 1)
	 <
	 ((int) floor(((TANGO__data->damlx - TANGO__data->lx) * 
                  TANGO__data->x_WIN_COORD_to_int) + 0.5)))
     return(0);
   if (((int) floor(((ty - TANGO__data->ty) * 
                  TANGO__data->y_WIN_COORD_to_int) + 0.5) - 1)
	 >
	 ((int)  ceil(((TANGO__data->damby - TANGO__data->ty) * 
                  TANGO__data->y_WIN_COORD_to_int) + 0.5)))
     return(0);
   if (((int)  ceil(((by - TANGO__data->ty) * 
                  TANGO__data->y_WIN_COORD_to_int) + 0.5) + 1)
	 <
	 ((int) floor(((TANGO__data->damty - TANGO__data->ty) * 
                  TANGO__data->y_WIN_COORD_to_int) + 0.5)))
     return(0);
   
   /* Old simple way */
   /*
   if (lx-0.001 > TANGO__data->damrx) return(0);
   if (rx+0.001 < TANGO__data->damlx) return(0);
   if (ty-0.001 > TANGO__data->damby) return(0);
   if (by+0.001 < TANGO__data->damty) return(0);
   */

   return(1);    /* If falls through to here */
}



/**************************************************************/
/* image_create -- Set up the TANGO_IMAGE data structure      */
/*		   for a given type of image.		      */
/* 							      */
/* RETURNS:  Newly created image.			      */
/**************************************************************/
/* 

TANGO_IMAGE
Ximage_create(type,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,
	     p11,p12,p13,p14,p15,p16,p17,p18,p19,p20)
   TANGO_IMAGE_TYPE  type;
   int p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20;
{
  //this is only a dummy
  return 0;
}
*/

/*
  FIXME: type is superfluid
 */
TANGO_IMAGE
image_create2( TANGO_IMAGE image, int type)
{
   TANGO_IMAGE new_image;
 
   new_image = (TANGO_IMAGE) malloc( sizeof( struct _IMAGE));
   new_image->type = type;
   switch (type)
   {
      case TANGO_IMAGE_TYPE_LINE:
	{
	                          TANGO_LINE_PTR line=(TANGO_LINE_PTR) image->object;
		  line_create(new_image, 
		     image->loc[0],image->loc[1],image->visible,
		     line->color,line->size[0],line->size[1],line->width,line->style,
		     line->arrow);
	 break;
	}
      case TANGO_IMAGE_TYPE_RECTANGLE:
	{
	  TANGO_RECTANGLE_PTR rectangle = (TANGO_RECTANGLE_PTR) image->object;
	  rectangle_create(new_image, 
			  image->loc[0],image->loc[1],image->visible,
			  rectangle->color,rectangle->size[0],rectangle->size[1],
			  rectangle->fill);

	 break;
	}
      case TANGO_IMAGE_TYPE_CIRCLE:
	{
	  TANGO_CIRCLE_PTR circle = (TANGO_CIRCLE_PTR) image->object;
	  circle_create(new_image, 
			image->loc[0],image->loc[1],image->visible,
			circle->color,circle->radius,circle->fill);


	  break;
	}
      case TANGO_IMAGE_TYPE_ELLIPSE:
	{
	  TANGO_ELLIPSE_PTR ellipse = (TANGO_ELLIPSE_PTR) image->object;
	  ellipse_create(new_image, 
		          image->loc[0],image->loc[1],image->visible,
		          ellipse->color,ellipse->size[0],ellipse->size[1],ellipse->fill);


	 break;
	}
      case TANGO_IMAGE_TYPE_POLYLINE:
	{
	  TANGO_POLYLINE_PTR polyline = (TANGO_POLYLINE_PTR) image->object;
	  WIN_COORD vx[7],vy[7];
	  int i;
	  for (i=0; i<7; i++)
	    { vx[i] = polyline->vertex[i][0];
	      vy[i] = polyline->vertex[i][1];
	    }	 
	  polyline_create(new_image, 
			image->loc[0],image->loc[1],image->visible,
			polyline->color,polyline->vertices,vx,vy,polyline->width,
			polyline->style,polyline->arrow);

	 break;
	}
      case TANGO_IMAGE_TYPE_POLYGON:
	{
	  TANGO_POLYGON_PTR polygon = (TANGO_POLYGON_PTR) image->object;
	  WIN_COORD vx[7],vy[7];
	  int i;
	  for (i=0; i<7; i++)
	    { vx[i] = polygon->vertex[i][0];
                               vy[i] = polygon->vertex[i][1];
	    }
 
	  polygon_create(new_image, 
			 image->loc[0],image->loc[1],image->visible,
			 polygon->color,polygon->sides,vx,vy,polygon->fill);

	  break;
	}
      case TANGO_IMAGE_TYPE_SPLINE:
	{
	  TANGO_SPLINE_PTR spline = (TANGO_SPLINE_PTR) image->object;
	  WIN_COORD vx[7],vy[7];
	  int i;
	  for (i=0; i<7; i++)
	   { vx[i] = spline->vertex[i][0];
                              vy[i] = spline->vertex[i][1];
                            }
 
	  spline_create(new_image, 
		       image->loc[0],image->loc[1],image->visible,
		       spline->color,spline->vertices,vx,vy,spline->width,spline->style);

	 break;
	}
      case TANGO_IMAGE_TYPE_TEXT:
	{
	// (TANGO_IMAGE image, p1=WIN_COORD lx, p2=WIN_COORD ly, p3=int vis, p4=int color,p5=char *fontname,p6=char *string,p7=int orient)
	  TANGO_TEXT_PTR text = (TANGO_TEXT_PTR) image->object;
	  text_create(new_image, 
		    image->loc[0],image->loc[1],image->visible,
		    text->color,text->font_name,text->text,text->orient);

	 break;
	}
#if 0
      case TANGO_IMAGE_TYPE_BITMAP:
	 /* locx,locy,vis,array,n,width,height */
	 bmap_create(new_image, p1,p2, p3,p4, p5, p6, p7, p8, p9);
	 break;
      case TANGO_IMAGE_TYPE_COMPOSITE:
	 composite_create(new_image, p1,p2, p3,p4, p5, p6);
	 break;
#endif
      default:
	 fprintf(stderr,
		 "Illegal TANGO_IMAGE_TYPE received by TANGOimage_create=%d\n",
		 type);
	 break;
   }

   new_image->nexti = NULL;

   return( new_image );
}



/**************************************************************/
/* image_copy -- Set up the TANGO_IMAGE data structure        */
/*		 identical to the one given.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
TANGO_IMAGE 
image_copy (TANGO_IMAGE image)
{
   TANGO_IMAGE          new_image;

   switch (image->type)
   {
      case TANGO_IMAGE_TYPE_LINE:
	 return(image_create2(image,image->type));
      case TANGO_IMAGE_TYPE_RECTANGLE:
	 return(image_create2(image,image->type));
     case TANGO_IMAGE_TYPE_CIRCLE:
	 return(image_create2(image,image->type));
      case TANGO_IMAGE_TYPE_ELLIPSE:
	 return(image_create2(image,image->type));
      case TANGO_IMAGE_TYPE_POLYLINE:
	 return(image_create2(image,image->type));
      case TANGO_IMAGE_TYPE_POLYGON:
	 return(image_create2(image,image->type));
      case TANGO_IMAGE_TYPE_SPLINE:
	 return(image_create2(image,image->type));
      case TANGO_IMAGE_TYPE_TEXT:
	 return(image_create2(image,image->type));

      case TANGO_IMAGE_TYPE_BITMAP:
	 new_image = (TANGO_IMAGE) malloc( sizeof( struct _IMAGE));
	 new_image->type = TANGO_IMAGE_TYPE_BITMAP;
	 bmap_copy(new_image, image);
	 new_image->nexti = NULL;
	 return(new_image);
      case TANGO_IMAGE_TYPE_COMPOSITE:
	new_image = (TANGO_IMAGE) malloc( sizeof( struct _IMAGE));
	new_image->type = TANGO_IMAGE_TYPE_COMPOSITE;
	copy_composite(image,new_image);
	new_image->nexti = NULL;
	return( new_image );
	 
      default:
	 fprintf(stderr,
		 "Illegal TANGO_IMAGE_TYPE received by TANGOimage_copy=%d\n",
		 image->type);
	 return( NULL );
   }
}

#if 0
TANGO_IMAGE 
image_copy (TANGO_IMAGE image)
{

   TANGO_LINE_PTR	line;
   TANGO_RECTANGLE_PTR  rectangle;
   TANGO_CIRCLE_PTR     circle;
   TANGO_TEXT_PTR	text;
   TANGO_ELLIPSE_PTR    ellipse;
   TANGO_POLYLINE_PTR   polyline;
   TANGO_POLYGON_PTR    polygon;
   TANGO_SPLINE_PTR     spline;
   // TANGO_BITMAP_PTR     bmap;
   //   double               vx[7],vy[7];
   //  int                  i;

   TANGO_IMAGE          new_image;

   switch (image->type)
   {
      case TANGO_IMAGE_TYPE_LINE:
	//line = (TANGO_LINE_PTR) image->object;
	 return(image_create2(image,image->type));

	 /*         return(image_create(TANGO_IMAGE_TYPE_LINE,
              image->loc[0],image->loc[1],image->visible,
              line->color,line->size[0],line->size[1],line->width,line->style,
              line->arrow));
	 */
      case TANGO_IMAGE_TYPE_RECTANGLE:
	//rectangle = (TANGO_RECTANGLE_PTR) image->object;
	 return(image_create2(image,image->type));
	 
/*
         return(image_create(TANGO_IMAGE_TYPE_RECTANGLE,
              image->loc[0],image->loc[1],image->visible,
              rectangle->color,rectangle->size[0],rectangle->size[1],
              rectangle->fill));
*/ 
     case TANGO_IMAGE_TYPE_CIRCLE:
       //circle = (TANGO_CIRCLE_PTR) image->object;
	 return(image_create2(image,image->type));
	 /*
         return(image_create(TANGO_IMAGE_TYPE_CIRCLE,
             image->loc[0],image->loc[1],image->visible,
             circle->color,circle->radius,circle->fill));
	 */
      case TANGO_IMAGE_TYPE_ELLIPSE:
	//ellipse = (TANGO_ELLIPSE_PTR) image->object;
	 return(image_create2(image,image->type));
	 /*
         return(image_create(TANGO_IMAGE_TYPE_ELLIPSE,
            image->loc[0],image->loc[1],image->visible,
            ellipse->color,ellipse->size[0],ellipse->size[1],ellipse->fill));
	 */
      case TANGO_IMAGE_TYPE_POLYLINE:
	//polyline = (TANGO_POLYLINE_PTR) image->object;
	 return(image_create2(image,image->type));
	 /*
         for (i=0; i<7; i++)
	   { vx[i] = polyline->vertex[i][0];
                            vy[i] = polyline->vertex[i][1];
           }
         return(image_create(TANGO_IMAGE_TYPE_POLYLINE,
            image->loc[0],image->loc[1],image->visible,
            polyline->color,polyline->vertices,vx,vy,polyline->width,
            polyline->style,polyline->arrow));
	 */
      case TANGO_IMAGE_TYPE_POLYGON:
	//polygon = (TANGO_POLYGON_PTR) image->object;
	 return(image_create2(image,image->type));
	 /*
         for (i=0; i<7; i++)
	   { vx[i] = polygon->vertex[i][0];
             vy[i] = polygon->vertex[i][1];
           }
         return(image_create(TANGO_IMAGE_TYPE_POLYGON,
            image->loc[0],image->loc[1],image->visible,
            polygon->color,polygon->sides,vx,vy,polygon->fill));
	 */
      case TANGO_IMAGE_TYPE_SPLINE:
	//spline = (TANGO_SPLINE_PTR) image->object;
	 return(image_create2(image,image->type));
	 /*
         for (i=0; i<7; i++)
	   { vx[i] = spline->vertex[i][0];
             vy[i] = spline->vertex[i][1];
           }
         return(image_create(TANGO_IMAGE_TYPE_SPLINE,
            image->loc[0],image->loc[1],image->visible,
            spline->color,spline->vertices,vx,vy,spline->width,spline->style));
	 */
      case TANGO_IMAGE_TYPE_TEXT:
	//text = (TANGO_TEXT_PTR) image->object;
	 return(image_create2(image,image->type));
	 /*
         return(image_create(TANGO_IMAGE_TYPE_TEXT,
            image->loc[0],image->loc[1],image->visible,
            text->color,text->font_name,text->text,text->orient));
	 */
      case TANGO_IMAGE_TYPE_BITMAP:
	 new_image = (TANGO_IMAGE) malloc( sizeof( struct _IMAGE));
	 new_image->type = TANGO_IMAGE_TYPE_BITMAP;
	 bmap_copy(new_image, image);
	 new_image->nexti = NULL;
	 return(new_image);
      case TANGO_IMAGE_TYPE_COMPOSITE:
         new_image = (TANGO_IMAGE) malloc( sizeof( struct _IMAGE));
         new_image->type = TANGO_IMAGE_TYPE_COMPOSITE;
         copy_composite(image,new_image);
         new_image->nexti = NULL;
         return( new_image );

      default:
	 fprintf(stderr,
		 "Illegal TANGO_IMAGE_TYPE received by TANGOimage_copy=%d\n",
		 image->type);
	 return( NULL );
   }
}
#endif


/***************************************************************/
/*							       */
/*  Method routines for the individual image types.  When a    */
/*	new image type xxx is created, it must supply the      */
/*	following five routines.			       */
/*							       */
/*	xxx_create - create an object of this type, and set    */
/*	    its values to the initial parameters	       */
/*	xxx_draw - draw the image on the screen 	       */
/*	xxx_trans - update the image's data structure with     */
/*	    a response to a non-standard transition unit       */
/*	xxx_loc - return an TANGO_LOC corresponding to the     */
/*	    location of the given part of the image	       */
/*	xxx_bb - return the bounding box that surrounds the    */
/*	    particular image				       */
/*							       */
/***************************************************************/

/*  #######################	LINE	  ###################  */

/**************************************************************/
/* line_create -- Create line image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
line_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int color, WIN_COORD sx, WIN_COORD sy, double width, double style, int arrow)
{
   TANGO_LINE_PTR   line;

   line = (TANGO_LINE_PTR) malloc(sizeof(struct _TANGO_LINE));
   line->color = color;
   line->size[0] = sx;
   line->size[1] = sy;
   line->width = width;
   line->style = style;
   line->arrow = arrow;
   if ((line->arrow == 1) || (line->arrow == 3))
   compute_arrow_off(sx,sy,&(line->arrowloc[0][0]),&(line->arrowloc[0][1]),
			&(line->arrowloc[1][0]),&(line->arrowloc[1][1]));
   if ((line->arrow == 2) || (line->arrow == 3))
   compute_arrow_off(-sx,-sy,&(line->arrowloc[2][0]),&(line->arrowloc[2][1]),
			&(line->arrowloc[3][0]),&(line->arrowloc[3][1]));

   image->object = line;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* line_draw -- Draw line image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
line_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   WIN_COORD x0,y0,x1,y1, xr,yr;
   int ax[4],ay[4];
   TANGO_LINE_STYLE old,lstyle;
   TANGO_LINE_PTR line = (TANGO_LINE_PTR) image->object;

   if (!image->visible)
      return;

   x0 = image->loc[0] + dx;
   y0 = image->loc[1] + dy;
   x1 = x0 + line->size[0];
   y1 = y0 + line->size[1];

   TANGO_color(line->color);
   lstyle = get_linestyle(line->width,line->style);

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   if (lstyle != TANGO_STYLE_SOLID)
      old = TANGO_line_style(lstyle);

   TANGO_Line(XPIXEL(xr,x0),YPIXEL(yr,y0), XPIXEL(xr,x1),YPIXEL(yr,y1));

   if ((line->arrow & 0x1) != 0)
      { ax[0] = XPIXEL(xr, x1 + line->arrowloc[0][0]);
	ay[0] = YPIXEL(yr, y1 + line->arrowloc[0][1]);
	ax[1] = XPIXEL(xr, x1 + line->arrowloc[1][0]);
	ay[1] = YPIXEL(yr, y1 + line->arrowloc[1][1]);
	TANGO_Line(XPIXEL(xr,x1),YPIXEL(yr,y1),ax[0],ay[0]);
	TANGO_Line(XPIXEL(xr,x1),YPIXEL(yr,y1),ax[1],ay[1]);
      }
   if ((line->arrow & 0x2) != 0)
      { ax[2] = XPIXEL(xr, x0 + line->arrowloc[2][0]);
	ay[2] = YPIXEL(yr, y0 + line->arrowloc[2][1]);
	ax[3] = XPIXEL(xr, x0 + line->arrowloc[3][0]);
	ay[3] = YPIXEL(yr, y0 + line->arrowloc[3][1]);
	TANGO_Line(XPIXEL(xr,x0),YPIXEL(yr,y0),ax[2],ay[2]);
	TANGO_Line(XPIXEL(xr,x0),YPIXEL(yr,y0),ax[3],ay[3]);
      }

   if (lstyle != TANGO_STYLE_SOLID)
      TANGO_line_style(old);

}



/**************************************************************/
/* line_trans -- Trans line image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
line_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_LINE_PTR line = (TANGO_LINE_PTR) image->object;

   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 line->width += dx;
	 if (line->width < 0.0)
	    line->width = 0.0;
	 else if (line->width > 1.0)
	    line->width = 1.0;
	 line->style += dy;
	 if (line->style < 0.0)
	    line->style = 0.0;
	 else if (line->style > 1.0)
	    line->style = 1.0;
	 break;
      case TANGO_TRANS_TYPE_RESIZE:
	 line->size[0] += dx;
	 line->size[1] += dy;
	 if ((line->arrow == 1) || (line->arrow == 3))
	    compute_arrow_off(line->size[0],line->size[1],
			&(line->arrowloc[0][0]),&(line->arrowloc[0][1]),
			&(line->arrowloc[1][0]),&(line->arrowloc[1][1]));
	 if ((line->arrow == 2) || (line->arrow == 3))
	    compute_arrow_off(-line->size[0],-line->size[1],
			&(line->arrowloc[2][0]),&(line->arrowloc[2][1]),
			&(line->arrowloc[3][0]),&(line->arrowloc[3][1]));
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 line->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
}



/**************************************************************/
/* line_loc -- Get specified location coords of line image.   */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
line_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   xmin,xmax,ymin,ymax;

   line_bb(image,&xmin,&ymax,&xmax,&ymin);

   switch (part)
   {
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(xmin,ymin) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((xmin+xmax)/2.0,ymin) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(xmax,ymin) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(xmax,(ymin+ymax)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(xmax,ymax) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((xmin+xmax)/2.0,ymax) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(xmin,ymax) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(xmin,(ymin+ymax)/2.0) );
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((xmin+xmax)/2.0,(ymin+ymax)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* line_bb -- Find bounding box of line image.		      */
/* 							      */
/*    This doesn't handle horizontal and vertical thick       */
/*    lines very well.                                        */
/*                                                            */
/* RETURNS:  None.					      */
/**************************************************************/
void 
line_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_LINE_PTR line;
   WIN_COORD ax0,ax1,ay0,ay1,xpad,ypad;

   line = (TANGO_LINE_PTR) image->object;

   *lx = (line->size[0] > ZERO ? image->loc[0] : image->loc[0]+line->size[0]);
   *rx = (line->size[0] > ZERO ? image->loc[0]+line->size[0] : image->loc[0]);
   *ty = (line->size[1] > ZERO ? image->loc[1] : image->loc[1]+line->size[1]);
   *by = (line->size[1] > ZERO ? image->loc[1]+line->size[1] : image->loc[1]);

   if (line->arrow == 0) {	/* add padding for non-arrow'd lines */
     if (line->width > 0.666667) { /* for thick lines, pad by 2 pixels on each side  */
       xpad = 2.0 / TANGO__data->x_WIN_COORD_to_int;
       ypad = 2.0 / TANGO__data->y_WIN_COORD_to_int; 
       *lx = *lx - xpad;
       *rx = *rx + xpad;
       *by = *by + ypad;
       *ty = *ty - ypad;
     }
     else if (line->width > 0.333333) {	/* NPM: for medium lines, pad by 1 pixel on each side */
       xpad = 1.0 / TANGO__data->x_WIN_COORD_to_int;
       ypad = 1.0 / TANGO__data->y_WIN_COORD_to_int;
       *lx = *lx - xpad;
       *rx = *rx + xpad;
       *by = *by + ypad;
       *ty = *ty - ypad;
     }
     else {}			/* no padding for thin lines */
   }
   else {
      if ((line->arrow & 0x1) != 0)    /* below is dumb, but it works */
         {        /* need to take arrows into account on boundbox */
           ax0 = image->loc[0]+line->size[0]+line->arrowloc[0][0];
           ax1 = image->loc[0]+line->size[0]+line->arrowloc[1][0];
           if (ax0 < *lx) *lx = ax0;
           if (ax1 < *lx) *lx = ax1;
           if (ax0 > *rx) *rx = ax0;
           if (ax1 > *rx) *rx = ax1;
           ay0 = image->loc[1]+line->size[1]+line->arrowloc[0][1];
           ay1 = image->loc[1]+line->size[1]+line->arrowloc[1][1];
           if (ay0 > *by) *by = ay0;
           if (ay1 > *by) *by = ay1;
           if (ay0 < *ty) *ty = ay0;
           if (ay1 < *ty) *ty = ay1;
         }
      if ((line->arrow & 0x2) != 0)
         { 
           ax0 = image->loc[0]+line->arrowloc[2][0];
           ax1 = image->loc[0]+line->arrowloc[3][0];
           if (ax0 < *lx) *lx = ax0;
           if (ax1 < *lx) *lx = ax1;
           if (ax0 > *rx) *rx = ax0;
           if (ax1 > *rx) *rx = ax1;
           ay0 = image->loc[1]+line->arrowloc[2][1];
           ay1 = image->loc[1]+line->arrowloc[3][1];
           if (ay0 > *by) *by = ay0;
           if (ay1 > *by) *by = ay1;
           if (ay0 < *ty) *ty = ay0;
           if (ay1 < *ty) *ty = ay1;
         }
      if (line->width > 0.666667) { /* for thick arrow'd lines, pad by */
                                    /*  1 pixel on each side  */
        xpad = 1.0 / TANGO__data->x_WIN_COORD_to_int;
        ypad = 1.0 / TANGO__data->y_WIN_COORD_to_int;
        *lx = *lx - xpad;
        *rx = *rx + xpad;
        *by = *by + ypad;
        *ty = *ty - ypad;
      }
  }
}



/*  #######################   RECTANGLE   ###################  */

/**************************************************************/
/* rectangle_create -- Create rectangle image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
rectangle_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int color, WIN_COORD sx, WIN_COORD sy, double fill)
{
   TANGO_RECTANGLE_PTR	rectangle;

   rectangle = (TANGO_RECTANGLE_PTR) malloc(sizeof(struct _TANGO_RECTANGLE));
   rectangle->color = color;
   rectangle->size[0] = sx;
   rectangle->size[1] = sy;
   rectangle->fill = fill;

   image->object = rectangle;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* rectangle_draw -- Draw rectangle image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
rectangle_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   WIN_COORD x0,y0,x1,y1, xr,yr;
   int fill_style,oldfill;
   TANGO_RECTANGLE_PTR rectangle = (TANGO_RECTANGLE_PTR) image->object;

   if (!image->visible)
      return;

   x0 = image->loc[0] + dx;
   y0 = image->loc[1] + dy;
   x1 = x0 + rectangle->size[0];
   y1 = y0 + rectangle->size[1];

   if (!TANGO__data->color_screen && TANGO__data->mono_fillstyle)
      TANGO_color(rectangle->color);

   fill_style = get_fillstyle(rectangle->fill);

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   if (fill_style == TANGO_FILL_OUTLINE)
      {
      if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
         TANGO_color(rectangle->color);
      TANGO_Rectangle(XPIXEL(xr,x0),YPIXEL(yr,y0),XPIXEL(xr,x1),YPIXEL(yr,y1));
      }
   else
      { oldfill = TANGO_fill_style(fill_style);
      if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
         TANGO_color(rectangle->color);
      TANGO_Fill_Rectangle(XPIXEL(xr,x0),YPIXEL(yr,y0),
     			   XPIXEL(xr,x1), YPIXEL(yr,y1));
      TANGO_fill_style(oldfill);
      }
}



/**************************************************************/
/* rectangle_trans -- Trans rectangle image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
rectangle_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_RECTANGLE_PTR rectangle = (TANGO_RECTANGLE_PTR) image->object;

   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 rectangle->fill +=  dx;
	 if (rectangle->fill < 0.0)
	    rectangle->fill = 0.0;
	 else if (rectangle->fill > 1.0)
	    rectangle->fill = 1.0;
	 break;
      case TANGO_TRANS_TYPE_RESIZE:
	 rectangle->size[0] += dx;
	 rectangle->size[1] += dy;
	 if (rectangle->size[0] < ZERO)
	    rectangle->size[0] = ZERO;
	 if (rectangle->size[1] < ZERO)
	    rectangle->size[1] = ZERO;
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 rectangle->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
}



/**************************************************************/
/* rectangle_loc -- Get specified location coords of	      */
/*		    rectangle image.			      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
rectangle_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   x0,y0,x1,y1;

   rectangle_bb(image,&x0,&y1,&x1,&y0);

   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((x0 + x1)/2.0,(y0 + y1)/2.0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(x0,y0) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((x0+ x1)/2.0,y0) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(x1,y0) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(x1,(y0 + y1)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(x1,y1) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((x0 + x1)/2.0,y1) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(x0,y1) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(x0,(y0 + y1)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* rectangle_bb -- Find bounding box of rectangle image.      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
rectangle_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_RECTANGLE_PTR rectangle;

   rectangle = (TANGO_RECTANGLE_PTR) image->object;
   *lx= image->loc[0];
   *ty= image->loc[1];
   *rx= *lx + rectangle->size[0];
   *by= *ty + rectangle->size[1];
}



/*  #######################    CIRCLE	  ###################  */

/**************************************************************/
/* circle_create -- Create circle image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
circle_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int color, WIN_COORD radius, double fill)
{
   TANGO_CIRCLE_PTR  circle;

   circle = (TANGO_CIRCLE_PTR) malloc(sizeof(struct _TANGO_CIRCLE));
   circle->color = color;
   circle->radius = radius;
   circle->fill = fill;

   image->object = circle;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* circle_draw -- Draw circle image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
circle_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_CIRCLE_PTR circle = (TANGO_CIRCLE_PTR) image->object;
   WIN_COORD x0,y0, xr,yr;
   int fill_style,oldfill;
   int radx,rady;

   if (!image->visible)
      return;

   x0 = image->loc[0] + dx;
   y0 = image->loc[1] + dy;

   if (!TANGO__data->color_screen && TANGO__data->mono_fillstyle)
      TANGO_color(circle->color);
   fill_style = get_fillstyle(circle->fill);

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;
   radx = (int) ROUND( circle->radius * xr );
   rady = (int) ROUND( circle->radius * yr );

   if (fill_style == TANGO_FILL_OUTLINE)
      {
        if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
   	   TANGO_color(circle->color);
	TANGO_Ellipse(XPIXEL(xr,x0), YPIXEL(yr,y0), radx, rady);
      }
   else
      { oldfill = TANGO_fill_style(fill_style);
        if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
   	   TANGO_color(circle->color);
	TANGO_Fill_Ellipse(XPIXEL(xr,x0), YPIXEL(yr,y0), radx, rady);
  	TANGO_fill_style(oldfill);
      }
}



/**************************************************************/
/* circle_trans -- Trans circle image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
circle_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_CIRCLE_PTR circle = (TANGO_CIRCLE_PTR) image->object;

   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 circle->fill += dx;
	 if (circle->fill < 0.0)
	    circle->fill = 0.0;
	 else if (circle->fill > 1.0)
	    circle->fill = 1.0;
	 break;
      case TANGO_TRANS_TYPE_RESIZE:
	 circle->radius += dx;
	 if (circle->radius < ZERO)
	    circle->radius = ZERO;
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 circle->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
}



/**************************************************************/
/* circle_loc -- Get specified location coords of circle      */
/*		 image.					      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
circle_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   x0,y0,corner;
   TANGO_CIRCLE_PTR circle = (TANGO_CIRCLE_PTR) image->object;

   x0 = image->loc[0];
   y0 = image->loc[1];
   corner = sqrt( (circle->radius * circle->radius) / 2.0 );
   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create(x0,y0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(x0-corner,y0-corner) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create(x0,y0-circle->radius) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(x0+corner,y0-corner) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(x0+circle->radius,y0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(x0+corner,y0+corner) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create(x0,y0+circle->radius) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(x0-corner,y0+corner) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(x0-circle->radius,y0) );
   }
   return 0;
}



/**************************************************************/
/* circle_bb -- Find bounding box of circle image.	      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
circle_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_CIRCLE_PTR circle;

   circle = (TANGO_CIRCLE_PTR) image->object;

   *lx = image->loc[0] - circle->radius;
   *rx = image->loc[0] + circle->radius;
   *ty = image->loc[1] - circle->radius;
   *by = image->loc[1] + circle->radius;
}



/*  #######################   ELLIPSE	#####################  */

/**************************************************************/
/* ellipse_create -- Create ellipse image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
ellipse_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int color, WIN_COORD rx, WIN_COORD ry, double fill)
{
   TANGO_ELLIPSE_PTR  ellipse;

   ellipse = (TANGO_ELLIPSE_PTR) malloc(sizeof(struct _TANGO_ELLIPSE));
   ellipse->color = color;
   ellipse->size[0] = rx;
   ellipse->size[1] = ry;
   ellipse->fill = fill;

   image->object = ellipse;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* ellipse_draw -- Draw ellipse image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
ellipse_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_ELLIPSE_PTR ellipse = (TANGO_ELLIPSE_PTR) image->object;
   WIN_COORD cx,cy,xr,yr;
   //   WIN_COORD rx,ry;
   int fill_style,oldfill;
   int radx,rady;

   if (!image->visible)
      return;

   cx = image->loc[0] + dx;
   cy = image->loc[1] + dy;
   //   rx = ellipse->size[0];
   //   ry = ellipse->size[1];

   if (!TANGO__data->color_screen && TANGO__data->mono_fillstyle)
      TANGO_color(ellipse->color);
   fill_style = get_fillstyle(ellipse->fill);

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;
   radx = (int) ROUND( ellipse->size[0] * xr );
   rady = (int) ROUND( ellipse->size[1] * yr );

   if (fill_style == TANGO_FILL_OUTLINE)
      {
        if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
   	   TANGO_color(ellipse->color);
	TANGO_Ellipse(XPIXEL(xr,cx),YPIXEL(yr,cy),radx,rady);
      }
   else
      { oldfill = TANGO_fill_style(fill_style);
        if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
   	   TANGO_color(ellipse->color);
	TANGO_Fill_Ellipse(XPIXEL(xr,cx),YPIXEL(yr,cy),radx,rady);
	TANGO_fill_style(oldfill);
      }
}



/**************************************************************/
/* ellipse_trans -- Trans ellipse image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
ellipse_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_ELLIPSE_PTR ellipse = (TANGO_ELLIPSE_PTR) image->object;

   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 ellipse->fill +=  dx;
	 if (ellipse->fill < 0.0)
	    ellipse->fill = 0.0;
	 else if (ellipse->fill > 1.0)
	    ellipse->fill = 1.0;
	 break;
      case TANGO_TRANS_TYPE_RESIZE:
	 ellipse->size[0] += dx;
	 ellipse->size[1] += dy;
	 if (ellipse->size[0] < ZERO)
	    ellipse->size[0] = ZERO;
	 if (ellipse->size[1] < ZERO)
	    ellipse->size[1] = ZERO;
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 ellipse->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
}



/**************************************************************/
/* ellipse_loc -- Get specified location coords of ellipse    */
/*		  image.				      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
ellipse_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   cx,cy,rx,ry;
   TANGO_ELLIPSE_PTR ellipse = (TANGO_ELLIPSE_PTR) image->object;

   cx = image->loc[0];
   cy = image->loc[1];
   rx = ellipse->size[0];
   ry = ellipse->size[1];
   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create(cx,cy) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(cx-rx,cy-ry) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create(cx,cy-ry) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(cx+rx,cy-ry) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(cx+rx,cy) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(cx+rx,cy+ry) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create(cx,cy+ry) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(cx-rx,cy+ry) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(cx-rx,cy) );
   }
   return 0;
}



/**************************************************************/
/* ellipse_bb -- Find bounding box of ellipse image.	      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
ellipse_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_ELLIPSE_PTR ellipse;

   ellipse = (TANGO_ELLIPSE_PTR) image->object;

   *lx = image->loc[0] - ellipse->size[0];
   *rx = image->loc[0] + ellipse->size[0];
   *ty = image->loc[1] - ellipse->size[1];
   *by = image->loc[1] + ellipse->size[1];
}



/*  #######################   POLYLINE	 ###################  */
/*			 at most 8 vertices		      */

/**************************************************************/
/* polyline_create -- Create polyline image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polyline_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int color, int vertices, double vertexX[], double vertexY[], double width, double style, int arrow)
{
   TANGO_POLYLINE_PTR polyline;
   int		      i;
   double	      fx,fy,bx,by;

   polyline = (TANGO_POLYLINE_PTR) malloc(sizeof(struct _TANGO_POLYLINE));
   polyline->color = color;
   polyline->vertices = vertices;
   for (i=0; i<vertices-1; ++i)
     { polyline->vertex[i][0] = vertexX[i];
       polyline->vertex[i][1] = vertexY[i];
     }
   polyline->width = width;
   polyline->style = style;
   polyline->arrow = arrow;
   if (polyline->arrow != 0)
      arrow_poly_direction(vertices,polyline->vertex,&fx,&fy,&bx,&by);
   if ((polyline->arrow == 1) || (polyline->arrow == 3))
      compute_arrow_off(fx,fy,&(polyline->arrowloc[0][0]),
			&(polyline->arrowloc[0][1]),
			&(polyline->arrowloc[1][0]),
			&(polyline->arrowloc[1][1]));
   if ((polyline->arrow == 2) || (polyline->arrow == 3))
      compute_arrow_off(bx,by,&(polyline->arrowloc[2][0]),
			&(polyline->arrowloc[2][1]),
			&(polyline->arrowloc[3][0]),
			&(polyline->arrowloc[3][1]));

   image->object = polyline;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* polyline_draw -- Draw polyline image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polyline_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_POLYLINE_PTR polyline = (TANGO_POLYLINE_PTR) image->object;
   WIN_COORD	      x0, y0, xr, yr, x1, y1;
   int		      i, pixX[9], pixY[9], ax[4], ay[4];
   TANGO_LINE_STYLE     lstyle,old;

   if (!image->visible)
      return;

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   x0 = image->loc[0] + dx;
   y0 = image->loc[1] + dy;
   pixX[0] = XPIXEL(xr,x0);
   pixY[0] = YPIXEL(yr,y0);
   for (i=1; i<polyline->vertices; ++i)
      { pixX[i] = XPIXEL( xr,x0 + polyline->vertex[i-1][0] );
	pixY[i] = YPIXEL( yr,y0 + polyline->vertex[i-1][1] );
      }

   TANGO_color(polyline->color);
   lstyle = get_linestyle(polyline->width,polyline->style);

   if (lstyle != TANGO_STYLE_SOLID)
      old = TANGO_line_style(lstyle);

   TANGO_Polyline(polyline->vertices,pixX,pixY);
   if ((polyline->arrow & 0x1) != 0)
      { x1 = x0 + polyline->vertex[polyline->vertices-2][0];
	y1 = y0 + polyline->vertex[polyline->vertices-2][1];
	ax[0] = XPIXEL(xr, x1 + polyline->arrowloc[0][0]);
	ay[0] = YPIXEL(yr, y1 + polyline->arrowloc[0][1]);
	ax[1] = XPIXEL(xr, x1 + polyline->arrowloc[1][0]);
	ay[1] = YPIXEL(yr, y1 + polyline->arrowloc[1][1]);
	TANGO_Line(XPIXEL(xr,x1),YPIXEL(yr,y1),ax[0],ay[0]);
	TANGO_Line(XPIXEL(xr,x1),YPIXEL(yr,y1),ax[1],ay[1]);
      }
   if ((polyline->arrow & 0x2) != 0)
      { ax[2] = XPIXEL(xr, x0 + polyline->arrowloc[2][0]);
	ay[2] = YPIXEL(yr, y0 + polyline->arrowloc[2][1]);
	ax[3] = XPIXEL(xr, x0 + polyline->arrowloc[3][0]);
	ay[3] = YPIXEL(yr, y0 + polyline->arrowloc[3][1]);
	TANGO_Line(XPIXEL(xr,x0),YPIXEL(yr,y0),ax[2],ay[2]);
	TANGO_Line(XPIXEL(xr,x0),YPIXEL(yr,y0),ax[3],ay[3]);
      }
   if (lstyle != TANGO_STYLE_SOLID)
      TANGO_line_style(old);

}



/**************************************************************/
/* polyline_trans -- Trans polyline image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polyline_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_POLYLINE_PTR polyline = (TANGO_POLYLINE_PTR) image->object;
   int i,d;
   double fx,fy,bx,by;

   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 polyline->width += dx;
	 if (polyline->width < 0.0)
	    polyline->width = 0.0;
	 else if (polyline->width > 1.0)
	    polyline->width = 1.0;
	 polyline->style += dy;
	 if (polyline->style < 0.0)
	    polyline->style = 0.0;
	 else if (polyline->style > 1.0)
	    polyline->style = 1.0;
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 polyline->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
   if ((d=decode_trans(trans_type)) > 0)
      { if (d >= 11)  /* GRABx */
	   { d = d%10;
	     polyline->vertex[d-1][0] += dx;
	     polyline->vertex[d-1][1] += dy;
	   }
	else if (d >= 1)  /* RESIZEx */
	   { for (i=d; i<polyline->vertices; ++i)
		{ polyline->vertex[i-1][0] += dx;
		  polyline->vertex[i-1][1] += dy;
		}
	   }
	if (polyline->arrow != 0)
	   arrow_poly_direction(polyline->vertices,polyline->vertex,
				&fx,&fy,&bx,&by);
	if ((polyline->arrow == 1) || (polyline->arrow == 3))
	   compute_arrow_off(fx,fy,&(polyline->arrowloc[0][0]),
			     &(polyline->arrowloc[0][1]),
			     &(polyline->arrowloc[1][0]),
			     &(polyline->arrowloc[1][1]));
	if ((polyline->arrow == 2) || (polyline->arrow == 3))
	   compute_arrow_off(bx,by,&(polyline->arrowloc[2][0]),
			     &(polyline->arrowloc[2][1]),
			     &(polyline->arrowloc[3][0]),
			     &(polyline->arrowloc[3][1]));
      }
}



/**************************************************************/
/* polyline_loc -- Get specified location coords of polyline  */
/*		   image.				      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
polyline_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   xmin,ymin,xmax,ymax;

   polyline_bb(image,&xmin,&ymax,&xmax,&ymin);

   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((xmin + xmax)/2.0,(ymin + ymax)/2.0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(xmin,ymin) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((xmin+ xmax)/2.0,ymin) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(xmax,ymin) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(xmax,(ymin + ymax)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(xmax,ymax) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((xmin + xmax)/2.0,ymax) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(xmin,ymax) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(xmin,(ymin + ymax)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* polyline_bb -- Find bounding box of polyline image.	      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polyline_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_POLYLINE_PTR polyline;
   WIN_COORD x,y,xmin,xmax,ymin,ymax,ax0,ay0,ax1,ay1,tox,toy,xpad,ypad;
   int i;

   polyline = (TANGO_POLYLINE_PTR) image->object;

   xmin = xmax = image->loc[0];
   ymin = ymax = image->loc[1];   /* Find the bounding box */
   for (i=0; i<(polyline->vertices - 1); ++i)
      { if ((x = (image->loc[0]+polyline->vertex[i][0])) > xmax) xmax = x;
	if ((x = (image->loc[0]+polyline->vertex[i][0])) < xmin) xmin = x;
	if ((y = (image->loc[1]+polyline->vertex[i][1])) > ymax) ymax = y;
	if ((y = (image->loc[1]+polyline->vertex[i][1])) < ymin) ymin = y;
      }
   *lx = xmin; *rx = xmax; *ty = ymin; *by = ymax;

   if (polyline->arrow == 0) {	/* add padding for non-arrow'd lines */
     if (polyline->width > 0.666667) { /* for thick lines, pad by 2 pixels on each side  */
       xpad = 2.0 / TANGO__data->x_WIN_COORD_to_int;
       ypad = 2.0 / TANGO__data->y_WIN_COORD_to_int; 
       *lx = *lx - xpad;
       *rx = *rx + xpad;
       *by = *by + ypad;
       *ty = *ty - ypad;
     }
     else if (polyline->width > 0.333333) { /* for medium lines, pad by 1 pixel on each side */
       xpad = 1.0 / TANGO__data->x_WIN_COORD_to_int;
       ypad = 1.0 / TANGO__data->y_WIN_COORD_to_int;
       *lx = *lx - xpad;
       *rx = *rx + xpad;
       *by = *by + ypad;
       *ty = *ty - ypad;
     }
     else {}			/* no padding for thin lines */
   }
   else {
      if ((polyline->arrow & 0x1) != 0)    /* below is dumb, but it works */
         {        /* need to take arrows into account on boundbox */
           tox = image->loc[0]+polyline->vertex[polyline->vertices-2][0];
           toy = image->loc[1]+polyline->vertex[polyline->vertices-2][1];
           ax0 = tox+polyline->arrowloc[0][0];
           ax1 = tox+polyline->arrowloc[1][0];
           if (ax0 < *lx) *lx = ax0;
           if (ax1 < *lx) *lx = ax1;
           if (ax0 > *rx) *rx = ax0;
           if (ax1 > *rx) *rx = ax1;
           ay0 = toy+polyline->arrowloc[0][1];
           ay1 = toy+polyline->arrowloc[1][1];
           if (ay0 > *by) *by = ay0;
           if (ay1 > *by) *by = ay1;
           if (ay0 < *ty) *ty = ay0;
           if (ay1 < *ty) *ty = ay1;
         }
      if ((polyline->arrow & 0x2) != 0)
         { 
           ax0 = image->loc[0]+polyline->arrowloc[2][0];
           ax1 = image->loc[0]+polyline->arrowloc[3][0];
           if (ax0 < *lx) *lx = ax0;
           if (ax1 < *lx) *lx = ax1;
           if (ax0 > *rx) *rx = ax0;
           if (ax1 > *rx) *rx = ax1;
           ay0 = image->loc[1]+polyline->arrowloc[2][1];
           ay1 = image->loc[1]+polyline->arrowloc[3][1];
           if (ay0 > *by) *by = ay0;
           if (ay1 > *by) *by = ay1;
           if (ay0 < *ty) *ty = ay0;
           if (ay1 < *ty) *ty = ay1;
         }
      if (polyline->width > 0.666667) { /* for thick arrow'd lines, pad by 1 pixel on each side  */
        xpad = 1.0 / TANGO__data->x_WIN_COORD_to_int;
        ypad = 1.0 / TANGO__data->y_WIN_COORD_to_int;
        *lx = *lx - xpad;
        *rx = *rx + xpad;
        *by = *by + ypad;
        *ty = *ty - ypad;
      }
    }
}



/*  #######################   POLYGON	###################  */
/*			  at most 8 sides		     */

/**************************************************************/
/* polygon_create -- Create polygon image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polygon_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int color, int sides, double vertexX[], double vertexY[], double fill)
{
   TANGO_POLYGON_PTR  polygon;
   int		      i;

   polygon = (TANGO_POLYGON_PTR) malloc(sizeof(struct _TANGO_POLYGON));
   polygon->color = color;
   polygon->sides = sides;
   for (i=0; i<7; ++i)
     { polygon->vertex[i][0] = vertexX[i];
       polygon->vertex[i][1] = vertexY[i];
     }
   polygon->fill = fill;

   image->object = polygon;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* polygon_draw -- Draw polygon image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polygon_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   int i;
   TANGO_POLYGON_PTR polygon = (TANGO_POLYGON_PTR) image->object;
   WIN_COORD x0, y0, xr,yr;
   int pixX[9], pixY[9];
   int fill_style,oldfill;

   if (!image->visible)
      return;

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   x0 = image->loc[0] + dx;
   y0 = image->loc[1] + dy;
   pixX[0] = XPIXEL(xr,x0);
   pixY[0] = YPIXEL(yr,y0);
   for (i=1; i<polygon->sides; ++i)
      { pixX[i] = XPIXEL( xr,x0 + polygon->vertex[i-1][0] );
	pixY[i] = YPIXEL( yr,y0 + polygon->vertex[i-1][1] );
      }

   if (!TANGO__data->color_screen && TANGO__data->mono_fillstyle)
      TANGO_color(polygon->color);
   fill_style = get_fillstyle(polygon->fill);

   if (fill_style == TANGO_FILL_OUTLINE)
      { /* kludge, since polyline doesn't complete the polygon */
	pixX[i] = pixX[0];
	pixY[i] = pixY[0];
        if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
   	   TANGO_color(polygon->color);
	TANGO_Polyline(polygon->sides+1,pixX,pixY);
      }
   else
      { oldfill = TANGO_fill_style(fill_style);
        if (TANGO__data->color_screen || !TANGO__data->mono_fillstyle)
   	   TANGO_color(polygon->color);
	TANGO_Polygon(polygon->sides,pixX,pixY);
	TANGO_fill_style(oldfill);
      }
}



/**************************************************************/
/* polygon_trans -- Trans polygon image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polygon_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_POLYGON_PTR  polygon = (TANGO_POLYGON_PTR) image->object;
   int i,d;

   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 polygon->fill +=  dx;
	 if (polygon->fill < 0.0)
	    polygon->fill = 0.0;
	 else if (polygon->fill > 1.0)
	    polygon->fill = 1.0;
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 polygon->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
   if ((d=decode_trans(trans_type)) >= 0) /* RESIZE or GRAB */
      { if (d >= 11)  /* GRABx */
	   { d = d%10;
	     polygon->vertex[d-1][0] += dx;
	     polygon->vertex[d-1][1] += dy;
	   }
	else if (d >= 1)  /* RESIZEx */
	   { for (i=d; i<polygon->sides; ++i)
		{ polygon->vertex[i-1][0] += dx;
		  polygon->vertex[i-1][1] += dy;
		}
	   }
      }
}



/**************************************************************/
/* polygon_loc -- Get specified location coords of polygon    */
/*		  image.				      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
polygon_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   xmin,ymin,xmax,ymax;

   polygon_bb(image,&xmin,&ymax,&xmax,&ymin);

   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((xmin + xmax)/2.0,(ymin + ymax)/2.0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(xmin,ymin) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((xmin+ xmax)/2.0,ymin) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(xmax,ymin) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(xmax,(ymin + ymax)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(xmax,ymax) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((xmin + xmax)/2.0,ymax) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(xmin,ymax) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(xmin,(ymin + ymax)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* polyline_bb -- Find bounding box of polyline image.	      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
polygon_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_POLYGON_PTR polygon;
   WIN_COORD x,y,xmin,xmax,ymin,ymax;
   int i;

   polygon = (TANGO_POLYGON_PTR) image->object;

   xmin = xmax = image->loc[0];
   ymin = ymax = image->loc[1];   /* Find the bounding box */
   for (i=0; i<(polygon->sides - 1); ++i)
      { if ((x = (image->loc[0]+polygon->vertex[i][0])) > xmax) xmax = x;
	if ((x = (image->loc[0]+polygon->vertex[i][0])) < xmin) xmin = x;
	if ((y = (image->loc[1]+polygon->vertex[i][1])) > ymax) ymax = y;
	if ((y = (image->loc[1]+polygon->vertex[i][1])) < ymin) ymin = y;
      }
   *lx = xmin; *rx = xmax; *ty = ymin; *by = ymax;
}



/*  #######################    SPLINE	 ###################  */
/*			 at most 8 vertices		      */

/**************************************************************/
/* spline_create -- Create spline image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
spline_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int color, int vertices, double vertexX[], double vertexY[], double width, double style)
{
   TANGO_SPLINE_PTR   spline;
   int		      i;

   spline = (TANGO_SPLINE_PTR) malloc(sizeof(struct _TANGO_SPLINE));
   spline->color = color;
   spline->vertices = vertices;
   for (i=0; i<7; ++i)
     { spline->vertex[i][0] = vertexX[i];
       spline->vertex[i][1] = vertexY[i];
     }
   spline->width = width;
   spline->style = style;

   image->object = spline;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* spline_draw -- Draw spline image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
spline_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   int		   i;
   TANGO_SPLINE_PTR spline = (TANGO_SPLINE_PTR) image->object;
   WIN_COORD	   x0, y0, xr,yr;
   int		   pixX[9], pixY[9];
   TANGO_LINE_STYLE  lstyle,old;

   if (!image->visible)
      return;

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   x0 = image->loc[0] + dx;
   y0 = image->loc[1] + dy;
   pixX[0] = XPIXEL(xr,x0);
   pixY[0] = YPIXEL(yr,y0);
   for (i=1; i<spline->vertices; ++i)
      { pixX[i] = XPIXEL( xr,x0 + spline->vertex[i-1][0] );
	pixY[i] = YPIXEL( yr,y0 + spline->vertex[i-1][1] );
      }

   TANGO_color(spline->color);
   lstyle = get_linestyle(spline->width,spline->style);

   if (lstyle != TANGO_STYLE_SOLID)
      old = TANGO_line_style(lstyle);

   TANGO_Spline(spline->vertices,pixX,pixY);

   if (lstyle != TANGO_STYLE_SOLID)
      TANGO_line_style(old);

}



/**************************************************************/
/* spline_trans -- Trans spline image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
spline_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_SPLINE_PTR spline = (TANGO_SPLINE_PTR) image->object;
   int i,d;

   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 spline->width += dx;
	 if (spline->width < 0.0)
	    spline->width = 0.0;
	 else if (spline->width > 1.0)
	    spline->width = 1.0;
	 spline->style += dy;
	 if (spline->style < 0.0)
	    spline->style = 0.0;
	 else if (spline->style > 1.0)
	    spline->style = 1.0;
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 spline->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
   if ((d=decode_trans(trans_type)) >= 0) /* RESIZE or GRAB */
      { if (d >= 11)  /* GRABx */
	   { d = d%10;
	     spline->vertex[d-1][0] += dx;
	     spline->vertex[d-1][1] += dy;
	   }
	else if (d >= 1)  /* RESIZEx */
	   { for (i=d; i<spline->vertices; ++i)
		{ spline->vertex[i-1][0] += dx;
		  spline->vertex[i-1][1] += dy;
		}
	   }
      }
}



/**************************************************************/
/* spline_loc -- Get specified location coords of spline      */
/*		 image.					      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
spline_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   xmin,ymin,xmax,ymax;

   spline_bb(image,&xmin,&ymax,&xmax,&ymin);

   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((xmin + xmax)/2.0,(ymin + ymax)/2.0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(xmin,ymin) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((xmin+ xmax)/2.0,ymin) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(xmax,ymin) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(xmax,(ymin + ymax)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(xmax,ymax) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((xmin + xmax)/2.0,ymax) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(xmin,ymax) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(xmin,(ymin + ymax)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* spline_bb -- Find bounding box of spline image.	      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
spline_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_SPLINE_PTR spline;
   WIN_COORD x,y,xmin,xmax,ymin,ymax,xpad,ypad;
   int i;

   spline = (TANGO_SPLINE_PTR) image->object;

   xmin = xmax = image->loc[0];
   ymin = ymax = image->loc[1];   /* Find the bounding box */
   for (i=0; i<(spline->vertices - 1); ++i)
      { if ((x = (image->loc[0]+spline->vertex[i][0])) > xmax) xmax = x;
	if ((x = (image->loc[0]+spline->vertex[i][0])) < xmin) xmin = x;
	if ((y = (image->loc[1]+spline->vertex[i][1])) > ymax) ymax = y;
	if ((y = (image->loc[1]+spline->vertex[i][1])) < ymin) ymin = y;
      }
   *lx = xmin; *rx = xmax; *ty = ymin; *by = ymax;

   /* Note that for the most part, padding splines to take care
      of thick lines going outside the bounding box is not needed.
      Spline bounding boxes are typically much larger than the drawn image.
      However, there are rare cases where the drawn image can intersect the 
      bounding box, so we pad just to prevent problems w/ those cases... */
   if (spline->width > 0.666667) { /* for thick lines, pad by 2 pixels on each side  */
     xpad = 2.0 / TANGO__data->x_WIN_COORD_to_int;
     ypad = 2.0 / TANGO__data->y_WIN_COORD_to_int; 
     *lx = *lx - xpad;
     *rx = *rx + xpad;
     *by = *by + ypad;
     *ty = *ty - ypad;
   }
   else if (spline->width > 0.333333) {	/* for medium lines, pad by 1 pixel on each side */
     xpad = 1.0 / TANGO__data->x_WIN_COORD_to_int;
     ypad = 1.0 / TANGO__data->y_WIN_COORD_to_int;
     *lx = *lx - xpad;
     *rx = *rx + xpad;
     *by = *by + ypad;
     *ty = *ty - ypad;
   }
   else {}			/* no padding for thin lines */
}



/*  #######################	TEXT	  ###################  */

/**************************************************************/
/* text_create -- Create text image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void
text_create(TANGO_IMAGE image, WIN_COORD lx,WIN_COORD ly,int vis,int color,char *fontname,char *string,int orient)
{
   TANGO_TEXT_PTR  text;

   text = (TANGO_TEXT_PTR) malloc(sizeof(struct _TANGO_TEXT));
   text->color = color;

   if ((fontname == NULL) || (*fontname == 0))
      { text->fontid = TANGO_inq_font();	/* Use current font */
	text->font_name[0] =  0;
      }
   else
      { text->fontid = TANGO_load_font(fontname);
	strcpy(text->font_name,fontname);
      }
   strcpy(text->text,string);
   text->orient = orient;

   TANGO_text_info(text->fontid,string,&(text->xext),&(text->yext),
		   &(text->xoff),&(text->yoff));

   image->object = text;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* text_draw -- Draw text image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
text_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_TEXT_PTR text = (TANGO_TEXT_PTR) image->object;
   WIN_COORD x0,y0, xr,yr;
   int oldfont;

   if (!image->visible)
      return;

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   if (text->orient == 0) /* lower left */
      { x0 = image->loc[0] + dx;  /* - (((WIN_COORD) text->xoff) / xr);?? */
	y0 = image->loc[1] + dy;
      }
   else /* centered */
      { x0 = image->loc[0] + dx 
           - ((((WIN_COORD) (text->xext - text->xoff)) / 2.0 ) / xr);
                 /* NPM: xext-xoff is width of string, shift origin to left 
                         by 1/2 total width */ 
	y0 = image->loc[1] + dy + ((((WIN_COORD) (text->yext - text->yoff)) 
                                        / 2.0) / yr); 
                 /* yext-yoff=asc, so center around middle of ascent */
      }

   oldfont = TANGO_font(text->fontid);
   TANGO_color(text->color);
   TANGO_Text(XPIXEL(xr,x0),YPIXEL(yr,y0),text->text);
   TANGO_font(oldfont);

}



/**************************************************************/
/* text_trans -- Trans text image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
text_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_TEXT_PTR text = (TANGO_TEXT_PTR) image->object;
   switch( trans_type )
   {
      case TANGO_TRANS_TYPE_FILL:
	 break;
      case TANGO_TRANS_TYPE_RESIZE:
	 break;
      case TANGO_TRANS_TYPE_COLOR:
	 text->color = inquire_pathcolor(dx,dy);
	 break;
      default:
	 break;
   }
}



/**************************************************************/
/* text_loc -- Get specified location coords of text image.   */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
text_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   lx,by,rx,ty;

   text_bb(image,&lx,&by,&rx,&ty);

   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((lx + rx)/2.0,(by + ty)/2.0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(lx,ty) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((lx+ rx)/2.0,ty) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(rx,ty) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(rx,(ty + by)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(rx,by) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((lx + rx)/2.0,by) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(lx,by) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(lx,(by + ty)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* text_bb -- Find bounding box of text image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
text_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_TEXT_PTR text;
   WIN_COORD ex,ox,ey,oy,xr,yr;

   text = (TANGO_TEXT_PTR) image->object;

/* old incorrect way */
/*   ex = (double) text->xext / (double) TANGO__data->width;
   ox = (double) text->xoff / (double) TANGO__data->width;
   ey = (double) text->yext / (double) TANGO__data->height;
   oy = (double) text->yoff / (double) TANGO__data->height; 
*/

   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   ex = (double) text->xext / xr;  /* Changed by so that bounding box */
   ox = (double) text->xoff / xr;  /* relative to how much we have zoomed in */
   ey = (double) text->yext / yr;
   oy = (double) text->yoff / yr;
   
   if (text->orient == 0) /* lower left */
      { *lx = image->loc[0] + ox;   
	*rx = image->loc[0] + ex;
	*by = image->loc[1] + oy;
	*ty = image->loc[1] - ey + oy;
      }
   else
      { *lx = image->loc[0] - ((ex-ox)/2.0) + ox;  
	*rx = image->loc[0] + ((ex-ox)/2.0) + ox;
	*by = image->loc[1] + ((ey-oy)/2.0) + oy;
	*ty = image->loc[1] - ((ey-oy)/2.0) + oy;

      /* Comments and help from Niels Mayer */
      /* NPM: for ctrd-text placed at <x>-<xext-xoff/2>, 
              LHS of b.b. is <x>-<xext-xoff/2>+<xoff> */
      /* NPM: for ctrd-text placed at <x>-<xext-xoff/2>, 
              RHS of b.b. is <x>-<xext-xoff/2>+<xext> == 
                   <x> - <xext/2> + <xoff/2> + <xext> == 
                   <x> + <xext/2> + <xoff/2> == 
                   <x> + <xext-xoff/2> + <xoff> */
      /* NPM: for ctrd-text placed at <y>+<(yext-yoff)/2>==<y>+<asc/2>, 
              bottom is <y>+<asc/2>+<desc> */
      /* NPM: for ctrd-text placed at <y>+<(yext-yoff)/2>==<y>+<asc/2>, 
              top is <y>-<asc/2> */
      }
}



/*  #######################    BITMAP	  ###################  */

/**************************************************************/
/* bmap_create -- Create bitmap image from color data.	      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
bmap_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, int *bmaps, int n, int width, int height)
{
   TANGO_BITMAP_PTR pmap;
   int		    x,y;
   int		    old = TANGO_color(TANGO_COLOR_WHITE);

   pmap = (TANGO_BITMAP_PTR) malloc(sizeof(struct _TANGO_BMAP));
   pmap->used = n;
   pmap->showing = 0;
   pmap->width = width;
   pmap->height = height;

   /* must remove clip_rectangle set by TANGO_damage_clear_screen */
   XSetClipMask(TANGO__data->display, TANGO_gc(), None);
   for (n = 0; n < pmap->used; ++n) {
      pmap->bmap[n] = XCreatePixmap(TANGO__data->display, 
                             DefaultRootWindow(TANGO__data->display),
			     (unsigned int) width, (unsigned int) height,
			     DefaultDepth(TANGO__data->display,
					 DefaultScreen(TANGO__data->display)));
      for (x = 0; x < width; ++x)
         for (y = 0; y < height; ++y) {
            TANGO_color(bmaps[n*width*height+y*width+x]);
	    XDrawPoint(TANGO__data->display, pmap->bmap[n], TANGO_gc(), x, y);
            }
      }
   TANGO_color(old);

   image->object = pmap;
   image->loc[0] = lx;
   image->loc[1] = ly;
   image->visible = vis;
}



/**************************************************************/
/* bmap_copy -- Create bitmap image from pixmap data.	      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
bmap_copy (TANGO_IMAGE destImage, TANGO_IMAGE srcImage)
{
   TANGO_BITMAP_PTR destPmap, srcPmap;
   int		    i;

   /* must remove clip_rectangle set by TANGO_damage_clear_screen */
   XSetClipMask(TANGO__data->display, TANGO_gc(), None);
   srcPmap = (TANGO_BITMAP_PTR) srcImage->object;
   destPmap = (TANGO_BITMAP_PTR) malloc(sizeof(struct _TANGO_BMAP));
   destPmap->used = srcPmap->used;
   destPmap->showing = srcPmap->showing;
   destPmap->width = srcPmap->width;
   destPmap->height = srcPmap->height;
   for (i = 0; i < srcPmap->used; ++i) {
      destPmap->bmap[i] = XCreatePixmap(TANGO__data->display, 
                            DefaultRootWindow(TANGO__data->display),
			    (unsigned int) (destPmap->width), 
                            (unsigned int) (destPmap->height),
			    DefaultDepth(TANGO__data->display,
				     DefaultScreen(TANGO__data->display)));
      XCopyArea(TANGO__data->display, srcPmap->bmap[i], destPmap->bmap[i], 
	        TANGO_gc(), 0, 0, 
                (unsigned int) srcPmap->width, 
                (unsigned int) srcPmap->height, 0, 0);
      }
   destImage->object = destPmap;
   destImage->loc[0] = srcImage->loc[0];
   destImage->loc[1] = srcImage->loc[1];
   destImage->visible = srcImage->visible;
}



/**************************************************************/
/* bmap_draw -- Draw current bitmap image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
bmap_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_BITMAP_PTR pmap = (TANGO_BITMAP_PTR) image->object;
   WIN_COORD x0,y0, xr,yr;

   if (!image->visible)
      return;

   x0 = image->loc[0] + dx;
   y0 = image->loc[1] + dy;
   xr = TANGO__data->x_WIN_COORD_to_int;
   yr = TANGO__data->y_WIN_COORD_to_int;

   XCopyArea(TANGO__data->display, pmap->bmap[pmap->showing], 
             TANGO__data->pixmap,
	     TANGO_gc(), 0, 0, (unsigned int) pmap->width, 
             (unsigned int) pmap->height,
	     XPIXEL(xr,x0),YPIXEL(yr,y0)); 

}



/**************************************************************/
/* bmap_trans -- Trans bmap image.			      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
bmap_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_BITMAP_PTR pmap = (TANGO_BITMAP_PTR) image->object;
   if (trans_type == TANGO_TRANS_TYPE_SHUFFLE) {
      pmap->showing = (pmap->showing + 1) % pmap->used;
      }
}



/**************************************************************/
/* bmap_loc -- Get specified location coords of bmap image.   */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
bmap_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD	   lx,by,rx,ty;

   bmap_bb(image,&lx,&by,&rx,&ty);

   switch (part) {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((lx + rx)/2.0,(by + ty)/2.0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(lx,ty) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((lx+ rx)/2.0,ty) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(rx,ty) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(rx,(ty + by)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(rx,by) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((lx + rx)/2.0,by) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(lx,by) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(lx,(by + ty)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* bmap_bb -- Find bounding box of bmap image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
bmap_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_BITMAP_PTR pmap = (TANGO_BITMAP_PTR) image->object;
   double w,h;

   w = (double) (pmap->width  - 1) / TANGO__data->x_WIN_COORD_to_int;
   h = (double) (pmap->height - 1) / TANGO__data->y_WIN_COORD_to_int;

   *lx = image->loc[0];
   *rx = image->loc[0] + w;
   *ty = image->loc[1];
   *by = image->loc[1] + h;
}



/*  #######################  COMPOSITE	  ###################  */

/**************************************************************/
/* composite_create -- Create composite image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
composite_create (TANGO_IMAGE image, WIN_COORD lx, WIN_COORD ly, int vis, TANGO_IMAGE_COMPONENT subimages[])
{
   TANGO_COMPOSITE_PTR	 composite;

   composite = (TANGO_COMPOSITE_PTR) malloc(sizeof(struct _TANGO_COMPOSITE));
   composite->image_list = read_composite(subimages);

   image->object = composite;
   image->visible = vis;
   image->loc[0] = lx;
   image->loc[1] = ly;
}



/**************************************************************/
/* composite_draw -- Draw composite image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
composite_draw (TANGO_IMAGE super_im, TANGO_IMAGE image, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_COMPOSITE_PTR composite = (TANGO_COMPOSITE_PTR) image->object;
   TANGO_IMAGE	   sub;

   if (!image->visible)
      return;

   for (sub=composite->image_list; sub; sub=sub->nexti)
      TANGO_image_draw(image, sub, image->loc[0], image->loc[1]);
   /* don't check vis here, but add absol pos of compos image to the draw */
}



/**************************************************************/
/* composite_trans -- Trans composite image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
composite_trans (TANGO_IMAGE image, TANGO_TRANS_TYPE trans_type, WIN_COORD dx, WIN_COORD dy)
{
   TANGO_IMAGE	sub;
   TANGO_COMPOSITE_PTR composite = (TANGO_COMPOSITE_PTR) image->object;;

   for (sub=composite->image_list; sub; sub=sub->nexti)
      TANGO_image_trans(sub, trans_type, dx, dy);

   /* probably want to have special handlers here rather than do it
      like a GROUP image, but for now, OK			    */
}



/**************************************************************/
/* composite_loc -- Get specified location coords of	      */
/*		    composite image.			      */
/* 							      */
/* RETURNS:  Specified TANGO_LOC.			      */
/**************************************************************/
TANGO_LOC 
composite_loc (TANGO_IMAGE image, TANGO_PART_TYPE part)
{
   WIN_COORD lx,by,rx,ty;

   composite_bb(image,&lx,&by,&rx,&ty);
   switch (part)
   {
      case TANGO_PART_TYPE_C :
	 return( TANGOloc_create((lx + rx)/2.0,(by + ty)/2.0) );
      case TANGO_PART_TYPE_NW :
	 return( TANGOloc_create(lx,ty) );
      case TANGO_PART_TYPE_N :
	 return( TANGOloc_create((lx+ rx)/2.0,ty) );
      case TANGO_PART_TYPE_NE :
	 return( TANGOloc_create(rx,ty) );
      case TANGO_PART_TYPE_E :
	 return( TANGOloc_create(rx,(ty + by)/2.0) );
      case TANGO_PART_TYPE_SE :
	 return( TANGOloc_create(rx,by) );
      case TANGO_PART_TYPE_S :
	 return( TANGOloc_create((lx + rx)/2.0,by) );
      case TANGO_PART_TYPE_SW :
	 return( TANGOloc_create(lx,by) );
      case TANGO_PART_TYPE_W :
	 return( TANGOloc_create(lx,(by + ty)/2.0) );
   }
   return 0;
}



/**************************************************************/
/* composite_bb -- Find bounding box of composite image.      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
composite_bb (TANGO_IMAGE image, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   TANGO_COMPOSITE_PTR composite;
   TANGO_IMAGE	sub;
   WIN_COORD x0,y0,x1,y1;

   composite = (TANGO_COMPOSITE_PTR) image->object;

   /* get first */
   TANGO_bounding_box(composite->image_list,lx,by,rx,ty);
   for (sub=composite->image_list->nexti; sub; sub=sub->nexti)
      { switch (sub->type)
	   { case TANGO_IMAGE_TYPE_LINE:
		line_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_RECTANGLE:
		rectangle_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_CIRCLE:
		circle_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_ELLIPSE:
		ellipse_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_POLYLINE:
		polyline_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_POLYGON:
		polygon_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_SPLINE:
		spline_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_TEXT:
		text_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     case TANGO_IMAGE_TYPE_BITMAP:
		bmap_bb(sub,&x0,&y1,&x1,&y0);
		break;
	     default:
		;
	    }
	if (x0 < *lx)
	   *lx = x0;
	if (x1 > *rx)
	   *rx = x1;
	if (y0 < *ty)
	   *ty = y0;
	if (y1 > *by)
	   *by = y1;
      }
   *lx += image->loc[0];  /* composite so add in overall loc to locals */
   *rx += image->loc[0];
   *by += image->loc[1];
   *ty += image->loc[1];
}



/*############################################################*/
/* read_composite -- Read a composite TANGO_IMAGE_TYPE from   */
/*		     a structure, create the sub-images, and  */
/*		     return the list of them.		      */
/* 							      */
/* RETURNS:  List of created sub-images.		      */
/*############################################################*/
TANGO_IMAGE 
read_composite (TANGO_IMAGE_COMPONENT images[])
{
   TANGO_IMAGE_TYPE  type;
   char 	    *args,*p;
   int		     num,i;
   TANGO_IMAGE	     image,head,tail;
   WIN_COORD	     lx,ly,sx,sy,rad,vx[7],vy[7];
   int		     color,arrow,orient,vertices;
   double	     wid,sty,fill;
   char 	     colorstr[STRINGLENGTH];
   char 	     fontname[STRINGLENGTH];
   char 	    *fname;
   char 	     text[STRINGLENGTH];

   head = tail = NULL;
   num = 0;
   do
      { type = images[num].type;
	args = images[num].args;
	if (type == TANGO_IMAGE_TYPE_COMPOSITE) break;

	switch (type)
	{
	   case TANGO_IMAGE_TYPE_LINE:
	      sscanf(args,"%lf %lf %s %lf %lf %lf %lf %d",
			  &lx,&ly,colorstr,&sx,&sy,&wid,&sty,&arrow);
	      color = get_color(colorstr);
      //  image = image_create(TANGO_IMAGE_TYPE_LINE,lx,ly,1,color, sx,sy,wid,sty,arrow);
	      line_create(image,lx,ly,1,color, sx,sy,wid,sty,arrow);
	      break;
	   case TANGO_IMAGE_TYPE_RECTANGLE:
	      sscanf(args,"%lf %lf %s %lf %lf %lf",
			&lx,&ly,colorstr,&sx,&sy,&fill);
	      color = get_color(colorstr);
      //  image = image_create(TANGO_IMAGE_TYPE_RECTANGLE,lx,ly,1,color,sx,sy,fill);
	      rectangle_create(image,lx,ly,1,color,sx,sy,fill);
	      break;
	   case TANGO_IMAGE_TYPE_CIRCLE:
	      sscanf(args,"%lf %lf %s %lf %lf",&lx,&ly,colorstr,&rad,&fill);
	      color = get_color(colorstr);
      //      image = image_create(TANGO_IMAGE_TYPE_CIRCLE,lx,ly,1,color,rad,fill);
	      circle_create(image, lx,ly,1,color,rad,fill);
	      break;
	   case TANGO_IMAGE_TYPE_ELLIPSE:
	      sscanf(args,"%lf %lf %s %lf %lf %lf",
			&lx,&ly,colorstr,&sx,&sy,&fill);
	      color = get_color(colorstr);
      //      image = image_create(TANGO_IMAGE_TYPE_ELLIPSE,lx,ly,1,color,sx,sy,fill);
	      ellipse_create(image, lx,ly,1,color,sx,sy,fill);
	      break;
	   case TANGO_IMAGE_TYPE_POLYLINE:
	      sscanf(args,"%lf %lf %s %lf %lf %d %d",
			&lx,&ly,colorstr,&wid,&sty,&arrow,&vertices);
	      color = get_color(colorstr);
	      p = args;
	      while (*p == ' ') p++;
	      for (i=1; i<=7; ++i)	   /* get past prelim args */
		 { while (*p != ' ') p++;
		   while (*p == ' ') p++;
		 }
	      for (i=0; i<vertices-1; ++i)
		 { sscanf(p,"%lf",&(vx[i]));
		   while (*p != ' ') p++;
		   while (*p == ' ') p++;
		   sscanf(p,"%lf",&(vy[i]));
		   while (*p != ' ')
		      { if (*p == '\0') break;
			p++;
		      }
		   if (*p != '\0')
		      while (*p == ' ') p++;
		 }
 //      image = image_create(TANGO_IMAGE_TYPE_POLYLINE,lx,ly,1,color,vertices,vx,vy,wid,sty,arrow);
				   
	      polyline_create(image,lx,ly,1,color,vertices,vx,vy,wid,sty,arrow);
	      break;
	   case TANGO_IMAGE_TYPE_POLYGON:
	      sscanf(args,"%lf %lf %s %lf %d",
			&lx,&ly,colorstr,&fill,&vertices);
	      color = get_color(colorstr);
	      p = args;
	      while (*p == ' ') p++;
	      for (i=1; i<=5; ++i)	   /* get past prelim args */
		 { while (*p != ' ') p++;
		   while (*p == ' ') p++;
		 }
	      for (i=0; i<vertices-1; ++i)
		 { sscanf(p,"%lf",&(vx[i]));
		   while (*p != ' ') p++;
		   while (*p == ' ') p++;
		   sscanf(p,"%lf",&(vy[i]));
		   while (*p != ' ')
		      { if (*p == '\0') break;
			p++;
		      }
		   if (*p != '\0')
		      while (*p == ' ') p++;
		 }
   // image = image_create(TANGO_IMAGE_TYPE_POLYGON,lx,ly,1,color,vertices, vx,vy,fill);
					
	      polygon_create(image,lx,ly,1,color,vertices, vx,vy,fill);
	      break;
	   case TANGO_IMAGE_TYPE_SPLINE:
	      sscanf(args,"%lf %lf %s %lf %lf %d",
			&lx,&ly,colorstr,&wid,&sty,&vertices);
	      color = get_color(colorstr);
	      p = args;
	      while (*p == ' ') p++;
	      for (i=1; i<=6; ++i)	   /* get past prelim args */
		 { while (*p != ' ') p++;
		   while (*p == ' ') p++;
		 }
	      for (i=0; i<vertices-1; ++i)
		 { sscanf(p,"%lf",&(vx[i]));
		   while (*p != ' ') p++;
		   while (*p == ' ') p++;
		   sscanf(p,"%lf",&(vy[i]));
		   while (*p != ' ')
		      { if (*p == '\0') break;
			p++;
		      }
		   if (*p != '\0')
		      while (*p == ' ') p++;
		 }
  //      image = image_create(TANGO_IMAGE_TYPE_SPLINE,lx,ly,1,color,vertices,vx,vy,wid,sty);
	      spline_create(image,lx,ly,1,color,vertices,vx,vy,wid,sty);
	      break;
	    case TANGO_IMAGE_TYPE_TEXT:
	      sscanf(args,"%lf %lf %s %s %s %d",
			&lx,&ly,colorstr,fontname,text,&orient);
	      color = get_color(colorstr);
	      if ((strcmp(fontname,"NULL") == 0) ||
		  (strcmp(fontname,"0") == 0))
		 fname = NULL;
	      else
		 fname = fontname;
//      image = image_create(TANGO_IMAGE_TYPE_TEXT,lx,ly,1,color,fname,text,orient);
					
	      text_create(image,lx,ly,1,color,fname,text,orient);
	      break;
	    case TANGO_IMAGE_TYPE_BITMAP:
	      COMPLAIN("Warning: TANGO_IMAGE_TYPE_BITMAP not %s\n",
		       "supported in COMPOSITE images.");
	      continue;
	    default :
	      COMPLAIN("Warning: Unknown image type in COMPOSITE image: %d\n",
		       type);
	      continue;
	}
	++num;

	image->nexti = NULL;
	if (!head)
	   head = image;
	else
	   tail->nexti = image;
	tail = image;
      } while(1==1);

   return(head);
}



/*############################################################*/
/* copy_composite -- Copy the objects inside a composite      */
/*		     image into another one.		      */
/* 							      */
/* RETURNS:  None.					      */
/*############################################################*/
void 
copy_composite (TANGO_IMAGE sourceim, TANGO_IMAGE destim)
{
   TANGO_COMPOSITE_PTR   co, toc;
   TANGO_IMAGE           head,tail,ci,new;

   head = tail = NULL;
   co = (TANGO_COMPOSITE_PTR) sourceim->object;
   toc = (TANGO_COMPOSITE_PTR) malloc( sizeof( struct _TANGO_COMPOSITE));
   destim->loc[0] = sourceim->loc[0];
   destim->loc[1] = sourceim->loc[1];
   destim->visible = sourceim->visible;
   
   for (ci=co->image_list; ci; ci=ci->nexti)
     { new = image_copy(ci);
       new->nexti = NULL;
       if (!head)
	   head = new;
       else
	   tail->nexti = new;
       tail = new;
     }
   toc->image_list = head;
   destim->object = toc;
}



/*############################################################*/
/* compute_arrow_off -- Figure out what the offsets should    */
/*			be for the points of an arrow.        */
/*			I haven't got a clue how this works,  */
/*			but it just does.		      */
/* 							      */
/* Many thanks to Eric Golin for code.			      */
/* 							      */
/* RETURNS:  None.					      */
/*############################################################*/

#define   PI   ((double)3.14159)
#define   ARC_HEAD_LENGTH   ((double)0.025)
#define   ARC_HEAD_ANGLE    PI/2.5

void 
compute_arrow_off (double dx, double dy, double *adx1, double *ady1, double *adx2, double *ady2)
{
   double arclen,len,theta,alpha,beta;

   if ((dx == 0.0) && (dy == 0.0))
      { *adx1 = *adx2 = *ady1 = *ady2 = 0.0;
	return;
      }

   arclen = ARC_HEAD_LENGTH;
   len = sqrt((dx*dx) + (dy*dy));
   if (len < arclen)
      arclen = len;

   theta = atan2(dx,dy);
   alpha = -(theta + ARC_HEAD_ANGLE);
   beta = PI - (theta - ARC_HEAD_ANGLE);
   *adx1 = arclen * cos(alpha);
   *ady1 = arclen * sin(alpha);
   *adx2 = arclen * cos(beta);
   *ady2 = arclen * sin(beta);
}



/*############################################################*/
/* arrow_poly_direction -- Determine what direction to point  */
/*			   the arrowheads on a polyline.      */
/*			   Tricky because offsets at the two  */
/*			   endpoints may be 0,0.  We want the */
/*			   last non-negative pair.	      */
/* 							      */
/* RETURNS:  None.					      */
/*############################################################*/
void 
arrow_poly_direction (int num, WIN_COORD vert[7][2], WIN_COORD *fx, WIN_COORD *fy, WIN_COORD *bx, WIN_COORD *by)
{
   WIN_COORD tempx[8],tempy[8];
   int	     i;

   for (i=1; i<num; ++i)   /* need a starting position too */
      { tempx[i] = vert[i-1][0];
	tempy[i] = vert[i-1][1];
      }
   tempx[0] = tempy[0] = 0.0;

   for (i=num-1; i>=1; --i)
      { *fx = tempx[i] - tempx[i-1];
	*fy = tempy[i] - tempy[i-1];
	if ((*fx != 0.0) || (*fy != 0.0))
	   break;
      }

   for (i=1; i<num; ++i)
      { *bx = tempx[i-1] - tempx[i];
	*by = tempy[i-1] - tempy[i];
	if ((*bx != 0.0) || (*by != 0.0))
	   break;
      }
}



/*############################################################*/
/* decode_trans -- Give a number representation of the        */
/*		   transition type.  Anything but RESIZEs and */
/*		   GRABs is -1.  RESIZE run in the single     */
/*		   digits, and GRABs in the tens.	      */
/* 							      */
/* RETURNS:  None.					      */
/*############################################################*/
int 
decode_trans (TANGO_TRANS_TYPE t)
{
   switch (t)
   { case TANGO_TRANS_TYPE_RESIZE:
	return(0);
     case TANGO_TRANS_TYPE_RESIZE1:
	return(1);
     case TANGO_TRANS_TYPE_RESIZE2:
	return(2);
     case TANGO_TRANS_TYPE_RESIZE3:
	return(3);
     case TANGO_TRANS_TYPE_RESIZE4:
	return(4);
     case TANGO_TRANS_TYPE_RESIZE5:
	return(5);
     case TANGO_TRANS_TYPE_RESIZE6:
	return(6);
     case TANGO_TRANS_TYPE_RESIZE7:
	return(7);
     case TANGO_TRANS_TYPE_GRAB1:
	return(11);
     case TANGO_TRANS_TYPE_GRAB2:
	return(12);
     case TANGO_TRANS_TYPE_GRAB3:
	return(13);
     case TANGO_TRANS_TYPE_GRAB4:
	return(14);
     case TANGO_TRANS_TYPE_GRAB5:
	return(15);
     case TANGO_TRANS_TYPE_GRAB6:
	return(16);
     case TANGO_TRANS_TYPE_GRAB7:
	return(17);
     default:
	return(-1);
   }
}



/**************************************************************/
/* inquire_pathcolor -- Given the dx and dy offsets of a      */
/*			transition unit, figure out which     */
/*		        color to change to.		      */
/* 							      */
/* RETURNS:  Color to change to.			      */
/**************************************************************/
int 
inquire_pathcolor (WIN_COORD dx, WIN_COORD dy)
{
   if (dx > 0.0)
      { if (dy > 0.0)
	   { if (ABS(dx) >= ABS(dy))
		return(TANGO_COLOR_MAROON);
	     else
		return(TANGO_COLOR_BLUE);
	   }
	else
	   { if (ABS(dx) > ABS(dy))
		return(TANGO_COLOR_WHITE);
	     else
		return(TANGO_COLOR_BLACK);
	   }
      }
   else if (dx < 0.0)
      { if (dy < 0.0)
	   { if (ABS(dx) >= ABS(dy))
		return(TANGO_COLOR_ORANGE);
	     else
		return(TANGO_COLOR_RED);
	   }
	else
	   { if (ABS(dx) > ABS(dy))
		return(TANGO_COLOR_YELLOW);
	     else
		return(TANGO_COLOR_GREEN);
	   }
      }
   else /* dx == 0.0 */
      { if (dy < 0.0)
	   return(TANGO_COLOR_RED);
	else if (dy > 0.0)
	   return(TANGO_COLOR_BLUE);
	else
	   return(TANGO_COLOR_WHITE);
      }
}



/**************************************************************/
/* get_color -- Given a string of a TANGO_COLOR, return its   */
/*		integer value.				      */
/* 							      */
/* RETURNS:  Integer value of color-string.		      */
/**************************************************************/
int 
get_color (char *str)
{
   if (strcmp("TANGO_COLOR_WHITE",str) == 0)
      return( TANGO_COLOR_WHITE );
   else if (strcmp("TANGO_COLOR_BLACK",str) == 0)
      return( TANGO_COLOR_BLACK );
   else if (strcmp("TANGO_COLOR_RED",str) == 0)
      return( TANGO_COLOR_RED );
   else if (strcmp("TANGO_COLOR_ORANGE",str) == 0)
      return( TANGO_COLOR_ORANGE );
   else if (strcmp("TANGO_COLOR_YELLOW",str) == 0)
      return( TANGO_COLOR_YELLOW );
   else if (strcmp("TANGO_COLOR_GREEN",str) == 0)
      return( TANGO_COLOR_GREEN );
   else if (strcmp("TANGO_COLOR_BLUE",str) == 0)
      return( TANGO_COLOR_BLUE );
   else if (strcmp("TANGO_COLOR_MAROON",str) == 0)
      return( TANGO_COLOR_MAROON );
   else
      return( TANGO_COLOR_BLACK );
}



/**************************************************************/
/* get_fillstyle -- Given a fill value between 0.0 and 1.0    */
/*		    return the corresponding TANGO fill style.*/
/* 							      */
/* RETURNS:  Corresponding TANGO fill style.		      */
/**************************************************************/
int 
get_fillstyle (double val)
{
   return (val >= 0.0 && val <= 1.0 ? (int)((val * 40.0)+.5) : 40); /* 40 fill */
							      /* styles */
}



/**************************************************************/
/* get_linestyle -- Given a width and a style, return an      */
/*		    TANGO_LINE_STYLE.			      */
/* 							      */
/* RETURNS:  TANGO line style.				      */
/**************************************************************/
TANGO_LINE_STYLE 
get_linestyle (double width, double style)
{
   if (width > 0.666667)
      { if (style < 0.333333)
	   return(TANGO_STYLE_THICKER_DOTTED);
	else if (style < 0.666667)
	   return(TANGO_STYLE_THICKER_DASHED);
	else
	   return(TANGO_STYLE_THICKER);
      }
   else if (width > 0.333333)
      { if (style < 0.333333)
	   return(TANGO_STYLE_THICK_DOTTED);
	else if (style < 0.666667)
	   return(TANGO_STYLE_THICK_DASHED);
	else
	   return(TANGO_STYLE_THICK);
      }
   else
      { if (style < 0.333333)
	   return(TANGO_STYLE_DOTTED);
	else if (style < 0.666667)
	   return(TANGO_STYLE_DASHED);
	else
	   return(TANGO_STYLE_SOLID);
      }
}



/**************************************************************/
/* TANGO_bounding_box -- Get the bounding box coordinates for */
/*			 the given image.		      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_bounding_box (TANGO_IMAGE im, WIN_COORD *lx, WIN_COORD *by, WIN_COORD *rx, WIN_COORD *ty)
{
   switch (im->type)
   { case TANGO_IMAGE_TYPE_LINE:
	line_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_RECTANGLE:
	rectangle_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_CIRCLE:
	circle_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_ELLIPSE:
	ellipse_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_POLYLINE:
	polyline_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_POLYGON:
	polygon_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_SPLINE:
	spline_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_TEXT:
	text_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_BITMAP:
	bmap_bb(im,lx,by,rx,ty);
	break;
     case TANGO_IMAGE_TYPE_COMPOSITE:
	composite_bb(im,lx,by,rx,ty);
	break;
     default:
	*lx = *by = *rx = *ty = 0.0;
    }
}



/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* Routines used to help speed up animations */

/**************************************************************/
/* TANGO_damage_clear_screen -- Redraw all images in background      */
/*			 color.				      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_damage_clear_screen (void)
{
   int clip_right_x, clip_bottom_y;
   XRectangle xrect;

   if (TANGO__data->motionblur) return;  /* used for window dumps; best to
                                            set the var through the debugger */

   if ((TANGO__data->damage_x = 
           (int) floor(((TANGO__data->damlx - TANGO__data->lx) * 
                         TANGO__data->x_WIN_COORD_to_int) + 0.5) - 1)
         < 0) {
     TANGO__data->damage_x = 0;
     TANGO__data->damlx = TANGO__data->lx; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
   }
   else if (TANGO__data->damage_x > TANGO__data->width) {
     TANGO__data->damage_x = TANGO__data->width;
     TANGO__data->damlx = TANGO__data->rx; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
   }

   if ((TANGO__data->damage_y = 
            (int) floor(((TANGO__data->damty - TANGO__data->ty) * 
                       TANGO__data->y_WIN_COORD_to_int) + 0.5) - 1)
        < 0) {
     TANGO__data->damage_y = 0;
     TANGO__data->damty = TANGO__data->ty; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
   }
   else if (TANGO__data->damage_y > TANGO__data->height) {
     TANGO__data->damage_y = TANGO__data->height;
     TANGO__data->damty = TANGO__data->by; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
   }

  if ((clip_right_x = 
            (int) ceil(((TANGO__data->damrx - TANGO__data->lx) * 
                    TANGO__data->x_WIN_COORD_to_int) + 0.5) + 1)
        < 0) {
     TANGO__data->damage_width = (unsigned int) 0;
     TANGO__data->damrx = TANGO__data->lx; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
   }
   else if (clip_right_x > TANGO__data->width) { /* limit the size of */
                                                 /* the damaged region */
     TANGO__data->damage_width = (unsigned int) 
                                 (TANGO__data->width - TANGO__data->damage_x); 
            /* limit it to the visible part of the drawing area */
     TANGO__data->damrx = TANGO__data->rx; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
   }
   else
     TANGO__data->damage_width = (unsigned int) 
                  (clip_right_x - TANGO__data->damage_x);
  
  if ((clip_bottom_y = 
             (int) ceil(((TANGO__data->damby - TANGO__data->ty) * 
                      TANGO__data->y_WIN_COORD_to_int) + 0.5) + 1)
        < 0) {
     TANGO__data->damage_height = 0;
     TANGO__data->damby = TANGO__data->ty; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
  }
  else if (clip_bottom_y > TANGO__data->height) { /* limit the size of */
                                                  /* the damaged region */
     TANGO__data->damage_height = (unsigned int) 
                                 (TANGO__data->height - TANGO__data->damage_y);
           /* limit it to the visible part of the drawing area */
     TANGO__data->damby = TANGO__data->by; 
      /* reset damage area so TANGO_image_intercepts() returns FALSE for */
      /* images outside of visible area, thus TANGO_image_draw() not called */
  }
  else
    TANGO__data->damage_height = (unsigned int) 
                 (clip_bottom_y - TANGO__data->damage_y);


  xrect.x      = (short) TANGO__data->damage_x;
  xrect.y      = (short) TANGO__data->damage_y;
  xrect.width  = (unsigned short) TANGO__data->damage_width;
  xrect.height = (unsigned short) TANGO__data->damage_height;
  XSetClipRectangles(TANGO__data->display, TANGO_gc(), 0, 0, &xrect, 1, 
                           YXBanded);

  TANGO_color(TANGO__data->bgcolor);
  XSetFillStyle(TANGO__data->display, TANGO_gc(), FillSolid);
  XFillRectangle(TANGO__data->display, TANGO__data->pixmap, TANGO_gc(),
		 TANGO__data->damage_x, TANGO__data->damage_y,
		 TANGO__data->damage_width, TANGO__data->damage_height);

   /* If subsequent routines do some X operations where they don't want
      this clip region, make sure to remove it in those routines */
}



/**************************************************************/
/* TANGO_clear_screen -- Redraw all images in background      */
/*			 color.				      */
/* 							      */
/* RETURNS:  None.					      */
/**************************************************************/
void 
TANGO_clear_screen (void)
{
   XRectangle xrect;

  /* Set the clip region and associated data (damage_x,damage_y, 
     damage_width,damage_height) to the size of the offscreen pixmap  */

  TANGO__data->damage_x = TANGO__data->damage_y = 0;
  TANGO__data->damage_width	= (unsigned int)   TANGO__data->width;
  TANGO__data->damage_height	= (unsigned int)   TANGO__data->height;
  xrect.x = xrect.y		= (short) 0;
  xrect.width			= (unsigned short) TANGO__data->width;
  xrect.height			= (unsigned short) TANGO__data->height;
  XSetClipRectangles(TANGO__data->display, TANGO_gc(), 0, 0, &xrect, 1, 
                                     YXBanded);

  TANGO_color(TANGO__data->bgcolor);
  XSetFillStyle(TANGO__data->display, TANGO_gc(), FillSolid);
  XFillRectangle(TANGO__data->display, TANGO__data->pixmap, TANGO_gc(),
		 /* TANGO__data->damage_x */ 0, /* TANGO__data->damage_y */ 0, 
		 TANGO__data->damage_width, TANGO__data->damage_height);
}




/**************************************************************/
/*****************   end of xtangoimage.c    ******************/
/**************************************************************/
