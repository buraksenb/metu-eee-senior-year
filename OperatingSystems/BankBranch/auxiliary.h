/*
 * Created by Burak ŞEN for EE442 HW1
 * Includes struct definitions and function prototypes which will be used
 * in the main.c file.
 */

#ifndef BANKBRANCH_AUXILIARY_H
#define BANKBRANCH_AUXILIARY_H

#endif //BANKBRANCH_AUXILIARY_H
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct BankClient_st {
    int id; // Client ID
    double duration; // banking transaction duration
}BankClient;


typedef struct Information_st {
    int paydeskNo;
    BankClient client;
} Information;

// Typical circular queue
typedef struct ClientQueue_st {
    BankClient* array;
    int maxSize;
    int currentSize;
    int frontIndex;
    int rearIndex;
}ClientQueue;

// Function prototypes which are implemented in aux.c file.
void Enqueue(ClientQueue* ptrClientQueue, BankClient* ptrNewClient);
BankClient* Dequeue(ClientQueue* ptrClientQueue);
double generateExponentialRandomValue(double rate);