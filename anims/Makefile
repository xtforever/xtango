#
# Simple Makefile to create animations
#
# Notice that if you're using the HP X Widget version of XTANGO,
# you must have the HP X Widget set library to link.
#

#
# Select appropriate Widget set:
#    Uncomment the first line below for HP X widgets; second for Athena.
#
#WIDGETS   = Xw
#WIDGETS   = Xaw
WIDGETS   = Xm

XDIR      = .
XTANGODIR = ../lib
XLIBDIR   = .

XTANGOBJ  = $(XTANGODIR)/xtango.o
INCLUDE   = -I$(XDIR) -I../include
#XLIB      = /usr/lib/libXm.a /usr/lib/libXmu.a /usr/lib/libXext.a\
#		/usr/lib/libXt.a /usr/lib/libX11.a
XLIB      = -L$(XLIBDIR) -l$(WIDGETS) -lXmu -lXext -lXt -lX11
MLIB      = -lm

TESTS     = allocator anim avl binoheap bintree bmap boyer bpack bres\
            bsearch bsort btree bubsort clip color combsort conhull\
            dfa dfs dijkstra dirviz fact fft fiboheap floydwar\
            gameoflife gen ghost graham hanoi hash heap hilbert kepler kmp\
            knight kruskal ksel list\
            matrixmult merge pairheap pc pol_calc polyscan\
            post postfix prim queens\
            quicksort quicky radix random rbtree rdblktree\
            roach shaker shellsort\
            shuffle spath test topsort treap tree tsp viewtran vmem

CC        = cc
FLAGS     = -g 
OPTFLAGS  = -O
CFLAGS    = $(FLAGS) $(INCLUDE)
OPTCFLAGS = $(OPTFLAGS) $(INCLUDE)

executable:
	$(CC) $(CFLAGS) $(PROG).c $(PROG)scenes.c $(XTANGOBJ)\
              $(XLIB) $(MLIB) -o $(PROG) 

all: 
	./doscript

clean:
	rm -f *.o

spotless:
	rm -f *.o
	rm -f $(TESTS)




# This is an example of the old-fashioned way.

anim.o: anim.c
	$(CC) $(FLAGS) $(INCLUDE) -c anim.c
animscenes.o: animscenes.c
	$(CC) $(FLAGS) $(INCLUDE) -c animscenes.c
anim: anim.o animscenes.o $(XTANGOBJ)
	$(CC) -o anim anim.o animscenes.o $(XTANGOBJ) $(XLIB) $(MLIB)


# end of Makefile
