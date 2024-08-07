#check whether a given no. is palindrome or not

#!/bin/bash
echo "Enter a string: "
read str1
length=${#str1}
for((i=$length-1;i>=0;i--))
do
 str2=$str2${str1:$i:1}
done
echo $str2
if [ $str1 = $str2 ]
then
 echo "$str1 is palindrome"
else
 echo "$str1 is not a palindrome"
fi
echo "$str1 and $str2"

//9(b) to print N odd numbers

//parent.c

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
	int n;
	int arr[100];
} pack_t;

int main() {
	key_t key = 1234;
	int shmid = shmget(key, sizeof(pack_t), 0666 | IPC_CREAT);
	pack_t* shm_ptr = (pack_t*) shmat(shmid, NULL, 0666);
	printf("[PARENT] Created shared memory with key = %d, id = %d, address = %p\n", key, shmid, shm_ptr);
	printf("[PARENT] Enter n: ");
	scanf("%d", &shm_ptr->n);
	pid_t child_pid;
	if ((child_pid = fork()) == 0) {
		execlp("./child", "./child", NULL); // Child
	} else {
		wait(NULL);
		printf("[PARENT] Child finished executing.\n");
		printf("[PARENT] The first %d odd numbers are: \n", shm_ptr->n);
		for (int i = 0; i < shm_ptr->n; i++)
			printf("%d ", shm_ptr->arr[i]);
		putchar('\n');
	}

	shmdt((void*) shm_ptr);
	shmctl(shmid, IPC_RMID, NULL); // Delete shared memory
	printf("[PARENT] Shared Memory Deleted\n");
	return 0;
}

//child.c

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

typedef struct {
	int n;
	int arr[100];
} pack_t;

int main() {
	key_t key = 1234;
	int shmid = shmget(key, sizeof(pack_t), 0666);
	pack_t* shm_ptr = (pack_t*) shmat(shmid, NULL, 0666);
	printf("[CHILD] Attached to shared memory with key = %d, id = %d. address = %p\n", key, shmid, shm_ptr);
	for (int i = 0; i < shm_ptr->n; i++)
		shm_ptr->arr[i] = 2*i + 1;
	printf("[CHILD] Finished generating first %d odd numbers\n", shm_ptr->n);
	shmdt((void*) shm_ptr);
	return 0;
}


// OUTPUT
// [PARENT] Created shared memory with key = 1234, id = 1, address = 0x7f84bd37e000
// [PARENT] Enter n: 10
// [CHILD] Attached to shared memory with key = 1234, id = 1, address = 0x7f84bd37e000
// [CHILD] Finished generating first 10 odd numbers
// [PARENT] Child finished executing.
// [PARENT] The first 10 odd numbers are: 
// 1 3 5 7 9 11 13 15 17 19
// [PARENT] Shared Memory Deleted
