#include "node.h"
#include "tatasLock.hpp"
#include <iostream>

template<class T>
class Queue {
public:
	char pad1[64];
	Node<T>* head;
	char pad2[64];
	Node<T>* tail;
	char pad3[64];
	Lock* h_lock;
	char pad4[64];
	Lock* t_lock;
	char pad5[64];
	Queue(int delayInit, int delayMax) {
		Node<T>* tmp = new Node<T>();
		tmp->next = NULL;
		head = tmp;
		tail = tmp;
		h_lock = new Lock(delayInit, delayMax);
		t_lock = new Lock(delayInit, delayMax);
	}

	void push(Node<T>* inNode) {
		//Node<T>* newNode = new Node<T>();
		//newNode->data = val;
		t_lock->lock();
		inNode->next = NULL;
		tail->next = inNode;
		tail = inNode;
		t_lock->unlock();
	} 
	
	bool pop(Node<T>** outNodeAddr) {
		h_lock->lock();
		Node<T>* node = head;
		Node<T>* newHead = head->next;
		if (newHead == NULL) {
			h_lock->unlock();
			return false;
		}
		
		node->data = newHead->data;
		//std::cout << node->data << std::endl;
		*outNodeAddr = node;
		head = newHead;
		h_lock->unlock();
		return true;
	}
};



