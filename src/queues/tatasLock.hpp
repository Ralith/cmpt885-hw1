#include "work.h"


class Lock {
private:
	volatile int lockvar[16];
	int delay;
	int max;
	
public:
	Lock(int delayInit, int delayMax) {
		lockvar[0] = 0;
		delay = delayInit;
		delayMax = delayMax;
	}
	void lock() {
		int d = delay;
		while(lockvar[0]);
		while (!__sync_bool_compare_and_swap(&lockvar[0],0,1)) {
			workKernel(delay);
			d = d < max ? d*2 : max;
			while(lockvar[0]);
		}
		
		
	}
	void unlock() {
		lockvar[0] = 0;
	}
};
