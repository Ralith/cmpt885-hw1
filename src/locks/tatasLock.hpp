class Lock {
private:
	volatile int lockvar;
public:
	Lock() {
		lockvar = 0;
	}
	void lock() {
		while(lockvar);
		while (__sync_lock_test_and_set(&lockvar,0,1)) {
			while(lockvar);
		}
		
		
	}
	void unlock() {
		lockvar = 0;
	}
};
