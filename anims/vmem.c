/***********************************************************************
*  This program runs an XTango algorithm animation of a virtual        *
*  memory page replacement scheme, on a very simplified scale.         *
*  Implemented by: Jennifer Lautenschlager                             *
*  Last changed:   October 19, 1993                                    *
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmem.h"
#include <X11/Xlib.h>
#include "xtango.h"

/* XTango functions, found in xxxxxscenes.c */
static NAME_FUNCT fcn[] = { {"Init",  1, {VOID, (FPTR)AnimInit}},
                            {"Move",  1, {VOID, (FPTR)AnimMove}},
                            {"Swap",  1, {VOID, (FPTR)AnimSwapOut}},
                            {"Slide", 1, {VOID, (FPTR)AnimSlideUp}},
                            {NULL,    0, NULL, NULL} };

/* General variables -- necessary for any method */
static int Lpage_frame[MAX_FRAMES], Rpage_frame[MAX_FRAMES];  /* Frame buffs */
static int left_frames = 0, right_frames = 0;                 /* Buff sizes  */
static int left_method=0, right_method=0;                     /* Buff methods*/
static int left_tail = 0, right_tail = 0;                     /* Buff ptrs   */

/* These variables are needed for time-stamped method (currently only LRU) */
static unsigned int Lpage_time[MAX_FRAMES], Rpage_time[MAX_FRAMES]; /*For LRU*/
static int cycle=0;       /* Clock cycle                          */        

/* Randomization values -- only needed if reference strings are randomized */
static int lower_limit;   /* Lower limit on randomization         */
static int upper_limit;   /* Upper limit on randomization         */
static int count;         /* How many frames to randomly generate */
static int seed;          /* Random number seed to use            */
static int to_random=FALSE;  /* Should we randomize?              */

/* These are activated through command line arguments */
extern int sound;   /* Whether or not to have sound */
extern int color;   /* Whether or not to have color */

#define NiceRand(num)    rand() % (num)   /* Random numbers within a range */

/****************************************************************
*  Returns string representing page-replacement method          *
****************************************************************/
static char *MethodName( method )
       int method;   /* Enumeration of method type */
{
    if( method == FIFO ) return( "FIFO" );
    return( "LRU" );
}

/****************************************************************
*  Interpret all of the command line arguments                  *
****************************************************************/
static int InterpretCommandLine( argc, argv )
       int argc; 
       char *argv[];
{
    int i,j;

    for( i = 1; i < argc; i++ )
    {
	/* Sound on */
	if( !strcmp(argv[i],"-s") )
	    sound = TRUE;
        /* Color on */
	else if( !strcmp(argv[i],"-c") )
	    color = TRUE;
        else if( !strcmp(argv[i],"-r") )
            to_random = TRUE; 
        else
            return 0; 
    }

    return(1);
}

/******************************************************
*  Looks for a frame in a certain buffer              *
******************************************************/
static int InBuffer(frame,ref,max_frames)
	 int frame[MAX_FRAMES];  /* Frame to search in       */
	 int ref;                /* Reference to search for  */
	 int max_frames;         /* Maximum frames to search */
{
    int pos=NOT_FOUND;   /* Position in buffer, or NOT_FOUND if not found */ 
    int i;

    for( i = 0; i < max_frames && pos==-1; i++ )
    {
	if( frame[i]==ref )
	    pos = i;
    }

    return pos;
}

/*********************************************************************
*  Does the right thing for the page reference in a LRU method       *
*********************************************************************/
static void LRUize(reference,side,swap_out,block)
	int reference;   /* Next number in reference string */
        int side;        /* Right or left side              */
        int *swap_out;   /* swap out code                   */
        int *block;      /* Block code                      */
{
    int i;      /* Loop variable */
    int *page_frame = (side == LEFT ? Lpage_frame : Rpage_frame );
    unsigned int *page_time = (side == LEFT ? Lpage_time : Rpage_time );
    int num_frames = (side == LEFT ? left_frames : right_frames );
    int *tail = (side == LEFT ? &left_tail : &right_tail );
    int victim = 0;

    *swap_out = REPEAT;

    /* No need to worry as much unless it's not there */
    if((*block=InBuffer(page_frame,reference,num_frames)) == NOT_FOUND)
    {
        *swap_out = NEW;

        /* See if there's room */
        if( *tail != num_frames )
        {  
            page_frame[*tail] = reference;
            page_time[*tail] = cycle;
            *block = (*tail)++;
        }
        else
        {
            /* Figure out who to kick out */

	    for( i = 1; i < num_frames; i++ )
	    {
		if( page_time[i] < page_time[victim] )
		    victim = i;
            }

	    TANGOalgoOp(fcn,"Swap",side,victim,num_frames);

	    page_frame[victim] = reference;
	    page_time[victim] = cycle;
	    *block = victim;
	}
    }
    else
    {
	page_time[*block] = cycle;   /* Update the last reference time */
    }
}

/*********************************************************************
*  Does the right thing for the page reference in a FIFO method      *
*********************************************************************/
static void FIFOize(reference,side,swap_out,block)
	int reference;   /* Next number in reference string */
        int side;        /* Right or left side              */
        int *swap_out;   /* swap out code                   */
        int *block;      /* Block code                      */
{
    int i;      /* Loop variable */
    int *page_frame = (side == LEFT ? Lpage_frame : Rpage_frame );
    int num_frames = (side == LEFT ? left_frames : right_frames );
    int *tail = (side == LEFT ? &left_tail : &right_tail );

    *swap_out = REPEAT;

    /* No need to worry if it's already there */
    if((*block=InBuffer(page_frame,reference,num_frames)) == NOT_FOUND)
    {
        *swap_out = NEW;

        /* See if there's room */
        if( *tail != num_frames )
        {  
            page_frame[*tail] = reference;
            *block = (*tail)++;
        }
        else
        {
            TANGOalgoOp(fcn,"Swap",side,0,num_frames);
            TANGOalgoOp(fcn,"Slide",side,1,num_frames);
            for( i = 0; i < num_frames-1; i++ )
	        page_frame[i] = page_frame[i+1];
            page_frame[num_frames-1] = reference;
	    *block = num_frames-1;
	}
    }
}

/*******************************************************
*  The workhouse function call                         *
*******************************************************/
main( argc, argv ) 
         int argc;
         char *argv[];
{
    int valid_entry = FALSE;                                 /* Status flag */
    int cont=TRUE;                                           /* Status flag */
    int i,reference;                                         /* Algo info   */
    int Lswap_out, Rswap_out, Lblock, Rblock;

    sound = FALSE;
    color = FALSE;

    if( argc > 1 )   /* If command line arguments */
    {
        if( !InterpretCommandLine(argc,argv) )
        {
	    /* Print the nice little 'you goofed' message here */
	    printf("The following command line arguments are implemented:\n");
	    printf("     -c    Color display\n");
	    printf("     -s    Sound on\n");
	    printf("     -r    Randomly generate reference string\n");
	    exit(0);
	}
    }

    /* Set things up */
    TANGOalgoOp(fcn,"BEGIN");

    /* Initialize */
    for( i = 0; i < MAX_FRAMES; i++ )
    {
        Lpage_frame[i] = NONE_LOADED;
	Rpage_frame[i] = NONE_LOADED;
    }

    /* Read user data */
    while( !valid_entry )
    {
        printf("How many virtual memory frames for left buffer ");
        printf("(%d-%d)? ",MIN_FRAMES,MAX_FRAMES);
        scanf("%d",&left_frames);
        valid_entry = ( left_frames>=MIN_FRAMES && left_frames<=MAX_FRAMES );
    }

    valid_entry = FALSE;
    while( !valid_entry )
    {
	printf("Page replacement method for left buffer?\n");
	printf("<1> FIFO   <2> LRU  ");
	scanf("%d",&left_method);
	left_method = toupper(left_method);
	valid_entry = (left_method>=1 && left_method<=2);
    }	

    valid_entry = FALSE;
    while( !valid_entry )
    {
        printf("How many virtual memory frames for right buffer ");
        printf("(%d-%d)? ",MIN_FRAMES,MAX_FRAMES);
        scanf("%d",&right_frames);
        valid_entry = ( right_frames>=MIN_FRAMES && right_frames<=MAX_FRAMES );
    }

    valid_entry = FALSE;
    while( !valid_entry )
    {
	printf("Page replacement method for right buffer?\n");
	printf("<1> FIFO   <2> LRU   ");
	scanf("%d",&right_method);
	right_method = toupper(right_method);
	valid_entry = (right_method>=1 && right_method<=2);
    }	

    /* Draw the screen */
    TANGOalgoOp(fcn,"Init",left_frames,MethodName(left_method),
			   right_frames,MethodName(right_method));

    /* If they don't want randomized data, let them input it */
    if( !to_random )
    {
        printf("Enter reference string one number at a time,\n");
        printf("or '-1' to quit\n");

        /* Let them keep entering stuff until they get bored */
        while( cont )
        {
            printf("Next reference ('-1' quits) ==> ");
            while( scanf("%d",&reference) != 1 );
            if( cont=(reference!=-1) )
            {
                if( left_method == FIFO )
		    FIFOize(reference,LEFT,&Lswap_out,&Lblock);
                else 
                    LRUize(reference,LEFT,&Lswap_out,&Lblock);

                if( right_method == FIFO )
                    FIFOize(reference,RIGHT,&Rswap_out,&Rblock);
                else 
		    LRUize(reference,RIGHT,&Rswap_out,&Rblock);

                TANGOalgoOp(fcn,"Move",Lswap_out,Lblock,
                            Rswap_out,Rblock,reference);
                cycle++;
            }
        }
    }
    else    /* Or else, get some ranges and generate it for them */
    {
	int reference, diff;

        printf("Random number seed? ");
        scanf("%d",&seed);
        srand(seed);
        printf("Lower limit? ");
	scanf("%d",&lower_limit);
	printf("Upper limit? ");
	scanf("%d",&upper_limit);
	printf("Number of references? ");
	scanf("%d",&count);
	diff = upper_limit - lower_limit + 1;

	for( i = 0; i < count; i++ )
	{
	    reference = lower_limit + NiceRand(diff);
	    printf("%d\n",reference);
            if( left_method == FIFO )
	        FIFOize(reference,LEFT,&Lswap_out,&Lblock);
            else if( left_method == LRU )
                LRUize(reference,LEFT,&Lswap_out,&Lblock);

            if( right_method == FIFO )
                FIFOize(reference,RIGHT,&Rswap_out,&Rblock);
            else if( right_method == LRU )
                LRUize(reference,RIGHT,&Rswap_out,&Rblock);

            TANGOalgoOp(fcn,"Move",Lswap_out,Lblock,
                        Rswap_out,Rblock,reference);
            cycle++;
	}
    }

    /* Close the window */
    TANGOalgoOp(fcn,"END");

    exit(0);
}






int WriteToAudio(file_name)
        char *file_name;
{       
    FILE *fp;
    char audio_data;
    long file_size;
    FILE *audio;

    if ((audio = fopen("/dev/audio","w")) == NULL)
    {   
        printf ("ERROR opening audio dev\n");
        return(2);
    }

    if ((fp = fopen(file_name,"r")) == NULL) 
    {
        printf ("unable to open sound file :%s:\n", file_name);
        return(1);
    }

    fseek(fp,0,2);
    file_size = ftell(fp);
    fseek(fp,0,0);

    while (file_size--)
    {
        audio_data = getc(fp);
        putc(audio_data,audio);
    } 

    fclose(fp);
    fclose (audio);
}



