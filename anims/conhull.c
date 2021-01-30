/*           AUTHOR   :-  SRINIVAS K. V.                */
/*                                                      */
/* This is code for solving CONVEX-HULL problem using   */
/* JARVIS MARCH method. This program finds out all the  */
/* HULL points from a given set of points.There are     */
/* interesting events in the code which specify the     */
/* algorithm operations.                                */ 


# include <stdio.h>
# include <math.h>
# include <string.h>
# include <malloc.h>
# include <xtango.h>

#define TRUE  1
#define FALSE 0
struct loc *create_point();
void add_to_list();
void remove_from_list();
void print_list();
struct loc *min_list();
struct loc *max_list();

void ANIMInit() ,  ANIMDraw() , ANIMSho_min() , ANIMComp() , ANIMChange_min(),
     ANIMGetpt() , ANIMComp_polar(), ANIMChange_polarmin() , ANIMClear() ,
     ANIMDra_final();

static NAME_FUNCT fxn[] = { { "Init",       1, { VOID, (FPTR)ANIMInit}},
                            { "Sho_min",    1, { VOID, (FPTR)ANIMSho_min}},
                            { "Comp",       1, { VOID, (FPTR)ANIMComp}},
                            { "Change_min", 1, { VOID, (FPTR)ANIMChange_min}},
                            { "Draw"      , 1, { VOID, (FPTR)ANIMDraw}},
                            { "Getpt"     , 1, { VOID, (FPTR)ANIMGetpt}},
                            { "Comp_polar", 1, { VOID, (FPTR)ANIMComp_polar}},
                            { "Change_polarmin",
                                           1, { VOID, (FPTR)ANIMChange_polarmin}},
                            { "Clear"     ,1, { VOID, (FPTR)ANIMClear}},
                            { "Dra_final" ,1, { VOID, (FPTR)ANIMDra_final}},
                            { NULL,         0, NULL, NULL}
                          };


enum { EQUAL=0 , LEFT , RIGHT };

struct loc {
             int x,y;
             int index;
             struct loc  *next;
             } *head,*tail,*stack;

int len=0,lowest;

double x,y;


struct loc  *create_point()
{
 struct loc *pt;

   pt = (struct loc  *) malloc(sizeof(struct loc ));
   pt->next = NULL;
   return(pt);
}

void add_to_list(p)
struct loc  *p;
{

    if ( head == NULL )
    {
       head = p;
       tail = p;
       p->next=NULL;
    }
    else
    {
       tail->next = p;
       tail = p;
       p->next = NULL;
    }
}



void remove_from_list(pt)
struct loc pt;
{

   struct loc *temp,*prev;

   prev = NULL;
   temp=head;
   if ( temp != NULL )
   {
        while( !((temp->x == pt.x) && (temp->y == pt.y)) && (temp != NULL))
        {     
               prev = temp;
               temp = temp->next;
        }
        if (( temp != NULL ) && ( prev != NULL)){
              prev->next = temp->next;
              if ( prev->next == NULL)tail = prev;}
        if (temp == head)
              head = head->next;
        

   }
}
void print_list(p)
struct loc  *p;
{
    struct loc *temp;

    temp = p;
     while(temp != NULL)
     {  
        printf("(%d %d)\n",temp->x,temp->y);
        temp = temp->next;
     }
}

struct loc *min_list(head)
struct loc *head;
{
 
   struct loc *temp,*min;
   int min_index=0;
   temp=head;min=temp;
   
   TANGOalgoOp(fxn,"Sho_min",min_index);
   while( temp != NULL)
   {
     TANGOalgoOp(fxn,"Comp",min_index,len);
     if ( (temp->y > min->y) || (( temp->y == min->y ) && (temp->x < min->x)) )
     {        TANGOalgoOp(fxn,"Change_min",min_index,len);
              min=temp;
              min_index = len;
     }
     temp = temp->next;
     len++;
   }
   TANGOalgoOp(fxn,"Sho_min",min_index);
   return(min);
}

struct loc *max_list(head)
struct loc *head;
{

   struct loc *temp,*max;
   temp=head;max=temp;

   while( temp != NULL)
   {
     if ( (temp->y > max->y) || (( temp->y == max->y ) && (temp->x > max->x)) )
              max=temp;
     temp = temp->next;
   }
   return(max);
}
  


int cross_prod(p0,p1,p2)
struct loc *p0,*p1,*p2;
{
   int prod;

   prod = ((p1->x - p0->x) * (p2->y - p0->y)) 
                   - ((p2->x - p0->x) * (p1->y - p0->y));   
   prod = -prod;
   if ( prod == 0 )
       return(EQUAL); 
   if ( prod < 0)
       return(LEFT);
   else
       return(RIGHT);
}

float dist(p1,p2)
struct loc *p1,*p2;
{
  float dis;
   dis = pow((double)(p1->x - p2->x),(double)2);
   dis = dis + pow((double)(p1->y - p2->y),(double)2);
   dis = sqrt(dis);
   /*dis = sqrt(pow((p1->x - p2->x),2) + pow((p1->y - p2->y),2));*/
   return(dis);
}

   

struct loc *min_polar_pos(p0)
struct loc *p0;
{
    struct loc *p1,*p2,*min;
    int res;

    p1 = head;p2 = p1->next; 
    min = p1;
    while( p2 != NULL )
    {
       res = cross_prod(p0,min,p2);
       TANGOalgoOp(fxn,"Comp_polar",min->index,p0->index,p2->index);
       if ( (res == RIGHT) || ((res == EQUAL) && (dist(p0,p2) > dist(p0,min))) ) 
       {
              TANGOalgoOp(fxn,"Change_polarmin",min->index,p0->index,p2->index);
              min = p2;
       }
       else
       TANGOalgoOp(fxn,"Clear",min->index,p0->index,p2->index);
       p1 = p2;
       /*printf("%d %d  %d %d\n",p1->x,p1->y,p2->x,p2->y);*/
       p2 = p2->next;
    }
    return(min);
}

print_msg()
{
  
  printf("     MENU\n");
  printf("     ____\n");
  printf("\n\n 1  : Enter the points from the screen.\n");
  printf("\n 2  : Enter the points form keyboard.\n");
  printf("\n 3  : Demo of the animation.\n");
  printf("\n Enter option please:");
}

int getopt()
{ int i,j;
   
     print_msg();
     scanf("%d",&i);
     while( ( i<1 ) || (i > 3))
     {
         printf("Enter a valid option please (1-3).\n\n");
         print_msg();
         scanf("%d",&i);fflush(stdin);
     }
     switch(i){
      case 1 : printf("\nPress the left button of the mouse after moving\n");
                 printf("the mouse cursor on the desired point.The algo.\n");
                 printf("will start as soon as the specified number of pts\n");
                 printf("have been entered.\n");
               break;
      case 2 : printf("\nEnter the points from the keyboard.\n");
                 printf("Enter values of x,y coordinates.\n");
                 printf("Screen top left corner (0,0)\n");
                 printf("Screen bottom right corner (999,999)\n");
               printf("Indicate end of input by -1 -1\n");
               break;
      case 3 : printf("\nThis is a demo for the algorithm.\n");
               printf("Points are read from conhull.in file.\n");
               printf("Make sure this file is in present\n");
               printf("directory.\n");
               printf("Press <Return> to continue and any other key to EXIT.\n");
               getchar();
               j = getchar();
               if ( j != 10 ) exit(0);
               break;
     }
     return(i);
} 

intro()
{
   system("clear");
   printf("This is an animation of CONVEX HULL algorithm.\n");
   printf("Given a set of points, the algorthm constructs\n");
   printf("a smallest convex polygon such that each point\n");
   printf("is either boundary or interior of the polygon.\n");
 printf("\nThe algorithm first finds the lowest point and\n");
   printf("then finds the leftmost point with respect to \n");
   printf("this lowest point.The leftmost point now is   \n");
   printf("focus of attention.Again lefmost point with \n");
   printf("respect to this new point is located. This process\n");
   printf("is repeated till algorithm reaches intial point\n");
   printf("it originally started from (i.e the lowest point)\n");
   printf("After reaching the highest point the algorithm\n");
   printf("starts looking for rightmost points.All the \n");
   printf("rightmost/lefmost points form the hull points.\n");
   printf("The lefmost/rightmost point at each stage is \n");
   printf("determined by comparing the polar angles of all\n");
  printf("the candidate points.The points are compared\n");
  printf("sequentially by increasing label number of points.\n");
  printf("The lowest numbered non-hull point is taken to be\n");
  printf("leftmost point initially\n"); 
   printf("\nThis particular convex hull algorithm\n");
   printf("is called JARVIS MARCH METHOD\n");
   printf("\n IF THIS IS NOT VERY CLEAR, GO AHEAD AND WATCH THE SHOW !!");
   printf("\n Press any key to continue...");
   getchar();
   system("clear");       
} 
    
main()
{
 struct loc *p0,*min,*p,*temp;
 int i,flag=TRUE,maxsize,DONE,opt;
 int max_x,min_x,max_y,min_y; 
 FILE *fp;

 i=0; 
 TANGOalgoOp(fxn, "BEGIN");
 fp = fopen("conhull.in","r");
 TANGOalgoOp(fxn, "Init");
 intro();
 opt = getopt();
 switch(opt){

  case 1 : printf("\n  Enter the number of points:");
           scanf("%d",&maxsize);
           for(i=0;i<maxsize;i++)
           {
              p = create_point();
              TANGOalgoOp(fxn, "Getpt");
              p->x = (x * 1000);p->y=(1000 * y);
              p->index = i;
              TANGOalgoOp(fxn,"Draw", p->x , p->y , i);
              add_to_list(p);
           }
           break;
  case 2 : printf("Enter the points.\n");
           DONE= FALSE; i=0;
           while( !DONE ) 
           {
              p = create_point();
              scanf("%d %d",&p->x,&p->y);
              /*p->x = (x * 1000);p->y=(1000 * y);*/
              p->index = i;
              if ( (p->x != -1) && (p->y != -1)){
              TANGOalgoOp(fxn,"Draw", p->x , p->y , i);
                    add_to_list(p);}
              if ( (p->x == -1) && (p->y == -1))
                   DONE = TRUE;
              i++;
            }
            break;
  case 3 : DONE= FALSE; i=0;
           while( !DONE )
           {
              p = create_point();
              fscanf(fp,"%d %d",&p->x,&p->y);
              p->index = i;
              if ( (p->x != -1) && (p->y != -1)){
                    add_to_list(p);
              TANGOalgoOp(fxn,"Draw", p->x , p->y , i);}
              if ( (p->x == -1) && (p->y == -1))
                   DONE = TRUE;
              i++;
            }
            fclose(fp);
            break;
  } 
 /*print_list(head);*/ 
 printf("------------------\n");
 p0 = min_list(head);
 lowest = p0->index;
 remove_from_list(p0);
 min = min_polar_pos(p0);
 TANGOalgoOp(fxn,"Dra_final",p0->index,min->index);
 while( min != p0)
 {
    temp = min;
    remove_from_list(min);
    min = min_polar_pos(min);
    TANGOalgoOp(fxn,"Dra_final",temp->index,min->index);
    if ( flag == TRUE ) 
    {
         add_to_list(p0);flag=FALSE;
    } 
  }
 TANGOalgoOp(fxn,"Dra_final",temp->index,p0->index);
 TANGOalgoOp(fxn, "END");
}
