/**********************************
 TPOPLOGICAL SORT (topsortscenes.c)

    by Marin Simina
 **********************************/

#include <stdio.h>
#include <xtangolocal.h>


double fillval = 1.0;
double unfillval = -1.0;
#define  SIZE	     0.04
#define  XTEXT       0.1
#define  YTEXT       0.05
#define  STEP        0.005



/* create location and adj matrix associations */

void
ANIMInit()
{
   ASSOCinit();
   ASSOCmake("ADJ",3);
   ASSOCmake("TEXT_AT",2);
}




/* save the locations of the vertices */

void
ANIMInput_loc(id,index,x,y)
   int id,index;
   double x,y;
{
   TANGO_LOC loc;

   loc = TANGOloc_create(x,y);
   ASSOCstore("ID",id,index,loc);
}

/* save the final locations of the vertices in the 
   topologically sorted representation */

void
ANIMSorted_loc(id,index,x,y)
   long id,index;
   double x,y;
{
   TANGO_LOC loc;

   loc = TANGOloc_create(x,y);
   ASSOCstore("ID",id,index,loc);
}
/* save the final PERMUTATED locations of the vertices in the 
   topologically sorted representation */

void
ANIMPsorted_loc(id,index,x,y)
   long id,index;
   double x,y;
{
   TANGO_LOC loc;

   loc = TANGOloc_create(x,y);
   ASSOCstore("ID",id,index,loc);
}



/* save the entries in the adjacency matrix */

void
ANIMInput_adj(id,from,to,edge)
   int id;
   int from,to;
   int edge;
{
   ASSOCstore("ADJ",id,from,to,edge);
}




/* recalling the locations of the vertices and the adjacency matrix, draw */
/* the graph								  */

void
ANIMDraw(id,num,adjid)
   long id;
   int num;
   int adjid;
{
   int i,j;

   TANGO_IMAGE im, txt;
   TANGO_LOC  pt[50];
   int adjmat[50][50];
   TANGO_PATH path;
   TANGO_TRANS trans;

   struct _IMAGE image; 
   struct _TANGO_TEXT text;
   struct _TANGO_CIRCLE circ;  

   image.type = TANGO_IMAGE_TYPE_TEXT;
   image.loc[0] = XTEXT;
   image.loc[1] = YTEXT;
   image.visible =1;
   text.color=TANGO_COLOR_BLACK;
   text.font_name[0]=0;
   strcpy(text.text,"TOPOLOGICAL SORT");
   text.orient=0;
   image.object = &text; 
   txt=TANGOimage_create(&image);

//   txt= TANGOimage_create (, , ,, TANGO_COLOR_BLACK,NULL, "TOPOLOGICAL SORT", 0);
			   
			   
   for (i=0; i<num; ++i) /* create array of TANGO_LOCS for locations */
      pt[i] = (TANGO_LOC) ASSOCretrieve("ID",id,i);

   for (i=0; i<num; ++i)  /* create the adjacency matrix */
      for (j=0; j<num; ++j)
	 adjmat[i][j] = (int) ASSOCretrieve("ADJ",adjid,i,j);

   TWISTcreate_graph(NULL,id,num,pt,adjmat,TANGO_IMAGE_TYPE_RECTANGLE,
		     SIZE,0.0);

   ASSOCmake("lozenge",0);  /* make the lozenge that moves around the graph */

   image.type = TANGO_IMAGE_TYPE_CIRCLE;
   image.loc[0] = 0.1;
   image.loc[1] = 0.1;
   image.visible = 0;
   circ.color = TANGO_COLOR_BLACK;
   circ.radius = SIZE/2.0;
   circ.fill = 1;
   image.object = &circ;   
   im=TANGOimage_create(&image); 

//   im = TANGOimage_create(,0.1,0.1,0,TANGO_COLOR_BLACK,SIZE/2.0,1.0);
			  
   ASSOCstore("lozenge",im);

   path = TANGOpath_null(4);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}


/* beginning of a new component, make the lozenge visible, and change the */
/* vertex to indicate it's been visited                                   */

void
ANIMNew_comp(id,vnum,val)
   long id,vnum,val;
{
   double x,y;
   char str[5];
   TANGO_IMAGE	 loz,image,new,txt;
   //   TANGO_LOC	 atpt,topt,center;
   TANGO_LOC	 atpt,topt;
   TANGO_PATH	 movepath,onepath,colpath,fillpath;
   TANGO_TRANS	 move,appear,t[2],change,fill,all;
   loz = (TANGO_IMAGE) ASSOCretrieve("lozenge");
   atpt = TANGOimage_loc(loz,TANGO_PART_TYPE_C);
   topt = (TANGO_LOC) ASSOCretrieve("ID",id,vnum);
   movepath = TANGOpath_distance(atpt,topt,1.0);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,loz,movepath);
     /* note, still invisible */
   fillpath = TANGOpath_create(1,&fillval,&fillval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,loz,fillpath);
   onepath = TANGOpath_null(1);     /* make visible */
   appear = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,loz,onepath);
   image = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,topt);
   sprintf(str,"%ld",val);
   TANGOloc_inquire(topt,&x,&y);

   struct _IMAGE ximage; 
   struct _TANGO_TEXT text;
   ximage.type = TANGO_IMAGE_TYPE_TEXT;
   ximage.loc[0] = x;
   ximage.loc[1] = y;
   ximage.visible =1;
   text.color=TANGO_COLOR_BLACK;
   strcpy(text.font_name,"variable");
   strcpy(text.text,str);
   text.orient=1;
   ximage.object = &text; 
   txt=TANGOimage_create(&ximage);
 
 //   txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,"variable",str,1);
			    
   ASSOCstore("TEXT_AT",id,topt,txt);
   colpath = TANGOpath_color(TANGO_COLOR_GREEN);
   t[0] = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,image,fillpath);
   t[1] = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,image,colpath);
   change = TANGOtrans_compose(2,t[0],t[1]);
	/* alter the vertex */

   all = TANGOtrans_concatenate(4,move,fill,appear,change);
   TANGOtrans_perform(all);

   TANGOpath_free(4,movepath,onepath,colpath,fillpath);
   TANGOtrans_free(7,move,appear,t[0],t[1],change,fill,all);
}




/* move the lozenge to the given vertex and change the vertex's type */

void
ANIMVisit(id,vnum,order)
   int id,vnum,order;
{
   double x,y;
   char str[5];
   TANGO_IMAGE	 loz,image,new,txt;
   TANGO_LOC	 atpt,topt,center;
   TANGO_PATH	 movepath,onepath,colpath,fillpath;
   TANGO_TRANS	 move,t[2],change,fill,all;

   loz = (TANGO_IMAGE) ASSOCretrieve("lozenge");

   fillpath = TANGOpath_create(1,&fillval,&fillval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,loz,fillpath);

   atpt = TANGOimage_loc(loz,TANGO_PART_TYPE_C);
   topt = (TANGO_LOC) ASSOCretrieve("ID",id,vnum);
   movepath = TANGOpath_distance(atpt,topt, STEP);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,loz,movepath);

   image = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,topt);
   sprintf(str,"%d",order);
   TANGOloc_inquire(topt,&x,&y);

   struct _IMAGE im; 
   struct _TANGO_TEXT text;
   im.type = TANGO_IMAGE_TYPE_TEXT;
   im.loc[0] = x;
   im.loc[1] = y;
   im.visible =1;
   text.color=TANGO_COLOR_BLACK;
   strcpy(text.font_name,"variable");
   strcpy(text.text,str);
   text.orient=1;
   im.object = &text; 
   txt=TANGOimage_create(&im);

//   txt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,x,y,1,TANGO_COLOR_BLACK,"variable",str,1);
			    
   ASSOCstore("TEXT_AT",id,topt,txt);
   onepath = TANGOpath_null(1);
   colpath = TANGOpath_color(TANGO_COLOR_GREEN);
   t[0] = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,image,fillpath);
   t[1] = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,image,colpath);
   change = TANGOtrans_compose(2,t[0],t[1]);

   all = TANGOtrans_concatenate(3,fill,move,change);
   TANGOtrans_perform(all);

   TANGOpath_free(4,movepath,onepath,colpath,fillpath);
   TANGOtrans_free(6,move,t[0],t[1],change,fill,all);
}





/* make the lozenge proceed back to the given vertex */

void
ANIMBacktrack(id,vnum)
   int id,vnum;
{
   TANGO_IMAGE	 loz;
   TANGO_LOC	 atpt,topt;
   TANGO_PATH	 path,fillpath;
   TANGO_TRANS	 move,fill;

   loz = (TANGO_IMAGE) ASSOCretrieve("lozenge");

   fillpath = TANGOpath_create(1,&unfillval,&unfillval);
   fill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL,loz,fillpath);
   TANGOtrans_perform(fill);

   atpt = TANGOimage_loc(loz,TANGO_PART_TYPE_C);
   topt = (TANGO_LOC) ASSOCretrieve("ID",id,vnum);
   path = TANGOpath_distance(atpt,topt, STEP);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,loz,path);
   TANGOtrans_perform(move);

   TANGOpath_free(2,path,fillpath);
   TANGOtrans_free(2,move,fill);
}





/* all the descendants of this vertex have been visited */
/* so change its color to indicate so			*/

void
ANIMDone(id,vnum, pred, tsid, vts)
   long id,vnum, pred, tsid, vts;
{
   double lx, ly;
   char str[5];
   TANGO_LOC   loc, ploc, tsloc;
   TANGO_IMAGE vertex,text, vnew, tnew;
   TANGO_PATH  path1,path2, tspath;
   TANGO_TRANS color1,color2,raise1,raise2,all, vmove, tmove, move;

   loc = (TANGO_LOC) ASSOCretrieve("ID",id,vnum);
   vertex = (TANGO_IMAGE) ASSOCretrieve("IMAGE_AT",id,loc);
   text = (TANGO_IMAGE) ASSOCretrieve("TEXT_AT",id,loc);
   path1 = TANGOpath_color(TANGO_COLOR_BLUE);
   color1 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,vertex,path1);
   raise1 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,vertex,path1);
   path2 = TANGOpath_color(TANGO_COLOR_WHITE);
   color2 = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR,text,path2);
   raise2 = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE,text,path2);
   all = TANGOtrans_compose(4,color1,raise1,color2,raise2);
   TANGOtrans_perform(all);

   ploc  = (TANGO_LOC) ASSOCretrieve("ID", id, pred);
   tsloc = (TANGO_LOC) ASSOCretrieve("ID", tsid, vts);
   lx= TANGOloc_X(loc); 
   ly= TANGOloc_Y(loc);

   struct _IMAGE image; 
   struct _TANGO_RECTANGLE rec;
   image.type = TANGO_IMAGE_TYPE_RECTANGLE;
   image.loc[0] = lx;
   image.loc[1] = ly;
   image.visible = 1;
   rec.color=TANGO_COLOR_RED;
   rec.size[0]=SIZE;
   rec.size[1]=SIZE;
   rec.fill = 1;
   image.object = &rec;   
   vnew=TANGOimage_create(&image); 
  
//   vnew= TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 
//			   lx, ly,
//			   TANGO_COLOR_RED, SIZE, SIZE, 1.0);
   sprintf(str, "%ld", vnum);

   struct _TANGO_TEXT xtext;
   image.type = TANGO_IMAGE_TYPE_TEXT;
   image.loc[0] = lx;
   image.loc[1] = ly;
   image.visible =1;
   xtext.color=TANGO_COLOR_BLACK;
   xtext.font_name[0]=0;
   strcpy(xtext.text,str);
   xtext.orient=1;
   image.object = &xtext; 
   tnew=TANGOimage_create(&image);
 
   //   tnew= TANGOimage_create(TANGO_IMAGE_TYPE_TEXT,
   //			   lx, ly, 1,
   //			   TANGO_COLOR_BLACK, NULL, str ,1);
   tspath= TANGOpath_distance(loc, tsloc, STEP);
   vmove= TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, vertex, tspath);
   tmove= TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, text, tspath);
   move=  TANGOtrans_compose(2, vmove, tmove);
   TANGOtrans_perform(move);

   TANGOpath_free(3,path1,path2, tspath);
   TANGOtrans_free(6,color1,raise1,color2,raise2, vmove, tmove);
}


/* recalling the locations of the vertices and the adjacency matrix,      */
/* redraw the graph						       	  */

#define YOFFSET 0.4

void
ANIMRedraw(id,num,adjid)
   int id;
   int num;
   int adjid;
{
   int i,j;
   //   double sx, sy, dx, dy, vx[4], vy[4], yoffset, loc;
   double sx, sy, dx, dy,  yoffset, loc;
   TANGO_IMAGE spl;
   TANGO_PATH path;
   TANGO_TRANS color, raise, all;
   TANGO_LOC  pt[50];
   int adjmat[50][50];


   for (i=0; i<num; ++i) /* create array of TANGO_LOCS for locations */
      pt[i] = (TANGO_LOC) ASSOCretrieve("ID",id,i);

   for (i=0; i<num; ++i)  /* create the adjacency matrix */
      for (j=0; j<num; ++j)
	 adjmat[i][j] = (int) ASSOCretrieve("ADJ",adjid,i,j);

   for(i=0; i<num; ++i)
     for(j= i+ 1; j< num; ++j)
       if (adjmat[i][j] == 1)      /*edge from i to j */
	 { if (i % 2 == 0)
	     { yoffset= YOFFSET;
	       loc= SIZE/ 2;
	     }
	   else
	     { yoffset= -YOFFSET;
	       loc= -SIZE/ 2;
	     }
	   yoffset= yoffset*(j-i)/num;

	   sx= TANGOloc_X(pt[i]);
	   sy= TANGOloc_Y(pt[i]) + loc;
	   dx= TANGOloc_X(pt[j]);
	   dy= TANGOloc_Y(pt[j]) + loc;
#if 0
	   vx[0]= (dx- sx)/3.0;
	   vy[0]= yoffset;
	   vx[1]= (dx- sx)*2.0/3.0;
	   vy[1]= yoffset;
	   vx[2]= (dx- sx);
	   vy[2]= 0.0;
#endif

	   struct _IMAGE image; 
	   struct _TANGO_POLYLINE poly;
	   image.type = TANGO_IMAGE_TYPE_POLYLINE;
	   image.loc[0] = sx;
	   image.loc[1] = sy;
	   image.visible = 1;
	   poly.color=TANGO_COLOR_BLACK;
	   poly.vertices=3;
	   poly.vertex[0][0]=(dx- sx)/3.0;
	   poly.vertex[0][1]=yoffset;
	   poly.vertex[1][0]=(dx- sx)*2.0/3.0;
	   poly.vertex[1][1]=yoffset;
	   poly.vertex[2][0]=dx-sx;
	   poly.vertex[2][1]=0.0;
	   poly.width=0.3;
	   poly.style=1.0;
	   poly.arrow=1;
	   image.object = &poly;   
	   spl=TANGOimage_create(&image); 
 
//	   spl= TANGOimage_create(TANGO_IMAGE_TYPE_POLYLINE, sx, sy,
//				  1, TANGO_COLOR_BLACK, 4, vx, vy,
//				  0.3, 1.0, 1);
	   path= TANGOpath_color(TANGO_COLOR_RED);
	   color= TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, spl,path);
	   raise= TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, spl, path);
	   all= TANGOtrans_compose(2, color, raise);
	   TANGOtrans_perform(all);
	   TANGOpath_free(1, path);
	   TANGOtrans_free(1, color, raise);
	   
	 }
}



/* the current connected component is ending, so make the */
/* lozenge invisible					  */

void
ANIMEnd_comp()
{
   TANGO_IMAGE loz;
   TANGO_PATH  path;
   TANGO_TRANS vis;

   loz = (TANGO_IMAGE) ASSOCretrieve("lozenge");
   path = TANGOpath_null(1);	  /* make it invisible */
   vis = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,loz,path);
   TANGOtrans_perform(vis);

   TANGOpath_free(1,path);
   TANGOtrans_free(1,vis);
}


