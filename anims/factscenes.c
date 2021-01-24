/* Calculation factorial by Vernard Martin */

#include <stdio.h>
#include "xtango.h"

void
ANIMInit( num )
int num;
{
   TANGO_IMAGE code[10];
   TANGO_IMAGE framebox,NUMtxt,PCtxt;
   TANGO_TRANS trans[10];
   TANGO_TRANS alltrans1,alltrans2;
   TANGO_PATH  path;
   char        str[80];
   int         lcv,temp;

   /* prepare a database of information */
   ASSOCinit();
   ASSOCmake("INVAL",0);
   ASSOCmake("CODE",1);
   ASSOCmake("FRAME",1);
   ASSOCmake("NUMTXT",1);
   ASSOCmake("NUMVAL",1);
   ASSOCmake("PCTXT",1);
   ASSOCmake("PCVAL",1);

   /* store the original programs input value */
   ASSOCstore("INVAL",num);

   /* create text images to display the code in fragments */
   code[0] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.01, 0.05, 1, 
   TANGO_COLOR_BLACK, NULL,"function factorial( num : integer) : integer ;",0);
   code[1] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.01, 0.08, 1, 
       TANGO_COLOR_BLACK, NULL,"begin",0);
   code[2] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.05, 0.11, 1, 
       TANGO_COLOR_BLACK, NULL,"if",0);
   code[3] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.09, 0.11, 1, 
       TANGO_COLOR_BLACK, NULL,"(num = 1)",0);
   code[4] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.26, 0.11, 1, 
       TANGO_COLOR_BLACK, NULL,"then",0);
   code[5] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.09, 0.14, 1, 
       TANGO_COLOR_BLACK, NULL,"factorial := 1",0);
   code[6] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.05, 0.17, 1, 
       TANGO_COLOR_BLACK, NULL,"else",0);
   code[7] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.09, 0.20, 1, 
       TANGO_COLOR_BLACK, NULL,"factorial := num * factorial(num -1)",0);
   code[8] = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.01, 0.23, 1, 
       TANGO_COLOR_BLACK, NULL,"end",0);

   /* store the code fragment images */
   for (lcv = 0; lcv <= 8; lcv++)
     ASSOCstore("CODE",lcv,code[lcv]);

   /* create a null path so that you can do a display only transition */
   path = TANGOpath_null(2);

   /* create a transition that displays the code fragments all at once */
   for (lcv = 0; lcv <= 8; lcv++)
     trans[lcv] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,code[lcv],path);

   /* build a trans so that all code fragments appear at once */
   alltrans1 = TANGOtrans_compose(9,trans[0],trans[1],trans[2],trans[3],
   trans[4],trans[5],trans[6],trans[7],trans[8]);

   /* display the code fragments */
   TANGOtrans_perform(alltrans1);

   /* free up data structures before you exit */
   TANGOpath_free(1,path);
   TANGOtrans_free(11,alltrans1,trans[0],trans[1],trans[2],trans[3],
   trans[4],trans[5],trans[6],trans[7],trans[8]);

}

/* ********************************************************** */
void 
ANIM_SC_Check( num )
   int         num;
{
   int         origvalue;
   double      newy;   
   char        pcvalstr[4],nvalstr[4];
   TANGO_IMAGE framebox, NUMtxt,PCtxt;
   TANGO_IMAGE NUMval,PCval;
   TANGO_IMAGE codefrag;
   TANGO_TRANS frame;
   TANGO_TRANS alltrans,trans[10],codetrans;
   TANGO_PATH  path,delaypath;
   int         lcv;


   codefrag = (TANGO_IMAGE) ASSOCretrieve("CODE",3);
   TANGOtrans_perform(TAPflash( codefrag,10));

   /* create a null path so that you can do a display only transition */
   path = TANGOpath_null(2);
   delaypath = TANGOpath_null(25);
 
   origvalue = ASSOCretrieve("INVAL");
   newy = 0.90 + ( (num - origvalue) * 0.10);

   /* images to draw the first frame */
   framebox = TANGOimage_create(TANGO_IMAGE_TYPE_RECTANGLE, 0.65, newy, 1,
       TANGO_COLOR_BLACK, 0.27, 0.05,0.0);
   NUMtxt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.78, newy+0.03, 1,
       TANGO_COLOR_BLACK, NULL,"num = ",0);
   sprintf(nvalstr,"%d",num);
   NUMval = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.87, newy+0.03, 1,
       TANGO_COLOR_BLACK, NULL,nvalstr,0);

   sprintf(pcvalstr,"%d",1);
   PCtxt = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.66, newy+0.03, 1,
       TANGO_COLOR_BLACK, NULL,"pc = ",0);
   PCval = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, 0.74, newy+0.03, 1,
       TANGO_COLOR_BLACK, NULL,pcvalstr,0);

   /* store the FRAME image pieces */
   ASSOCstore("FRAME",num,framebox);
   ASSOCstore("NUMTXT",num,NUMtxt);
   ASSOCstore("NUMVAL",num,NUMval);
   ASSOCstore("PCTXT",num,PCtxt);
   ASSOCstore("PCVAL",num,PCval);

   /* create a transition that displays the complete frame at once */
   trans[0] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,framebox,path);
   trans[1] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,NUMtxt,path);
   trans[2] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,PCtxt,path);
   trans[3] = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,delaypath);

   /* build a trans so that all code fragments appear at once */
   alltrans = TANGOtrans_compose(4,trans[0],trans[1],trans[2],trans[3]);
 
   /* display the code frame */
   TANGOtrans_perform(alltrans);
 
   /* free up data structures before you exit */
   TANGOpath_free(2,path,delaypath);
   TANGOtrans_free(5,alltrans,trans[0],trans[1],trans[2],trans[3]);
}


/* ********************************************************** */
void 
ANIM_SC_Found()
{
}

/* ********************************************************** */
void 
ANIM_Pop_Stack( num )
  int num;
{
   TANGO_IMAGE framebox, NUMtxt,PCtxt;
   TANGO_IMAGE NUMval,PCval;
   TANGO_TRANS trans[6],alltrans;
   TANGO_PATH  path,delaypath;


   /* retrieve the FRAME image pieces */
   framebox =  (TANGO_IMAGE) ASSOCretrieve("FRAME",num);
   NUMtxt = (TANGO_IMAGE) ASSOCretrieve("NUMTXT",num);
   NUMval = (TANGO_IMAGE) ASSOCretrieve("NUMVAL",num);
   PCtxt  = (TANGO_IMAGE) ASSOCretrieve("PCTXT",num);
   PCval  = (TANGO_IMAGE) ASSOCretrieve("PCVAL",num);

   path = TANGOpath_null(1);
   delaypath = TANGOpath_null(25);

     trans[0] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,framebox,path);
     trans[1] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,NUMtxt,path);
     trans[2] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,NUMval,path);
     trans[3] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,PCtxt,path);
     trans[4] = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE,PCval,path);
     trans[5] = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,delaypath);

   alltrans = TANGOtrans_compose(6,trans[0],trans[1],trans[2],trans[3],
   trans[4],trans[5]);

   /* remove frame from the visible stack */
   TANGOtrans_perform(alltrans);

   TANGOpath_free(1,path);

   TANGOtrans_free(7,trans[0],trans[1],trans[2],trans[3],trans[4],trans[5],
                   alltrans);

}

/* ********************************************************** */
void 
ANIM_Step_Closer()
{
}

/* ********************************************************** */
void 
ANIM_Recurse()
{
}
