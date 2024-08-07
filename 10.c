/*program to implement the cat command which prints the content of the file*/
//simulate CAT cmd (VIEW only)
#include<stdio.h>
#include<stdlib.h>
void main(int argc, char *argv[])
{
FILE *fp;
char ch;
if(argc<2)
{
printf("File not entered");
exit(1);
}
fp=fopen(argv[1],"r");
if(fp==NULL)
{
printf("File does not exist\n");
exit(1);
}
while((ch=fgetc(fp))!=EOF)
printf("%c",ch);
fclose(fp);
printf("\n");
}

//execution-----:-
//gedit -s cat.c
//gcc cat.c
// ./a.out cat.c



//10(b)-------------------prime range shm_open and mmap

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

//execution-------:---
// cc parent.c -lrt
// cc child.c -lrt -lm
// ./a.out 2 20
