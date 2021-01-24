#
# Makefile to create "xtango.o"
#
# Default widget set is Athena.  Type "make hp" to use HP X widgets.
#

HP	  = hp
ATHENA    = athena
MOTIF	  = motif

BFILES    = xtangoanim.o xtangoassoc.o xtangodraw.o xtangogc.o	      \
	    xtangoimage.o xtangoloc.o xtangoop.o xtangopath.o 	      \
	    xtangospline.o xtangotrans.o xtangotwist.o xtangowindow.o \
	    xtangoxt.o xtangotap.o
CFILES    = xtangoanim.c xtangoassoc.c xtangodraw.c xtangogc.c	      \
	    xtangoimage.c xtangoloc.c xtangoop.c xtangopath.c	      \
	    xtangospline.c xtangotrans.c xtangotwist.c xtangowindow.c \
	    xtangoxt.c xtangotap.c
HFILES    = xtango.h xtangolocal.h xtangosup.h xtangoversion.h

CC	  = cc
CPP	  = /lib/cpp

#
# Modify this include directory as needed (or delete if not needed)
#
XINCLUDE  = /usr/include/X11

FLAGS	  = -g -Wall -fdiagnostics-color=never
OPTFLAGS  = -O
CFLAGS    = $(FLAGS) -I$(XINCLUDE)
OPTCFLAGS = $(OPTFLAGS) -I$(XINCLUDE)

$(ATHENA): $(BFILES) xtango$(ATHENA).o $(HFILES)
	(ld -r -o xtango.o $(BFILES) xtango$(ATHENA).o; cp xtango.o ../lib/.)
	$(CPP) <../lib/xtango$(ATHENA).res >../xtango.res

$(HP): $(BFILES) xtango$(HP).o $(HFILES)
	(ld -r -o xtango.o $(BFILES) xtango$(HP).o; cp xtango.o ../lib/.)
	$(CPP) <../lib/xtango$(HP).res >../xtango.res

$(MOTIF): $(BFILES) xtango$(MOTIF).o $(HFILES)
	(ld -r -o xtango.o $(BFILES) xtango$(MOTIF).o; cp xtango.o ../lib/.)
	$(CPP) <../lib/xtango$(MOTIF).res >../xtango.res

$(BFILES) xtango$(ATHENA).o xtango$(HP).o xtango$(MOTIF).o: $(HFILES)

install:
	cp xtango.h ../include/xtango.h

clean:
	rm -f $(BFILES) xtango$(ATHENA).o xtango$(HP).o  xtango$(MOTIF).o \
			xtango.o

spotless:
	rm -f $(BFILES) xtango.o ../include/xtango.h ../lib/xtango.o \
	      ../*.res xtango$(ATHENA).o xtango$(HP).o xtango$(MOTIF).o

# end of Makefile
