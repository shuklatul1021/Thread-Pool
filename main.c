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
    pthread_t thread[THREAD];
    task_t task_queue[QUEUE_SIZE];
    int queue;
    int queue_front;
    int queue_back;
    int stop; 
} threadpool_t;

void *thread_function(void *threadpool){
    threadpool_t *pool = (threadpool_t*)threadpool;

    while(1){
        
    }

}


void thread_pool_init(threadpool_t *pool){
    pool->queue = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->stop = 0;

    pthread_mutex_init(&(pool->lock) , NULL);
    pthread_cond_init(&(pool->notify) , NULL);

    for(int i = 0; i < THREAD ; i++){
        pthread_create(&(pool->thread[i]) , NULL , thread_function , pool);
    }
}

int main(){
    thread_pool_init();

}