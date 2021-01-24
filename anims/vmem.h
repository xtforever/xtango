/*** FIFOPAGING.H ***/

#ifndef _FIFOPAGING
#define _FIFOPAGING

/*** DEFINES ***/

#define TRUE           1
#define FALSE          0

#define SCREEN_SIZE  1.0        /* Screen is 1.0 x 1.0                      */
#define MARGIN       0.1        /* The margin width/height                  */ 
#define MAX_SIZE     0.8        /* How tall/wide our "marginized" window is */
#define MAX_FRAMES    10        /* Maximum # of frames we'll allow          */
#define MIN_FRAMES     3        /* Minimum # of frames we'll allow          */
#define FRAME_HEIGHT MAX_SIZE/MAX_FRAMES   /* Height of each frame          */
#define FRAME_WIDTH  0.2        /* How wide is the frame?                   */
#define NONE_LOADED   -1        /* Signify no frame in that buffer space    */
#define LEFT           0        /* Refers to buffer on the left             */
#define RIGHT          1        /* Refers to buffer on the right            */
#define CENTERED       1        /* Center the text at location              */
#define NOT_FOUND     -1        /* Frame not in a particular buffer         */

#define Boing()        WriteToAudio("boing.au")
#define Clap()         WriteToAudio("clap.au")
#define Splat()        WriteToAudio("splat1.au")

enum { NEW=0, SWAP, REPEAT };
enum { FIFO=1, LRU, SECOND_CHANCE };

/*** FUNCTION PROTOTYPES ***/

/*******************************************
*  Initialize the animation window         *
*******************************************/
void AnimInit();
void AnimMove();
void AnimSwapOut();
void AnimSlideUp();
void WriteToAudio();

/*** EXTERNS ***/
int sound;
int color;

#endif
