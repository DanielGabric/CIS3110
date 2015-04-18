/*
Name: Daniel Gabric
Student ID: ******
Assignment #2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0
#define boolean int


#define LIMIT 300



typedef struct 
{
    int process_number;
    int arrival;
    int ogArrival;
    int * cpuTime;
    int * ioTime;
    int thread_number;
    int numOfBursts;
    int i;
    int run_time;
    int io_time;
}Information;




typedef struct
{
	int priority;

	Information info;
}PriorityQueue;

void insertPQ(PriorityQueue** pq, double priority, Information info, int * pos);
Information removePQ(PriorityQueue **pq, int *pos,int p);
