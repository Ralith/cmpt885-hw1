class Lock {
private:
	volatile int lockvar;
public:
	Lock() {
		lockvar = 0;
	}
	void lock() {
		while(lockvar);
		while (!__sync_bool_compare_and_swap(&lockvar,0,1)) {
			while(lockvar);
		}
		
		
	}
	void unlock() {
		lockvar = 0;
	}
};