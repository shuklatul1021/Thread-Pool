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
        pthread_mutex_lock(&(pool->lock));

        while(pool->queue == 0 && !pool->stop){
            pthread_cond_wait(&(pool->notify) , &(pool->lock));
        }

        if(pool->stop){
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }

        task_t task = pool->task_queue[pool->queue_front];
        pool->queue_front = (pool->queue_front + 1) % QUEUE_SIZE;
        pool->queue--;

        pthread_mutex_unlock(&(pool->lock));

        (*(task.fn))(task.arg);
    }

    return NULL;
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


void thread_distroy(threadpool_t *pool){
    pthread_mutex_lock(&(pool->lock));
    pool->stop = 1;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
 

    for(int i = 0 ; i < THREAD ; i++){
        pthread_join(pool->thread[i] , NULL);
    }

    pthread_mutex_distroy(&(pool->lock));
    pthread_cond_distroy(&(pool->notify));
}


void example_task(){
    int *num = (int*)arg;
    printf("Processing Task %d\n" , *num);
    sleep(1);
    free(arg);
}


void threadpool_add_task(threadpool_t *pool , void (*fun)(void*) , void *arg){
    pthread_mutex_lock(&(pool->lock));
    
    int next_rear = (pool->queue_back + 1) % QUEUE_SIZE;
    if(pool->queue < QUEUE_SIZE){
        pool->task_queue[pool->queue_back].fn = function;
        pool->task_queue[pool->queue_back].arg = arg;
        pool->queue_back++;
        pthread_cond_signal(&(pool->notify));
    }else{
        printf("Task Queue Is Full! Cannot add more task")
    }

    pthread_mutex_unlock(&(pool->lock));
}

int main(){
    threadpool_t pool;
    thread_pool_init(&pool);

    for(int i = 0 ; i < 100 ; i++){
        int *task_num = malloc(sizeof(int));
        *task_num = i;
        threadpool_add_task(&pool , example_task , task_num);
    }

    sleep(5);
    thread_distroy(&pool);
    return 0;
}