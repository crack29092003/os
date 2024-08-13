//program with two threads and main thread; also passing parameters to child threads
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
void *sum(void *arg);
void *mul(void *arg);
int sm=0, prod=1;
void main(int argc, char * argv[])
{
  pthread_t T1,T2;
  pthread_attr_t attr;//set of thread attributes
  pthread_attr_init(&attr);//get the default attributes
    
  pthread_create(&T1, &attr, sum, argv[1]);
  pthread_create(&T2, &attr, mul, argv[1]);
  pthread_join(T1,NULL);
  pthread_join(T2,NULL);
  
  printf("Inside main thread\n");
  printf("sum=%d\n",sm);
  printf("product=%d\n",prod);
 }
void *sum(void *parm) 
{ 
  int i, n; 
   n = atoi(parm);//string to integer i.e., ASCII to int
  printf("inside  sum thread\n");
  
  for(i=1; i<=n;i++)
  {
    sm+=i;     
  }
 printf("sum thread completed\n");
}
void *mul(void *parm)
{
  int i, n;
  n = atoi(parm);
  printf("inside mul thread\n");
  for(i=2; i<=n;i++)
  {
    prod =prod *i;
  }
  printf("mul thread completed  product\n");
}

//11(b)----------producer-consumer using mutex and semaphores-----

/* Solution for producer-consumer problem using mutex and semaphore. 3 producers and 3 consumers are used to demonstrate the solution.*/
//to compile:  gcc filename.c -pthread
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#define MaxItems 3 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 3 // Size of the buffer

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand()%100; // Produce an random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
int main()
{   

    pthread_t pro[3],con[3];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[3] = {1,2,3}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 3; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 3; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 3; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 3; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
  }


//execution----:-------
// gedit -s pc.c
//gcc pc.c -pthread
// ./a.out
