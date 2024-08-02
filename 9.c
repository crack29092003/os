//Write a shell program to check whether a given no. is a palindrome or not
#!/bin/bash
# Program to find if a given string or number is palindrome or not
echo "Enter the string to be checked: "
read str
length=${#str}
for ((i=length-1; i>=0; i--))
do
reverse=$reverse${str:$i:1}
done
if [[ $reverse == "$str" ]]
then
echo "The string $str is a palindrome"
else
echo "The string $str is not a palindrome"
fi

/*Write a program to generate and print N ODD numbers with the
following requirements:
 - Parent program should create a child and distribute the task of
generating odd numbers to its child.
 - The code for generating odd numbers should reside in different
program.
 - Child should write the generated odd numbers to a shared memory.
 - Parent process has to print the odd numbers by retrieving from the
shared memory.
i) Implement the above using shmget and shmat*/
//parent.c
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int n;
    int arr[100];
} pack_t;

int main() {
    key_t key = 1234;
    int shmid = shmget(key, sizeof(pack_t), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    pack_t* shm_ptr = (pack_t*) shmat(shmid, NULL, 0);
    if (shm_ptr == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    printf("[PARENT] Created shared memory with key = %d, id = %d, address = %p\n", key, shmid, shm_ptr);
    printf("[PARENT] Enter n: ");
    scanf("%d", &shm_ptr->n);

    pid_t child_pid;
    if ((child_pid = fork()) == 0) {
        execlp("./child", "./child", NULL); // Child
    } else if (child_pid > 0) {
        wait(NULL);
        printf("[PARENT] Child finished executing.\n");
        printf("[PARENT] The first %d odd numbers are: \n", shm_ptr->n);
        for (int i = 0; i < shm_ptr->n; i++) {
            printf("%d ", shm_ptr->arr[i]);
        }
        putchar('\n');

        shmdt((void*) shm_ptr);
        shmctl(shmid, IPC_RMID, NULL); // Delete shared memory
        printf("[PARENT] Shared Memory Deleted\n");
    } else {
        perror("fork");
        shmdt((void*) shm_ptr);
        shmctl(shmid, IPC_RMID, NULL); // Delete shared memory
        exit(1);
    }

    return 0;
}


//child.c
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>

typedef struct {
    int n;
    int arr[100];
} pack_t;

int main() {
    key_t key = 1234;
    int shmid = shmget(key, sizeof(pack_t), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    pack_t* shm_ptr = (pack_t*) shmat(shmid, NULL, 0);
    if (shm_ptr == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    printf("[CHILD] Attached to shared memory with key = %d, id = %d. address = %p\n", key, shmid, shm_ptr);

    for (int i = 0; i < shm_ptr->n; i++) {
        shm_ptr->arr[i] = 2 * i + 1;
    }

    printf("[CHILD] Finished generating first %d odd numbers\n", shm_ptr->n);

    shmdt((void*) shm_ptr);

    return 0;
}
