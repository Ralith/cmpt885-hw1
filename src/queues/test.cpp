#include <iostream>
#include "node.h"
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <assert.h>
#include <cstdio>

#define MILLION 1000000

#ifdef ONELOCK
#include "1lockQueue.hpp"
#endif

#ifdef TWOLOCK
#include "2lockQueue.hpp"
#endif

using namespace std;

const int DEFAULT_THREADS(2);
const int DEFAULT_TARGETSUM(1000);
const int RAND_OPS(100);

struct timeval start, stop;

void setArgs(int argc, char **argv);
void* thread_kernel(void *args);
void init();


vector< vector<bool> > qOperation;
vector< Node<int> > nodePool;
vector< vector< Node<int>* > > nodePtrPool;

volatile int oneTimeBarrier;
int numThreads;
int targetSum;
int backOffInit, backOffMax;
Queue<int>* q;

struct ThreadArgs {
	int tid;
	int work;
};



int main(int argc, char* argv[]) {
	setArgs(argc, argv);
	init();
	
	// create the Q
	q= new Queue<int>(backOffInit,backOffMax);
	
	// preload Q so we don't accidentally underflow
	vector< Node<int > > preloadedNodes(numThreads*RAND_OPS/2);
	for (int i = 0; i < preloadedNodes.size(); i++) {
		q->push(&preloadedNodes[i]);
	}
	
	vector<pthread_t> threads(numThreads);
	vector<ThreadArgs> args(numThreads);
	for (int i = 0; i < numThreads; ++i) {
		args[i].work = targetSum/numThreads;
		args[i].tid = i;
		pthread_create(&(threads[i]), NULL, thread_kernel, &(args[i]));
	}
	

	// Wait for threads to finish
	for (int i = 0; i < numThreads; ++i) {
		pthread_join(threads[i], NULL);		// wait for threads to finish
	}

	gettimeofday(&stop, NULL);

	cout << "time:"<< (suseconds_t)((stop.tv_sec - start.tv_sec)*MILLION) + stop.tv_usec - start.tv_usec << endl;
	
}

void* thread_kernel(void *args) {
	ThreadArgs *my_args = (ThreadArgs*) args;
	int myTid = my_args->tid;
	int work = my_args->work;
	int myVal = __sync_sub_and_fetch(&oneTimeBarrier,1);
	while(oneTimeBarrier);
	if (myVal == 0) {
		gettimeofday(&start, NULL);
	}
	
	
	for (int i = 0; i < work; ++i) {
		bool pushOp = qOperation[myTid][i%RAND_OPS];
		if (pushOp) {
			//cout << i << ", tid: " << myTid << " : pushing" << endl;
			Node<int>* node = nodePtrPool[myTid].back();
			nodePtrPool[myTid].pop_back();
			q->push(node);
		} else {
			Node<int>* node;
			//cout <<  i << ", tid: " << myTid << " : popping" << endl;
			assert(q->pop(&node) && "The queue shouldn't be empty, it was preloaded with enough items");
			nodePtrPool[myTid].push_back(node);
		}
	}
}

void setArgs(int argc, char **argv) {
	
	numThreads = DEFAULT_THREADS;
	targetSum = DEFAULT_TARGETSUM;
	backOffInit = 10;
	backOffMax = 10000;
	/* todo code to parse for command line options
	   for now assume that first argument is threads; 2nd is target sum
	*/
	
	
	int opt;
	while((opt = getopt(argc, argv, "t:i:a:b:")) != -1) {
		switch(opt) {
		case 't':
			numThreads = atoi(optarg);
			break;

		case 'i':
			targetSum = atoi(optarg);
			break;
		case 'a':
			backOffInit = atoi(optarg);
			break;
		case 'b':
			backOffMax = atoi(optarg);
			break;
		default:
		  fprintf(stderr, "Usage: %s [-t threads] [-i operations] [-a lockBackOffInit] [-b lockBackOffMax]\n", argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}

	
	
}

void init() {
	oneTimeBarrier = numThreads;
	
	// Make random order of operations, need to blance push and pops
	// each thread has own random pool.
	qOperation.resize(numThreads);
	
	for (int i = 0; i < qOperation.size(); i++) {
		vector<bool>::iterator it;
		it = qOperation[i].begin();
		qOperation[i].insert(it, RAND_OPS/2, true);
		it = qOperation[i].begin();
		qOperation[i].insert(it, RAND_OPS/2, false);
		std::random_shuffle ( qOperation[i].begin(), qOperation[i].end() );
	}
	
	//for (int i = 0; i < qOperation.size(); i++) {
	//	cout << qOperation[i].size() << endl;
	//}
	
	// Create a local pool of nodes for each thread
	nodePool.resize(numThreads*RAND_OPS/2);
	
	// 
	nodePtrPool.resize(numThreads);
	int nodeCnt = 0;
	for (int i = 0; i < nodePtrPool.size(); i++) {
		nodePtrPool[i].resize( RAND_OPS/2 );
		for (int j = 0; j < nodePtrPool[i].size(); j++) {
			nodePool[nodeCnt].data = nodeCnt;
			nodePtrPool[i][j] = &nodePool[nodeCnt++];
		}
	}
}
