#include <pthread.h>

class Lock {
private:
	pthread_mutex_t mutex;
public:
	Lock() {
		pthread_mutex_init(&mutex, NULL);
	}
	~Lock() {
		pthread_mutex_destroy(&mutex);
	}
	void lock() {
		pthread_mutex_lock(&mutex);
	}
	void unlock() {
		pthread_mutex_unlock(&mutex);
	}
};