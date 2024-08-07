//Program to demonstrate the use of I/O system calls for both console and file I/O
//----use of I/O system calls for console//

#include<unistd.h>
#include<stdio.h>
#define BUFFER_SIZE 30
int main()
{
 char buffer[BUFFER_SIZE];
 int n=read(0,buffer,BUFFER_SIZE);
 printf("Number of characters read:%d\n",n);
 write(1,buffer,n);
 return 0;
}


//part-a2----------------------------------------------------
//----use of I/O system calls for FILE I/O//

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define BUFFER_SIZE 30
int main()
{
 int fd=open("myfile.txt",O_RDWR),n;
 char buffer[BUFFER_SIZE];
 printf("File discriptor is %d\n",fd);
 if(fd==-1)
 {
  printf("Failed to open file.\n");
  exit(0);
 }
 printf("Reading first 10 characters from file: \n");
 n=read(fd,buffer,10);
 write(1,buffer,n);
 printf("\nSkipping 5 characters from current position in the file: \n");
 lseek(fd,5,SEEK_CUR);
 n=read(fd,buffer,10);
 write(1,buffer,n);
 printf("\nGoing 10 characters before the current position in the file: \n");
 lseek(fd,-10,SEEK_CUR);
 n=read(fd,buffer,10);
 write(1,buffer,n);
 printf("\nGoing to 5th last character in the file:\n");
 lseek(fd,-5,SEEK_END);
 n=read(fd,buffer,5);
 write(1,buffer,n);
 printf("\nGoing to the 3rd character in the file: \n");
 lseek(fd,3,SEEK_SET);
 n=read(fd,buffer,5);
 write(1,buffer,n);
 return 0;
}

//FCFS CODE------------------------------
// FCFS scheduling algorithm
#include<stdio.h>
typedef struct node{
    int processId;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    int responseTime;
} Process;

void fcfs(Process[],int);
int main()
{
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
     Process processes[n];
 // Accept process details from the user
    for (int i = 0; i < n; i++) 
    {
        printf("Process %d\n", i + 1);
        printf("Enter Arrival Time: ");
        scanf("%d", &processes[i].arrivalTime);
        printf("Enter Burst Time: ");
        scanf("%d", &processes[i].burstTime);
        processes[i].processId = i + 1;
        printf("\n");
    }
    // Sort processes based on arrival time (if needed)
       for (int i = 0; i < n - 1; i++)
       {
           for (int j = 0; j < n - i - 1; j++) 
           {
               if (processes[j].arrivalTime > processes[j + 1].arrivalTime) 
               {
                   Process temp = processes[j];
                   processes[j] = processes[j + 1];
                   processes[j + 1] = temp;
               }
           }
       } // sorting over
    	fcfs(processes,n);
return 0;
}

void fcfs(Process processes[], int n)
{
   int elapsedTime = 0;
   float avgWaitingTime = 0;
   float avgTurnAroundTime = 0;
   float avgResponseTime = 0;
   int totalWaitingTime = 0;
   int totalTurnAroundTime = 0;
   int totalResponseTime = 0;
      
    printf("\nGantt chart\n");  
    for(int i = 0; i<n; i++)
    {
        if(processes[i].arrivalTime > elapsedTime) // to take care of the idle time
          elapsedTime = elapsedTime + (processes[i].arrivalTime - elapsedTime);
        
        int temp = elapsedTime; 
        processes[i].responseTime = elapsedTime - processes[i].arrivalTime;  
        elapsedTime+=processes[i].burstTime;
        processes[i].completionTime = elapsedTime;
        processes[i].turnAroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
        printf("|(%d)  P%d  (%d)",temp,processes[i].processId,elapsedTime);
    }
    
    printf("|\n");
    printf("Observation Table\nPID \tAT \t BT \tCT \tTT \tWT \tRT \n");
     
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", processes[i].processId, processes[i].arrivalTime,
               processes[i].burstTime,processes[i].completionTime,processes[i].turnAroundTime, processes[i].waitingTime, processes[i].responseTime);
               
        totalTurnAroundTime+= processes[i].turnAroundTime;
        totalWaitingTime+= processes[i].waitingTime;
        totalResponseTime+=processes[i].responseTime;       
    }
       
    avgTurnAroundTime = (float)totalTurnAroundTime/n;
    avgWaitingTime    = (float)totalWaitingTime/n;
    avgResponseTime   = (float)totalResponseTime/n;
    
    printf("\nAverage Waiting Time: %.2lf\n", avgWaitingTime);
    printf("Average Turnaround Time: %.2lf\n", avgTurnAroundTime);
    printf("Average Response Time: %.2lf\n", avgResponseTime);
}

