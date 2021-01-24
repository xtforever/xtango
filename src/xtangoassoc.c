/**************************************************************/
/*							      */
/*		   ASSOCIATION PACKAGE (assoc)		      */
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
/*****************	 include files       ******************/ 
/**************************************************************/

#include  "xtangolocal.h"

/**************************************************************/
/*****************	LOCAL data types     ******************/
/**************************************************************/

#define 	PRIME	997    /* number to mod by */

typedef struct RECORD_NODE   /* structure to store records */
{
   int key1;
   int key2;
   int key3;
   int key4;
   int key5;
   int record;	 /* universal value, must be cast be receiving routine */
   struct RECORD_NODE *next;
} *RECORD_PTR;


typedef struct TABLE_NODE    /* a hash table */
{
   char name[100];		/* association name */
   int keys;			/* number of keys in this one */
   RECORD_PTR bucket[PRIME];	/* buckets of the hash table */
   struct TABLE_NODE *next;
} *TABLE_PTR;

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

TABLE_PTR  Hash[PRIME];      /* primary hash table structure */

int  ASSOC_init = 0;	     /* set to 1 when ASSOCinit() called */

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/

TABLE_PTR	table_exists();
RECORD_PTR	record_exists();
int             remove_record();
int		same_record();
int		hash_string();
int		hash_keys();
void		assoc_dump();


/***************************************************************/
/*							       */
/*   ASSOCinit - must be called to start-up.  Sets up some     */
/*	basic associations.				       */
/*							       */
/***************************************************************/

void
ASSOCinit()
{
   int i;

   if (ASSOC_init) return;
   ASSOC_init = 1;

   DEBUG("ASSOCinit()\n");

   for (i=0; i<PRIME; ++i)
      Hash[i] = NULL;

   ASSOCmake("ID",2);
   ASSOCmake("ID3",3);
   ASSOCmake("IMAGE_AT",2);
}



/***************************************************************/
/*							       */
/*   ASSOCmake - receive a name of an association to create    */
/*	and the number of keys involved in this association.   */
/*							       */
/***************************************************************/

void
ASSOCmake(name,keys)
   char *name;
   int keys;
{
   TABLE_PTR table;
   int val;
   int index;

   if (!ASSOC_init) ASSOCinit();

   DEBUG("ASSOCmake(\"%s\",%d)\n", name, keys);

   if (table_exists(name))
      return;

   table = (TABLE_PTR) malloc( sizeof( struct TABLE_NODE ) );
   val = hash_string(name);
   strcpy(table->name,name);

/* for (index=0,ptr=name; *ptr; ++index,ptr++)
      table->name[index] = *ptr;
   table->name[index+1] = 0;	 */
   table->keys = keys;
   for (index=0; index<PRIME; ++index)
      table->bucket[index] = NULL;
   table->next = Hash[val];
   Hash[val] = table;
}



/***************************************************************/
/*							       */
/*   ASSOCstore - store a record in a certain association under*/
/*	a set of keys.					       */
/*							       */
/***************************************************************/

void
ASSOCstore(name,p1,p2,p3,p4,p5,p6)
   char *name;
   int p1,p2,p3,p4,p5,p6;
{
   TABLE_PTR table;
   int keys;
   int hash_val;
   RECORD_PTR r;

   if (!ASSOC_init) ASSOCinit();

   DEBUG("ASSOCstore(\"%s\",%d,%d,%d,%d,%d,%d)\n", name, p1,p2,p3,p4,p5,p6);

   if (!(table = table_exists(name)))
      { fprintf(stderr,"Attempt to set association %s which does not exist\n",name);
	return;
      }

   keys = table->keys;
   hash_val = hash_keys(keys,p1,p2,p3,p4,p5);

   if (!(r = record_exists(table,hash_val,p1,p2,p3,p4,p5)))
      { r = (RECORD_PTR) malloc( sizeof( struct RECORD_NODE ) );
	r->next = table->bucket[hash_val];
	table->bucket[hash_val] = r;
      }
   if (keys == 0)
      r->record = p1;
   if (keys > 0)
      { r->key1 = p1;
	r->record = p2;
      }
   if (keys > 1)
      { r->key2 = p2;
	r->record = p3;
      }
   if (keys > 2)
      { r->key3 = p3;
	r->record = p4;
      }
   if (keys > 3)
      { r->key4 = p4;
	r->record = p5;
      }
   if (keys > 4)
      { r->key5 = p5;
	r->record = p6;
      }
}



/***************************************************************/
/*							       */
/*   ASSOCmember - tell if a record has been saved under the   */
/*      given association and keys.     		       */
/*							       */
/***************************************************************/

int 
ASSOCmember(name,p1,p2,p3,p4,p5)
   char *name;
   int p1,p2,p3,p4,p5;
{
   TABLE_PTR t;
   int keys;
   int hash_val;

   if (!ASSOC_init) ASSOCinit();

   DEBUG("ASSOCmember(\"%s\",%d,%d,%d,%d,%d)\n", name, p1,p2,p3,p4,p5);

   if (!(t = table_exists(name)))
      return(0);
   keys = t->keys;
   hash_val = hash_keys(keys,p1,p2,p3,p4,p5);
   if (!record_exists(t,hash_val,p1,p2,p3,p4,p5))
      return(0);
   else 
      return(1);
}



/***************************************************************/
/*							       */
/*   ASSOCdelete - remove a record from the association table  */
/*							       */
/***************************************************************/

int 
ASSOCdelete(name,p1,p2,p3,p4,p5)
   char *name;
   int p1,p2,p3,p4,p5;
{
   TABLE_PTR t;
   int keys;
   int hash_val;

   if (!ASSOC_init) ASSOCinit();

   DEBUG("ASSOCdelete(\"%s\",%d,%d,%d,%d,%d)\n", name, p1,p2,p3,p4,p5);

   if (!(t = table_exists(name)))
      { fprintf(stderr,"Attempt to delete association %s which does not exist\n",name);
	return(0);
      }

   keys = t->keys;
   hash_val = hash_keys(keys,p1,p2,p3,p4,p5);
   if (!remove_record(t,hash_val,p1,p2,p3,p4,p5)) 
      { fprintf(stderr,"Attempt to delete association %s which does not exist\n",name);
        return(0);
      }
   else 
      return(1);
}



/***************************************************************/
/*							       */
/*   ASSOCretrieve - return the record saved under the given   */
/*	association and keys.				       */
/*							       */
/***************************************************************/

int  /* universal ptr */
ASSOCretrieve(name,p1,p2,p3,p4,p5)
   char *name;
   int p1,p2,p3,p4,p5;
{
   TABLE_PTR t;
   RECORD_PTR r;
   int keys;
   int hash_val;

   if (!ASSOC_init) ASSOCinit();

   DEBUG("ASSOCretrieve(\"%s\",%d,%d,%d,%d,%d)\n", name, p1,p2,p3,p4,p5);

   if (!(t = table_exists(name)))
      { fprintf(stderr,"Attempt to get association %s which does not exist\n",name);
	return(0);
      }

   keys = t->keys;
   hash_val = hash_keys(keys,p1,p2,p3,p4,p5);

   if (!(r = record_exists(t,hash_val,p1,p2,p3,p4,p5)))
      { switch (keys)
	   { case 0:
		fprintf(stderr,"No record for assoc %s\n",name);
                if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
 		   assoc_dump();
		break;
	     case 1:
		fprintf(stderr,"No record for assoc %s, keys = (%d)\n",name,p1);
                if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
 		   assoc_dump();
		break;
	     case 2:
		fprintf(stderr,"No record for assoc %s, keys = (%d,%d)\n",name,p1,p2);
                if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
 		   assoc_dump();
		break;
	     case 3:
		fprintf(stderr,"No record for assoc %s, keys = (%d,%d,%d)\n",name,p1,p2,p3);
                if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
 		   assoc_dump();
		break;
	     case 4:
		fprintf(stderr,"No record for assoc %s, keys = (%d,%d,%d,%d)\n",name,p1,p2,p3,p4);
                if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
 		   assoc_dump();
		break;
	     case 5:
		fprintf(stderr,"No record for assoc %s, keys = (%d,%d,%d,%d,%d)\n",name,p1,p2,p3,p4,p5);
                if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
 		   assoc_dump();
		break;
	   }
	return(0);
      }
   else
      return(r->record);
}



/***************************************************************/
/*							       */
/*   ASSOC_clear - clear out the hash table of all association */
/*	relations.					       */
/*							       */
/***************************************************************/

void
ASSOC_clear()
{
   int i,j;
   TABLE_PTR table;
   RECORD_PTR record;

   for (i=0; i<PRIME; ++i)
      { while (Hash[i])
	   { table = Hash[i];
	     for (j=0; j<PRIME; ++j)
		{ while (table->bucket[j])
		     { record = table->bucket[j];
		       table->bucket[j] = table->bucket[j]->next;
		       free(record);
		     }
		  table->bucket[j] = NULL;
		}
	     Hash[i] = Hash[i]->next;
	     free(table);
	   }
	Hash[i] = NULL;
      }

   ASSOC_init = 0;
}



/***************************************************************/
/*							       */
/*   table_exists - return the table of the given association, */
/*	or NULL if it does not exist.			       */
/*							       */
/***************************************************************/

TABLE_PTR
table_exists(name)
   char *name;
{
   int val;
   TABLE_PTR tp;

   val = hash_string(name);
   for (tp = Hash[val]; tp; tp=tp->next)
      if (!(strcmp(name,tp->name)))
	 break;
   return(tp);
}



/***************************************************************/
/*							       */
/*   record_exists - return the record saved under the given   */
/*	keys in the given table if it exists, NULL otherwise.  */
/*							       */
/***************************************************************/

RECORD_PTR
record_exists(tp,hash_val,k1,k2,k3,k4,k5)
   TABLE_PTR tp;
   int hash_val;
   int k1,k2,k3,k4,k5;
{
   RECORD_PTR rp;

   for (rp=tp->bucket[hash_val]; rp; rp=rp->next)
      if (same_record(rp,tp->keys,k1,k2,k3,k4,k5))
	 break;
   return(rp);
}



/***************************************************************/
/*							       */
/*   remove_record - clear a record out of the data base       */
/*							       */
/***************************************************************/

int
remove_record(tp,hash_val,k1,k2,k3,k4,k5)
   TABLE_PTR tp;
   int hash_val;
   int k1,k2,k3,k4,k5;
{
   RECORD_PTR rp,old;

   for (rp=tp->bucket[hash_val]; rp; rp=rp->next) 
      { if (same_record(rp,tp->keys,k1,k2,k3,k4,k5)) 
           { if (rp == tp->bucket[hash_val]) 
                { tp->bucket[hash_val] = rp->next;
                  free(rp);
                }
             else 
 	        { old->next = rp->next;
                  free(rp);
                }
             return(1);
           }
        old = rp;
      }
   return(0);
}



/***************************************************************/
/*							       */
/*   same_record - return 1 if the given RECORD_PTR has the    */
/*	given set of keys.				       */
/*							       */
/***************************************************************/

int
same_record(rp,keys,k1,k2,k3,k4,k5)
   RECORD_PTR rp;
   int keys;
   int k1,k2,k3,k4,k5;
{
   switch(keys)
      {  case 0:
	    return(1);
	 case 1:
	    return((rp->key1 == k1));
	 case 2:
	    return((rp->key1 == k1) && (rp->key2 == k2));
	 case 3:
	    return((rp->key1 == k1) && (rp->key2 == k2) && (rp->key3 == k3));
	 case 4:
	    return((rp->key1 == k1) && (rp->key2 == k2) && (rp->key3 == k3)
		      && (rp->key4 == k4));
	 case 5:
	    return((rp->key1 == k1) && (rp->key2 == k2) && (rp->key3 == k3)
		      && (rp->key4 == k4) && (rp->key5 == k5));
      }
    return(0);
}



/***************************************************************/
/*							       */
/*   hash_string - hash the given string by adding all of its  */
/*	characters as they are mapped to integers	       */
/*							       */
/***************************************************************/

int
hash_string(name)
   char *name;
{
   int val;
   char *ptr;

   for (val=0,ptr=name; *ptr; ++ptr)
      val += (int) *ptr;
   return( val % PRIME );
}



/***************************************************************/
/*							       */
/*   hash_keys - return the hash value of the given number of  */
/*	the given keys. 				       */
/*							       */
/***************************************************************/

int
hash_keys(num,k1,k2,k3,k4,k5)
   int num;
   int k1,k2,k3,k4,k5;
{
   int val;

   val = 0;
   if (num > 0)  val += abs(k1);
   if (num > 1)  val += abs(k2);
   if (num > 2)  val += abs(k3);
   if (num > 3)  val += abs(k4);
   if (num > 4)  val += abs(k5);
   return( val % PRIME );
}



/***************************************************************/
/*							       */
/*   assoc_dump - write out all the current associations.      */
/*							       */
/***************************************************************/

void
assoc_dump()
{
   int i,j;
   TABLE_PTR table;
   RECORD_PTR record;

   fprintf(stderr,"Dumping associations\n");
   for (i=0; i<PRIME; ++i)
      { table = Hash[i];
	while (table)
	   { fprintf(stderr,"\nAssociation %s with keys\n",table->name);
	     for (j=0; j<PRIME; ++j)
		{ record = table->bucket[j];
		  while (record)
		     { switch(table->keys)
		       { case 0:
			    fprintf(stderr,"(No parameters)\n");
			    break;
			 case 1:
			    fprintf(stderr,"%d\n",record->key1);
			    break;
			 case 2:
			    fprintf(stderr,"%d %d\n",record->key1,record->key2);
			    break;
			 case 3:
			    fprintf(stderr,"%d %d %d\n",record->key1,record->key2,record->key3);
			    break;
			 case 4:
			    fprintf(stderr,"%d %d %d %d\n",record->key1,record->key2,
				     record->key3,record->key4);
			    break;
			 case 5:
			    fprintf(stderr,"%d %d %d %d %d\n",record->key1,record->key2,
				     record->key3,record->key4,record->key5);
			    break;
			 default:
			    /* ? */;
		       }
		       record = record->next;
		     }
		  }
	     table = table->next;
	   }
      }
   fprintf(stderr,"\n\n");
}

/**************************************************************/
/*****************   end of xtangoassoc.c    ******************/
/**************************************************************/
