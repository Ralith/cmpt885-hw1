
class Lock {
private:
	volatile int lockvar;
public:
	Lock() {
		lockvar = 0;
	}
	void lock() {
		while(__sync_lock_test_and_set(&lockvar,1));		
	}
	void unlock() {
		lockvar = 0;
	}
};
