/******************************************************************
Program:        Boyer-Moore Algorithm Animation
Author:         Wes Hunter
Date:           12 May 92
Description:    This file implements the animation scenes for the
		Boyer-Moore pattern matching algorithm.  Combined
		with the associated algorithm file, it uses the
		TANGO animation package to animate the operation
		of the algorithm.  The only valid symbols allowed
		in the text and pattern are upper and lower case
		alphabetic characters.
*******************************************************************/


#include "boyer.h"

BOOL textReset[MAX_STRING_LEN];
BOOL patternReset[MAX_STRING_LEN];

void
BuildStringImage(stringID, string, charWidth, bottom)
   int stringID;
   char *string;
   double charWidth, bottom;
{
   int stringLength;
   char s[2];
   int i;
   TANGO_IMAGE charBox;
   TANGO_LOC center;
   double x, y;
   TANGO_IMAGE charImage;

   stringLength = strlen(string);
   TWISTcreate_image_array(NULL, stringID, stringLength, 
                           TANGO_IMAGE_TYPE_RECTANGLE, 1, 0, LEFT, bottom, 
                           NULL, charWidth, NULL, LINE_HEIGHT, 0.0, 1, 
                           TANGO_COLOR_YELLOW, 1.0);
   s[1] = NULL;
   for(i = 0; i < stringLength; i++) {
      charBox = (TANGO_IMAGE) ASSOCretrieve("ID", stringID, i);
      center = TANGOimage_loc(charBox, TANGO_PART_TYPE_C);
      TANGOloc_inquire(center, &x, &y);
      s[0] = string[i];
      charImage = TANGOimage_create(TANGO_IMAGE_TYPE_TEXT, x, y, 1,
                                    TANGO_COLOR_BLACK, "variable", s, 1);
      ASSOCstore("TEXT", charBox, charImage);
   }
}   

void
ANIMInit(pattern, text)
   char *pattern, *text;
{
   double         charWidth;
   TANGO_PATH     path;
   TANGO_TRANS    trans;
   int i;

   ASSOCinit();
   ASSOCmake("TEXT", 1);
   charWidth = LINE_WIDTH / (double) max(strlen(pattern), strlen(text));
   BuildStringImage(PATTERN_ID, pattern, charWidth, BOTTOM);
   BuildStringImage(TEXT_ID, text, charWidth, BOTTOM - (3 * LINE_HEIGHT));
   path = TANGOpath_null(1);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1, path);
   TANGOtrans_free(1, trans);
   for(i = 0; i < MAX_STRING_LEN; i++) {
      textReset[i] = FALSE;
      patternReset[i] = FALSE;
   }
}



void
ANIMCompare(patternIndex, textIndex, equal)
   int patternIndex, textIndex;
   BOOL equal;
{
   TANGO_IMAGE    patternBox;
   TANGO_LOC      patternStart;
   TANGO_LOC      stop;
   TANGO_IMAGE    patternChar;
   TANGO_PATH     patternPath;
   TANGO_TRANS    moveBox;
   TANGO_TRANS    moveChar;
   TANGO_TRANS    compose;
   TANGO_IMAGE    textBox;
   TANGO_LOC      textStart;
   TANGO_PATH     textPath;
   TANGO_IMAGE    textChar;
   TANGO_PATH     colorPath;
   TANGO_TRANS    colorTrans;
   TANGO_PATH     reversePattern;
   TANGO_PATH     reverseText;

   patternBox = (TANGO_IMAGE) ASSOCretrieve("ID", PATTERN_ID, patternIndex);
   patternStart = TANGOimage_loc(patternBox, TANGO_PART_TYPE_S);
   stop = TANGOloc_create(TANGOloc_X(patternStart), 
                          TANGOloc_Y(patternStart) - (1.5 * LINE_HEIGHT));
   patternChar = (TANGO_IMAGE) ASSOCretrieve("TEXT", patternBox);
   patternPath = TANGOpath_motion(patternStart, stop, TANGO_PATH_TYPE_STRAIGHT);
   moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patternBox, patternPath);
   moveChar =TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patternChar, patternPath);
   compose = TANGOtrans_compose(2, moveBox, moveChar);
   TANGOtrans_free(2, moveBox, moveChar);

   textBox = (TANGO_IMAGE) ASSOCretrieve("ID", TEXT_ID, textIndex);
   textStart = TANGOimage_loc(textBox, TANGO_PART_TYPE_S);
   textPath = TANGOpath_motion(textStart, stop, TANGO_PATH_TYPE_STRAIGHT);
   textChar = (TANGO_IMAGE) ASSOCretrieve("TEXT", textBox);
   moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, textBox, textPath);
   moveChar = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, textChar, textPath);
   compose = TANGOtrans_compose(3, compose, moveBox, moveChar);

   TANGOtrans_perform(compose);
   TANGOtrans_free(3, compose, moveBox, moveChar);

   if(equal) {
      textReset[textIndex] = TRUE;
      patternReset[patternIndex] = TRUE;
      colorPath = TANGOpath_color(TANGO_COLOR_GREEN);
      compose =TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, patternBox, colorPath);
      colorTrans =TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, textBox, colorPath);
      compose = TANGOtrans_compose(2, compose, colorTrans);
      TANGOtrans_perform(compose);
      TANGOtrans_free(2, compose, colorTrans);
      TANGOpath_free(1, colorPath);
   } else {
      reversePattern = TANGOpath_reverse(patternPath);
      reverseText = TANGOpath_reverse(textPath);
      moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patternBox, 
                                  reversePattern);
      moveChar = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patternChar, 
                                   reversePattern);
      compose = TANGOtrans_compose(2, moveBox, moveChar);
      TANGOtrans_free(2, moveBox, moveChar);
      moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, textBox, reverseText);
      moveChar =TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, textChar, reverseText);
      compose = TANGOtrans_compose(3, compose, moveBox, moveChar);
      TANGOtrans_perform(compose);
      TANGOtrans_free(3, compose, moveBox, moveChar);
      TANGOpath_free(2, reverseText, reversePattern);
   }

   TANGOpath_free(2, patternPath, textPath);
}



void
ANIMResetText()
{
   int i;
   TANGO_IMAGE box, charImage;
   TANGO_LOC start, stop;
   TANGO_PATH path;
   TANGO_TRANS moveBox, moveChar, compose;

   compose = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, TANGOpath_null(1));
   for(i = 0; i < MAX_STRING_LEN; i++) {
      if(textReset[i]) {
         textReset[i] = FALSE;
         box = (TANGO_IMAGE) ASSOCretrieve("ID", TEXT_ID, i);
         start = TANGOimage_loc(box, TANGO_PART_TYPE_S);
         stop = TANGOloc_create(TANGOloc_X(start), 
                                TANGOloc_Y(start) - (1.5 * LINE_HEIGHT));
         charImage = (TANGO_IMAGE) ASSOCretrieve("TEXT", box);
         path = TANGOpath_motion(start, stop, TANGO_PATH_TYPE_STRAIGHT);
         moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, box, path);
         moveChar = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, charImage, path);
         compose = TANGOtrans_compose(3, compose, moveBox, moveChar);
         TANGOtrans_free(2, moveBox, moveChar);
         TANGOpath_free(1, path);
         path = TANGOpath_color(TANGO_COLOR_YELLOW);
         moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, box, path);
         compose = TANGOtrans_compose(2, compose, moveBox);
         TANGOtrans_free(1, moveBox);
         TANGOpath_free(1, path);
      }

      if(patternReset[i]) {
         patternReset[i] = FALSE;;
         box = (TANGO_IMAGE) ASSOCretrieve("ID", PATTERN_ID, i);
         start = TANGOimage_loc(box, TANGO_PART_TYPE_S);
         stop = TANGOloc_create(TANGOloc_X(start), 
                                TANGOloc_Y(start) + (1.5 * LINE_HEIGHT));
         charImage = (TANGO_IMAGE) ASSOCretrieve("TEXT", box);
         path = TANGOpath_motion(start, stop, TANGO_PATH_TYPE_STRAIGHT);
         moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, box, path);
         moveChar = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, charImage, path);
         compose = TANGOtrans_compose(3, compose, moveBox, moveChar);
         TANGOtrans_free(2, moveBox, moveChar);
         TANGOpath_free(1, path);
         path = TANGOpath_color(TANGO_COLOR_YELLOW);
         moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, box, path);
         compose = TANGOtrans_compose(2, compose, moveBox);
         TANGOtrans_free(1, moveBox);
         TANGOpath_free(1, path);
      }
   }

   TANGOtrans_perform(compose);
   TANGOtrans_free(1, compose);
}



void
ANIMShowShifts(textIndex, patternIndex, goodSuffixOffset, badCharOffset, 
               patternLength)
   int textIndex, patternIndex, badCharOffset, patternLength, goodSuffixOffset;
{
   TANGO_IMAGE textMarker, image, patternMarker;
   TANGO_LOC top, stop;
   double startY, startX, charWidth;
   TANGO_TRANS move, compose;
   int i;
   TANGO_PATH path;

   /* display the bad character markers */
   image = (TANGO_IMAGE) ASSOCretrieve("ID", TEXT_ID, textIndex);
   top = TANGOimage_loc(image, TANGO_PART_TYPE_N);
   startY = TANGOloc_Y(top) - LINE_HEIGHT;
   startX = TANGOloc_X(top);
   textMarker = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, startX, startY,
                                  TRUE, TANGO_COLOR_BLACK, 0.0, LINE_HEIGHT,
                                  0.0, 1.0, 1);

   image = (TANGO_IMAGE) ASSOCretrieve("ID", PATTERN_ID, patternIndex);
   top = TANGOimage_loc(image, TANGO_PART_TYPE_N);
   startY = TANGOloc_Y(top) - LINE_HEIGHT;
   startX = TANGOloc_X(top);
   patternMarker = TANGOimage_create(TANGO_IMAGE_TYPE_LINE, startX, startY,
                                     TRUE, TANGO_COLOR_BLACK, 0.0, LINE_HEIGHT,
                                     0.0, 1.0, 1);

   /* move the bad marker in the pattern */
   charWidth = TANGOloc_X(TANGOimage_loc(image, TANGO_PART_TYPE_E)) - 
               TANGOloc_X(TANGOimage_loc(image, TANGO_PART_TYPE_W));
   stop = TANGOloc_create(TANGOloc_X(top) - (badCharOffset * charWidth),
                          startY);
   path = TANGOpath_motion(TANGOloc_create(startX, startY), stop, 
                           TANGO_PATH_TYPE_STRAIGHT);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, patternMarker, path);
   TANGOtrans_perform(move);
   TANGOtrans_free(1, move);

   /* color the good suffix characters */
   compose = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, TANGOpath_null(1));
   path = TANGOpath_color(TANGO_COLOR_GREEN);
   for(i = textIndex+1; i < textIndex + patternLength - patternIndex; i++) {
      image = (TANGO_IMAGE) ASSOCretrieve("ID", TEXT_ID, i);
      move = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, image, path);
      compose = TANGOtrans_compose(2, compose, move);
      TANGOtrans_free(1, move);
   }
   for(i = max(0, patternIndex - goodSuffixOffset + 1); 
                i < patternLength - goodSuffixOffset; i++) {
      image = (TANGO_IMAGE) ASSOCretrieve("ID", PATTERN_ID, i);
      move = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, image, path);
      compose = TANGOtrans_compose(2, compose, move);
      TANGOtrans_free(1, move);
   }
   TANGOtrans_perform(compose);
   TANGOtrans_free(1, compose);
   TANGOpath_free(1, path);

   /* flash longest path..... */
   if(goodSuffixOffset > badCharOffset) {
      compose = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, 
                                  TANGOpath_null(1));
      path = TANGOpath_null(32);
      for(i = textIndex+1; i < textIndex + patternLength - patternIndex; i++) {
         image = (TANGO_IMAGE) ASSOCretrieve("ID", TEXT_ID, i);
         move = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, image, path);
         compose = TANGOtrans_compose(2, compose, move);
         TANGOtrans_free(1, move);
      }
      for(i = max(0, patternIndex - goodSuffixOffset + 1); 
                  i < patternLength - goodSuffixOffset; i++) {
         image = (TANGO_IMAGE) ASSOCretrieve("ID", PATTERN_ID, i);
         move = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, image, path);
         compose = TANGOtrans_compose(2, compose, move);
         TANGOtrans_free(1, move);
      }
      TANGOtrans_perform(compose);
      TANGOtrans_free(1, compose);
      TANGOpath_free(1, path);
   } else {
      compose = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, patternMarker, 
                                  TANGOpath_null(32));
      move = TANGOtrans_create(TANGO_TRANS_TYPE_VISIBLE, textMarker, 
                               TANGOpath_null(32));
      compose = TANGOtrans_compose(2, compose, move);
      TANGOtrans_perform(compose);
      TANGOtrans_free(2, compose, move);
   }

   /* fix the colors.... */
   compose = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, TANGOpath_null(1));
   path = TANGOpath_color(TANGO_COLOR_YELLOW);
   for(i = textIndex + 1; i < textIndex + patternLength - patternIndex; i++) {
      image = (TANGO_IMAGE) ASSOCretrieve("ID", TEXT_ID, i);
      move = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, image, path);
      compose = TANGOtrans_compose(2, compose, move);
      TANGOtrans_free(1, move);
   }
   for(i = max(0, patternIndex - goodSuffixOffset + 1); 
             i < patternLength - goodSuffixOffset; i++) {
      image = (TANGO_IMAGE) ASSOCretrieve("ID", PATTERN_ID, i);
      move = TANGOtrans_create(TANGO_TRANS_TYPE_COLOR, image, path);
      compose = TANGOtrans_compose(2, compose, move);
      TANGOtrans_free(1, move);
   }

   /* erase the arrows.... */
   move = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, patternMarker, path);
   compose = TANGOtrans_compose(2, compose, move);
   TANGOtrans_free(1, move);
   move = TANGOtrans_create(TANGO_TRANS_TYPE_DELETE, textMarker, path);
   compose = TANGOtrans_compose(2, compose, move);
   TANGOtrans_perform(compose);
   TANGOtrans_free(1, compose);
   TANGOpath_free(1, path);
}



void
ANIMShiftPattern(charOffsets, patternLength)
   int charOffsets, patternLength;
{
   TANGO_IMAGE image, box, charImage;
   double charWidth;
   int i;
   TANGO_PATH path;
   TANGO_LOC start, stop;
   TANGO_TRANS compose, moveBox, moveChar;

   image = (TANGO_IMAGE) ASSOCretrieve("ID", TEXT_ID, 1);
   charWidth = TANGOloc_X(TANGOimage_loc(image, TANGO_PART_TYPE_E)) -
               TANGOloc_X(TANGOimage_loc(image, TANGO_PART_TYPE_W));
   compose = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY, NULL, TANGOpath_null(1));
   for(i = 0; i < patternLength; i++) {
      box = (TANGO_IMAGE) ASSOCretrieve("ID", PATTERN_ID, i);
      charImage = (TANGO_IMAGE) ASSOCretrieve("TEXT", box);
      start = TANGOimage_loc(box, TANGO_PART_TYPE_S);
      stop = TANGOloc_create(TANGOloc_X(start) + (charWidth * charOffsets),
                             TANGOloc_Y(start));
      path = TANGOpath_motion(start, stop, TANGO_PATH_TYPE_STRAIGHT);
      moveBox = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, box, path);
      moveChar = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE, charImage, path);
      compose = TANGOtrans_compose(3, compose, moveBox, moveChar);
      TANGOtrans_free(2, moveBox, moveChar);
      TANGOpath_free(1, path);
   }
   TANGOtrans_perform(compose);
   TANGOtrans_free(1, compose);
}
