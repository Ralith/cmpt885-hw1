#!/bin/sh

MAXTHREADS=24
SUM=1000000
INCREMENT=3
ITERATIONS=5
CPUS=4
CORES=6

if [ `uname` = 'Darwin' ]; then
    THRCOUNTER="jot - 1 $MAXTHREADS $INCREMENT"
    ITERCOUNTER="jot - 1 $ITERATIONS"
else
    THRCOUNTER="seq 1 $INCREMENT $MAXTHREADS"
    ITERCOUNTER="jot - 1 $ITERATIONS"
fi

ALLCORES="`seq -s , 0 $CPUS $[$CPUS*$CORES-1]`,`seq -s , 1 $CPUS $[$CPUS*$CORES-1]`,`seq -s , 2 $CPUS $[$CPUS*$CORES-1]`,`seq -s , 3 $CPUS $[$CPUS*$CORES-1]`"

echo -e "Prog\tIter\tThreads\tTime"
for PROG in bin/counter/*; do
    for THREADS in `$THRCOUNTER`; do
        for ITERATION in `$ITERCOUNTER`; do
	    COREIDS=`echo $ALLCORES |cut -d , -f -$THREADS`
	    read RESULT TIME <<< `taskset -c $COREIDS $CORES $PROG -t $THREADS -i $SUM |cut -d : -f 2`
	    echo -e "`basename $PROG`\t${ITERATION}\t${THREADS}\t${TIME}"
        done
    done
done
