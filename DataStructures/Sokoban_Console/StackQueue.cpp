#include "StackQueue.h"
#include <iostream>

template <typename T>
StackQueue<T>::StackQueue()
	:top(0),bottom(0),isthereFirstElement(false)
{
}

template <typename T>
void StackQueue<T>::push_front(T objtopush)
{
	// If StackQueue is empty push data to top. However, do not increment it.
	// Moreover, assing true to isthereFirstElement to know StackQueue is not empty.
	if(StackQueueEmpty())
	{
		Stacklist[top] = objtopush;
		isthereFirstElement = true;
	}
	// General case when top is not at the end of array
	else if(top < maxstacksize -1)
	{
		top++;
		Stacklist[top] = objtopush;
	}

	else if( top == maxstacksize -1)
	{
		// Since this StackQueue has ability to pop data from rear, bottom may be larger than 0
		// In this case, even if top is at the end of array StackQueue is not full
		// To solve this rotate StackQueue to left with the bottom's distance from 0
		// This is only done when top is at the end of array for optimization
		if( bottom > 0)
		{
			int botcounter = 0;
			for(int i = 0 ; i < maxstacksize - bottom ; i++)
			{
				Stacklist[botcounter] = Stacklist[botcounter + bottom];
				botcounter++;
			}
			top = top - bottom;
			bottom = 0;
			top++;
			Stacklist[top] = objtopush;
		}
		else
		{
			std::cerr << "Stack is full\n";
		}
	}
}

template <typename T>
T StackQueue<T>::pop_front()
{
	// When stackqueue is not empty decrement top and return the old top value
	if (!StackQueueEmpty())
	{
			top--;
			return Stacklist[top + 1];
	}
		std::cerr << "Stack is empty\n";
		exit(1);

}

template <typename T>
T StackQueue<T>::pop_rear()
{
	// When stackqueue is not empty increment bottom and return the old bottom value
	if(!StackQueueEmpty())
	{
	bottom++;
	return Stacklist[bottom - 1];
	}
		exit(1);
 }

template<typename T>
bool StackQueue<T>::StackQueueEmpty() const
{
    // If one of these statements are true stackqueue is empty
	if(!isthereFirstElement || bottom > top || top < 0)
	{
		return true;
	}
	return false;

}

template<typename T>
bool StackQueue<T>::StackQueueFull() const
{
    // We have to check bottom as well to determine if StackQueue is full or not
    // due to StackQueue has pop_rear method.
	if(top == maxstacksize-1 && bottom == 0)
	{
		return true;
	}
	return false;
}
