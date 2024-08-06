//Write a shell program to generate prime numbers in a given range 

#!/bin/bash
# Program to generate the prime numbers of a given range
echo "Enter the lower bound: "
read n1
echo "Enter the upper bound: "
read n2
for ((i=n1; i<=n2; i++))
do
for ((j=2; j<$i/2; j++))
do
if [[ $((i%j)) == 0 ]]
then
flag=1
break
fi
done
if [[ $flag == 0 ]]
then
echo -ne "$i " #This prints the numbers in a single line
fi
flag=0
done

// Implement Priority Pre-emptive scheduling algorithm by defining
// process structure. 
#include<stdio.h>
#include<stdlib.h>
typedef struct process {
 int Id, AT, BT, CT, TAT, WT, priority;
} pro;
pro p[15];
void main() {
 int n, tempBT[15], total_WT=0, total_TAT=0;
 float avg_WT=0, avg_TAT=0;
 printf("\nEnter the number of processes:\n");
 scanf("%d", &n);
 printf("\nEnter the arrival time, burst time and priority of the process:\n");
 printf("AT BT p\n");
 for(int i=0; i<n; i++) {
 p[i].Id = (i+1);
 scanf("%d%d%d", &p[i].AT, &p[i].BT, &p[i].priority);
 tempBT[i] = p[i].BT;
 }
 printf("\nGantt Chart:\n");
 int minIndex, minPriority, completed=0, curTime = 0;
 while(completed != n) {
 minIndex = -1;
 minPriority = 9999;
 for(int i=0; i<n; i++) {
 if(p[i].AT <= curTime && p[i].BT>0) {
 if(p[i].priority < minPriority || (p[i].priority == minPriority && p[i].AT <
p[minIndex].AT)) {
 minPriority = p[i].priority;
 minIndex = i;
 }
 }
 }
 curTime++;
 if (minIndex != -1)
 {
 p[minIndex].BT--;
 printf("| P%d(1) %d", p[minIndex].Id, curTime);
 if (p[minIndex].BT == 0)
 {
 p[minIndex].CT = curTime;
 p[minIndex].TAT = p[minIndex].CT - p[minIndex].AT;
 p[minIndex].WT = p[minIndex].TAT - tempBT[minIndex];
 total_TAT += p[minIndex].TAT;
 total_WT += p[minIndex].WT;
 completed++;
 }
 }
 }
 printf("|\n");
 avg_TAT = (float)total_TAT/n;
 avg_WT = (float)total_WT/n;
 //Printing the table of processes with details
 printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tP\n");
 for(int i=0; i<n; i++) {
 printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].Id, p[i].AT, tempBT[i], p[i].CT, p[i].TAT,
p[i].WT, p[i].priority);
 }
 printf("\nAverage TAT = %.2f\nAverage WT = %.2f\n", avg_TAT, avg_WT);
}
