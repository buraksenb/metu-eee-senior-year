//
// Created by burak on 03.05.2021.
//
#include <stdlib.h>
#include "auxiliary.h"
#include <time.h>
#include <limits.h>

ThreadInfo* workingThreadSet;
ThreadInfo* allThreads;
int LENGTH_OF_THREAD_ARRAY = 5;
int totalThreadNumber;
int* shareArray;
int* lotteryArray;
int* lotteryThreadIndex;
int* nValueArray;
int* remainingTimeArray;
int totalShare = 0 ;
int currentIndex = 0;
int previousIndex = 0;
int threadtoGenerate = 1;
int finishedtThreads = 0 ;

// SCHEDULER_TYPE = 1 for PWF_Scheduler.
// SCHEDULER_TYPE = 0 for SRTF_Scheduler.
#define SCHEDULER_TYPE 1

// Since information o threads are printed at many locations, to avoid code repetition overall print information
// function is implemented.
void printInformation()
{
    int i;
    int countOfReady = 0;
    if(currentIndex != 0)
        printf("running->T%d\tready->",workingThreadSet[currentIndex].ID);
    else
        printf("running->\tready->");
    for(i=1;i<(LENGTH_OF_THREAD_ARRAY + 1); i++)
        if(workingThreadSet[i].state == STATE_READY)
        {
            printf("T%d,", workingThreadSet[i].ID);
            countOfReady++;
        }
    countOfReady = LENGTH_OF_THREAD_ARRAY - countOfReady;
    for(i=0;i<countOfReady;i++)
        printf("   ");
    printf("\tfinished->");
    for(i=0;i<totalThreadNumber; i++)
        if(allThreads[i].state == STATE_EXIT)
            printf("T%d,",allThreads[i].ID);
    printf("\n");
}
// After a thread is finished or a new thread enters to the working threads, we have to update lottery share such that
// fairness continues.
// lotteryArrayIndex is kept since we may have T1,T3,T5 threads left and lotteryArray array implementation needs continuous
// distribution. For exmaple if T1,T3,T5 have 3,5,7 shares respectively,
// lotteryThreadIndex will be {1,3,5,-1,-1} and lotteryArray will be {3,8,15,-1,-1} -1 is the stopping point.
void updateLotteryShare()
{
    int i=1;
    totalShare = 0;
    int currentLotteryIndex = 0;
    for(; i < (LENGTH_OF_THREAD_ARRAY + 1); i++)
    {
        // If threads in workingThreadSet is ready or running then they are eligible to the lottery array.
        if(workingThreadSet[i].state == STATE_RUNNING || workingThreadSet[i].state == STATE_READY)
        {
            totalShare +=workingThreadSet[i].totalDuration;
            lotteryArray[currentLotteryIndex] = totalShare;
            lotteryThreadIndex[currentLotteryIndex] = i;
            currentLotteryIndex++;
        }
    }
    while(currentLotteryIndex < LENGTH_OF_THREAD_ARRAY)
    {
        lotteryThreadIndex[currentLotteryIndex] = -1;
        lotteryArray[currentLotteryIndex] = -1;
        currentLotteryIndex++;
    }
}
int playLottery()
{
    /* Operation of playLottery function is explained below:
     * Let's say we have 3 threads with shares 2,4,6. Total share is 12.
     * Then a random variable with range 1-to-12 generated. After that winner will be
     * decided. Decision will be:
     * Thread 1: 1,2
     * Thread 2: 3,4,5,6
     * Thread 3: 7,8,9,10,11,12
     * For example, if 5 was generated, Thread 2 is the winner of the lottery and its index will be returned.
     */
    int i;
    int randomLottery;
    int lottIndex;
    int randomIndex = 1;
    do {
        randomLottery = (rand() % totalShare) + 1  ;
        for(i=0; i < LENGTH_OF_THREAD_ARRAY; i++)
        {
            if(lotteryThreadIndex[i] == -1)
            {
                break;
            }
            lottIndex = lotteryThreadIndex[i];
             // Since threadArrau[0] is reserved for main(), i+1 must be used for accessing workingThreadSet.
            if(randomLottery <= lotteryArray[i] &&
               ((workingThreadSet[lottIndex].state == STATE_RUNNING) ||
                (workingThreadSet[lottIndex].state == STATE_READY)))
            {
                randomIndex = lotteryThreadIndex[i];
                break;
            }
        }
    }while((workingThreadSet[randomIndex].state != STATE_READY) && (workingThreadSet[randomIndex].state != STATE_RUNNING));

    return randomIndex;
}

void runThread()
{
    // Set alarm interrupt and swap contexts between current and next threads.
    // alarm(2);
    swapcontext(workingThreadSet[previousIndex].context, workingThreadSet[currentIndex].context);
}

// Compare remaning times of threads to determine shortest one for SRTF scheduler
int determineShortestRemaining()
{
    int i;
    int shortestRemainingTime = INT_MAX;
    int shortestRemainingIndex = 1;
    for(i=0; i < LENGTH_OF_THREAD_ARRAY; i++)
    {

        //printf("%d ",remainingTimeArray[i]) ;
        // Compare each threads' remaining time and find the thread with shortest time remaining.
        if(remainingTimeArray[i] < shortestRemainingTime && remainingTimeArray[i] > 0)
        {
            shortestRemainingTime = remainingTimeArray[i];
            shortestRemainingIndex = i+1;
        }
    }
   // printf("\n");
    //printf("ShortestIndex = %d, ShortestValue = %d\n",shortestRemainingIndex,shortestRemainingTime);
    return shortestRemainingIndex;
}
void SRTF_Scheduler()
{
    int i;
    // Set the shortest remanining time and index to 0 for comparison
    int shortestRemainingIndex;
    // Check if last working thread is finished, if that is the case go to main thread.
    if(allThreads[workingThreadSet[currentIndex].ID -1 ].state == STATE_EXIT)
    {
        currentIndex = 0;
        workingThreadSet[currentIndex].state = STATE_RUNNING;
        setcontext(workingThreadSet[0].context);
    }
    shortestRemainingIndex = determineShortestRemaining();
    // Update the global variable and swap to the corresponding thread
    remainingTimeArray[shortestRemainingIndex-1] -=  2;
    if(currentIndex == shortestRemainingIndex)
    {
       // printInformation();
        alarm(2);
        // Context switching is not required.
    }
    else
    {
        previousIndex = currentIndex;
        if(workingThreadSet[previousIndex].state != STATE_EMPTY) {
            workingThreadSet[previousIndex].state = STATE_READY;
        }
        currentIndex = shortestRemainingIndex;
        workingThreadSet[currentIndex].state = STATE_RUNNING;
        printInformation();
        alarm(2);
        runThread();
    }
}
void PWF_Scheduler()
{
    int i;
    // Check if last working thread is finished, if that is the case go to main thread.
    if(allThreads[workingThreadSet[currentIndex].ID -1 ].state == STATE_EXIT)
    {
        currentIndex = 0;
        workingThreadSet[currentIndex].state = STATE_RUNNING;
        setcontext(workingThreadSet[0].context);
    }
    int randomIndex = playLottery();

   // printf("randomIndex = %d, currentIndex = %d\n ",randomIndex,currentIndex);
    if(currentIndex == randomIndex)
    {
      //  printInformation();
        // Context switching is not required.
        alarm(2);
    }
    else
    {
        previousIndex = currentIndex;
        if(workingThreadSet[previousIndex].state != STATE_EMPTY)
            workingThreadSet[previousIndex].state = STATE_READY;

        currentIndex = randomIndex;
       // printf("curent : %d , prev : %d",currentIndex,previousIndex);
        workingThreadSet[currentIndex].state = STATE_RUNNING;
        printInformation();
        alarm(2);
        runThread();
    }
}

// According to SCHEDULER_TYPE, call PWF or SRTF scheduler
void timerISR(int signum) {
    #if SCHEDULER_TYPE
        PWF_Scheduler();
    #else
        SRTF_Scheduler();
    #endif
}
// Exit thread with threadNumber which is the index of the thread in workingThreadSet.
// Free the stack and updateLotteryShare if PWF scheduler is active.
void exitThread(int threadNumber)
{
    workingThreadSet[threadNumber].state = STATE_EMPTY;
   // printf("threadnumber : %d to exit\n",threadNumber);
    allThreads[workingThreadSet[threadNumber].ID -1 ].state = STATE_EXIT;
    finishedtThreads++;
    free(workingThreadSet[threadNumber].stack);
    if(finishedtThreads != totalThreadNumber)
    {
        #if SCHEDULER_TYPE
            updateLotteryShare();
        #endif

    }

   //printf("Exit thread ID %d,state : %d\n", workingThreadSet[threadNumber].ID, workingThreadSet[threadNumber].state);
   // Call scheduler.
#if SCHEDULER_TYPE
    PWF_Scheduler();
#else
    SRTF_Scheduler();
#endif
}

void simpleCounter(int arrayIndex,int n) {

    int i;
    int j;
    for (i = 1; i <= n; i++) {
        // If desired value is reached, stop the alarm.
        for (j = 1; j < workingThreadSet[arrayIndex].ID; j++)
            printf("\t");

        printf("%d\n", i);
        if(i == n)
            alarm(0);
        sleep(1);
    }
    exitThread(arrayIndex);
}
void initializeThread()
{
    int i;
    workingThreadSet = malloc(sizeof(ThreadInfo) * (LENGTH_OF_THREAD_ARRAY + 1));
    allThreads = malloc(sizeof(ThreadInfo) * totalThreadNumber);
    // Main thread structure initialization
    workingThreadSet[0].state = STATE_READY;
    workingThreadSet[0].ID = 0;
    workingThreadSet[0].totalDuration = 0;
    workingThreadSet[0].stack = malloc(STACK_SIZE);
    workingThreadSet[0].context = malloc(sizeof(ucontext_t));

    // Initialize workingThreadSet structs
    for(i=1; i< (LENGTH_OF_THREAD_ARRAY + 1); i++)
    {
        workingThreadSet[i].state = STATE_EMPTY;
        workingThreadSet[i].ID = -1;
        workingThreadSet[i].totalDuration = -1;
        workingThreadSet[i].context = NULL;
        workingThreadSet[i].stack = NULL;
    }
    // Initialize all thread structs
    for(i=0; i< totalThreadNumber; i++)
    {
        allThreads[i].state = STATE_EMPTY;
        allThreads[i].ID = i+1;
        allThreads[i].totalDuration = -1;
        allThreads[i].context = malloc(sizeof(ucontext_t));
        allThreads[i].stack = malloc(STACK_SIZE);
        allThreads[i].totalDuration = nValueArray[i];

    }
    // Since duration values are passed to the threads there is no need for this array anymore, thus free.
    free(nValueArray);
}
int createThread()
{

    int i;
    int isFull = 1;
    // If there is empty spot in workingThreadSet, break the loop and continue to create, otherwise return -1.
    for(i=1 ; i< (LENGTH_OF_THREAD_ARRAY + 1); i++ )
    {
        if(workingThreadSet[i].state == STATE_EMPTY)
        {
            isFull = 0;
            break;
        }
    }
    if(isFull == 1)
    {
      //  printf("Thread gen failed\n");
        return -1;
    }
    // Get context and pass the information of the thread which will be created to the workingThreadSet.
    getcontext(allThreads[threadtoGenerate-1].context);
    allThreads[threadtoGenerate-1].state = STATE_READY;
    workingThreadSet[i].state = allThreads[threadtoGenerate-1].state;
    workingThreadSet[i].ID = allThreads[threadtoGenerate-1].ID;
    // printf("workingThreadIndex : %d , threadID %d\n",i,workingThreadSet[i].ID);
    workingThreadSet[i].totalDuration = allThreads[threadtoGenerate-1].totalDuration;
    workingThreadSet[i].context = allThreads[threadtoGenerate-1].context;
    workingThreadSet[i].stack = allThreads[threadtoGenerate-1].stack;
    workingThreadSet[i].context->uc_stack.ss_sp = workingThreadSet[i].stack;
    workingThreadSet[i].context->uc_stack.ss_size = STACK_SIZE;
    workingThreadSet[i].context->uc_stack.ss_flags =0;
    #if !SCHEDULER_TYPE
    remainingTimeArray[i-1] = workingThreadSet[i].totalDuration;
    #endif
    threadtoGenerate++;
  //  printf("Thread generated %d \n ",workingThreadSet[i].ID);
    makecontext(workingThreadSet[i].context, (void*)simpleCounter, 3, i, workingThreadSet[i].totalDuration);
    return 0;
}



int main(int argc, char **argv)
{

    srand((unsigned)time(NULL));
    int i;
    totalThreadNumber = argc -1 ;
    // If totalThreadNumber is lower than 5 update the corresponding valeu.
    if(totalThreadNumber < 5)
    {
        LENGTH_OF_THREAD_ARRAY = totalThreadNumber;
    }
    // Request memory for required arrays.
    shareArray = malloc(totalThreadNumber * sizeof (int));
    lotteryArray  = malloc(LENGTH_OF_THREAD_ARRAY * sizeof (int));
    remainingTimeArray  = malloc(LENGTH_OF_THREAD_ARRAY * sizeof (int));
    nValueArray =  malloc(totalThreadNumber * sizeof (int));
    lotteryThreadIndex =  malloc(LENGTH_OF_THREAD_ARRAY * sizeof (int));
    for(i=1; i<argc;i++)
    {
        nValueArray[i-1] = atoi(argv[i]);
        totalShare += nValueArray[i-1];
        lotteryArray[i-1] = totalShare;
    }
    // Calculate shares of threads.
    printf("Share:\n");
    int gcd = greatestCommonDivider(nValueArray, totalThreadNumber);
    totalShare = totalShare/ gcd;
    for(i=0; i < totalThreadNumber; i++)
    {
        shareArray[i] = nValueArray[i]/gcd;
        printf("%d/%d  ",shareArray[i],totalShare);
    }
    printf("\n");

    // Initialize timer interrupt and threads.
    signal(SIGALRM,timerISR);
    initializeThread();
    getcontext(workingThreadSet[0].context);
    // At the beginning create at most 5 threads before starting.
    while (threadtoGenerate < 6 && threadtoGenerate <= totalThreadNumber)
    {
        createThread();
    }

    // At most 5 threads are created, then determine shares of the threads and activate corresponding scheduler.
    updateLotteryShare();
    getcontext(workingThreadSet[0].context);
    #if  SCHEDULER_TYPE
    PWF_Scheduler();
    #else
    SRTF_Scheduler();
    #endif
    int retval = 0;
    // While finishedThreads are lower than the total thread number stay inside loop
    while(finishedtThreads < totalThreadNumber)
    {
        // If there are threads to generate, call createThread if a thread is generated(empty spot in workinThreadSet)
        // updateShare.
        if(threadtoGenerate <= totalThreadNumber)
        {
            retval  = createThread();
            if(retval == -1 )
            {
                // Do nothing;
            }
            else
            {
                #if  SCHEDULER_TYPE
                updateLotteryShare();
                #endif
            }
        }
        // update currentIndex to 0 which is main thread and call scheduler. This process does not change whether a new thread
        // is created or not.
        currentIndex= 0;
        workingThreadSet[currentIndex].state = STATE_READY;
        #if  SCHEDULER_TYPE
        PWF_Scheduler();
        #else
        SRTF_Scheduler();
        #endif
    }
    // Print the last information line and free memory.
    printInformation();
    free(lotteryThreadIndex);
    free(shareArray);
    free(remainingTimeArray);
    free(lotteryArray);
    free(workingThreadSet);
    return 0;
}
