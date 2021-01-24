/******************************************************************
Program:	Boyer-Moore Algorithm Animation
Author:		Wes Hunter
Date:		12 May 92
Description:	This file implements the Boyer-Moore pattern
		matching algorithm.  Combined with the associated
		scene file, it uses the TANGO animation package
		to animate the operation of the algorithm.  The
		only valid symbols allowed in the text and pattern
		are upper and lower case alphabetic characters.
*******************************************************************/


#include "boyer.h"


NAME_FUNCT fcns[] = {{"Init", 1, {VOID, (FPTR)ANIMInit}},
                     {"Compare", 1, {VOID, (FPTR)ANIMCompare}},
                     {"ResetText", 1, {VOID, (FPTR)ANIMResetText}},
                     {"ShowShifts", 1, {VOID, (FPTR)ANIMShowShifts}},
                     {"ShiftPattern", 1, {VOID, (FPTR)ANIMShiftPattern}},
                     {NULL, 0, NULL, NULL}};

char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";


int
max(i1, i2)
   int i1, i2;
{
   if(i1 > i2)
      return(i1);
   return(i2);
}


int *
ComputeLastOccurrence(pattern, patternLength, alpha)
   char *pattern, *alpha;
   int patternLength;
{
   int *lambda;
   int i;

   lambda = (int *) malloc(sizeof(int) * ((int) 'z' + 1));
   for(i = 0; i < strlen(alphabet); i++)
      lambda[(int) (alphabet[i])] = 0;
   for(i = 1; i <= patternLength; i++)
      lambda[(int) (pattern[i-1])] = i;
   return(lambda);
}


int *
ComputePrefix(pattern)
   char *pattern;
{
   int *pi;
   int m;
   int k;
   int q;

   m = strlen(pattern);
   pi = (int *) malloc(sizeof(int) * (m+1));
   pi[1] = 0;
   k = 0;
   for(q = 2; q <= m; q++) {
      while((k > 0) && (pattern[k] != pattern[q-1]))
         k = pi[k];
      if(pattern[k] == pattern[q-1])
         k++;
      pi[q] = k;
   }
   return(pi);
}



char *
ReverseString(string)
   char *string;
{
   char *reverse;
   int i;
   int j;

   j = 0;
   reverse = (char *) malloc(strlen(string) + 1);
   for(i = strlen(string) - 1; i >= 0; i--) {
      reverse[j] = string[i];
      j++;
   }
   reverse[j] = NULL;
   return(reverse);
}



int *
ComputeGoodSuffix(pattern, patternLength)
   char *pattern;
   int patternLength;
{
   int *pi0, *pi1;
   char *reverse;
   int j, l;
   int *gamma;

   pi0 = ComputePrefix(pattern);
   reverse = ReverseString(pattern);
   pi1 = ComputePrefix(reverse);
   free(reverse);
   gamma = (int *) malloc(sizeof(int) * (patternLength + 1));
   for(j = 0; j <= patternLength; j++)
      gamma[j] = patternLength - pi0[patternLength];
   for(l = 1; l <= patternLength; l++) {
      j = patternLength - pi1[l];
      if(gamma[j] > (l - pi1[l]))
         gamma[j] = l - pi1[l];
   }
   free(pi0);
   free(pi1);
   return(gamma);
}



int
BoyerMooreMatch(pattern, text)
   char *pattern, *text;
{
   int n, m, s, j;
   int *lambda, *gamma;

   n = strlen(text);
   m = strlen(pattern);
   lambda = ComputeLastOccurrence(pattern, m, alphabet);
   gamma = ComputeGoodSuffix(pattern, m);
   s = 0;

   TANGOalgoOp(fcns, "Init", pattern, text);
   while(s <= n-m) {
      j = m;
      TANGOalgoOp(fcns, "Compare", j-1, s+j-1, pattern[j-1] == text[s+j-1]);
      while((j > 0) && (pattern[j-1] == text[s+j-1])) {
         j -=1;
         if(j > 0)
            TANGOalgoOp(fcns, "Compare", j-1, s+j-1,
                        pattern[j-1] == text[s+j-1]);
      }
      if(0 == j) {
         free(lambda);
         free(gamma);
         return(s);
      } else {
         TANGOalgoOp(fcns, "ResetText");
         TANGOalgoOp(fcns, "ShowShifts", s+j-1, j-1, gamma[j],
                     j - lambda[text[s+j-1]], m);
         TANGOalgoOp(fcns, "ShiftPattern", 
                     max(gamma[j], j-lambda[text[s+j-1]]), m);
         s += max(gamma[j], j - lambda[text[s+j-1]]);
      }
   }
   free(lambda);
   free(gamma);
   return(-1);
}




int
main(argc, argv)
   int argc;
   char **argv;
{
   char  pattern[MAX_STRING_LEN];
   char  text[MAX_STRING_LEN];
   int i;

   printf("\nThis program uses TANGO to animate the Boyer-Moore");
   printf("\npattern matching algorithm.  For this demonstration,");
   printf("\nthe only allowed symbols in the text and pattern are");
   printf("\nupper and lower case alphabetic characters.");
   printf("\nEnter pattern: ");
   scanf("%s", pattern);
   printf("Enter text: ");
   scanf("%s", text);

   TANGOalgoOp(fcns, "BEGIN"); 
   i = BoyerMooreMatch(pattern, text);
   if(-1 != i)
      printf("\nFound at: %d\n", i);
   else
      printf("\nDid not find the pattern.");
   TANGOalgoOp(fcns, "END"); 
}
