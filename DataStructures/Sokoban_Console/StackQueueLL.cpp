#include "StackQueueLL.h"
#include  <iostream>
// Please read the explanation at StackQueueLL.h

template<typename T>
// front is the tail
void StackQueueLL<T>::push_front(T _data)
{
    // create a node in heap memory
    Node* newnode = new Node(_data);
    Node* tmp;
    // If head is nullptr. Then, linked list is empty
    if (head == nullptr)
    {
        head = newnode;
        tail = newnode;
    }
    else
    {
        // To add Node at the end of list, first store tail
        tmp = tail;
        // next node of old tail is the new tail which is newnode
        tmp->next = newnode;
        // previous of newnode(new tail) is old tail(temp)
        newnode->prev = tmp;
        // newnode is tail
        tail = newnode;
    }
    sizeoflist++;
}
template<typename T>
T StackQueueLL<T>::pop_front()
{
    T popdata;
    // If head is nullptr list is empty
    if (head == nullptr)
    {
        std::cerr << "Head is null";
        exit(1);
    }
    // if tail->prev ==nullptr head and tail are same and list has only 1 node
    if (tail->prev == nullptr)
    {
        popdata = head->data;
        // Since head and tail are equal, only delete tail is used to free heap allocated memory.
        delete tail;
        head = nullptr;
        tail = nullptr;
        return popdata;
    }
    // Otherwise it is easy to pop tail
    // Delete tail and make previous node new tail
    popdata = tail->data;
    Node* tmp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    // delete old tail
    delete tmp;
    tmp = nullptr;
    sizeoflist--;
    return popdata;
}
template<typename T>
T StackQueueLL<T>::pop_rear()
{
    T popdata;
    if (head == nullptr)
    {
         // If head is nullptr list is empty
        std::cerr << "Head is null";
        exit(1);
    }
    if (head->next == nullptr)
    {
        popdata = head->data;
        // Since head and tail are equal, only delete head is used
        delete head;
        head = nullptr;
        tail = nullptr;
        return popdata;
    }
    // Next node of head becomes the new Head.
    // return data of old head and free the heap allocated memory.
    popdata = head->data;
    Node* tmp = head;
    head = head->next;
    head->prev = nullptr;
    delete tmp;
    tmp = nullptr;
    sizeoflist--;
    return popdata;
}

template<typename T>
bool StackQueueLL<T>::StackQueueEmpty() const
{
    // If head or tail is nullptr list is empty
    if (head == nullptr || tail == nullptr)
    {
        return true;
    }
    return false;
}
