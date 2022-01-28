#pragma once
#include "Sokoban.h"
const int maxstacksize = 200;
template<typename  T>
class StackQueue
{
private:
	T Stacklist[maxstacksize];
	int top;
	int bottom;
	// To know StackQueue is not empty when top = 0 and bot = 0 easily
	bool isthereFirstElement;
public:
	StackQueue();
	void push_front(T objtopush);
	T pop_front();
	T pop_rear();
	bool StackQueueEmpty() const;
	bool StackQueueFull() const;
		
};

