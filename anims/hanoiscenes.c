#include <stdio.h>
#include "xtango.h"

#define  PEG	 0   /* ids used in associations */
#define  DISK	 1
#define  OUTLINE 2
#define  POINTS  3
#define  PEGTOP  4

#define  DISK_HT   0.1

/* draw the 3 pegs, create a two-dimensional array of positions for the  */
/* disks to go onto the pegs, and draw the initial set of disks on peg 0 */

void
ANIMInit(disks)
   int disks;
{
   int i;
   double xvals[10], stack_top, peg_ht;
   TANGO_LOC	 peg_pos;
   TANGO_IMAGE	 disk,outline;
   TANGO_PATH	 path;
   TANGO_TRANS	 trans;

   ASSOCinit();

   ASSOCmake("NUM_DISKS",0);      /* save the number of disks */
   ASSOCstore("NUM_DISKS",disks);

   ASSOCmake("DISK_AT",1);
   ASSOCmake("OUTLINE_AT",1);

   peg_ht = (double)(disks+1) * DISK_HT;  /* create 3 pegs to hold disks */
   TWISTcreate_image_array(NULL,PEG,3,TANGO_IMAGE_TYPE_LINE,1,0,0.2,0.9,
                    NULL,0.0,NULL,peg_ht,0.3,1,TANGO_COLOR_BLACK,0.0,0.0,1.0);

   TWISTcreate_loc_array(NULL,PEGTOP,3,1,0.2,0.9-peg_ht,0.3);

   stack_top = 0.9 - ((double)disks * DISK_HT);
   TWISTcreate_2d_loc_array(NULL,POINTS,disks,3,0.2,stack_top+DISK_HT,0.3,DISK_HT);
   /* make 2d array where each column is a peg and each row position */
   /* holds a disk						     */

   for (i=0; i<disks; ++i)
      xvals[i] = (double)(i+1) / (double)disks;

   /* put the disks on the first peg */
   TWISTcreate_image_array(NULL,DISK,disks,TANGO_IMAGE_TYPE_RECTANGLE,0,1,
          0.2,stack_top,xvals,0.3,NULL,DISK_HT,0.0,1,TANGO_COLOR_YELLOW,1.0);
   TWISTcreate_image_array(NULL,OUTLINE,disks,TANGO_IMAGE_TYPE_RECTANGLE,0,1,
           0.2,stack_top,xvals,0.3,NULL,DISK_HT,0.0,1,TANGO_COLOR_BLACK,0.0);

   /* set the disk images at the appropriate TANGO_LOCS */
   for (i=0; i<disks; ++i)
      { peg_pos = (TANGO_LOC) ASSOCretrieve("ID3",POINTS,i,0);
	disk = (TANGO_IMAGE) ASSOCretrieve("ID",DISK,i);
	outline = (TANGO_IMAGE) ASSOCretrieve("ID",OUTLINE,i);
	ASSOCstore("DISK_AT",peg_pos,disk);
	ASSOCstore("OUTLINE_AT",peg_pos,outline);
      }

   path = TANGOpath_null(3);
   trans = TANGOtrans_create(TANGO_TRANS_TYPE_DELAY,NULL,path);
   TANGOtrans_perform(trans);
   TANGOpath_free(1,path);
   TANGOtrans_free(1,trans);
}




/* given there are fdisks on peg fpeg, move the top disk onto peg tpeg */
/* which currently has tdisk number of disks on it.		       */

void
ANIMMove(fpeg,fdisk,tpeg,tdisk)
   int fpeg,fdisk,tpeg,tdisk;
{
   int		 n;
   int		 top_spot,open_spot;
   TANGO_LOC	 atpt,frompt,topt,topeg_top,frompeg_top;
   TANGO_IMAGE	 disk,rect;
   TANGO_PATH	 path[3],movepath;
   TANGO_TRANS	 move[2],compose;

   n = (int)ASSOCretrieve("NUM_DISKS");

   top_spot = n - fdisk;   /* find top disk on peg fpeg */
   atpt = (TANGO_LOC) ASSOCretrieve("ID3",POINTS,top_spot,fpeg);
   disk = (TANGO_IMAGE) ASSOCretrieve("DISK_AT",atpt);
   rect = (TANGO_IMAGE) ASSOCretrieve("OUTLINE_AT",atpt);

   frompt = TANGOimage_loc(rect,TANGO_PART_TYPE_S);
   frompeg_top = (TANGO_LOC) ASSOCretrieve("ID",PEGTOP,fpeg);
   topeg_top = (TANGO_LOC) ASSOCretrieve("ID",PEGTOP,tpeg);
   open_spot = n - tdisk - 1;  /* find loc on tpeg to put this disk */
   topt = (TANGO_LOC) ASSOCretrieve("ID3",POINTS,open_spot,tpeg);

   path[0] = TANGOpath_distance(frompt,frompeg_top,0.03);
   path[1] = TANGOpath_distance(frompeg_top,topeg_top,0.03);
   path[2] = TANGOpath_distance(topeg_top,topt,0.03);
   movepath = TANGOpath_concatenate(3,path[0],path[1],path[2]);

   move[0] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,disk,movepath);
   move[1] = TANGOtrans_create(TANGO_TRANS_TYPE_MOVE,rect,movepath);
   compose = TANGOtrans_compose(2,move[0],move[1]);
   TANGOtrans_perform(compose);

   ASSOCstore("DISK_AT",topt,disk);  /* tell where this disk is now located */
   ASSOCstore("OUTLINE_AT",topt,rect);  /* tell where this outline is now located */

   TANGOpath_free(4,path[0],path[1],path[2],movepath);
   TANGOtrans_free(3,move[0],move[1],compose);
}
