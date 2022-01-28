/*
 * Created by Burak ŞEN for EE442 HW1
 * Includes main thread function and logic.
 */

#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <time.h>
#include "auxiliary.h"
#define MAX 100000

pthread_t* PayDeskThreads;
pthread_t SupervisorThread;

pthread_mutex_t* mutexQueueArray;
pthread_mutex_t mutexCondUpdate;
pthread_mutex_t mutexServedOrDiscardedClients;

pthread_cond_t* condDeskBlockCheckArray;
pthread_cond_t condSupervisor;

Information lastServedClientInfo;
ClientQueue* allClientQueues;
unsigned char* payThreadIDs;
int isDataUpdated;
unsigned char* ServedOrDiscardedClients;

void* SupervisorThreadFunction(void* args)
{
    int i;
    for(i=0; i< MAX; i++)
    {
        pthread_mutex_lock(&mutexCondUpdate);
        /// CRITICAL SECTION BEGINS
        while(isDataUpdated != 1)
            {
                pthread_cond_wait(&condSupervisor,&mutexCondUpdate);
            }
            printf("Desk %d served Client %d in %lf seconds.\n",lastServedClientInfo.paydeskNo,
                   lastServedClientInfo.client.id,lastServedClientInfo.client.duration);
            pthread_mutex_lock(&mutexServedOrDiscardedClients);
            /// ANOTHER CRITICAL SECTION BEGINS
                ServedOrDiscardedClients[lastServedClientInfo.client.id] = 1;
            /// ANOTHER CRITICAL SECTION ENDS
        pthread_mutex_unlock(&mutexServedOrDiscardedClients);
            isDataUpdated = 0;
        /// CRITICAL SECTION ENDS
        pthread_mutex_unlock(&mutexCondUpdate);
    }
    pthread_exit(0);
}

void* DeskThreadFunction(void* args)
{
    unsigned char index = *((unsigned char*)args);

    // Desk threads must not able to reach addresses other Desk threads reach. Thus, addresses of ClientQueue,Mutex and
    // pthread_cond are taken with a constant pointer such that each desk thread can only access and modify its corresponding
    // variables.
    ClientQueue* const corrClientQueue = &allClientQueues[index];
    pthread_mutex_t* const corrMutex = &mutexQueueArray[index];
    pthread_cond_t* const corrCond = &condDeskBlockCheckArray[index];
    BankClient*  servedClient;
    double duration;
    unsigned  int servingTime;
    int i;
    for(i =0 ; i< MAX; i++)
    {
        pthread_mutex_lock(corrMutex);
        while(corrClientQueue->currentSize == 0)
            pthread_cond_wait(corrCond,corrMutex);

        pthread_mutex_unlock(corrMutex);
        // Serving time is uniformly distributed between 0-1 seconds, however sleep() function takes inputs as unsigned int.
        // Thus, Serving time will be multiplied with 10^6 to convert it int microseconds, then usleep() will be called.
        if(corrClientQueue->currentSize != 0)
        {
             duration = corrClientQueue->array[corrClientQueue->frontIndex].duration;
             servingTime = (unsigned int) (duration * 1000000.0);
             usleep(servingTime);

             pthread_mutex_lock(corrMutex);
             pthread_mutex_lock(&mutexCondUpdate);
             /// CRITICAL SECTION BEGINS
                servedClient = Dequeue(corrClientQueue);
                lastServedClientInfo.paydeskNo = index;
                lastServedClientInfo.client.id = servedClient->id;
                lastServedClientInfo.client.duration = servedClient->duration;
                isDataUpdated = 1;
                pthread_cond_signal(&condSupervisor);
             /// CRITICAL SECTION ENDS
             pthread_mutex_unlock(&mutexCondUpdate);
             pthread_mutex_unlock(corrMutex);
        }
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    // Such that each run is unique.
    srand((unsigned)time(NULL));

    // Initialize variables with their default values
    int ClientNumber = 20;
    int PayThreadCount = 3;
    int MaxSizeofQueue = 3;
    float RateOfGeneration = 100.0f;
    float RateOfDuration  = 10.0f;

    // Check for optional arguments and modify the values according to it.
    // This command line parser code is highly referenced from https://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html link.
    int opt;
    while((opt = getopt(argc, argv, "c:n:q:g:d:")) != -1)
    {
        switch(opt)
        {
            case 'c':
                ClientNumber = atoi(optarg);
                break;
            case 'n':
                PayThreadCount = atoi(optarg);
                break;
            case 'q':
                MaxSizeofQueue = atoi(optarg);
                break;
            case 'g':
                RateOfGeneration = atof(optarg);
                break;
            case 'd':
                RateOfDuration = atof(optarg);
                break;
            default:
                break;
        }
    }
    // Print the current configuration.
    printf("NUM_CLIENTS      : %d\n",ClientNumber);
    printf("NUM_DESKS        : %d\n",PayThreadCount);
    printf("QUEUE_SIZE       : %d\n",MaxSizeofQueue);
    printf("DURATION_RATE    : %f\n",RateOfDuration);
    printf("GENERATION_RATE  : %f\n",RateOfGeneration);

    // Request dynamic memory for all pay desks and their corresponding queues and pthread variables.
    PayDeskThreads = malloc(sizeof(pthread_t) * PayThreadCount);
    mutexQueueArray = malloc(sizeof(pthread_mutex_t) * PayThreadCount);
    condDeskBlockCheckArray = malloc(sizeof(pthread_cond_t) * PayThreadCount);

    allClientQueues = malloc(sizeof(ClientQueue) * PayThreadCount);
    payThreadIDs = malloc(sizeof(int)* PayThreadCount);
    ServedOrDiscardedClients = malloc(sizeof (unsigned char) * ClientNumber);

    // Since only 1 BankClient will be entered into queue at any time in the program. One dynamically allocated
    // BankClient variable is enough to enqueue client to the corresponding queue.
    BankClient* generatedClientToEnqueue = malloc(sizeof (BankClient));

    // Initialize queues.
    int i;
    for(i=0 ; i < PayThreadCount ; i++)
    {
        // IDs of threads are from 0 to (Thread Count - 1)
        payThreadIDs[i] = (unsigned  char)i;
        // Allocate dynamic memory for each queue and initialize its variables, imitates Constructor operation of
        // a class.
        allClientQueues[i].array = malloc(sizeof(BankClient) * MaxSizeofQueue);
        allClientQueues[i].maxSize = MaxSizeofQueue;
        allClientQueues[i].currentSize = 0;
        allClientQueues[i].rearIndex =  -1;
        allClientQueues[i].frontIndex = -1;
        // Initialize required mutex and condition variables.
        pthread_mutex_init(&mutexQueueArray[i],NULL);
        pthread_cond_init(&condDeskBlockCheckArray[i], 0);
    }
    pthread_mutex_init(&mutexCondUpdate,NULL);
    pthread_mutex_init(&mutexServedOrDiscardedClients,NULL);
    pthread_cond_init(&condSupervisor,0);

    for(i = 0; i < PayThreadCount ; i++)
    {
        pthread_create(&(PayDeskThreads[i]),NULL, DeskThreadFunction,(void*) &(payThreadIDs[i]));
    }
    pthread_create(&SupervisorThread, NULL, SupervisorThreadFunction, NULL);

    unsigned int gentimeMicroSeconds ;
    double gentimeSeconds;
    unsigned int indexOfSmallestQueue;
    unsigned int lengthOfSmallestQueue;
    int nextClientID = 0;
    int remainingClients = ClientNumber;

    for(; remainingClients > 0; remainingClients--)
    {
        // Generate randomly distributed generation value such that the main thread can sleep after generating the client
        gentimeSeconds = generateExponentialRandomValue(RateOfGeneration);
        gentimeMicroSeconds = (unsigned int) (gentimeSeconds * 1000000.0);

        // Set length of smallest queue to maximum and index to -1. Thus, if all the queues are full(at max size)
        // indexofSmallestQueue will be equal to -1 as well.
        lengthOfSmallestQueue = MaxSizeofQueue;
        indexOfSmallestQueue = -1;

        // To find smallest queue, clientqueues must not be accessed or modified since while determining the
        // smallest queue the sizes may change. Thus, mutexes will be locked.
        for(i=0; i < PayThreadCount; i++) {
            pthread_mutex_lock(&mutexQueueArray[i]);
        }
        /// CRITICAL SECTION BEGINS
            for(i=0; i < PayThreadCount; i++)
            {
                    if(lengthOfSmallestQueue > allClientQueues[i].currentSize)
                    {
                        indexOfSmallestQueue = i;
                        lengthOfSmallestQueue = allClientQueues[i].currentSize;
                    }
                    else
                    {
                        // Do nothing
                    }
            }
        /// CRITICAL SECTION ENDS
        for(i=0; i < PayThreadCount; i++) {
            pthread_mutex_unlock(&mutexQueueArray[i]);
        }
        // Check if all the queues are full.
        if(indexOfSmallestQueue != -1 && lengthOfSmallestQueue != MaxSizeofQueue)
        {
            // Update generated client variable before queueing.
            generatedClientToEnqueue->id = nextClientID;
            generatedClientToEnqueue->duration = generateExponentialRandomValue(RateOfDuration);
            printf("Client %d arrived.\n",nextClientID);
            pthread_mutex_lock(&mutexQueueArray[indexOfSmallestQueue]);
            /// CRITICAL SECTION BEGINS
                if(lengthOfSmallestQueue == 0)
                {
                    Enqueue(&(allClientQueues[indexOfSmallestQueue]),generatedClientToEnqueue);
                    pthread_cond_signal(&condDeskBlockCheckArray[indexOfSmallestQueue]);
                }
                else
                {
                    Enqueue(&(allClientQueues[indexOfSmallestQueue]),generatedClientToEnqueue);
                }
            /// CRITICAL SECTION ENDS
            pthread_mutex_unlock(&mutexQueueArray[indexOfSmallestQueue]);
        }
        else
            {
            pthread_mutex_lock(&mutexServedOrDiscardedClients);
            /// CRITICAL SECTION BEGINS
                ServedOrDiscardedClients[nextClientID] = 1;
                printf("Client %d discarded.\n",nextClientID);
            /// CRITICAL SECTION ENDS
            pthread_mutex_unlock(&mutexServedOrDiscardedClients);
        }
        nextClientID++;
        usleep(gentimeMicroSeconds) ;
    }

    /* I could not find a way to end Threads using pthread_join, the program seemed to not stop. That's why stop condition
     * for the program is that if each element of the ServedOrDiscardedClient array is 1 stop the threads. At that point
     * there is no new clients generating or a client to serve in a queue. Thus, when this condition becomes true program
     * leaves the infinite loop and cancels the threads.
     */
    unsigned char checkClientProcess;
    while(1)
    {
        checkClientProcess = 0;
        pthread_mutex_lock(&mutexServedOrDiscardedClients);
        /// CRITICAL SECTION BEGINS
            for(i =0 ; i<ClientNumber ; i++)
            {
                checkClientProcess += ServedOrDiscardedClients[i];
            }
            if(checkClientProcess == ClientNumber)
            {
                pthread_mutex_unlock(&mutexServedOrDiscardedClients);
                break;
            }
        /// CRITICAL SECTION ENDS
        pthread_mutex_unlock(&mutexServedOrDiscardedClients);
    }
    // Cancel the threads since required operation is finished.
    pthread_cancel(SupervisorThread);
    for(i = 0; i < PayThreadCount ; i++)
    {
        pthread_cancel(PayDeskThreads[i]);
    }
    // Destroy mutexes and condition variables before exit.
    pthread_cond_destroy(&condSupervisor);
    pthread_mutex_destroy(&mutexCondUpdate);
    pthread_mutex_destroy(&mutexServedOrDiscardedClients);
    for(i = 0; i < PayThreadCount ; i++)
    {
        pthread_mutex_destroy(&mutexQueueArray[i]);
        pthread_cond_destroy(&condDeskBlockCheckArray[i]);
    }
    // Free dynamic memory before exit.
    for(i=0 ; i < PayThreadCount ; i++)
    {
        free(allClientQueues[i].array);
    }
    free(condDeskBlockCheckArray);
    free(mutexQueueArray);
    free(PayDeskThreads);
    free(allClientQueues);
    free(payThreadIDs);
    free(ServedOrDiscardedClients);
    free(generatedClientToEnqueue);
    printf("Process is finished, all the threads are closed and dynamic memory has been freed.\n");
    return 0;
}
