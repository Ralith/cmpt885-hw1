class Lock {
private:
  volatile int queue, dequeue;
public:
	Lock() : queue(0), dequeue(0) {
	}

	void lock() {
    int ticket = __sync_fetch_and_add(&queue, 1);
    while(dequeue != ticket);
	}

	void unlock() {
		++dequeue;
	}
};
