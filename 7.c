// Simulation of "grep" command 
  #include<stdio.h> 
  #include<string.h> 
  void main() 
  { 
  char fn[10],pat[10],temp[200]; 
  FILE *fp; 
  printf("Enter file name\n"); 
  scanf("%s",fn); 
  printf("Enter pattern to be searched\n"); 
  scanf("%s",pat); 
  fp=fopen(fn,"r"); 
  while(!feof(fp)) 
  { 
  
  fgets(temp,100,fp); 
  if(strstr(temp,pat))//finds the first occurrence of the substring in the string 
    printf("%s",temp); 
  } 
  fclose(fp); 
  }
//ii) Using shmget
//parent.c:
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
	int k;
	int n1, n2, n3;
	const int SIZE = 4096;
	int shmid;
	void *ptr;
	shmid = shmget((key_t) 1122, 4096, 0666 | IPC_CREAT);
	ptr = shmat(shmid, NULL, 0666);
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
	return 1;
	}
	pid = fork();
	if (pid == 0)
	{
	execlp("./fib", "fib", argv[1], NULL);
	}
	else if (pid > 0)
	{
	wait(NULL);
	printf("\nPARENT: child completed\n");
	printf("Parent printing:\n");
	printf("%s ", (char *)ptr);
	shmdt(ptr);
	
	}
	return 0;
}

child.c:
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
	int shmid = shmget((key_t) 1122, 4096, 0666);
	ptr = shmat(shmid, NULL, 0666);
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
	while (k != i)
	{
	n3 = n1 + n2;
	sprintf(ptr, "%d ", n3);
	printf("%d ", n3);
	n1 = n2;
	n2 = n3;
	ptr += strlen(ptr);
	k++;
	}
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}

// Output:
// $ gcc parent.c
// $ gcc child.c -o fib
// $ ./a.out 5
// CHILD:
// 0 1 1 2 3
// PARENT: child completed
// Parent printing:
// 0 1 1 2 3


//execution step:-
//cc parent.c -lrt
//cc child.c -o fib -lrt
// ./a.out 7

