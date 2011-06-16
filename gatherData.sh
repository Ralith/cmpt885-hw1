#!/bin/sh

MAXTHREADS=24
SUM=1000000
ITERATIONS=5

if [ `uname` = 'Darwin' ]; then
    COUNTER='jot -'
else
    COUNTER='seq'
fi

echo -e "Prog\tIter\tThreads\tTime"
for PROG in bin/counter/*; do
    for THREADS in `$COUNTER 1 $MAXTHREADS`; do
        for ITERATION in `$COUNTER 1 $ITERATIONS`; do
	    read RESULT TIME <<< `$PROG -t $THREADS -i $SUM |cut -d : -f 2`
	    echo -e "`basename $PROG`\t${ITERATION}\t${THREADS}\t${TIME}"
        done
    done
done
