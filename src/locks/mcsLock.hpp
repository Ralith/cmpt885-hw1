#include "../atomic_ops.h"

class Lock {
private:
	volatile mcs_qnode_t *queue;
public:
	Lock() : queue(0) {}
	void lock() {
		mcs_qnode_t *thread_node = new mcs_qnode_t;
		mcs_acquire(&queue, thread_node);
	}
	void unlock() {
		mcs_qnode_t *thread_node;
		while(thread_node->next) {
			thread_node = thread_node->next;
		}
		mcs_release(&queue, thread_node);
	}
};
