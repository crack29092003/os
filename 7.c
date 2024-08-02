//Write a program to simulate grep command using system calls
#include <stdio.h>
#include <string.h>

int main() {
    char fn[100], pat[100], temp[200];
    FILE *fp;

    printf("Enter file name: ");
    scanf("%99s", fn);

    printf("Enter pattern to be searched: ");
    scanf("%99s", pat);

    fp = fopen(fn, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (fgets(temp, sizeof(temp), fp) != NULL) {
        if (strstr(temp, pat) != NULL) {
            printf("%s", temp);
        }
    }

    fclose(fp);
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
i) Implement the above using shmget and shmat
Note: Shared object should be removed at the end in the program*/
//parent.c
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int i;
    pid_t pid;
    const int SIZE = 4096;
    int shm_fd;
    void *ptr;

    if (argc > 1)
    {
        sscanf(argv[1], "%d", &i);
        if (i < 1)
        {
            printf("Error input: %d\n", i);
            return 0;
        }
    }
    else
    {
        printf("Error: not passing N in command line\n");
        exit(0);
    }

    pid = fork();
    if (pid == 0) // CHILD
    {
        execlp("./child", "child", argv[1], NULL);
    }
    else if (pid > 0)
    {
        wait(NULL);
        printf("\nPARENT: child completed\n");

        shm_fd = shm_open("VSS", O_RDONLY, 0666);
        if (shm_fd == -1)
        {
            perror("shm_open");
            exit(1);
        }

        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }

        printf("Parent printing:\n");
        printf("%s\n", (char *)ptr);

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

int main(int argc, char *argv[])
{
    int k = 2, n1, n2, n3;
    void *ptr;
    const int SIZE = 4096;

    int shm_fd = shm_open("VSS", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(shm_fd, SIZE) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
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

    while (k < i)
    {
        n3 = n1 + n2;
        sprintf(ptr, "%d ", n3);
        printf("%d ", n3);
        n1 = n2;
        n2 = n3;
        ptr += strlen(ptr);
        k++;
    }

    printf("\n");
    return 0;
}

