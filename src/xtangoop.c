/**************************************************************/
/*							      */
/*		XTANGO ANIMATION PACKAGE (op)		      */
/*							      */
/**************************************************************/
/*	Copyright 1990 Georgia Institute of Technology	      */
/*		John T. Stasko and Doug Hayes		      */
/**************************************************************/
/*							      */
/* Modification Date  Description			      */
/* -----------------  --------------------------------------- */
/* 90/10/26 JDH	      Baselined source.                       */
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

#define FUNCT_NOT_FOUND    -1
#define FUNCT_INIT         -2
#define FUNCT_ALREADY_INIT -3
#define FUNCT_END          -4

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

static int find_function();

/**************************************************************/
/*****************	  entry points       ******************/
/**************************************************************/

char *TANGOalgoOp();


/**************************************************************/
/* TANGOalgoOp -- Call all functions on the named function-   */
/*		  list and return the appropriate type from   */
/*		  the last function called.		      */
/* 							      */
/* RETURNS:  Appropriate type from last function called.      */
/**************************************************************/
char *
TANGOalgoOp(nmfnc, fname, p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,
			  p16,p17,p18,p19,p20)
   NAME_FUNCT nmfnc[];
   char *fname;
   long p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20;
{
   char *retval = NULL;
   int  i,j;

   if (TANGO__data)
      DEBUG("TANGOalgoOp(0x%p,\"%s\",%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld...)\n", 
	 nmfnc, fname, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);

   //   printf("fname=%s\n",fname);
   switch (i = find_function(nmfnc, fname)) {
      case FUNCT_NOT_FOUND    : COMPLAIN("ERROR: TANGOalgoOp: %s (\"%s\")\n",
					 "unknown function name", fname); break;
      case FUNCT_INIT	      : TANGOinit(); break;
      case FUNCT_ALREADY_INIT : COMPLAIN("WARNING: TANGOalgoOp: BEGIN %s\n",
					 "called again"); break;
      case FUNCT_END	      : TANGOend(); break;
      default		      :
	 for (j=0; j < nmfnc[i].numfunctions; j++)
	    switch (nmfnc[i].funct[j].functype) {
	       case VOID     : (*nmfnc[i].funct[j].function)(p1,p2,p3,p4,p5,p6,
						  p7,p8,p9,p10,p11,p12,p13,p14,
						  p15,p16,p17,p18,p19,p20);
			       retval = (char *)NULL;
			       break;
	       case CHAR_PTR : retval =   (char *)(*nmfnc[i].funct[j].function)(p1,p2,
						p3,p4,p5,p6,p7,p8,p9,p10,p11,
						p12,p13,p14,p15,p16,17,p18,
						p19,p20);
			       break;
	       default       : COMPLAIN("ERROR: TANGOalgoOP: %s (\"%d\")\n",
					"Unknown functType",
					nmfnc[i].funct[j].functype);
	       }
      }


   return retval;
}



/**************************************************************/
/* find_function -- Find function-list corresponding to name. */
/* 							      */
/* RETURNS:  TRUE(1) if found or FALSE(0).		      */
/**************************************************************/
static int
find_function(nmfnc, fname)
   NAME_FUNCT nmfnc[];
   char *fname;
{
   int i, found;

   if (!strcmp(fname, "BEGIN"))
      return (TANGO__data ? FUNCT_ALREADY_INIT : FUNCT_INIT);
   else if (!strcmp(fname, "END"))
      return FUNCT_END;
   else  /* NULL terminates function list...should probably check all NULLs */
      for (i=found=0; !found && *(nmfnc[i].functname); i += (!found ? 1 : 0))
         found = !strcmp(fname, nmfnc[i].functname);

   return (found ? i : FUNCT_NOT_FOUND);
}

/**************************************************************/
/*****************     end of xtangoop.c     ******************/
/**************************************************************/
