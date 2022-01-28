/* 

Berkay ŞAHİN 2232668 EE442 HW2 - PWFscheduler

References:
https://pubs.opengroup.org/onlinepubs/7908799/xsh/ucontext.h.html
https://pubs.opengroup.org/onlinepubs/009695399/functions/getcontext.html
https://pubs.opengroup.org/onlinepubs/009695399/functions/makecontext.html
https://pubs.opengroup.org/onlinepubs/009695399/basedefs/signal.h.html 

*/

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>

#define ARRAY_LENGTH 5
#define EMPTY 0
#define READY 1
#define RUNNING 2
#define FINISHED 3
#define STACK_SIZE 4096
#define TRUE 1
#define FALSE 0

/* type ThreadInfo */
typedef struct ThreadInfo{
    ucontext_t context;
    int state; // 0=empty, 1=ready, 2=running, 3=finished
    int count;
    int index_at_current; 
} ThreadInfo;

ThreadInfo** current_threads; // this array holds main at 0, and the other 5 threads in progress
ThreadInfo** all_threads; // this array holds all threads given by user, but not the main
int global_var[14]; /* this array holds some global variables needed in the functions
                      [0-4]: total count numbers of current arrays 
                      [5-9]: cumulative sum of count numbers for lottery, 9th is the total
                      [10]: overall thread number that is finished
                      [11]: scheduler trigger
                      [12]: current thread index in progress
                      [13]: next thread index, we both need them separately
                   */ 
void runThread(int count, int index_no, int thread_number);




void initializeThread(int thread_number){
    current_threads = malloc((ARRAY_LENGTH+1) * sizeof(ThreadInfo*));
    all_threads = malloc(thread_number * sizeof(ThreadInfo*));
    
    int i;
    for(i=0; i<ARRAY_LENGTH+1; i++){
        current_threads[i] = malloc(sizeof(ThreadInfo));
    }
    for(i=1; i<ARRAY_LENGTH+1; i++){
        current_threads[i] = NULL;
    }
    current_threads[0]->state = EMPTY;
    current_threads[0]->count = -1;
    current_threads[0]->index_at_current = -1;
    //main thread

    for(i=0; i<thread_number; i++){
        all_threads[i] = malloc(sizeof(ThreadInfo));
    }
    for(i=0; i<thread_number; i++){
        all_threads[i]->state = EMPTY;
        all_threads[i]->count = -1;
        all_threads[i]->index_at_current = i;
    }
}



int createThread(int thread_number){
    int k;
    int l; // all_threads are placed started from the first index to current_threads
    int local_sum;
    int n;
    for(k=1; k<ARRAY_LENGTH+1; k++){
        if(current_threads[k]==NULL || current_threads[k]->state==FINISHED || current_threads[k]->state==EMPTY){
            for(l=0; l<thread_number; l++){  
                if(all_threads[l]->state==EMPTY){
                    all_threads[l]->state = READY;
                    all_threads[l]->index_at_current = k;
                    current_threads[k] = all_threads[l];
                    getcontext(&all_threads[l]->context);
                    // link ???
                    all_threads[l]->context.uc_stack.ss_sp = malloc(STACK_SIZE);
                    all_threads[l]->context.uc_stack.ss_size = STACK_SIZE;
                    makecontext(&all_threads[l]->context, (void*) runThread, 4, 
                    all_threads[l]->count, all_threads[l]->index_at_current, thread_number);

                    global_var[k-1] = all_threads[l]->count;
                    local_sum = 0;
                    for(n=0; n<ARRAY_LENGTH; n++){
                        local_sum = local_sum + global_var[n];
                        global_var[n+5] = local_sum;
                    }

                    return 0;
                }
            }
        }
    }
    printf("\nerror at createThread function");
    return -1;
}



int lotto(){
    global_var[13] = 1;
    int lotto_var = (rand() % global_var[9]) + 1; // random selection from all tickets

    int lotto_i;
    for(lotto_i=0; lotto_i<ARRAY_LENGTH; lotto_i++){
        if(current_threads[lotto_i+1] == NULL){
            global_var[13] = 1;
        }
        else{
            if(lotto_var <= global_var[lotto_i+5]){
                global_var[13] = lotto_i + 1; // next thread selected
                break;
            }
        }
    }
    return lotto_var;
}


void scheduler(int thread_number){
    for(; global_var[11]>0; global_var[11]--){
        getcontext(&current_threads[global_var[12]]->context);
        swapcontext(&current_threads[global_var[12]]->context, &current_threads[0]->context);
    } // context switching for main
    lotto();
    printf("curr : %d, next %d\n",global_var[12],global_var[13]);
    if(global_var[12] != global_var[13]){
        if(current_threads[global_var[12]]->state != FINISHED){ // if the thread is finished
            current_threads[global_var[12]]->state = READY; // after the signal check, set ready
        }
        // set the next thread running
        current_threads[global_var[13]]->state = RUNNING;
        int thread_i;
        printf("dd1\n");
        printf("\nrunning>T%d\tready>", current_threads[global_var[13]]->index_at_current);
        printf("dd2\n");
        for(thread_i=1; thread_i<ARRAY_LENGTH+1; thread_i++){
            if(current_threads[thread_i]->state == READY){
                printf("T%d",current_threads[thread_i]->index_at_current);
            }
        }
        printf("thread_i : %d",thread_i);
        if(current_threads[thread_i-1]->state != READY ){
            printf("\b");
        }

        printf("\tfinished>");
        for(thread_i=0; thread_i<thread_number; thread_i++){
            if(all_threads[thread_i]->state == FINISHED){
                printf("T%d", all_threads[thread_i]->index_at_current);
            }
        }
        if(all_threads[thread_i-1]->state != FINISHED ){
            printf("\b");
        }
        alarm(2); // set the timer
        // swap the context for next thread
        int completed_i = global_var[12];
        global_var[12] = global_var[13];
        printf("before switc\n");
        swapcontext(&current_threads[completed_i]->context, &current_threads[global_var[12]]->context);
    }
    else{
        printf("\nrunning>T%d\tready>", (global_var[13]-1));
        int thread_i;
        for(thread_i=1; thread_i<ARRAY_LENGTH+1; thread_i++){
            if(current_threads[thread_i]->state == READY){
                printf("T%d",current_threads[thread_i]->index_at_current);
            }
        }
        if(current_threads[thread_i-1]->state != READY ){
            printf("\b");
        }
        printf("\tfinished>");
        for(thread_i=0; thread_i<thread_number; thread_i++){
            if(all_threads[thread_i]->state == FINISHED){
                printf("T%d", all_threads[thread_i]->index_at_current);
            }
        }
        if(all_threads[thread_i-1]->state != FINISHED ){
            printf("\b");
        }
        printf("endofsch");
        alarm(2);
    }
}
void exitThread(int index_no, int thread_number){
    int local_sum = 0;
    int e;
    all_threads[index_no]->state = FINISHED;
    global_var[10] = global_var[10] + 1;
    global_var[11] = TRUE;

    for(e=0; e<ARRAY_LENGTH; e++){
        local_sum = local_sum + global_var[e];
        global_var[e+5] = local_sum;
    } // update count numbers

    free(all_threads[index_no]->context.uc_stack.ss_sp);

    scheduler(thread_number);
}


void runThread(int count, int index_no, int thread_number){
    int m;
    int tab_no;
    for(m=1; m<(count+1); m++){
        for(tab_no=1; tab_no<index_no; tab_no++){
            printf("\t");
        }
        printf("%d\n", m);
        sleep(1);
    }
    alarm(0);
    exitThread(index_no, thread_number);
}






//gcd functions, ref: geeksforgeeks.org/gcd-two-array-numbers
int GCD(int a, int b){
    if(a == 0){
        return b;
    }
    return GCD(b%a, a);
}
int findGCD(int arr[], int n){
    int result = arr[0];
    for(int gcdi=1; gcdi<n; gcdi++){
        result = GCD(arr[gcdi], result);
        if(result == 1){
            return 1;
        }
    }
    return result;
}





int main(int argc, char** argv){

    global_var[12] = 1;
    global_var[13] = global_var[12];
    srand((int) time(NULL));
    signal(SIGALRM, scheduler);
    /* The SIGALRM signal is active when a time interval specified 
    in the call of the alarm function expires */
    int thread_number = argc - 1;
    int* user_inputs;
    user_inputs = malloc(sizeof(int) * thread_number);
    
    int i;
    for(i=0; i<thread_number; i++){
        user_inputs[i] = atoi(argv[i+1]); // user_inputs[] holds the counting numbers of the threads
    }

    initializeThread(thread_number);
    for(i=0; i<thread_number; i++){
        all_threads[i]->count = user_inputs[i];
    }
    /* These count numbers can be sent to the createThread function, and they would be
    placed into array there; however, I prefer to assign count numbers seperately for convenience */
    createThread(thread_number);
    createThread(thread_number);
    createThread(thread_number);
    createThread(thread_number);
    createThread(thread_number);

    printf("\nShare:\n");
    int common_div = findGCD(global_var, 5);
    int min_count, min_all;
    if(common_div == 0)
        common_div = 1;
    for(int p=0; p<5; p++){
        min_count = global_var[p]/common_div;
        min_all = global_var[9]/common_div;
        printf("%d/%d", min_count, min_all);
    }

    printf("\nThreads:\n");
    for(int p=0; p<thread_number; p++){
        printf("T%d\t", all_threads[p]->index_at_current);
    }

    int l_var = lotto();
    global_var[12] = global_var[13]; // current is set next
    current_threads[global_var[13]]->state = RUNNING; // started running the process that is picked
    printf("\nrunning>T%d\tready>", current_threads[global_var[13]]->index_at_current);
    int thread_i;
    for(thread_i=1; thread_i<ARRAY_LENGTH+1; thread_i++){
        if(current_threads[thread_i]->state == READY){
            printf("T%d",current_threads[thread_i]->index_at_current);
        }
    }
    if(current_threads[thread_i-1]->state != READY ){
        printf("\b");
    }
    printf("\tfinished>");
    for(thread_i=0; thread_i<thread_number; thread_i++){
        if(all_threads[thread_i]->state == FINISHED){
            printf("T%d", all_threads[thread_i]->index_at_current);
        }
    }
    if(all_threads[thread_i-1]->state != FINISHED ){
        printf("\b");
    }
    // the same print functions as in scheduler
    getcontext(&current_threads[0]->context); // get main
    alarm(2); // set alarm
    swapcontext(&current_threads[0]->context, &current_threads[global_var[13]]->context);

    // for addition of 5 threads
    while(global_var[10] != thread_number-1){
        createThread(thread_number);
        int l_var2 = lotto();
        if(global_var[12]  == global_var[13]){
            current_threads[global_var[13]]->state = RUNNING;
            printf("\nrunning>T%d\tready>", current_threads[global_var[13]]->index_at_current);
            int thread_i;
            for(thread_i=1; thread_i<ARRAY_LENGTH+1; thread_i++){
                if(current_threads[thread_i]->state == READY){
                    printf("T%d",current_threads[thread_i]->index_at_current);
                }
            }
            if(current_threads[thread_i-1]->state != READY ){
                printf("\b");
            }
            printf("\tfinished>");
            for(thread_i=0; thread_i<thread_number; thread_i++){
                if(all_threads[thread_i]->state == FINISHED){
                    printf("T%d", all_threads[thread_i]->index_at_current);
                }
            }
            if(all_threads[thread_i-1]->state != FINISHED ){
                printf("\b");
            }
            alarm(2);
            swapcontext(&current_threads[0]->context, &current_threads[global_var[13]]->context);
        }
        else if(global_var[12]  != global_var[13]){
            if(current_threads[global_var[12]]->state != FINISHED){
                current_threads[global_var[12]]->state = READY;
            }
            else{

            }
            current_threads[global_var[13]]->state = RUNNING;
            printf("\nrunning>T%d\tready>", current_threads[global_var[13]]->index_at_current);
            int thread_i;
            for(thread_i=1; thread_i<ARRAY_LENGTH+1; thread_i++){
                if(current_threads[thread_i]->state == READY){
                    printf("T%d",current_threads[thread_i]->index_at_current);
                }
            }
            if(current_threads[thread_i-1]->state != READY ){
                printf("\b");
            }
            printf("\tfinished>");
            for(thread_i=0; thread_i<thread_number; thread_i++){
                if(all_threads[thread_i]->state == FINISHED){
                    printf("T%d", all_threads[thread_i]->index_at_current);
                }
            }
            if(all_threads[thread_i-1]->state != FINISHED ){
                printf("\b");
            }
            alarm(2);
            int temp_thread = global_var[global_var[12]];
            global_var[12] = global_var[13];
            swapcontext(&current_threads[0]->context, &current_threads[global_var[13]]->context);   
        }
    }
    printf("running>\tready>\t\tfinished>");
    for(int pi=0; pi<thread_number; pi++){
        printf("T%d", all_threads[pi]->index_at_current);
    }
    printf("\n");
    return 0;
}




