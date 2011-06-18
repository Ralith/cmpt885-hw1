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
	Lock* lock;
	char pad4[64];
	Queue(int delayInit, int delayMax) {
		Node<T>* tmp = new Node<T>();
		tmp->next = NULL;
		head = tmp;
		tail = tmp;
		lock = new Lock(delayInit, delayMax);
	}

	void push(Node<T>* inNode) {
		//Node<T>* newNode = new Node<T>();
		//newNode->data = val;
		lock->lock();
		inNode->next = NULL;
		tail->next = inNode;
		tail = inNode;
		lock->unlock();
	} 
	
	bool pop(Node<T>** outNodeAddr) {
		lock->lock();
		Node<T>* node = head;
		Node<T>* newHead = head->next;
		if (newHead == NULL) {
			lock->unlock();
			return false;
		}
		
		node->data = newHead->data;
		*outNodeAddr = node;
		head = newHead;
		lock->unlock();
		return true;
	}
};



