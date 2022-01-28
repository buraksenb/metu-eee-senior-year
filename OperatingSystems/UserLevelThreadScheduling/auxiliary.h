//
// Created by burak on 5/7/2021.
//

#ifndef E2235646_EE442_HW2_AUXILIARY_H
#define E2235646_EE442_HW2_AUXILIARY_H

#endif //E2235646_EE442_HW2_AUXILIARY_H
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <poll.h>
#define  STACK_SIZE 4096
#define  STATE_EMPTY 0
#define  STATE_READY 1
#define  STATE_RUNNING 2
#define  STATE_EXIT 3

typedef struct ThreadInfo_t {
    unsigned char state;
    ucontext_t* context;
    char* stack;
    unsigned char ID;
    int totalDuration;
} ThreadInfo;

int gcdOfTwo(int x,int y);
int greatestCommonDivider(int* array, int length);

