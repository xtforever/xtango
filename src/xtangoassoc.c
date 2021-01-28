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

#define 	PRIME	997	/* number to mod by */

typedef struct RECORD_NODE {	/* structure to store records */
	long key1;
	long key2;
	long key3;
	long key4;
	long key5;
	int record;		/* universal value, must be cast be receiving routine */
	struct RECORD_NODE *next;
} *RECORD_PTR;


typedef struct TABLE_NODE {	/* a hash table */
	char name[100];		/* association name */
	long keys;		/* number of keys in this one */
	RECORD_PTR bucket[PRIME];	/* buckets of the hash table */
	struct TABLE_NODE *next;
} *TABLE_PTR;

/**************************************************************/
/*****************	GLOBAL variables     ******************/
/**************************************************************/

TABLE_PTR Hash[PRIME];		/* primary hash table structure */

int ASSOC_init = 0;		/* set to 1 when ASSOCinit() called */

/**************************************************************/
/*****************	LOCAL variables      ******************/
/**************************************************************/

/**************************************************************/
/*****************      LOCAL functions      ******************/
/**************************************************************/
#if 0
TABLE_PTR table_exists();
RECORD_PTR record_exists();
int remove_record();
int same_record();
int hash_string();
int hash_keys();
void assoc_dump();
#endif

extern TABLE_PTR table_exists(char *name);
int hash_string(char *name);

extern RECORD_PTR record_exists(TABLE_PTR tp, int hash_val, long k1, long k2, long k3, long k4, long k5);
                                  int remove_record(TABLE_PTR tp, int hash_val, long k1, long k2, long k3, long k4, long k5);
                                        int same_record(RECORD_PTR rp, long keys, long k1, long k2, long k3, long k4, long k5);
                                                                                       int hash_keys(int num, long k1, long k2, long k3, long k4, long k5);
void assoc_dump(void);


/***************************************************************/
/*							       */
/*   ASSOCinit - must be called to start-up.  Sets up some     */
/*	basic associations.				       */
/*							       */
/***************************************************************/

void ASSOCinit(void)
{
	int i;

	if (ASSOC_init)
		return;
	ASSOC_init = 1;

	DEBUG("ASSOCinit()\n");

	for (i = 0; i < PRIME; ++i)
		Hash[i] = NULL;

	ASSOCmake("ID", 2);
	ASSOCmake("ID3", 3);
	ASSOCmake("IMAGE_AT", 2);
}



/***************************************************************/
/*							       */
/*   ASSOCmake - receive a name of an association to create    */
/*	and the number of keys involved in this association.   */
/*							       */
/***************************************************************/

void ASSOCmake(char *name, int keys)
{
	TABLE_PTR table;
	int val;
	int index;

	if (!ASSOC_init)
		ASSOCinit();

	DEBUG("ASSOCmake(\"%s\",%d)\n", name, keys);

	if (table_exists(name))
		return;

	table = (TABLE_PTR) malloc(sizeof(struct TABLE_NODE));
	val = hash_string(name);
	strcpy(table->name, name);

/* for (index=0,ptr=name; *ptr; ++index,ptr++)
      table->name[index] = *ptr;
   table->name[index+1] = 0;	 */
	table->keys = keys;
	for (index = 0; index < PRIME; ++index)
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

void ASSOCstore(char *name, long k1, long k2, long k3, long k4, long k5, long k6)
{
	TABLE_PTR table;
	int keys;
	int hash_val;
	RECORD_PTR r;

	if (!ASSOC_init)
		ASSOCinit();

	DEBUG("ASSOCstore(\"%s\",%ld,%ld,%ld,%ld,%ld,%ld)\n", name, k1, k2, k3,k4, k5, k6);
	      
	table = table_exists(name);
	if (!table) {
		fprintf(stderr,"Attempt to set association %s which does not exist\n",name);
		return;
	}

	keys = table->keys;
	hash_val = hash_keys(keys,k1, k2, k3,k4, k5);

	r = record_exists(table, hash_val,k1, k2, k3,k4, k5);
	if (!r) {
		r = (RECORD_PTR) malloc(sizeof(struct RECORD_NODE));
		r->next = table->bucket[hash_val];
		table->bucket[hash_val] = r;
	}
	if (keys == 0)
		r->record = k1;
	if (keys > 0) {
		r->key1 = k1;
		r->record = k2;
	}
	if (keys > 1) {
		r->key2 = k2;
		r->record = k3;
	}
	if (keys > 2) {
		r->key3 = k3;
		r->record = k4;
	}
	if (keys > 3) {
		r->key4 = k4;
		r->record = k5;
	}
	if (keys > 4) {
		r->key5 = k5;
		r->record = k6;
	}
}



/***************************************************************/
/*							       */
/*   ASSOCmember - tell if a record has been saved under the   */
/*      given association and keys.     		       */
/*							       */
/***************************************************************/

int ASSOCmember(char *name, long k1, long k2, long k3, long k4, long k5)
{
	TABLE_PTR t;
	int keys;
	int hash_val;

	if (!ASSOC_init)
		ASSOCinit();

	DEBUG("ASSOCmember(\"%s\",%ld,%ld,%ld,%ld,%ld)\n", name, k1, k2, k3,k4, k5);
	      
	t = table_exists(name);
	if (!t)
		return (0);
	keys = t->keys;
	hash_val = hash_keys(keys, k1, k2, k3,k4, k5);
	if (!record_exists(t, hash_val, k1, k2, k3,k4, k5))
		return (0);
	else
		return (1);
}



/***************************************************************/
/*							       */
/*   ASSOCdelete - remove a record from the association table  */
/*							       */
/***************************************************************/

int ASSOCdelete(char *name, long k1, long k2, long k3, long k4, long k5)
{
	TABLE_PTR t;
	int keys;
	int hash_val;

	if (!ASSOC_init)
		ASSOCinit();

	DEBUG("ASSOCdelete(\"%s\",%ld,%ld,%ld,%ld,%ld)\n", name, k1, k2, k3,k4, k5);
	      
	t = table_exists(name);
	if (!t) {
		fprintf(stderr,"Attempt to delete association %s which does not exist\n",name);
		return (0);
	}

	keys = t->keys;
	hash_val = hash_keys(keys, k1, k2, k3,k4, k5);

	if (!remove_record(t, hash_val, k1, k2, k3,k4, k5)) {
		fprintf(stderr,"Attempt to delete association %s which does not exist\n",name);			
		return (0);
	} else
		return (1);
}



/***************************************************************/
/*							       */
/*   ASSOCretrieve - return the record saved under the given   */
/*	association and keys.				       */
/*							       */
/***************************************************************/

long  ASSOCretrieve(char *name, long k1, long k2, long k3, long k4, long k5)
{
	TABLE_PTR t;
	RECORD_PTR r;
	int keys;
	int hash_val;

	if (!ASSOC_init)
		ASSOCinit();

	DEBUG("ASSOCretrieve(\"%s\",%ld,%ld,%ld,%ld,%ld)\n", name, k1, k2, k3,k4, k5);
	      
	t = table_exists(name);
	if (!t) {
		fprintf(stderr,"Attempt to get association %s which does not exist\n",name);
		return (0);
	}

	keys = t->keys;
	hash_val = hash_keys(keys, k1, k2, k3, k4, k5);
	r = record_exists(t, hash_val, k1, k2, k3, k4, k5);
	if (!r) {
		switch (keys) {
		case 0:
			fprintf(stderr, "No record for assoc %s\n", name);
			if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
				assoc_dump();
			break;
		case 1:
			fprintf(stderr,
				"No record for assoc %s, keys = (%ld)\n",name, k1);
			if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
				assoc_dump();
			break;
		case 2:
			fprintf(stderr,"No record for assoc %s, keys = (%ld,%ld)\n",name, k1, k2);
			if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
				assoc_dump();
			break;
		case 3:
			fprintf(stderr,
				"No record for assoc %s, keys = (%ld,%ld,%ld)\n",
				name, k1, k2, k3);
			if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
				assoc_dump();
			break;
		case 4:
			fprintf(stderr,
				"No record for assoc %s, keys = (%ld,%ld,%ld,%ld)\n",
				name, k1, k2, k3, k4);
			if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
				assoc_dump();
			break;
		case 5:
			fprintf(stderr,
				"No record for assoc %s, keys = (%ld,%ld,%ld,%ld,%ld)\n",
				name, k1, k2, k3, k4, k5);
			if (TANGO__data->debug & TANGO_DEBUG_EXTERNAL)
				assoc_dump();
			break;
		}
		return (0);
	} else
		return (r->record);
}



/***************************************************************/
/*							       */
/*   ASSOC_clear - clear out the hash table of all association */
/*	relations.					       */
/*							       */
/***************************************************************/

void ASSOC_clear(void)
{
	int i, j;
	TABLE_PTR table;
	RECORD_PTR record;

	for (i = 0; i < PRIME; ++i) {
		while (Hash[i]) {
			table = Hash[i];
			for (j = 0; j < PRIME; ++j) {
				while (table->bucket[j]) {
					record = table->bucket[j];
					table->bucket[j] =table->bucket[j]->next;
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

TABLE_PTR table_exists(char *name)
{
	int val;
	TABLE_PTR tp;

	val = hash_string(name);
	for (tp = Hash[val]; tp; tp = tp->next)
		if (!(strcmp(name, tp->name)))
			break;
	return (tp);
}



/***************************************************************/
/*							       */
/*   record_exists - return the record saved under the given   */
/*	keys in the given table if it exists, NULL otherwise.  */
/*							       */
/***************************************************************/

RECORD_PTR
record_exists(TABLE_PTR tp, int hash_val, long k1, long k2, long k3, long k4,
	      long k5)
{
	RECORD_PTR rp;

	for (rp = tp->bucket[hash_val]; rp; rp = rp->next)
		if (same_record(rp, tp->keys, k1, k2, k3, k4, k5))
			break;
	return (rp);
}



/***************************************************************/
/*							       */
/*   remove_record - clear a record out of the data base       */
/*							       */
/***************************************************************/

int
remove_record(TABLE_PTR tp, int hash_val, long k1, long k2, long k3, long k4,long k5)
{
	RECORD_PTR rp, old;

	for (rp = tp->bucket[hash_val]; rp; rp = rp->next) {
		if (same_record(rp, tp->keys, k1, k2, k3, k4, k5)) {
			if (rp == tp->bucket[hash_val]) {
				tp->bucket[hash_val] = rp->next;
				free(rp);
			} else {
				old->next = rp->next;
				free(rp);
			}
			return (1);
		}
		old = rp;
	}
	return (0);
}



/***************************************************************/
/*							       */
/*   same_record - return 1 if the given RECORD_PTR has the    */
/*	given set of keys.				       */
/*							       */
/***************************************************************/

int
same_record(RECORD_PTR rp, long keys, long k1, long k2, long k3, long k4,long k5)	    
{
	switch (keys) {
	case 0:
		return (1);
	case 1:
		return ((rp->key1 == k1));
	case 2:
		return ((rp->key1 == k1) && (rp->key2 == k2));
	case 3:
		return ((rp->key1 == k1) && (rp->key2 == k2)
			&& (rp->key3 == k3));
	case 4:
		return ((rp->key1 == k1) && (rp->key2 == k2)
			&& (rp->key3 == k3)
			&& (rp->key4 == k4));
	case 5:
		return ((rp->key1 == k1) && (rp->key2 == k2)
			&& (rp->key3 == k3)
			&& (rp->key4 == k4) && (rp->key5 == k5));
	}
	return (0);
}



/***************************************************************/
/*							       */
/*   hash_string - hash the given string by adding all of its  */
/*	characters as they are mapped to integers	       */
/*							       */
/***************************************************************/

int hash_string(char *name)
{
	int val;
	char *ptr;

	for (val = 0, ptr = name; *ptr; ++ptr)
		val += (int) *ptr;
	return (val % PRIME);
}



/***************************************************************/
/*							       */
/*   hash_keys - return the hash value of the given number of  */
/*	the given keys. 				       */
/*							       */
/***************************************************************/

int hash_keys(int num, long k1, long k2, long k3, long k4, long k5)
{
	long val;

	val = 0;
	if (num > 0)
		val += abs(k1);
	if (num > 1)
		val += abs(k2);
	if (num > 2)
		val += abs(k3);
	if (num > 3)
		val += abs(k4);
	if (num > 4)
		val += abs(k5);
	return (val % PRIME);
}



/***************************************************************/
/*							       */
/*   assoc_dump - write out all the current associations.      */
/*							       */
/***************************************************************/

void assoc_dump(void)
{
	int i, j;
	TABLE_PTR table;
	RECORD_PTR record;

	fprintf(stderr, "Dumping associations\n");
	for (i = 0; i < PRIME; ++i) {
		table = Hash[i];
		while (table) {
			fprintf(stderr, "\nAssociation %s with keys\n",table->name);
			for (j = 0; j < PRIME; ++j) {
				record = table->bucket[j];
				while (record) {
					switch (table->keys) {
					case 0:
						fprintf(stderr,"(No parameters)\n");
						break;
					case 1:
						fprintf(stderr, "%ld\n",
							record->key1);
						break;
					case 2:
						fprintf(stderr, "%ld %ld\n",
							record->key1,
							record->key2);
						break;
					case 3:
						fprintf(stderr,
							"%ld %ld %ld\n",
							record->key1,
							record->key2,
							record->key3);
						break;
					case 4:
						fprintf(stderr,
							"%ld %ld %ld %ld\n",
							record->key1,
							record->key2,
							record->key3,
							record->key4);
						break;
					case 5:
						fprintf(stderr,
							"%ld %ld %ld %ld %ld\n",
							record->key1,
							record->key2,
							record->key3,
							record->key4,
							record->key5);
						break;
					default:
						/* ? */ ;
					}
					record = record->next;
				}
			}
			table = table->next;
		}
	}
	fprintf(stderr, "\n\n");
}

/**************************************************************/
/*****************   end of xtangoassoc.c    ******************/
/**************************************************************/
