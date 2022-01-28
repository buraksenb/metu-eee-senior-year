/*
 * Created by Burak ŞEN for EE442 HW1
 * Includes function implementations which will be used
 * in the main.c file.
 */
#include "auxiliary.h"

// To generate exponentially distributed random value with changing rate.
double generateExponentialRandomValue(double rate)
{
    // To get random value between 0-1.
    double x = ((double) rand()) / ((double) RAND_MAX) ;

    // Compute uniform random variable according to formula.
    double result = -(log(1-x)/rate);
    return result;
}



/*
 * Dequeue and Enqueue functions are highly referenced from https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
 * web page which implements Circular Queue in C++ language. Since main purpose of the homework is not implementing this circular queue while
 * its purpose is understanding multithreading, mutexes and pthreads, I have just converted that implementation in C style and tested that
 * it works as intended.
*/

// Variation of the typical dequeue operation for circular queues.
BankClient* Dequeue(ClientQueue* ptrClientQueue)
{
    BankClient* retPtr = 0;
    if(ptrClientQueue->frontIndex == -1)
    {
        printf("[ERROR]: Queue is empty, this is not desired!\n");
        return (BankClient*) 0;
    }
    // Get address of front of the queue
    retPtr = &(ptrClientQueue->array[ptrClientQueue->frontIndex]);
    //  ptrClientQueue->array[ptrClientQueue->frontIndex].duration = 0.0;
    //  ptrClientQueue->array[ptrClientQueue->frontIndex].id = 0;
    ptrClientQueue->currentSize -= 1;
    // If frontIndex and rearIndex are equal then queue is now empty and adjust the indexes.
    if(ptrClientQueue->frontIndex == ptrClientQueue->rearIndex)
    {
        ptrClientQueue->frontIndex = -1;
        ptrClientQueue->rearIndex = -1;
    }
    // If frontIndex is at the end of array, since this is circular. Make frontIndex point to the start of the array
    // element.
    else if(ptrClientQueue->frontIndex == ptrClientQueue->maxSize -1)
    {
        ptrClientQueue->frontIndex = 0;
    }
    // Otherwise simple operation with frontIndex is incremented for next element.
    else
    {
        ptrClientQueue->frontIndex += 1;
    }
    // Return the address of element.
    return retPtr;
}

void Enqueue(ClientQueue* ptrClientQueue, BankClient* ptrNewClient)
{
    // Check if queue is full.
    if((ptrClientQueue->frontIndex == 0 && ptrClientQueue->rearIndex == (ptrClientQueue->maxSize))
       || (ptrClientQueue->rearIndex == ((ptrClientQueue->frontIndex-1) % ptrClientQueue->maxSize)))
    {
        printf("[ERROR]: Queue is full, this is not desired!\n");
        return;
    }
    // If frontIndex is -1 then queue is empty.
    else if(ptrClientQueue->frontIndex  == -1)
    {
        ptrClientQueue->frontIndex = 0;
        ptrClientQueue->rearIndex = 0;
        ptrClientQueue->array[0].duration = ptrNewClient->duration;
        ptrClientQueue->array[0].id = ptrNewClient->id;
    }
    // If rearIndex is at the end of queue while frontIndex is not 0, then new rear is the start of the queue.
    else if((ptrClientQueue->rearIndex == (ptrClientQueue->maxSize -1)) && ptrClientQueue->frontIndex  != 0)
    {
        ptrClientQueue->rearIndex = 0;
        ptrClientQueue->array[ptrClientQueue->rearIndex].duration = ptrNewClient->duration;
        ptrClientQueue->array[ptrClientQueue->rearIndex].id = ptrNewClient->id;
    }
    // Normal queue operation.
    else
    {
        ptrClientQueue->rearIndex +=1;
        ptrClientQueue->array[ptrClientQueue->rearIndex].duration = ptrNewClient->duration;
        ptrClientQueue->array[ptrClientQueue->rearIndex].id = ptrNewClient->id;

    }
    ptrClientQueue->currentSize++;
}
