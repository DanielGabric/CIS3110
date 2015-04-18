/*
Name: Daniel Gabric
Student ID: ******
Assignment #2
*/
#include "scheduler.h"

int main(int argc,char * argv[])
{
	boolean isFCFS = true;
	boolean isRR = false;
	boolean isD = false;
	boolean isV = false;
	int quantum = -1;
	int size=0;
	int i,j,k;
    int junk;
    int process_number;
    int process_switch;
    int thread_switch;
    int arrival;
    int bursts;
    int M;
    int N;
    int thread_num;
    PriorityQueue *pq;
    char * descriptive;
    

    pq = malloc(sizeof(PriorityQueue)*LIMIT);

    /*Parses for the flags*/
	for(i=1;i<argc;++i)
	{
		if(!strcmp(argv[i],"-r"))
		{
			isFCFS = false;
			isRR = true;
			quantum = atoi(argv[i+1]);
			break;
		}
		if(!strcmp(argv[i],"-d"))
		{
			isD = true;
			continue;
		}
		if(!strcmp(argv[i],"-v"))
		{
			isV = true;
		}
	}
    /*Number of processes*/
    scanf("%d",&N);
   

    /*If is descriptive then malloc memory*/
    if(isD)
    {
        
        descriptive = malloc(sizeof(char)*(LIMIT*LIMIT));
        
    }

    /*thread and process switch input*/
    scanf("%d",&thread_switch);
    scanf("%d",&process_switch);
    int sizeOF = 0;
    int m = 0;





    /*Getting all info from text file and then printing it*/
    for(i=0;i<N;++i)
    {
        scanf("%d",&process_number);
        scanf("%d",&M);

        for(j=0;j<M;++j)
        {
            Information info;
            scanf("%d",&thread_num);

            scanf("%d",&arrival);
            scanf("%d",&bursts);
            info.ioTime = malloc(sizeof(int)*bursts);
            info.cpuTime = malloc(sizeof(int)*bursts);
            info.arrival = arrival;
            info.ogArrival = arrival;
            info.thread_number = thread_num;
            info.process_number = process_number;

            info.i = 0;
            info.process_number = process_number;
            info.numOfBursts = bursts;
            info.run_time = 0;
            info.io_time = 0;

            /*adding the different cpu/io bursts*/
            for(k=0;k<bursts;++k)
            {
                scanf("%d",&junk);
                scanf("%d",&info.cpuTime[k]);
                if(k>=bursts-1)
                {
                    info.ioTime[k]=0;
                    continue;
                }
                scanf("%d",&info.ioTime[k]);
                
            }

            /*Add to PQ here*/
            sizeOF++;
            insertPQ(&pq,arrival,info,&size);
        }
    }
    
    if(isRR)
    {
        printf("Round Robin (quantum = %d):\n\n",quantum);
    }
    else if(isFCFS)
    {
    	printf("FCFS:\n\n");
    }

    /*Not used*/
    boolean cpu_idle = false;
    boolean io_idle = false;
    int total_time = 0;

    /*Records the previous thread*/
    int prev_thread = -1;

    /*average turn around and previous process and runtime/idletime/currenttime*/
    float total_turn_around = 0;
    int prev_process = -1;
    int current_time = 0;
    int run_time = 0;
    int idle_time = 0;

    /*CPU SIMULATOR*/
    while(size!=0)
    {
    	
        Information removed = removePQ(&pq,&size,prev_process);
        /*State change*/
        if(isV)
        {
            if(removed.ogArrival == removed.arrival)
            {
                printf("At time %d: Thread %d of Process %d moves from %s to %s\n",current_time,removed.thread_number,removed.process_number,"new","ready");
            }
        }
        
        if((prev_process != -1 &&prev_thread != -1)) 
        {   

            if(removed.process_number == prev_process && removed.thread_number != prev_thread)
            {
        	    idle_time+=thread_switch;
           	    current_time+=thread_switch;
            }
            else if (removed.process_number != prev_process && removed.thread_number == prev_thread)
            {
            	current_time+=process_switch;
            	idle_time+=process_switch;
            }
        }
            
        if(removed.arrival > current_time)
        {
            int r = removed.arrival - current_time;
        	current_time += r;
        	idle_time +=r;
        }
        else
        {
        	removed.arrival += current_time - removed.arrival;
        }
        /*State change*/
        if(isV)
        {
            printf("At time %d: Thread %d of Process %d moves from %s to %s\n",current_time,removed.thread_number,removed.process_number,"ready","running");
        }
        if(isFCFS)
        {
            /**FIRST COME FIRST SERVE CODE**/
            run_time += removed.cpuTime[removed.i];
            current_time += removed.cpuTime[removed.i];
            removed.arrival += removed.cpuTime[removed.i];
            removed.run_time += removed.cpuTime[removed.i];
        }
        else if(isRR)
        {
            /***ROUND ROBIN CODE***/
        	if(removed.cpuTime[removed.i]>quantum)
            {
            	current_time+= quantum;
            	run_time += quantum;
            	removed.arrival += quantum;
            	removed.run_time += quantum;
            	removed.cpuTime[removed.i]-=quantum;
            	prev_process = removed.process_number;
                prev_thread = removed.thread_number;
            	insertPQ(&pq,removed.arrival,removed,&size);
                /*State change*/
                if(isV)
                    printf("At time %d: Thread %d of Process %d moves from %s to %s\n",removed.arrival,removed.thread_number,removed.process_number,"running","ready");
            	continue;
            }
            else
            {
            	removed.run_time += removed.cpuTime[removed.i];
            	current_time += removed.cpuTime[removed.i];
            	run_time += removed.cpuTime[removed.i];
            	removed.arrival+=removed.cpuTime[removed.i];

            }
        }

        /*If we are not at the last burst yet*/
        if(removed.i <= removed.numOfBursts-1)
        {
            /*State change*/
            if(isV)
                printf("At time %d: Thread %d of Process %d moves from %s to %s\n",removed.arrival,removed.thread_number,removed.process_number,"running","blocked");
            removed.arrival += removed.ioTime[removed.i];
            removed.io_time += removed.ioTime[removed.i];
            /*State change*/
            if(isV)
                printf("At time %d: Thread %d of Process %d moves from %s to %s\n",removed.arrival,removed.thread_number,removed.process_number,"blocked","ready");
            removed.i++;
        }
        
        /*If we're on the last burst*/
        if(removed.i==removed.numOfBursts)
        {
            /*State change*/
         	if(isV)
                printf("At time %d: Thread %d of Process %d moves from %s to %s\n",removed.arrival,removed.thread_number,removed.process_number,"running","terminated");
            if(isD)
            {
                /*Buffer for the descriptions*/
                char buffer[LIMIT];
                /*This makes it easier to add multiple arguments to a char * */
            	sprintf(buffer,"Thread %d of Process %d:\n\narrival time: %d\nservice time: %d\nI/O time: %d\nturnaround time: %d\nfinish time: %d\n\n"
                		,removed.thread_number,removed.process_number,removed.ogArrival,removed.run_time,removed.io_time,removed.arrival-removed.ogArrival,removed.arrival);
                /*Concatenating the buffer to the description*/
                strncat(descriptive,buffer,strlen(buffer));
            }
            total_turn_around += removed.arrival-removed.ogArrival;
            /*Freeing memory of the bursts*/
            free(removed.ioTime);
            free(removed.cpuTime);
            continue;
        }
        
        /*Keeping track of previous processes/threads*/
        prev_process = removed.process_number;
        prev_thread = removed.thread_number;
        insertPQ(&pq,removed.arrival,removed,&size);
    }

    /*prints description if flag was set*/
    if(isD)
        printf("%s\n",descriptive);
    /*Divides total turn around by number of threads to get avgTurnAround*/
    total_turn_around/=sizeOF;
    int CPU_percent = (run_time*100)/(current_time);
    /*Regular print*/
    printf("Total Time required is %d time units\n",current_time);
    printf("Average Turnaround Time is %.2f time units\n",total_turn_around);
    printf("CPU Utilization is %d%%\n",CPU_percent);
    free(pq);
    if(isD)
        free(descriptive);
    exit(0);

    return 0;
}



/****PRIORITYQUEUE*****/

void insertPQ(PriorityQueue ** pq, double priority, Information info, int * pos)
{
    (*pq)[(*pos)].priority = priority;
    (*pq)[(*pos)++].info = info;
    int position=(*pos)-1;
    while(true)	
    {
        if(position==0)break;
        int parent = (position-1)/2;
        if((*pq)[position].priority>=(*pq)[parent].priority)break;
        else
        {
            PriorityQueue  temp= (*pq)[position];
            (*pq)[position]=(*pq)[parent];
            (*pq)[parent]=temp;
            position = parent;
        }
    }

}





Information removePQ(PriorityQueue  **pq, int * pos, int p)
{
    Information removed = (*pq)[0].info;
    
    
    (*pq)[0]=(*pq)[--(*pos)];
    int position = 0;

    while(true)
    {
        int child1 = 2*position +1,child2=2*position+2;
        int biggerChild;
        if(child1>=(*pos))break;
        if(child2>=(*pos))biggerChild = child1;
        else biggerChild = child2;
        if((*pq)[biggerChild].priority>=(*pq)[position].priority)break;
        else
        {
            PriorityQueue temp = (*pq)[position];
            (*pq)[position]=(*pq)[biggerChild];
            (*pq)[biggerChild]=temp;
            position = biggerChild;
        }

    }
    return removed;
}
/*********END PRIORITYQUEUE*********/


//while (!EventQ.IsEmpty())
//{
//    nextEvent = EventQ.pop();
//    switch (nextEvent ->type) {
//        case ARRIVAL:
//        /* create a new process and place it into ReadyQ */
//        ...
//            break;
//        case PREEMPTION: //for RR
//        /* current process preempted into ReadyQ */
//            cpu_idle = TRUE;
//...
//            break;
//        case IO_BURST:
//        /* move the current process into eventQ for completion */
//            cpu_idle = TRUE;
//...
//            break;
//        case IO_COMPLETE:
//        /* put the process in ReadyQ */
//            io_idle = TRUE;
//...
//            break;
//        case THREAD_TERMINATION or PROCESS_TERMINATION :
//        /* update statistics*/
//            cpu_idle = TRUE;
//...
//            break;
//    }
//    if (cpu_idle && !ReadyQ.IsEmpty()) {
//        cpu_idle = FALSE;
//        ReadyQ.pop(); /* run the next process from the ready Q */
// …
//    }
//} 


