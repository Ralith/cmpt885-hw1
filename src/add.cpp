//#include <ctype.h>
//#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include "hrtime.h"
#include <iostream>
#include <vector>
#include <pthread.h>
#include "locks.h"

using namespace std;
    
const int DEFAULT_THREADS = 4;
const int DEFAULT_TARGETSUM = 10000;
int numThreads;
int targetSum;
volatile int counter;
volatile int oneTimeBarrier;
Lock* lock;

unsigned long long start, stop;

void setArgs(int argc, char **argv);
void* thread_kernel(void *args);

struct ThreadArgs {
	int thread_num;
	int core_affinity;			
	int work;
};

int main (int argc, char **argv) {
	setArgs(argc, argv);
	counter = 0;
	oneTimeBarrier = numThreads;
	lock = new Lock();
	// Spawn threads;
	vector<pthread_t> threads(numThreads);
	vector<ThreadArgs> args(numThreads);
	for (int i = 0; i < numThreads; ++i) {
		args[i].work = targetSum/numThreads;
		pthread_create(&(threads[i]), NULL, thread_kernel, &(args[i]));
	}
	
	// Wait for threads to finish
	for (int i = 0; i < numThreads; ++i) {
		pthread_join(threads[i], NULL);		// wait for threads to finish
	}
	stop = getElapsedTime();
	
	cout << "sum:"<< counter << endl;
	cout << "time:"<< (stop-start)/1000 << endl;
}


void* thread_kernel(void *args) {
	ThreadArgs *my_args = (ThreadArgs*) args;
	int work = my_args->work;
	int myVal = __sync_sub_and_fetch(&oneTimeBarrier,1);
	while(oneTimeBarrier);
	if (myVal == 0) {
		start = getElapsedTime();
	}
	for (int i = 0; i < work; ++i) {
		lock->lock();
		counter++;
		lock->unlock();
	}
}


void setArgs(int argc, char **argv) {
	
	numThreads = DEFAULT_THREADS;
	targetSum = DEFAULT_TARGETSUM;
	
	/* todo code to parse for command line options
	   for now assume that first argument is threads; 2nd is target sum
	*/
	
	if (argc > 1) {
		// -t #cores
		numThreads = atoi(argv[1]);
	}
	if (argc > 2) {
		// -i #targetSum
		targetSum = atoi(argv[2]);
	}
}
