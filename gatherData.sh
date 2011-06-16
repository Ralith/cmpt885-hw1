#!/bin/sh

MAXTHREADS=24
SUM=1000000
if [ `uname` = 'Darwin' ]; then
    COUNTER='jot -'
else
    COUNTER='seq'
fi

for PROG in bin/counter/*; do
    for THREADS in `$COUNTER 1 $MAXTHREADS`; do
	read RESULT TIME <<< `$PROG -t $THREADS -i $SUM |cut -d : -f 2`
	echo "`basename $PROG`\t${THREADS}\t${TIME}"
    done
done