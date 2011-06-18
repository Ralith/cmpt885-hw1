TARGETS = oneLockQueue twoLockQueue
FILES = node.h test.cpp work.h work.cpp tatasLock.hpp

all: $(TARGETS) 

oneLockQueue: 1lockQueue.hpp $(FILES) work.o
	g++ -lpthread -DONELOCK -O3 work.o -o $@ test.cpp

twoLockQueue: 2lockQueue.hpp $(FILES) work.o
	g++ -lpthread -DTWOLOCK -O3 work.o -o $@ test.cpp

work.o: work.cpp
	g++ -c -o work.o work.cpp