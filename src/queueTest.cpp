
#include <vector>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

void* thread_kernel(void *args);
//int plat_affinity_setcpu(pthread_t thread, int cpuid);

/*
int plat_affinity_setcpu(pthread_t thread, int cpuid){
	//printf("setting affinity for process %i to core %i\n", (int)thread, cpuid);
	cpu_set_t mask;
	CPU_ZERO( &mask );
	CPU_SET(cpuid, &mask);
	return sched_setaffinity(thread, sizeof(mask), &mask);
}
*/