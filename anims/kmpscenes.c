/* These functions animate the Knuth-Morris-Pratt string */
/* matching algorithm.  They only animate the scan part  */
/* of the algorithm and not the creation of the finite   */
/* state automata.                                       */

#include <stdio.h>
#include <xtangolocal.h>

#define patID      0	  /* pattern string ID     */
#define subID      1	  /* subject string ID     */
#define maxLength 50      /* maximum string length */


/* Create an association and draw the rectangles */
/* corresponding to pattern and subject          */

void ANIMInit(pattern, subject)
   char pattern[];
   char subject[];
{

#define   SX     0.05     /* starting x location  */
#define   SY     0.9      /* starting y location  */
#define   SIDE   0.9      /* */
#define   HEIGHT 0.1	  /* height of rectangles */

   double       width;	  /* width of rectangles            */
   TANGO_PATH	path;	  /* a null path to display images  */
   TANGO_TRANS  delay;    /* a transition to display images */
   void         SetupStringImages(); /* function to create array images */

/* Create associations for the strings          */

   ASSOCinit();
   ASSOCmake("TEXT", 1);

   width = SIDE /  (double) (strlen(subject) + strlen(pattern));
 
/* set up the string images with each letter in a rectangle */

   SetupStringImages(patID, pattern, width, SY);
   SetupStringImages(subID, subject, width, SY-HEIGHT);

/* draw the two strings with each letter in a rectangle  */
/* pattern will be drawn directly below subject          */

   path = TANGOpath_null(1);
   delay = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(delay);
   TANGOpath_free(1, path);
   TANGOtrans_free(1, delay);
}



/* Function to create array of images and associations for */
/* the text string.                                        */

void SetupStringImages(id, theString, width, yloc)
   int    id;
   char   theString[];
   double width;
   double yloc;

{
   int          len;      /* the length of theString           */
   int          i;        /* loop variable                     */
   TANGO_LOC    center;   /* holds center of rectangle         */
   TANGO_IMAGE  rect;     /* pointer to a string rectangle     */
   TANGO_IMAGE  line;     /* pointer to the text within rect   */
   double       x, y;     /* the window location to place text */
   char         str[2];   /* used to set up text in rectangles */

   struct _IMAGE image; 
   struct _TANGO_TEXT text;
 
   len = strlen(theString);

/* create an array of rectangle images */

   TWISTcreate_image_array(NULL, id, len, TANGO_IMAGE_TYPE_RECTANGLE,
                           1, 0, SX, yloc, NULL, width, NULL, HEIGHT,
                           0.0, 1, TANGO_COLOR_BLUE, 1.0);

/* create the text images to go inside each rectangle */

   str[1] = '\0';
   for (i=0; i<len; i++)
   { 
      rect = (TANGO_IMAGE) ASSOCretrieve("ID", id, i);
      center = TANGOimage_loc(rect, TANGO_PART_TYPE_C);
      TANGOloc_inquire(center, &x, &y);
      str[0] = theString[i];

      image.type =TANGO_IMAGE_TYPE_TEXT;
      image.loc[0] = x;
      image.loc[1] = y;
      image.visible =1;
      text.color=TANGO_COLOR_BLACK;
      text.font_name[0]=0;
      strcpy(text.text,str);
      text.orient=1;
      image.object = &text; 
      line=TANGOimage_create(&image);

//    text = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1, TANGO_COLOR_BLACK, NULL, str, 1);
                               
      ASSOCstore("TEXT", rect, line);
    }
}



/* make two rectangles simultaneously flash */

void ANIMCompare(patn, subn)
   int patn;
   int subn;
{
   TANGO_IMAGE  patrect;  /* pointer to pattern rectangle images   */
   TANGO_IMAGE  subrect;  /* pointer to subject rectangle images   */
   TANGO_PATH   fillpath; /* the path used to flash the rectangles */
   TANGO_PATH   path;     /* temporary path used to create flash   */
   TANGO_TRANS  patfill;  /* transition to flash pattern rectangle */
   TANGO_TRANS  subfill;  /* transition to flash subject rectangle */
   TANGO_TRANS  colortext;/* transition to change color of text    */
   TANGO_TRANS  compose;  /* composition of patfill and subfill    */
   double       flash[2]; /* holds relative path offsets           */

/* get the proper rectangle images for subject and pattern */

   patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, patn);
   subrect = (TANGO_IMAGE) ASSOCretrieve("ID", subID, subn);

/* set the flash path */

   flash[0] = -1.0; /* clear */
   flash[1] =  1.0; /* fill */
   path = TANGOpath_create(2, flash, flash);
   fillpath = TANGOpath_iterate(path, 2.0);

/* create the transitions, compose and perform them */

   patfill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, patrect, fillpath);
   subfill = TANGOtrans_create(TANGO_TRANS_TYPE_FILL, subrect, fillpath);
   compose = TANGOtrans_compose(2, patfill, subfill);
   TANGOtrans_perform(compose);

   TANGOpath_free(2, path, fillpath);
   TANGOtrans_free(3, patfill, subfill, compose);
}



/* the characters match so color them both yellow */

void ANIMMatch(patn, subn)
   int patn;
   int subn;
{
   TANGO_IMAGE  patrect;  /* pointer to pattern rectangle images   */
   TANGO_IMAGE  subrect;  /* pointer to subject rectangle images   */
   TANGO_PATH   path;     /* change color path                     */
   TANGO_TRANS  patcolor; /* transition to change pattern color    */
   TANGO_TRANS  subcolor; /* transition to change subject color    */
   TANGO_TRANS  compose;  /* composition of patcolor and subcolor  */

/* get pointers to pattern and subject rectangle images */

   patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, patn);
   subrect = (TANGO_IMAGE) ASSOCretrieve("ID", subID, subn);

/* set the change color path */

   path = TANGOpath_color(TANGO_COLOR_YELLOW);

/* set the transitions, compose and perform them */

   patcolor = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, patrect, path);
   subcolor = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, subrect, path);
   compose = TANGOtrans_compose(2, patcolor, subcolor);
   TANGOtrans_perform(compose);

   TANGOpath_free(1, path);
   TANGOtrans_free(3, patcolor, subcolor, compose);
}



/* slide the pattern string right n positions      */
/* reset the colors of subject and pattern strings */

void ANIMUpdate(patlen, patn, subn, n)
   int  patlen;
   int  patn;
   int  subn;
   int  n;
{
   int          i;        /* loop index variable                   */
   TANGO_IMAGE  patrect;  /* pointer to Pattern rectangle images   */
   TANGO_IMAGE  subrect;  /* pointer to subject rectangle images   */
   TANGO_IMAGE  pattext;  /* pointer to pattern text image         */
   TANGO_LOC    loc1;     /* from location                         */
   TANGO_LOC    loc2;     /* to location                           */
   TANGO_PATH   path;     /* straight path of one rectangle width  */
   TANGO_PATH   npath;    /* n iterations of path                  */
   TANGO_PATH   colorpath;/* path to change the color of a rect    */
   TANGO_TRANS  colorrect;/* transition to change color of a rect  */
   TANGO_TRANS  moverect; /* transition to move a rect over npath  */
   TANGO_TRANS  movetext; /* transition to move the text over npath*/
   TANGO_TRANS  compose;  /* composition of all the transitions    */

/* set up the n length path to move each rectangle of pattern */

   patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, 0);
   loc1 = TANGOimage_loc(patrect, TANGO_PART_TYPE_SW);
   loc2 = TANGOimage_loc(patrect, TANGO_PART_TYPE_SE);
   pattext = (TANGO_IMAGE) ASSOCretrieve("TEXT", patrect);
   path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
   npath = TANGOpath_iterate(path, (double) n);

/* set up transitions for each rectangle of pattern string */

   moverect = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patrect, npath);
   movetext = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, pattext, npath);
   compose = TANGOtrans_compose(2, moverect, movetext);
   TANGOtrans_free(2, moverect, movetext);

   for (i=1; i<patlen; i++)
   {
      patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, i);
      pattext = (TANGO_IMAGE) ASSOCretrieve("TEXT", patrect);
      moverect = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patrect, npath);
      movetext = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, pattext, npath);
      compose = TANGOtrans_compose(3, compose, moverect, movetext);
      TANGOtrans_free(2, moverect, movetext);
   }

   colorpath = TANGOpath_color(TANGO_COLOR_BLUE);

/* reset proper pattern string rectangles to blue */

   for (i=patn-1; (i>=patn-n-1) && (i>=0); i--)
   {
      patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, i);
      colorrect = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, patrect, colorpath);
                            
      compose = TANGOtrans_compose(2, compose, colorrect);
      TANGOtrans_free(1, colorrect);
   }

/* reset proper subject string rectangles to blue */

   for (i=subn-patn; i<=subn-patn+n-1; i++)
   {
      subrect = (TANGO_IMAGE) ASSOCretrieve("ID", subID, i);
      colorrect = TANGOtrans_create
                              (TANGO_TRANS_TYPE_COLOR, subrect, colorpath);
      compose = TANGOtrans_compose(2, compose, colorrect);
      TANGOtrans_free(1, colorrect);
   }

/* do the transition */

   TANGOtrans_perform(compose);

   TANGOtrans_free(1, compose);
   TANGOpath_free(3, path, npath, colorpath);

}



/* slide the pattern string up onto subject string  */
/* change the color of pattern string to green      */

void ANIMSuccess(patlen)
   int patlen;
{
   int          i;        /* loop index variable                   */
   TANGO_IMAGE  patrect;  /* pointer to pattern rectangle images   */
   TANGO_IMAGE  pattext;  /* pointer to pattern text image         */
   TANGO_LOC    loc1;     /* from location                         */
   TANGO_LOC    loc2;     /* to location                           */
   TANGO_PATH   colorpath;/* path to change color of rect to green */
   TANGO_PATH   path;     /* path to move pattern onto subject     */
   TANGO_PATH   raisepath;/* path to raise pattern to the top      */
   TANGO_TRANS  colorrect;/* transition for the color path         */
   TANGO_TRANS  moverect; /* transition to move rect up            */
   TANGO_TRANS  movetext; /* transition to move text up            */
   TANGO_TRANS  raiserect;/* transition to move rect up            */
   TANGO_TRANS  raisetext;/* transition to move text up            */
   TANGO_TRANS  compose;  /* composition of all transitions        */
   double       raise[1]; /* path array for raising rectangles     */

/* set up paths to follow for success */

   patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, 0);
   loc1 = TANGOimage_loc(patrect, TANGO_PART_TYPE_SW);
   loc2 = TANGOimage_loc(patrect, TANGO_PART_TYPE_NW);
   pattext = (TANGO_IMAGE) ASSOCretrieve("TEXT", patrect);
   colorpath = TANGOpath_color(TANGO_COLOR_GREEN);
   path = TANGOpath_motion(loc1, loc2, TANGO_PATH_TYPE_STRAIGHT);
   raise[0] = 1.0;
   raisepath = TANGOpath_create(1, raise, raise);

/* set up the transitions and compose them for all rectangles */
/* of the pattern string                                      */

   colorrect = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, patrect, colorpath);
   moverect = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patrect, path);
   movetext = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, pattext, path);
   raiserect = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, patrect, raisepath);
   raisetext = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, pattext, raisepath);
   compose = TANGOtrans_compose(5, colorrect, raiserect, raisetext,
                                   moverect, movetext);
   TANGOtrans_free(5, colorrect, raiserect, raisetext, moverect, movetext);

   for (i=1; i<patlen; i++)
   {
      patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, i);
      pattext = (TANGO_IMAGE) ASSOCretrieve("TEXT", patrect);
      colorrect = TANGOtrans_create
                            (TANGO_TRANS_TYPE_COLOR, patrect, colorpath);
      moverect = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patrect, path);
      movetext = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, pattext, path);
      raiserect = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, patrect, raisepath);
      raisetext = TANGOtrans_create(TANGO_TRANS_TYPE_RAISE, pattext, raisepath);
      compose = TANGOtrans_compose(6, compose, colorrect, raiserect, raisetext,
                                      moverect, movetext);
      TANGOtrans_free(5, colorrect, raiserect, raisetext, moverect, movetext);
   }

/* perform the transitions */

   TANGOtrans_perform(compose);

   TANGOtrans_free(1, compose);
   TANGOpath_free(3, colorpath, raisepath, path);
}



/* change unsuccessful pattern string's color to red */

void ANIMFailure(patlen)
   int patlen;
{
   int          i;        /* loop index variable                   */
   TANGO_IMAGE  patrect;  /* pointer to pattern rectangle images   */
   TANGO_PATH   colorpath;/* path to color pattern rects to red    */
   TANGO_TRANS  colorrect;/* transition to color rects of pattern  */
   TANGO_TRANS  compose;  /* composition of color transitions      */

/* set up the color path for the rectangles of pattern */

   patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, 0);
   colorpath = TANGOpath_color(TANGO_COLOR_RED);

/* set up the color transitions for all rectangles */

   colorrect = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, patrect, colorpath);
   compose = TANGOtrans_compose(1, colorrect);
   TANGOtrans_free(1, colorrect);

   for (i=1; i<patlen; i++)
   {
      patrect = (TANGO_IMAGE) ASSOCretrieve("ID", patID, i);
      colorrect = TANGOtrans_create
                            (TANGO_TRANS_TYPE_COLOR, patrect, colorpath);
      compose = TANGOtrans_compose(2, compose, colorrect);
      TANGOtrans_free(1, colorrect);
   }

/* perform the transitions */

   TANGOtrans_perform(compose);

   TANGOtrans_free(1, compose);
   TANGOpath_free(1, colorpath);
}

