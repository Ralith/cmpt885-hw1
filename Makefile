# Directory locations
LOCKS_DIR := src/locks
BUILD_DIR := src/build
COUNTER_BIN := bin/counter
QUEUE_BIN := bin/queue

LOCKS_HPPS := $(wildcard $(LOCKS_DIR)/*Lock.hpp)
LOCKS := $(LOCKS_HPPS:$(LOCKS_DIR)/%.hpp=%)

COUNTER_EXECS := $(LOCKS:%=$(COUNTER_BIN)/%)


# Compile flags
CXX := -O3 
LDFLAGS := -lpthread
CC := g++

all: $(COUNTER_EXECS) src/locks.h

$(COUNTER_BIN)/%: $(LOCKS_DIR)/%.hpp src/locks.h src/add.cpp
	$(CC) $(CXX) $(LDFLAGS) -I$(LOCKS_DIR) -D___$(@:$(COUNTER_BIN)/%=%)___ -o $@ src/add.cpp

header: 
	rm -f src/locks.h
	for lock in $(LOCKS) ; do \
		echo "#ifdef ___"$$lock"___" >> src/locks.h; \
		echo "#include \""$$lock".hpp\"" >> src/locks.h; \
		echo "#endif" >> src/locks.h; \
	done
	cat src/locks.h

test: $(COUNTER_EXECS)
	for prog in $(COUNTER_EXECS) ; do \
		echo $$prog; \
		$$prog; \
	done

clean:
	rm -rf bin/counter/*