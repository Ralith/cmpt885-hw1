//#include <ctype.h>
#include <cstdio>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
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


struct timeval start, stop;

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


	// Spawn threads;
	vector<pthread_t> threads(numThreads);
	vector<ThreadArgs> args(numThreads);
	for (int i = 0; i < numThreads; ++i) {
		args[i].work = targetSum/numThreads;
		if(i == 0) {
			// Account for remainder from integer division.
			args[i].work += targetSum % numThreads;
		}
		pthread_create(&(threads[i]), NULL, thread_kernel, &(args[i]));
	}
	
	// Wait for threads to finish
	for (int i = 0; i < numThreads; ++i) {
		pthread_join(threads[i], NULL);		// wait for threads to finish
	}
	gettimeofday(&stop, NULL);
	
	cout << "sum:"<< counter << endl;
	cout << "time:"<< (stop.tv_sec - start.tv_sec)*1e6 + (stop.tv_usec - start.tv_usec) << endl;
}


void* thread_kernel(void *args) {
	ThreadArgs *my_args = (ThreadArgs*) args;
	int work = my_args->work;
	int myVal = __sync_sub_and_fetch(&oneTimeBarrier,1);
	while(oneTimeBarrier);
	if (myVal == 0) {
		gettimeofday(&start, NULL);
	}
	for (int i = 0; i < work; ++i) {
		int myCnt = counter;
		while (!__sync_bool_compare_and_swap(&counter, myCnt, myCnt+1)) {
			myCnt = counter;
		}
	}
}


void setArgs(int argc, char **argv) {
	
	numThreads = DEFAULT_THREADS;
	targetSum = DEFAULT_TARGETSUM;

	int opt;
	while((opt = getopt(argc, argv, "t:i:")) != -1) {
		switch(opt) {
		case 't':
			numThreads = atoi(optarg);
			break;

		case 'i':
			targetSum = atoi(optarg);
			break;

		default:
		  fprintf(stderr, "Usage: %s [-t threads] [-i sum]\n", argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}
}
