//11(a)---main thread------

//parent.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
int main (int arguementCount, char *arguementVector[]) {
int i, k, n1, n2, sharedMemoryFileDescriptor, j;
const int SIZE = 4096;
pid_t processID;
void *ptr;
if (arguementCount > 2) {
sscanf(arguementVector[1], "%d", &i);
sscanf(arguementVector[2], "%d", &j);
if (i < 2) {
printf("Error input: %d\n", i);
return 0;
}
} else {
printf("Error: Minimum of two arguements must be passed as command line
arguements!");
exit(0);
}
processID = fork();
if (processID == 0) {
execlp("./child.c", "prime", arguementVector[1], arguementVector[2], NULL);
} else if (processID > 0) {
wait(NULL);
printf("\nParent: child complete!\n");
sharedMemoryFileDescriptor = shm_open("VSS", O_RDONLY, 0666);
ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED,
sharedMemoryFileDescriptor, 0);
printf("%s", (char *) ptr);
shm_unlink("VSS");
}
return 0;
}

//child.c
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<stdlib.h>
#include <math.h>
int main(int arguementCount,char *arguementVector[]) {
void *ptr;
int sharedMemoryFileDescriptor = shm_open("VSS", O_CREAT | O_RDWR, 0666);
ftruncate(sharedMemoryFileDescriptor,4096);
ptr = mmap(0,4096,PROT_WRITE, MAP_SHARED, sharedMemoryFileDescriptor, 0);
printf("CHILD:\n");
int i = atoi(arguementVector[1]);
int j = atoi(arguementVector[2]);
int flag = 0;
printf("The prime numbers in the range %d and %d are:\n", i, j);
for (int num = i; num < j; num++) {
flag = 0;
for (int k = 2; k <= n/2; k++) {
if (num % k == 0) {
flag = 1;
break;
}
}
if (flag == 0) {
printf("%d\t", num);
}
}
printf("\n");
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
