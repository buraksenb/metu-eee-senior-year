#pragma once
// PLEASE READ THIS

// StackQueue class implementation has top and bottom and has pop_front and pop_rear methods.
// Thus this StackQueueLL class which is linked list implementation of StackQueue has pop_front and pop_rear as well
// However, I did not want to push HEAD of the linked list every time. Thus, FRONT of the StackQueue is actually the TAIL.
// Whereas, REAR of the StackQueue is the HEAD.

// StackQueue front --> StackQueueLL tail
// StackQueue rear  --> StackQueueLL head

template <typename T>
class StackQueueLL {
public:
    // Define a Node class for linked list
    class Node {
    public:

        Node* next;
        Node* prev;
        T data;
        Node() : next(nullptr), prev(nullptr) {}

        Node(const T& _data) : next(nullptr), prev(nullptr), data(_data) {}
        // copy constructor
        Node(const Node& other) : next(other.next), prev(other.prev),
            data(other.data) {}
        // assignment operator
        Node& operator=(const Node& other) {

        	next = other.next;
            prev = other.prev;
            data = other.data;
            return *this;
        }
        ~Node() {}

    };

private:

    Node* head;
    Node* tail;
    int sizeoflist;

public:
	StackQueueLL():head(nullptr),tail(nullptr),sizeoflist(0){}
    void push_front(T _data);
    T pop_front();
    T pop_rear();
    bool StackQueueEmpty() const;
    int getSizeofList() const { return sizeoflist; }
};

