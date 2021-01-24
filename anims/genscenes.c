/*

                     Animation of a Genetic Algorithm
                                   by
                              David Brogan
*/

#include <stdio.h>
#include <math.h>
#include "xtango.h"

void ANIMInitCities (cities)
int cities[40];
{
  TANGO_LOC loc,loc1, loc2;
  TANGO_IMAGE label;
  TANGO_PATH path;
  TANGO_TRANS trans;
  char str[4];
  int count;
  double x, y, x1, x2, y1, y2;
  double vxlist[1];
  double vylist[1];

  ASSOCinit ();
  ASSOCmake ("locs",1);
  ASSOCmake ("results", 1);
  ASSOCmake ("Soln_images", 2);
  ASSOCmake ("results_loc", 1);
  ASSOCmake ("results_image", 1);
  ASSOCmake ("ims",1);
  ASSOCmake ("genes_loc",2);
  ASSOCmake ("genes", 2);
  ASSOCmake ("genes_image", 2);
  

  for (count = 0; count < 10; count++)
  {
    x = (double) cities[count];
    y = (double) cities[count+10];
    x = x / 1000;
    y = y/1000;
    printf ("x[%d]: %f    y[%d]: %f\n", count, x, count, y);
 
    loc =TANGOloc_create (x,y);
    ASSOCstore ("locs", count, loc);
    sprintf (str, "%d", count);
    label = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x, y,1,TANGO_COLOR_BLACK, "variable",str,1);
    path = TANGOpath_null (1);
    trans = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, label, path);
    TANGOtrans_perform(trans);
  }
  TANGOpath_free (1,path);
  TANGOtrans_free (1,trans);
}  


void ANIMDrawPaths (tour)
int tour[10];
{
  TANGO_LOC loc1, loc2;
  TANGO_IMAGE label;
  TANGO_PATH path;
  TANGO_TRANS trans;
  double x1, y1, x2, y2;
  double vxlist[1], vylist[1];
  int count;

  for (count = 0; count < 9; count++)
  {
    loc1 = (TANGO_LOC) ASSOCretrieve ("locs", tour[count]);
    loc2 = (TANGO_LOC) ASSOCretrieve ("locs", tour[count + 1]);
    TANGOloc_inquire (loc1, &x1, &y1);
    TANGOloc_inquire (loc2, &x2, &y2);
    vxlist[0] = x2-x1;
    if (vxlist[0] > 0) {
      x1 = x1 + .01;
      vxlist[0] = vxlist[0] -.02; }
    else if (vxlist[0] < 0) {
      x1 = x1 - .01;
      vxlist[0] = vxlist[0] + .02; }


    vylist[0] = y2-y1;
    if (vylist[0] > 0) {
      y1 = y1 + .01;
      vylist[0] = vylist[0] - .02; }
    else if (vylist[0] < 0) {
      y1 = y1 - .01;
      vylist[0] = vylist[0] + .02; }

    label = TANGOimage_create (TANGO_IMAGE_TYPE_POLYLINE, x1, y1, 1, TANGO_COLOR_BLACK,2, vxlist, vylist, 0.5, 1.0, 0);
    ASSOCstore ("ims", count, label);
    path = TANGOpath_null (1);
    trans = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, label, path);
    TANGOtrans_perform (trans);

  }
  loc1 = (TANGO_LOC) ASSOCretrieve ("locs", tour[9]);
  loc2 = (TANGO_LOC) ASSOCretrieve ("locs", tour[0]);
  TANGOloc_inquire (loc1, &x1, &y1);
  TANGOloc_inquire (loc2, &x2, &y2);
  vxlist[0] = x2-x1;
  if (vxlist[0] > 0) {
    x1 = x1 + .01;
    vxlist[0] = vxlist[0] -.02; }
  else if (vxlist[0] < 0) {
    x1 = x1 - .01;
    vxlist[0] = vxlist[0] + .02; }


  vylist[0] = y2-y1;
  if (vylist[0] > 0) {
    y1 = y1 + .01;
    vylist[0] = vylist[0] - .02; }
  else if (vylist[0] < 0) {
    y1 = y1 - .01;
    vylist[0] = vylist[0] + .02; }

  label = TANGOimage_create (TANGO_IMAGE_TYPE_POLYLINE, x1, y1, 1, TANGO_COLOR_BLACK,2, vxlist, vylist, 0.5, 1.0, 0);
  ASSOCstore ("ims", 9, label);
  path = TANGOpath_null (1);
  trans = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, label, path);
  TANGOtrans_perform (trans);

  TANGOpath_free (1,path);
  TANGOtrans_free (1,trans);
}

void ANIMRemovePaths ()
{
  int count;
  TANGO_LOC loc;
  TANGO_IMAGE label;
  TANGO_PATH path;
  TANGO_TRANS trans;

  for (count = 0; count < 9; count++)
  {
    label = (TANGO_IMAGE) ASSOCretrieve ("ims", count);
    path = TANGOpath_null (1);
    trans = TANGOtrans_create (TANGO_TRANS_TYPE_DELETE, label, path);
    TANGOtrans_perform (trans);

  }
  label = (TANGO_IMAGE) ASSOCretrieve ("ims", 9);
  path = TANGOpath_null (1);
  trans = TANGOtrans_create (TANGO_TRANS_TYPE_DELETE, label, path);
  TANGOtrans_perform (trans);
  TANGOpath_free (1,path);
  TANGOtrans_free (1,trans);
}



void ANIMShowGenes (chrom1, pop_count)
int chrom1[10];
int pop_count;

{
  double x, y, space = 0.04;
  int chrom_count;
  TANGO_LOC loc;
  TANGO_IMAGE label;
  TANGO_PATH path;
  TANGO_TRANS trans;
  char str[10];

  y = 0.70 + (space * pop_count);
  
    x = 0.05;
    for (chrom_count = 0; chrom_count < 10; chrom_count ++)
    {
      sprintf (str, "%d", chrom1[chrom_count]);
      loc = TANGOloc_create (x,y);
      ASSOCstore ("genes", pop_count, chrom_count, chrom1[chrom_count]);
      ASSOCstore ("genes_loc", pop_count, chrom_count,loc);
       switch (pop_count) {
          case 0 : {label = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x, y, 1, TANGO_COLOR_RED, "variable", str, 1); break;}
          case 1 : {label = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x, y, 1, TANGO_COLOR_GREEN, "variable", str, 1); break;}
          case 2 : {label = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x, y, 1, TANGO_COLOR_BLUE, "variable", str, 1); break;}
          default : {label = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x, y, 1, TANGO_COLOR_BLACK, "variable", str, 1);break;}
      
        }

      ASSOCstore ("genes_image", pop_count, chrom_count, label);
      path = TANGOpath_null (1);
      trans = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, label, path);
      TANGOtrans_perform (trans);
      x = x + space;
    }
  TANGOpath_free (1, path);
  TANGOtrans_free (1, trans);
}   




void ANIMMoveBest (best)
int best;
{
int i, value;
double x1, x2, y1, y2, space = .04;
char str[10];
TANGO_LOC loc;
TANGO_IMAGE element[10];
TANGO_PATH path[10];
TANGO_TRANS trans[10];
TANGO_TRANS finalGeneTrans;

  for (i = 0; i< 10; i++)
  {
    value = (int) ASSOCretrieve ("genes", best, i);
    sprintf (str, "%d", value);
    loc = (TANGO_LOC) ASSOCretrieve ("genes_loc", best, i);
    TANGOloc_inquire (loc, &x1, &y1);
    y2 = 0.70;
    x2 = .5 + x1;
    element[i] = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x1, y1, 1, TANGO_COLOR_BLACK, "variable", str, 1);
    ASSOCstore ("Soln_images", 0, i, element[i]);
    path [i] = TANGOpath_motion (TANGOloc_create (x1, y1), TANGOloc_create (x2, y2), TANGO_PATH_TYPE_STRAIGHT);
    trans[i] = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, element[i], path[i]);
  }
  finalGeneTrans = TANGOtrans_compose (10, trans[0], trans[1], trans[2], trans[3], trans[4], trans[5], trans[6], trans[7], trans[8], trans[9]);
  TANGOtrans_perform (finalGeneTrans);
  for (i = 0; i < 10; i ++)
  {
    TANGOpath_free (1, path[i]);
    TANGOtrans_free (1, trans[i]);
  }

}


void ANIMDropDown (gene, from_pos, to_pos)
int gene;
int from_pos;
int to_pos;
{
int i, value;
double x1, x2, y1, y2, space = .04;
char str[10];
TANGO_LOC loc;
TANGO_IMAGE element;
TANGO_PATH path;
TANGO_TRANS trans;

  value = (int) ASSOCretrieve ("genes", gene, from_pos);
  ASSOCstore ("results", to_pos, value);
  sprintf (str, "%d", value);
  loc = (TANGO_LOC) ASSOCretrieve ("genes_loc", gene, from_pos);
  TANGOloc_inquire (loc, &x1, &y1);
  y2 = .9;
  x2 = .05 + (to_pos * .04);
  switch (gene) {
    case 0 : {element = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x1, y1, 1, TANGO_COLOR_RED, "variable", str, 1); break;}
    case 1 : {element = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x1, y1, 1, TANGO_COLOR_GREEN, "variable", str, 1); break;}
    case 2 : {element = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x1, y1, 1, TANGO_COLOR_BLUE, "variable", str, 1); break;}
    default : {element = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x1, y1, 1, TANGO_COLOR_BLACK, "variable", str, 1);break;}

  }
  ASSOCstore ("results_image", to_pos, element);
  loc = TANGOloc_create (x2, y2);
  ASSOCstore ("results_loc", to_pos, loc);
  path  = TANGOpath_motion (TANGOloc_create (x1, y1), TANGOloc_create (x2,y2), TANGO_PATH_TYPE_STRAIGHT);
  trans = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, element, path);
  TANGOtrans_perform (trans);
  TANGOpath_free (1, path);
  TANGOtrans_free (1, trans);
}


void ANIMMutate (one, two)
int one;
int two;
{
int i, value1, value2;
double x1, x2, y1, y2, space = .04;
char str1[10], str2[10];
TANGO_LOC loc1, loc2;
TANGO_IMAGE element1, element2, old_elm1, old_elm2;
TANGO_PATH path1, path2;
TANGO_TRANS trans1, trans2, comb_trans;

  value1 = (int) ASSOCretrieve ("results", one);
  sprintf (str1, "%d", value1);
value2 = (int) ASSOCretrieve ("results", two);
  sprintf (str2, "%d", value2);
  loc1 = (TANGO_LOC) ASSOCretrieve ("results_loc", one);
  TANGOloc_inquire (loc1, &x1, &y1);
  loc2 = (TANGO_LOC) ASSOCretrieve ("results_loc", two); 
  TANGOloc_inquire (loc2, &x2, &y2);
  old_elm1 = (TANGO_IMAGE) ASSOCretrieve ("results_image", one);
  old_elm2 = (TANGO_IMAGE) ASSOCretrieve ("results_image", two);
  trans1 = TANGOtrans_create (TANGO_TRANS_TYPE_DELETE, old_elm1, TANGOpath_null(1));
  trans2 = TANGOtrans_create (TANGO_TRANS_TYPE_DELETE, old_elm2, TANGOpath_null(1));
  TANGOtrans_perform (trans1);
  TANGOtrans_perform (trans2);
  
  element1 = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x1, y1, 1, TANGO_COLOR_MAROON, "variable", str1, 1);
  element2 = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x2, y2, 1, TANGO_COLOR_MAROON, "variable", str2, 1);
  ASSOCstore ("results_image", one, element2);
  ASSOCstore ("results_image", two, element1);
  path1 = TANGOpath_motion (loc1, loc2, TANGO_PATH_TYPE_CLOCKWISE);
  path2 = TANGOpath_motion (loc2, loc1, TANGO_PATH_TYPE_COUNTERCLOCKWISE);
  trans1 = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, element1, path1);
  trans2 = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, element2, path2);
  comb_trans = TANGOtrans_compose (2, trans1, trans2);
  TANGOtrans_perform (comb_trans);
  TANGOpath_free (1, path1);
  TANGOpath_free (1, path2);
  TANGOtrans_free (1, trans1);
  TANGOtrans_free (1, trans2);
}


void ANIMRemoveResults ()
{
TANGO_LOC loc;
TANGO_IMAGE element[10];
TANGO_PATH path[10];
TANGO_TRANS trans[10];
TANGO_TRANS finalGeneTrans;
int i;

  for (i = 0; i < 10; i++)
  {
    element[i] = (TANGO_IMAGE) ASSOCretrieve ("results_image", i);
    path [i] = TANGOpath_null (1);
    trans [i] = TANGOtrans_create (TANGO_TRANS_TYPE_DELETE, element[i], path[i])
;
  }
  finalGeneTrans = TANGOtrans_compose (10, trans[0], trans[1], trans[2], trans[3
], trans[4], trans[5], trans[6], trans[7], trans[8], trans[9]);
  TANGOtrans_perform (finalGeneTrans);
  for (i = 0; i < 10; i ++)
  {
    TANGOpath_free (1, path[i]);
    TANGOtrans_free (1, trans[i]);
  }
}


void ANIMMoveResults (pos)
int pos;
{
  int i, value;
double x1, x2, y1, y2, space = .04;
char str[10];
TANGO_LOC loc;
TANGO_IMAGE element[10];
TANGO_PATH path[10];
TANGO_TRANS trans[10];
TANGO_TRANS finalGeneTrans;

  for (i = 0; i< 10; i++)
  {
    value = (int) ASSOCretrieve ("results", i);
    sprintf (str, "%d", value);
    loc = (TANGO_LOC) ASSOCretrieve ("results_loc", i);
    TANGOloc_inquire (loc, &x1, &y1);
    y2 = 0.70 + (.04 * pos);
    x2 = .5 + x1;
    element[i] = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x1, y1, 1, TANGO_COLOR_BLACK, "variable", str, 1);
    ASSOCstore ("Soln_images", pos, i, element[i]);
    path [i] = TANGOpath_motion (TANGOloc_create (x1, y1), TANGOloc_create (x2,y2), TANGO_PATH_TYPE_STRAIGHT);
    trans[i] = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, element[i], path[i]);
  }
  finalGeneTrans = TANGOtrans_compose (10, trans[0], trans[1], trans[2], trans[3], trans[4], trans[5], trans[6], trans[7], trans[8], trans[9]);
  TANGOtrans_perform (finalGeneTrans);
  for (i = 0; i < 10; i ++)
  {
    TANGOpath_free (1, path[i]);
    TANGOtrans_free (1, trans[i]);
  }

}
 
void ANIMRemoveSolution (gene)
int gene;
{
TANGO_LOC loc;
TANGO_IMAGE element[10];
TANGO_PATH path[10];
TANGO_TRANS trans[10];
TANGO_TRANS finalGeneTrans;
int i;

  for (i = 0; i < 10; i++)
  {
    element[i] = (TANGO_IMAGE) ASSOCretrieve ("Soln_images", gene, i);
    path [i] = TANGOpath_null (1);
    trans [i] = TANGOtrans_create (TANGO_TRANS_TYPE_DELETE, element[i], path[i]);
  }
  finalGeneTrans = TANGOtrans_compose (10, trans[0], trans[1], trans[2], trans[3], trans[4], trans[5], trans[6], trans[7], trans[8], trans[9]);
  TANGOtrans_perform (finalGeneTrans);
  for (i = 0; i < 10; i ++)
  {
    TANGOpath_free (1, path[i]);
    TANGOtrans_free (1, trans[i]);
  }
}

void ANIMRemoveGene (chrom1, pop_count)
int chrom1[10];
int pop_count;

{
  double x, y, space = 0.04;
  int chrom_count;
  TANGO_LOC loc;
  TANGO_IMAGE label;
  TANGO_PATH path;
  TANGO_TRANS trans;
  char str[10];

  y = 0.70 + (space * pop_count);

    x = 0.05;
    for (chrom_count = 0; chrom_count < 10; chrom_count ++)
    {
      sprintf (str, "%d", chrom1[chrom_count]);
      loc = TANGOloc_create (x,y);
      label = TANGOimage_create (TANGO_IMAGE_TYPE_TEXT, x, y, 1,TANGO_COLOR_WHITE, "variable", str, 1);

      path = TANGOpath_null (1);
      trans = TANGOtrans_create (TANGO_TRANS_TYPE_MOVE, label, path);
      TANGOtrans_perform (trans);
      x = x + space;
    }
  TANGOpath_free (1, path);
  TANGOtrans_free (1, trans);
}


