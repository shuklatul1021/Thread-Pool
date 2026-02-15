#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD 8
#define QUEUE_SIZE 100


typedef struct{
    void (*fn)(void *arg);
    void *arg;
} task_t;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t thread[THREAD_POOL_SIZE];
    task_t task_queue[QUEUE_SIZE];
    int queue;
    int queue_front;
    int queue_back;
    int stop; 
} threadpool_t;


void thread_pool_init(threadpool_t *pool){
    pool->queue = 0;
    pool->
}

int main(){

}