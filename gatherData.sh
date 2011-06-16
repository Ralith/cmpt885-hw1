#!/bin/sh

MAXTHREADS=24
SUM=1000000
INCREMENT=2
ITERATIONS=5

if [ `uname` = 'Darwin' ]; then
    THRCOUNTER="jot - 1 $MAXTHREADS $INCREMENT"
    ITERCOUNTER="jot - 1 $ITERATIONS"
else
    THRCOUNTER="seq 1 $INCREMENT $MAXTHREADS"
    ITERCOUNTER="seq 1 $ITERATIONS"
fi

echo -e "Prog\tIter\tThreads\tTime"
for PROG in bin/counter/*; do
    for THREADS in `$THRCOUNTER`; do
        for ITERATION in `$ITERCOUNTER`; do
	    read RESULT TIME <<< `$PROG -t $THREADS -i $SUM |cut -d : -f 2`
	    echo -e "`basename $PROG`\t${ITERATION}\t${THREADS}\t${TIME}"
        done
    done
done
