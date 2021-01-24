/*******  element.h  ********/

#define TRUE 1
#define FALSE 0
#define TABLESIZE 11
#define TABLEMIN 0
#define TABLEMAX TABLESIZE - 1
#define NOTINTABLE -1


typedef unsigned char  boolean;

typedef int  keyType;

typedef struct {
    keyType key;
    char name[81];
} elementType;

/* enum tableIndex {TABLEMIN..TABLEMAX}; */
typedef int tableIndex;
 
enum statusType {empty, deleted, valid};

typedef struct  {
    elementType element;
    enum statusType status;
    } tableType;


