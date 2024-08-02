// Write a program to simulate cat command using system calls.
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    char ch;

    if (argc < 2) {
        printf("File not entered\n");
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("File does not exist\n");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }

    fclose(fp);
    printf("\n");

    return 0;
}

/*Write a program to generate and print Prime nos. between a given
3
range (between M & N) with the following requirements:
 - M & N should be passed as command line arguments
 - Error checking should be done to verify the required no. of arguments
at the command line
 - Parent program should create a child and distribute the task of
generating Prime numbers to its child.
 - The code for generating Prime numbers should reside in different
program.
 - Child should write the generated Prime numbers to a shared memory.
 - Parent process has to print the Prime numbers by retrieving from the
shared memory.
i) Implement the above using shm_open and mmap
*/
//parent.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>

#define SIZE 4096

int main(int argc, char *argv[]) {
    int i, j, shm_fd;
    pid_t pid;
    void *ptr;

    if (argc != 3) {
        fprintf(stderr, "Error: Minimum of two arguments must be passed as command line arguments!\n");
        exit(1);
    }

    sscanf(argv[1], "%d", &i);
    sscanf(argv[2], "%d", &j);

    if (i < 2) {
        fprintf(stderr, "Error input: %d\n", i);
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        execlp("./child", "child", argv[1], argv[2], NULL);
        perror("execlp failed");
        exit(1);
    } else {
        wait(NULL);
        printf("\nParent: child complete!\n");

        shm_fd = shm_open("VSS", O_RDONLY, 0666);
        if (shm_fd == -1) {
            perror("shm_open");
            exit(1);
        }

        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }

        printf("%s", (char *) ptr);

        munmap(ptr, SIZE);
        close(shm_fd);
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
#include <stdlib.h>

#define SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error: Exactly two arguments are required!\n");
        exit(1);
    }

    void *ptr;
    int sharedMemoryFileDescriptor = shm_open("VSS", O_CREAT | O_RDWR, 0666);
    ftruncate(sharedMemoryFileDescriptor, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, sharedMemoryFileDescriptor, 0);

    printf("CHILD:\n");

    int i = atoi(argv[1]);
    int j = atoi(argv[2]);
    int flag = 0;

    char buffer[SIZE];
    snprintf(buffer, sizeof(buffer), "The prime numbers in the range %d and %d are:\n", i, j);
    strcat(ptr, buffer);
    ptr += strlen(buffer);

    for (int num = i; num <= j; num++) {
        flag = 0;
        if (num < 2) {
            continue;
        }
        for (int k = 2; k <= num / 2; k++) {
            if (num % k == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            snprintf(buffer, sizeof(buffer), "%d\t", num);
            strcat(ptr, buffer);
            ptr += strlen(buffer);
        }
    }

    snprintf(buffer, sizeof(buffer), "\n");
    strcat(ptr, buffer);
    ptr += strlen(buffer);

    munmap(ptr, SIZE);
    close(sharedMemoryFileDescriptor);

    return 0;
}
