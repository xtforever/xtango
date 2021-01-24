/*  Hashing by Dean Jerding */

/*************************************************************/
/***  This module uses XTANGO routines to animate cyclic   ***/
/***  linear probing in a hash table.  Operations which    ***/
/***  are supported include add, update, in table?, and    ***/
/***  delete elements with given keys.  The keys are ints  ***/
/***  and the data are strings, for this implementation.   ***/
/***  See hash.README for more details                     ***/
/*************************************************************/

/******  hash.c  *****/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "hash.h"
#include <xtango.h>

/*** TANGO animation declarations ***/

extern void Anim_Init();
extern void Anim_Action();
extern void Anim_NewElement();
extern void Anim_ElementAtIndex();
extern void Anim_KeyMatch();
extern void Anim_UpdateElement();
extern void Anim_AddElement();
extern void Anim_CompareKeys();
extern void Anim_CheckStatus();
extern void Anim_UpdateStatus();

/*** more external prototypes ***/


/*** internal prototypes ***/

boolean CL_SearchForElement();
void CL_UpdateElement();
void CL_RemoveElement();
void CL_InsertElement();
void CL_InitializeTable();
boolean CL_Add();
boolean CL_Delete();
boolean CL_InTable();
void CL_PrintElement();
void CL_PrintTable();
void CL_CleanUp();
keyType GetKey();
tableIndex HashFunc();
boolean AreEqual();

/*** global variables ***/

NAME_FUNCT fmap[] = { {"Init", 1, {VOID, (FPTR)Anim_Init}},
                      {"Action", 1, {VOID, (FPTR)Anim_Action}},
                      {"NewElement", 1, {VOID, (FPTR)Anim_NewElement}},
                      {"ElementAtIndex", 1, {VOID, (FPTR)Anim_ElementAtIndex}},
                      {"KeyMatch", 1, {VOID, (FPTR)Anim_KeyMatch}},
                      {"UpdateElement", 1, {VOID, (FPTR)Anim_UpdateElement}},
                      {"AddElement", 1, {VOID, (FPTR)Anim_AddElement}},
                      {"CompareKeys", 1, {VOID, (FPTR)Anim_CompareKeys}},
                      {"CheckStatus", 1, {VOID, (FPTR)Anim_CheckStatus}},
                      {"UpdateStatus", 1, {VOID, (FPTR)Anim_UpdateStatus}},
                      {NULL, 0, NULL, NULL}
                    };

tableType  table[TABLESIZE];
int numElements = 0;


/*********/

main()  
{
  FILE *fptr;
  char ch;
  char string[81];
  elementType element;
  keyType key;

  printf("Hashing\n\n");
  TANGOalgoOp(fmap, "BEGIN");

  CL_InitializeTable();
 
  fptr = fopen("hash.in", "r");
  while (fgets(string, 80, fptr) != NULL)  {
    switch(string[0])  {
      case 'a' :
        fgets(string, 80, fptr);
        element.key = atoi(string);
        fgets(string, 80, fptr);
        strcpy(string, strtok(string, "\n"));
        strncpy(element.name, string, 50);
        CL_Add(element);
        break;
      case 'd' :
        fgets(string, 80, fptr);
        key = atoi(string);
        CL_Delete(key);
        break;
      case 'i' :
        fgets(string, 80, fptr);
        key = atoi(string);
        CL_InTable(key);
        break;
      case 'e' :
        fgets(string, 80, fptr);
        key = atoi(string);
        CL_PrintElement(key);
        break;
      case 't' :
        CL_PrintTable();
        break;
      default:
        printf("\nError in input file!\n");
    } 
  }
  fclose(fptr);

  TANGOalgoOp(fmap, "END");
}



void CL_InitializeTable()
{
  tableIndex i;

  TANGOalgoOp(fmap, "Init", TABLEMIN, TABLEMAX);
  TANGOalgoOp(fmap, "Action", "Initialize Table");

  for (i = TABLEMIN; i<=TABLEMAX; i++)  {
    table[i].status = empty;
    TANGOalgoOp(fmap, "ElementAtIndex", i);
    TANGOalgoOp(fmap, "UpdateStatus", i, " E");
  }  
}

/************/

void CL_CleanUp()
{
}

/************/

boolean CL_Add(element)
  elementType element;
{
  keyType key;
  tableIndex index;

  TANGOalgoOp(fmap, "Action", "Add/Update Element");

  key = GetKey(element);

  TANGOalgoOp(fmap, "NewElement", key, element.name);

  if (CL_SearchForElement(key, &index))  {
    CL_UpdateElement(element, index);
    return(TRUE);
  }
  else if (numElements < TABLESIZE)  { 
    CL_InsertElement(element, index);
    ++numElements;
    return(TRUE);
  }
  return(FALSE);
}

/**************/

boolean CL_Delete(key)
  keyType key;
{
  tableIndex index;

  TANGOalgoOp(fmap, "Action", "Delete Element");
  TANGOalgoOp(fmap, "NewElement", key, "");

  if (CL_SearchForElement(key, &index))  {
    CL_RemoveElement(index);
    --numElements;
    return(TRUE);
  }
  return(FALSE);
}

/**************/

boolean CL_InTable(key)
  keyType key;
{
  tableIndex index;
  boolean inTable;

  TANGOalgoOp(fmap, "Action", "Element with Key in Table?");
  TANGOalgoOp(fmap, "NewElement", key, "");

  inTable = CL_SearchForElement(key, &index);
  if (inTable)  TANGOalgoOp(fmap, "KeyMatch", index);
  return(inTable);
}

/**************/

void CL_PrintElement(key)
  keyType key;
{
  tableIndex index;

  if (CL_SearchForElement(key, &index))  {
    printf("\nindex = %d, key = %d, data = %s\n", index, key,
           table[index].element.name);
  }
  else
    printf("\nElement is not in table.\n");
}

/**************/

void CL_PrintTable()
{
  tableIndex i;

  printf("\nCL Table Printout\n\n");
  for (i = TABLEMIN; i <= TABLEMAX; i++)  
    printf("index = %d, status = %d, key = %d, data = %s\n", i, 
           table[i].status, table[i].element.key, table[i].element.name);
}

/**************/

boolean CL_SearchForElement(key, pindex)
  keyType key;
  tableIndex *pindex;
{
  int i;
  tableIndex firstDeletedElement;
  boolean foundDeletedElement = FALSE;
 
  *pindex = HashFunc(key);

  TANGOalgoOp(fmap, "ElementAtIndex", *pindex);

  i = TABLEMIN;

  TANGOalgoOp(fmap, "CheckStatus", *pindex);
  while ((table[*pindex].status != empty) && (i <= TABLEMAX))  {
    TANGOalgoOp(fmap, "CompareKeys", *pindex);
    if (AreEqual(key, table[*pindex].element.key))  
      return(TRUE);
    else  {
      if (!foundDeletedElement)  {
        foundDeletedElement = (table[*pindex].status == deleted);
        firstDeletedElement = *pindex;
      }
      ++i;
      *pindex = (HashFunc(key) + i) % TABLESIZE;
      TANGOalgoOp(fmap, "ElementAtIndex", *pindex);
    } 
    TANGOalgoOp(fmap, "CheckStatus", *pindex);
  } 
  if (foundDeletedElement)  
    *pindex = firstDeletedElement;
  if (i > TABLESIZE)
    *pindex = NOTINTABLE;
  return(FALSE); 
}

/**************/

void CL_UpdateElement(element, index)
  elementType element;
  tableIndex index;
{
  table[index].element = element;
  TANGOalgoOp(fmap, "KeyMatch", index);
  TANGOalgoOp(fmap, "UpdateElement", index);
}

/**************/
	
void CL_InsertElement(element, index)
  elementType element;
  tableIndex index;
{
  table[index].element = element;
  TANGOalgoOp(fmap, "AddElement", index);
  table[index].status = valid;
  TANGOalgoOp(fmap, "UpdateStatus", index, " V");
}

/**************/

void CL_RemoveElement(index)
  tableIndex index;
{
  table[index].status = deleted;
  TANGOalgoOp(fmap, "KeyMatch", index);
  TANGOalgoOp(fmap, "UpdateStatus", index, " D");
}

/***********/

keyType GetKey(element)
  elementType element;
{
  return(element.key);
}

/***********/

tableIndex HashFunc(key)
  keyType key;
{
  return(key % TABLESIZE);
}

/***********/

boolean AreEqual(key1, key2)
  keyType key1, key2;
{
  return(key1 == key2);
}
