#! /bin/ksh 
###############################################################################
#
# File:         doselect
# RCS:          $Header: $
# Description:  Select animation to generate
# Author:       Chuck Untulis, RCS/HPLabs Hewlett-Packard Company
# Address:      chuck@hplcau.hpl.hp.com
# Phone:        (415)857-7160
# Created:      Tue Oct  8 14:26:57 1991
# Modified:     Tue Oct  8 14:55:55 1991 (Chuck Untulis) chuck@hplcau
# Language:     Ksh
# Package:      N/A
# Status:       Experimental
#
#
###############################################################################

print
print "Select animation to run"
print "=================================="

PS3='Pick one of the above (return to see the list again): '
select i in "clean .o"\
	"clean .o and apps" \
        "build all" \
	"build anim" \
	"run anim"\
	"build bmap" \
	"run bmap" \
	"build bpack" \
	"run bpack" \
	"build bsearch" \
	"run bsearch" \
	"build bsort" \
	"run bsort" \
	"build bubsort" \
	"run bubsort" \
	"build color" \
	"run color" \
	"build dfs" \
	"run dfs" \
	"build hanoi" \
	"run hanoi" \
	"build heap" \
	"run heap" \
	"build kmp" \
	"run kmp" \
	"build merge" \
	"run merge" \
	"build pc" \
	"run pc" \
	"build post" \
	"run post" \
	"build quicksort" \
	"run quicksort" \
	"build random" \
	"run random" \
	"build roach" \
	"run roach" \
	"build shuffle" \
	"run shuffle" \
	"build spath" \
	"run spath" \
	"build test" \
	"run test" \
	"build tree" \
	"run tree" \
	"Done (quit)"

do case $i in 
	"clean .o" )            make clean;;
	"clean .o and apps" )   make spotless;;
	"build all" )           make all;;
	"build anim" )		make PROG=anim;;
	"run anim" )		anim;;
	"build bmap" )		make PROG=bmap;;
	"run bmap" )		bmap;;
	"build bpack" )		make PROG=bpack;;
	"run bpack" )		bpack;;
	"build bsearch" )	make PROG=bsearch;;
	"run bsearch" )		bsearch;;
	"build bsort" )		make PROG=bsort;;
	"run bsort" )		bsort;;
	"build bubsort" )	make PROG=bubsort;;
	"run bubsort" )		bubsort;;
	"build color" )		make PROG=color;;
	"run color" )		color;;
	"build dfs" )		make PROG=dfs;;
	"run dfs" )		dfs;;
	"build hanoi" )		make PROG=hanoi;;
	"run hanoi" )		hanoi;;
	"build heap" )		make PROG=heap;;
	"run heap" )		heap;;
	"build kmp" )		make PROG=kmp;;
	"run kmp" )		kmp;;
	"build merge" )		make PROG=merge;;
	"run merge" )		merge;;
	"build pc" )		make PROG=pc;;
	"run pc" )		pc;;
	"build post" )		make PROG=post;;
	"run post" )		post;;
	"build quicksort" )	make PROG=quicksort;;
	"run quicksort" )	quicksort;;
	"build random" )	make PROG=random;;
	"run random" )		random;;
	"build roach" )		make PROG=roach;;
	"run roach" )		roach;;
	"build shuffle" )	make PROG=shuffle;;
	"run shuffle" )		shuffle;;
	"build spath" )		make PROG=spath;;
	"run spath" )		spath;;
	"build test" )		make PROG=test;;
	"run test" )		test;;
	"build tree" )		make PROG=tree;;
	"run tree" )		tree;;
	"Done (quit)" )         exit;;     
	"")			print "You must select one of the above";;
	esac
done

