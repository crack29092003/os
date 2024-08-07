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

//zombie------------------------------------------------
//Program to demonstrate the creation of zombie process.

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
 pid_t child_pid,my_pid,parent_pid;
 int i=10;
 child_pid=fork();
 if(child_pid<0)
 {
  printf("Fork failed.Exiting!\n");
  exit(0);
 }
 if(child_pid==0)
 {
  //child process
  printf("[CHILD]this is the child process.\n");
  my_pid=getpid();
  parent_pid=getppid();
  printf("[CHILD]My pid is %d\n",my_pid);
  printf("[CHILD]My parent's pid is %d\n",parent_pid);
  printf("[CHILD]Exiting.\n");
  exit(0);
 }
 else
 {
  //Parent process
  printf("[PARENT]This is the parent process.\n");
  my_pid=getpid();
  parent_pid=getppid();
  printf("[PARENT]My pid is %d\n",my_pid);
  printf("[PARENT]My parent's pid is %d\n",parent_pid);
  printf("[PARENT]Sleeping for 10 seconds.\n");
  sleep(10);
  printf("[PARENT]child pid= %d has ended,but it has an entry in process table.\n"
  "[PARENT]It is a zombie process.\n",child_pid);
 }
 return 0;
}

//orphan-----------------------------------------------
//Program to demonstrate the creation of orphan process.

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
 pid_t child_pid,my_pid,parent_pid;
 int i=10;
 child_pid=fork();
 if(child_pid<0)
 {
  printf("Fork failed.Exiting!\n");
  exit(0);
 }
 if(child_pid==0)
 {
  //child process
  printf("[CHILD]this is the child process.\n");
  my_pid=getpid();
  parent_pid=getppid();
  printf("[CHILD]My pid is %d\n",my_pid);
  printf("[CHILD]My parent's pid is %d\n",parent_pid);
  printf("[CHILD]Sleeping for 10 seconds.\n");
  sleep(10);
  printf("[CHILD]My parent ended.So I am an orphan process adopted by init process.\n");
 }
 else
 {
  //Parent process
  printf("[PARENT]This is the parent process.\n");
  my_pid=getpid();
  parent_pid=getppid();
  printf("[PARENT]My pid is %d\n",my_pid);
  printf("[PARENT]My parent's pid is %d\n",parent_pid);
  printf("[PARENT]Exiting.\n");
  exit(0);
 }
 return 0;
}
