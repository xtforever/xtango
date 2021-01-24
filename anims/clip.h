typedef struct
   {
   double xval, yval;    /* The x and y values for the vertex */
   }   VERTEX;
   
typedef struct nodetag
   {
   VERTEX vert;                  /* The actual vertex */
   struct nodetag *nextnode;     /* Pointer to the next NODE in the list */
   }   NODE;                     /* The type for nodes */
   

