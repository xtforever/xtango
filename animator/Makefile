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
WIDGETS   = Xaw
#WIDGETS   = Xm

XDIR      = .
XTANGODIR = ../lib
XLIBDIR   = .

XTANGOBJ  = $(XTANGODIR)/xtango.o
INCLUDE   = -I$(XDIR) -I../include
XLIB      = -L$(XLIBDIR) -l$(WIDGETS) -lXmu -lXext -lXt -lX11
MLIB      = -lm

CC        = gcc
FLAGS     = -g -Wall -fdiagnostics-color=never
OPTFLAGS  = -O
CFLAGS    = $(FLAGS) $(INCLUDE)
OPTCFLAGS = $(OPTFLAGS) $(INCLUDE)

animator: animator.c $(XTANGOBJ)
	$(CC) $(CFLAGS) -o animator animator.c $(XTANGOBJ) $(XLIB) $(MLIB)

clean:
	rm -f *.o animator

spotless:
	rm -f *.o
	rm -f $(TESTS)

# end of Makefile

