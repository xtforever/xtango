
/* This is the driver program for the Knuth-Morris-Pratt  */
/* string matching algorithm.  There are comments showing */
/* the animation functions to be called from the KMPScan  */
/* function.  Only the KMPScan function is actually begin */
/* animated.                                              */

#include <stdio.h>
#include "xtango.h"

void ANIMInit(), ANIMCompare(), ANIMMatch(), ANIMUpdate(), ANIMSuccess(),
           ANIMFailure();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)ANIMInit}},
			    {"Compare", 1, {VOID, (FPTR)ANIMCompare}},
			    {"Match",   1, {VOID, (FPTR)ANIMMatch}},
			    {"Update",   1, {VOID, (FPTR)ANIMUpdate}},
			    {"Success",   1, {VOID, (FPTR)ANIMSuccess}},
			    {"Failure",   1, {VOID, (FPTR)ANIMFailure}},
			    {NULL,	  0,  NULL, NULL} };


#define maxLength 50
#define true  1
#define false 0

int main ()
{
   char pattern[maxLength], subject[maxLength];
   int fLink[maxLength];
   int KMPScan();
   void ConstructFLink();

/* Input the strings */

   printf("\nEnter pattern string:");
   scanf("%s",pattern);

   printf("\nEnter subject string:");
   scanf("%s",subject);

/* Construct the fLink array */

   TANGOalgoOp(fcn, "BEGIN");
   ConstructFLink(pattern, fLink);

/* Scan subject for an occurance of pattern */

   if (KMPScan(pattern, subject, fLink))
      printf("\nPattern found\n");
   else
      printf("\nPattern not found\n");

   TANGOalgoOp(fcn, "END");
}

/* ConstructFLink creates the fLink array used in the */
/* KMPScan function.                                  */

void ConstructFLink(pattern, fLink)
   char pattern[];
   int  fLink[];
{
   int  i, j, l;

   fLink[1] = 0;
   i = 2;
   l = strlen(pattern);
   while (i <= l)
   {
      j = fLink[i - 1];
      while ((j != 0) && (pattern[j-1] != pattern[i - 2]))
	 j = fLink[j];
      fLink[i] = j + 1;
      i = i + 1;
   }
}

/* KMPScan algorithm scans the subject string to find */
/* an occurance of the pattern string.  It uses the   */
/* fLink array to resolve which characters of pattern */
/* and subject to compare next.                       */

int KMPScan(pattern, subject, fLink)
   char pattern[], subject[];
   int  fLink[];
{
   int i, j, subl, patl;
   int theSame;

   i = 1;
   j = 1;
   patl = strlen(pattern);
   subl = strlen(subject);

/* Init(pattern, subject)                 */
/* stores and draws pattern and subject   */

   TANGOalgoOp(fcn, "Init", pattern, subject);

   while (i <= subl) 
   {
      theSame = false;
      while ((j != 0) && !(theSame))
      {

/* Compare(j-1, i-1)                   */
/* flash pattern[j-1] and subject[i-1] */

   TANGOalgoOp(fcn, "Compare", j-1, i-1);

         if (pattern[j-1] == subject[i-1])
         {

/* Match(j-1, i-1)                               */
/* color pattern[j-1] and subject[i-1] to yellow */

   TANGOalgoOp(fcn, "Match", j-1, i-1);

            theSame = true;
         }
         else
         {

/* Update(patl, j, i, j-fLink[j])                        */
/* Move pattern (j - fLink[j]) blocks to the right       */
/* Change (j - fLink[j]) blocks of subject back to blue  */
/* Change (j - fLink[j]) blocks of pattern back to blue  */

   TANGOalgoOp(fcn, "Update", patl, j, i, j-fLink[j]);

            j = fLink[j];
         }
      }
      if (j == patl)
      {

/* Success(patl)                         */
/* Change all colors of pattern to green */
/* Move pattern onto subject blocks      */

   TANGOalgoOp(fcn, "Success", patl);

         return(true);
      }
      else
      {   
	 i = i + 1;
	 j = j + 1;
      }
   }

/* Failure(patl)                   */
/* Change colors of pattern to red */

   TANGOalgoOp(fcn, "Failure", patl);

   return(false);
}
