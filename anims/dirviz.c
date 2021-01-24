#include <stdio.h>
#include <string.h>
#include "../src/xtangolocal.h"
#include "dirviz.h"
#define NEW(s) (s *)malloc(sizeof(s))

/* By Mark Gray (vatavian@cc.gatech.edu) 1 June 1992
   Written for John Stasko's Visualization in Programming course
   Inspired by TreeViz for the Macintosh by Brian Johnson based on
   the treemap concept originated by Ben Schneiderman
 */

int DEPTH=1;
int MAXDEPTH=1;
int CONSTANT_SIZE=0;
int LOG_SIZE=0;

FILE *ls, *date;
int thismonth;
treenode root;
treenode *curroot;
int VERTICALSLICE = 1;
void AnimInit(), AnimUpdate(), AnimFollowMouse();

static NAME_FUNCT fcn[] = { {"Init",	  1, {VOID, (FPTR)AnimInit}},
			    {"Update",    1, {VOID, (FPTR)AnimUpdate}},
			    {"Follow",    1, {VOID, (FPTR)AnimFollowMouse}},
			    {NULL,	  0,  NULL, NULL} 
			  };

void printinstructions()
{
  printf("Before pressing the 'run animation' button, resize the xtango window to occupy\n");
  printf("the whole screen for best results.  Resizing it later or using the 'refresh'\n");
  printf("button will disable mouse input in the map window.\n");
  printf("Running this program with any argument will suppress this message.\n");
  printf("\n");
  printf("This program produces a map of disk space usage starting in the\n");
  printf("current directory and recursing below it.  Initially the map is\n");
  printf("one level deep, meaning it displays a rectangle for each file or\n");
  printf("subdirectory in the current directory (similar information to an\n");
  printf("ls -s command).  Clicking in a rectangle displays more inforamtion\n");
  printf("about the associated file or directory.  Clicking a second time on a\n");
  printf("directory rectangle changes into that directory.  The '..' button\n");
  printf("changes back up from a subdirectory.\n");
  printf("Color can be used to represent each file's type");
#ifdef OWNERSHIP
  printf(", age, or owner\n");
  printf("by pressing the 'Type', 'Date' or 'Owner' button.\n");
#else
  printf(" or age\n");
  printf("by pressing the 'Type' or 'Date' buttons.\n");
#endif
  printf("This makes many things black on monochrome screens.\n");
  printf("Types are determined by file name and include: \n");
  printf(" Red   for core, *~, *%%, *#\n");
  printf(" Brown for *akefile\n");
  printf(" Green for *.c *.cc *.C *.h *.H\n");
  printf(" SlateGray for *.o\n");
  printf(" Blue for *.icon, *.rs *.gif, *.jpg, *.ps *.eps\n");
  printf(" SkyBlue for *.au, *.snd, *.voc, *.wav\n");
  printf(" OliveDrab for *.Z, *.zip, *.zoo, *.lha, *.tar, *.lzh\n");
  printf("'Deeper' divides all subdirectories into smaller rectangles\n");
  printf("representing files and directories contained in them.\n");
  printf("'Show All' is equivalent to pressing the Deeper button\n");
  printf("as many times as it takes for all subdirectories to be opened\n");
  printf("so all the space on the map is filled with rectangles representing\n");
  printf("files and none are directories.  Animation is slower when more\n");
  printf("rectangles are moving, and Show All on a large directory subtree\n");
  printf("will slow subsequent animation significantly.\n");
  printf("Fewer frames of animation are generated when more rectangles must be moved,\n");
  printf("but even one frame of animation can be slow when hundreds of rectangles move.\n");
#ifdef FRAMEADJUST
  printf("The '+' and '-' buttons increase and decrease the number of frames\n");
  printf("used in each animation.  Decreasing the number of frames increases speed.\n");
#else
  printf("Compile with FRAMEADJUST defined to be able to adjust how many frames are used.\n");
#endif
  printf("'Hide' will remove the currently selected file or subdirectory from the map.\n");
  printf("This was originally called 'Delete' but proved to be dangerously fun to use.\n");
  printf("\n");
}

InitRoot(root)
     treenode *root;   
{
  FILE *pwd = popen("pwd", "r");
  root->name        = (char *)malloc(256);
  fscanf(pwd, "%s", root->name);
  pclose(pwd);
  root->permissions = (char *)malloc(256);
  root->owner       = (char *)malloc(256);
  root->month       = (char *)malloc(4);
  root->yeartime    = (char *)malloc(6);
  ls = popen("/bin/ls -ld .", "r");
  fscanf(ls, "%s %*d %s %d %s %d %s %*s", root->permissions, root->owner, 
	 &(root->size), root->month, &(root->date), root->yeartime);
  root->size = 0;
  root->files = 0;
  pclose(ls);
  root->sibling = 0;
  root->child = 0;
  root->parent = 0;
  root->visible = 1;
  root->image = 0;
  root->border = 0;
  root->label = 0;
  root->x1 = root->y1 = 0;
  root->x2 = 1;
  root->y2 = texttop;
}

char *
InputTree(root, cwd, level)
     treenode *root;
     char *cwd;
     int level;
{
  char *retname;
  char *p;
  char subname[256];
  char subdirname[256];
  char permissions[256];
  int  cwdlen = strlen(cwd), prefixlen=cwdlen+(cwdlen>0);
  treenode *prevnode = 0;
  treenode *newnode;
  int  temp;

  if(level>MAXDEPTH) MAXDEPTH=level;
  if(EOF!=fscanf(ls, "%s %d", subdirname, &temp) && temp)
    while(EOF!=fscanf(ls, "%s",permissions))
      {
	if(strlen(permissions)==10
	   && strspn(permissions, "drwx-"))
	  {
	    newnode = NEW(treenode);	
            p = (char *) malloc(strlen(permissions)+1);
            strcpy(p,permissions);
	    newnode->permissions = p;
	    newnode->owner       = (char *)malloc(256);
	    newnode->month       = (char *)malloc(4);
	    newnode->yeartime    = (char *)malloc(6);
	    newnode->name        = (char *)malloc(256);
	    newnode->child = 0;
	    newnode->parent = root;
	    newnode->sibling = 0;
	    newnode->visible = 0;
	    newnode->image = 0;
	    newnode->border = 0;
	    newnode->label = 0;
	    newnode->x1 = newnode->x2 = newnode->y1 = newnode->y2 = 0;
	    if(prevnode) prevnode->sibling = newnode;
	    else root->child = newnode;
	    fscanf(ls, "%*d %s %d %s %d %s %s ", newnode->owner, &(newnode->size), 
		   newnode->month, &(newnode->date), newnode->yeartime, newnode->name);
	    if(permissions[0]=='d') newnode->size = 0;
	    else 
	      if(CONSTANT_SIZE) {newnode->size = 1; root->size++;}
	      else 
		if(LOG_SIZE) 
		  {
		    temp=0;
		    while(newnode->size>>=1) temp++;
		    newnode->size = temp;
		    root->size += temp;
		  }
		else root->size += newnode->size;
	    newnode->files = 1;
	    prevnode = newnode;
	  }
	else if(!strcmp(permissions, "lrwxrwxrwx")) /* ignore the link */
	  fscanf(ls, "%*d %*s %*d %*s %*d %*s %*s -> %*s");
	else if(!strcmp(permissions, "total")) /* ignore */
	  fscanf(ls, "%*d");
	else if(strchr(permissions, ':')) 
	  {/* this line starts a new subdirectory listing */
	    while(permissions[0] != '\0')
	      {
		if(!strncmp(cwd, permissions, cwdlen) 
		   && permissions[cwdlen] == '/' || cwd[0] == '\0')
		  {
		    strncpy(subname, permissions+prefixlen, (strlen(permissions)-prefixlen));
		    subname[strlen(permissions)-prefixlen-1] = '\0';
		    for(newnode=root->child; 
			newnode && strcmp(newnode->name, subname);
			newnode = newnode->sibling);
		    if(newnode) 
		      { 
			subdirname[0] = '\0';
			if(cwd[0]!='\0')
			  { strcpy(subdirname, cwd);
			    strcat(subdirname, "/");
			  }
			strcat(subdirname, subname);
			retname=InputTree(newnode, subdirname, (level+1));
			root->size += newnode->size;
			if(retname) strcpy(permissions, retname);
			else permissions[0] = '\0';
		      }
		    else 
		      { 
			permissions[0] = '\0';
		      }
		  }
		else 
                   { p = (char *) malloc(strlen(permissions)+1);
                     strcpy(p,permissions);
                     return(p);
                   }
	      }
	  }
      }
  return 0;
}

main(argc, argv)
     int argc;
     char *argv[];
{ int tmp;
  if(argc<2) printinstructions();
  else if(argv[1][0] == 'c') CONSTANT_SIZE=1;
  else if(argv[1][0] == 'l') LOG_SIZE=1;
  TANGOalgoOp(fcn, "BEGIN");

  InitRoot(&root);
  curroot = &root;
  date = popen("date '+%m'", "r");
  fscanf(date, "%d", &thismonth);
  pclose(date);

  ls = popen("/bin/ls -alR", "r");
  InputTree(curroot, "", 1);
  pclose(ls);

  TANGOalgoOp(fcn, "Init", curroot, 0.0, 0.0, 1.0, texttop, VERTICALSLICE, 0);

  TANGOalgoOp(fcn, "Follow", curroot);
  TANGOalgoOp(fcn, "END");
}
