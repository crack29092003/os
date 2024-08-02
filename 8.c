//Write a program to simulate ls command using system calls
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    DIR *dp;
    struct dirent *dirp;

    if (argc < 2) {
        printf("\nYou are not passing the directory\n");
        exit(1);
    }

    if ((dp = opendir(argv[1])) == NULL) {
        printf("\nCannot open directory %s\n", argv[1]);
        exit(1);
    }

    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp->d_name);
    }

    closedir(dp);
    return 0;
}

/*Write a program to generate and print Fibonacci series with the
following requirements:
 - Parent program should create a child and distribute the task of
generating Fibonacci no to its child.
 - The code for generating Fibonacci series should reside in different
program.
 - Child should write the generated Fibonacci series to a shared memory.
 - Parent process has to print by retrieving the Fibonacci series from the
shared memory.
i) Implement the above using shm_open and mmap*/
//parent.c
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int i;
    pid_t pid;
    const int SIZE = 4096;
    int shmid;
    void *ptr;

    if (argc > 1) {
        sscanf(argv[1], "%d", &i);
        if (i < 1) {
            printf("Error input: %d\n", i);
            return 0;
        }
    } else {
        printf("Error: Not passing N in command line\n");
        return 1;
    }

    shmid = shmget((key_t) 1122, SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    ptr = shmat(shmid, NULL, 0);
    if (ptr == (void *) -1) {
        perror("shmat");
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        execlp("./child", "child", argv[1], NULL);
    } else if (pid > 0) {
        wait(NULL);
        printf("\nPARENT: child completed\n");
        printf("Parent printing:\n");
        printf("%s\n", (char *)ptr);
        shmdt(ptr);
        shmctl(shmid, IPC_RMID, NULL);
    } else {
        perror("fork");
        return 1;
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
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int k = 2, n1, n2, n3;
    void *ptr;
    const int SIZE = 4096;

    int shmid = shmget((key_t) 1122, SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    ptr = shmat(shmid, NULL, 0);
    if (ptr == (void *) -1) {
        perror("shmat");
        return 1;
    }

    printf("CHILD:\n");
    int i = atoi(argv[1]);
    n1 = 0;
    n2 = 1;
    sprintf(ptr, "%d ", n1);
    ptr += strlen(ptr);
    printf("%d ", n1);
    sprintf(ptr, "%d ", n2);
    ptr += strlen(ptr);
    printf("%d ", n2);

    while (k < i) {
        n3 = n1 + n2;
        sprintf(ptr, "%d ", n3);
        printf("%d ", n3);
        n1 = n2;
        n2 = n3;
        ptr += strlen(ptr);
        k++;
    }

    printf("\n");

    shmdt(ptr);

    return 0;
}

