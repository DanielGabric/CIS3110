/*
Name: Daniel Gabric
ID: ******
Assignment #3 PART 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct M
{
    int size;
    char pid;
    int isHole;
    int numTimes;
    int howOld;
    struct M * next;
}Slot,Queue;

Slot * create1(char pid, int size, int isHole, int numTimes,int howOld);
Queue * push(Queue * process,int pid, int size, int numTimes);
Queue * pop(Queue ** process);
Queue * create(char pid, int size,int numTimes);
int countHoles(Slot * process);
void next_fit(Queue * process);
void best_fit(Queue * process);
void worst_fit(Queue * process);
void first_fit(Queue * process);
int keepLooping(Queue * process);
Slot * merge(Slot * memory);

int main(int argc, char * argv[])
{
    if(argc!=2)
    {
        printf("You need one argument which is the testfile\n");
        return -1;
    }
    FILE * filePtr = fopen(argv[1],"r");
    if(!filePtr)
    {
        printf("Fatal error in opening testfile\n");
        return -1;
    }
    char processID;
    int sizeMB;
    char buffer[10];
    Queue * process = NULL;
    char * token;

    while(fgets(buffer,9,filePtr)!=NULL)
    {

        processID = buffer[0];
        token = (char*)strtok(buffer," ");
        token = (char*)strtok(NULL,"\n");
        sizeMB = atoi(token);
        process = push(process,processID,sizeMB,0); 
    }
    printf("First Fit\n");
    first_fit(process);
    printf("Best Fit\n");
    best_fit(process);
    printf("Next Fit\n");
    next_fit(process);
    printf("Worst Fit\n");
    worst_fit(process);
    fclose(filePtr);
    return 0;
}

void worst_fit(Queue*process)
{
    Queue * copy = NULL;
    Queue * temp = process;
    
    int i;
    Slot * memory = create1('-',128,1,0,0);
    while(temp!=NULL)
    {
        copy=push(copy,temp->pid,temp->size,0);
        temp=temp->next;
    }

    int j;
    int oldest=0;
    int totalProcess=0;
    int totalHoles=1;
    int currentAmtProcesses=0;
    int currentAmtHoles=1;
    int totalLoads=0;
    int totalMemory=0;
    int memoryTaken=0;
    
    for(i=0;keepLooping(copy);++i)
    {
        Queue * toAdd = pop(&copy);
        
        Slot * tempMem = memory;
        j=0;
        int isGood=0;
        
        do
        {
            tempMem = memory;

            tempMem = merge(tempMem);
            currentAmtHoles = countHoles(tempMem);

            if(tempMem->next==NULL)
            {
                if(!tempMem->isHole)
                {
                    copy = push(copy,tempMem->pid,tempMem->size,tempMem->numTimes+1);
                    memoryTaken-=tempMem->size;
                    currentAmtProcesses--;
                    /*Print something here*/
                }

                Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                totalLoads++;
                currentAmtProcesses++;
                newMemory->next=newHole;
                memory = newMemory;
                totalProcess+=currentAmtProcesses;
                totalHoles += currentAmtHoles;
                memoryTaken+=newMemory->size;
                totalMemory+=(memoryTaken*100);
                /*PRINT OFF SOMETHING HERE*/
                printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                goto out;
            }
            if(tempMem->isHole)
            {
                if(tempMem->size >=toAdd->size)
                {
                    Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                    Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    if(tempMem->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next;
                    }


                    totalLoads++;
                    currentAmtProcesses++;
                    
                    memory = newMemory;
                    totalProcess+=currentAmtProcesses;
                    totalHoles += currentAmtHoles;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                    goto out;
                }
            }
            int counter=0;
            int max = -1;
            int saveCount=-1;
            tempMem = memory;
            while(tempMem->next!=NULL)
            {
                
                if(tempMem->next->isHole && tempMem->next->size >=toAdd->size)
                {
                    
                    if(max< tempMem->next->size-toAdd->size)
                    {
                        max = tempMem->next->size-toAdd->size;
                        saveCount = counter;

                    }
                    counter++;
                }
                
                tempMem = tempMem->next;
            }
            tempMem=memory;
            
            if(max!=-1)
            {
                if(saveCount==-1)
                {
                    if(!tempMem->isHole)
                        break;
                    if(tempMem->size < toAdd->size)
                        break;

                    Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                    Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    if(tempMem->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next;
                    }


                    totalLoads++;
                    currentAmtProcesses++;
                    
                    memory = newMemory;
                    totalProcess+=currentAmtProcesses;
                    totalHoles += currentAmtHoles;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                    goto out;
                }
                else
                {
                    int fd=0;
                    
                    while(tempMem->next!=NULL)
                    {
                        if(tempMem->next->isHole)fd++;
                        if(fd==saveCount+1)
                        {
                            break;
                        }
                        tempMem=tempMem->next;
                    }
                    currentAmtProcesses++;
                    totalHoles+=currentAmtHoles;
                    totalProcess+=currentAmtProcesses;
                    isGood=1;
                    
                    Slot * newHole = create1('-',tempMem->next->size-toAdd->size,1,0,0);
                    Slot * newMemory=create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    totalLoads++;
                    if(tempMem->next->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next->next;
                    }
                    
                    free(tempMem->next);
                    tempMem->next = newMemory;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    /*PRINT OFF SOMETHING HERE*/
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                
                    goto out;
                }
            }
            /*EXTRACT LONGEST*/
            Slot * toExtract = memory;
            while(toExtract!=NULL)
            {
                
                if(toExtract->howOld==oldest&&!toExtract->isHole)
                {
                    
                    currentAmtProcesses--;
                    memoryTaken-=toExtract->size;
                    copy = push(copy,toExtract->pid,toExtract->size,toExtract->numTimes+1);
                    toExtract->isHole=1;
                    oldest++;
                    break;
                }
                toExtract=toExtract->next;
            }
        }while(!isGood);
        out:
        free(toAdd);
    }

    printf("Total loads = %d, average #processes = %.1f, average #holes = %.1f, cumulative %%mem = %d\n",
    totalLoads, ((float)(totalProcess)/(float)(i)),((float)(totalHoles)/(float)(i)),totalMemory/(128*i));
}

void best_fit(Queue*process)
{
    Queue * copy = NULL;
    Queue * temp = process;
    
    int i;
    Slot * memory = create1('-',128,1,0,0);
    while(temp!=NULL)
    {
        copy=push(copy,temp->pid,temp->size,0);
        temp=temp->next;
    }

    int j;
    int oldest=0;
    int totalProcess=0;
    int totalHoles=1;
    int currentAmtProcesses=0;
    int currentAmtHoles=1;
    int totalLoads=0;
    int totalMemory=0;
    int memoryTaken=0;
    
    for(i=0;keepLooping(copy);++i)
    {
        Queue * toAdd = pop(&copy);
        
        Slot * tempMem = memory;
        j=0;
        int isGood=0;
        
        do
        {
            tempMem = memory;

            tempMem = merge(tempMem);
            //printSlots(tempMem);
            //printf("\n\n");
            currentAmtHoles = countHoles(tempMem);

            if(tempMem->next==NULL)
            {
                if(!tempMem->isHole)
                {
                    copy = push(copy,tempMem->pid,tempMem->size,tempMem->numTimes+1);
                    memoryTaken-=tempMem->size;
                    currentAmtProcesses--;
                    /*Print something here*/
                }

                Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                totalLoads++;
                currentAmtProcesses++;
                newMemory->next=newHole;
                memory = newMemory;
                totalProcess+=currentAmtProcesses;
                totalHoles += currentAmtHoles;
                memoryTaken+=newMemory->size;
                totalMemory+=(memoryTaken*100);
                /*PRINT OFF SOMETHING HERE*/
                printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                goto out;
            }
            if(tempMem->isHole)
            {
                if(tempMem->size >=toAdd->size)
                {
                    Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                    Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    if(tempMem->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next;
                    }


                    totalLoads++;
                    currentAmtProcesses++;
                    
                    memory = newMemory;
                    totalProcess+=currentAmtProcesses;
                    totalHoles += currentAmtHoles;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                    goto out;
                }
            }
            int counter=0;
            int min = 999999;
            int saveCount=-1;
            tempMem = memory;
            while(tempMem->next!=NULL)
            {
                
                if(tempMem->next->isHole && tempMem->next->size >=toAdd->size)
                {
                    
                    if(min > tempMem->next->size-toAdd->size)
                    {
                        min = tempMem->next->size-toAdd->size;
                        saveCount = counter;

                    }
                    counter++;
                }
                
                tempMem = tempMem->next;
            }
            tempMem=memory;
            
            if(min!=999999)
            {
                if(saveCount==-1)
                {
                    if(!tempMem->isHole)
                        break;
                    if(tempMem->size < toAdd->size)
                        break;

                    Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                    Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    if(tempMem->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next;
                    }


                    totalLoads++;
                    currentAmtProcesses++;
                    
                    memory = newMemory;
                    totalProcess+=currentAmtProcesses;
                    totalHoles += currentAmtHoles;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                    goto out;
                }
                else
                {
                    int fd=0;
                    
                    while(tempMem->next!=NULL)
                    {
                        if(tempMem->next->isHole)fd++;
                        if(fd==saveCount+1)
                        {
                            break;
                        }
                        tempMem=tempMem->next;
                    }
                    currentAmtProcesses++;
                    totalHoles+=currentAmtHoles;
                    totalProcess+=currentAmtProcesses;
                    isGood=1;
                    
                    Slot * newHole = create1('-',tempMem->next->size-toAdd->size,1,0,0);
                    Slot * newMemory=create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    totalLoads++;
                    if(tempMem->next->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next->next;
                    }
                    
                    free(tempMem->next);
                    tempMem->next = newMemory;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    /*PRINT OFF SOMETHING HERE*/
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                
                    goto out;
                }
            }
            /*EXTRACT LONGEST*/
            Slot * toExtract = memory;
            while(toExtract!=NULL)
            {
                
                if(toExtract->howOld==oldest&&!toExtract->isHole)
                {
                    
                    currentAmtProcesses--;
                    memoryTaken-=toExtract->size;
                    copy = push(copy,toExtract->pid,toExtract->size,toExtract->numTimes+1);
                    toExtract->isHole=1;
                    oldest++;
                    break;
                }
                toExtract=toExtract->next;
            }
        }while(!isGood);
        out:
        free(toAdd);
    }
    printf("Total loads = %d, average #processes = %.1f, average #holes = %.1f, cumulative %%mem = %d\n",
    totalLoads, ((float)(totalProcess)/(float)(i)),((float)(totalHoles)/(float)(i)),totalMemory/(i*128));
}

void next_fit(Queue * process)
{
    Queue * copy = NULL;
    Queue * temp = process;
    
    int i;
    Slot * memory = create1('-',128,1,0,0);
    while(temp!=NULL)
    {
        copy=push(copy,temp->pid,temp->size,0);
        temp=temp->next;
    }

    int j;
    int oldest=0;
    int totalProcess=0;
    int totalHoles=1;
    int currentAmtProcesses=0;
    int currentAmtHoles=1;
    int totalLoads=0;
    int totalMemory=0;
    int memoryTaken=0;
    char lastPid='-';
    
    for(i=0;keepLooping(copy);++i)
    {
        Queue * toAdd = pop(&copy);
        
        Slot * tempMem = memory;
        j=0;
        int isGood=0;
   
        
        do
        {
            tempMem = memory;

            tempMem = merge(tempMem);

            //printSlots(tempMem);
            //printf("\n\n");
            currentAmtHoles = countHoles(tempMem);
            if(lastPid=='-')
            {
                isGood=1;
                currentAmtProcesses++;
                totalHoles+=currentAmtHoles;
                totalProcess+=currentAmtProcesses;
                Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                newMemory->next = newHole;
                memory = newMemory;
                memoryTaken+=newMemory->size;
                totalMemory+=(memoryTaken*100);
                totalLoads++;
                lastPid = toAdd->pid;
                printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                goto out;
            }

            while(tempMem->next!=NULL&&tempMem->pid != lastPid)tempMem=tempMem->next;


            char saveIt = lastPid;
            int flag = 0;
            while(1)
            {
                flag=1;
                
                if(tempMem->next!=NULL&&saveIt==tempMem->next->pid&&flag)
                {
                    break;
                }
               
                if(tempMem->next!=NULL &&tempMem->next->isHole&& tempMem->next->size >= toAdd->size)
                {
                    
                    Slot * newHole = create1('-',tempMem->next->size-toAdd->size,1,0,0);
                    Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    if(tempMem->next->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next->next;
                    }

                    lastPid = toAdd->pid;
                    totalLoads++;
                    currentAmtProcesses++;
                    
                    tempMem->next = newMemory;
                    totalProcess+=currentAmtProcesses;
                    totalHoles += currentAmtHoles;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));

                    goto out;
                }
                
                if(tempMem->next==NULL)
                {

                    lastPid = toAdd->pid;

                    //printf("%c %d %d %d %d %d\n",memory->pid,memory->size,toAdd->size,memory->isHole,memory->howOld,oldest);
                    if(memory->isHole&&memory->size>=toAdd->size)
                    {

                        Slot * newHole = create1('-',memory->size-toAdd->size,1,0,0);
                        Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                        if(memory->size!=toAdd->size)
                        {
                            newMemory->next=newHole;
                            newHole->next = memory->next;
                        }
                        else
                        {
                            free(newHole);
                            newMemory->next=tempMem->next->next;
                        }
                        lastPid = toAdd->pid;
                        totalLoads++;
                        currentAmtProcesses++;
                        
                        memory = newMemory;
                        totalProcess+=currentAmtProcesses;
                        totalHoles += currentAmtHoles;
                        memoryTaken+=newMemory->size;
                        totalMemory+=(memoryTaken*100);
                        printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                        currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                        goto out;
                    }
                    else if(memory->isHole && memory->size < toAdd->size)
                    {
                        
                        tempMem=memory;
                        
                        break;
                    }
                    
                    tempMem=memory;
                    break;
                }
                else
                {
                    
                    tempMem = tempMem->next;
                }
                

            }
            
            /*EXTRACT LONGEST*/
            Slot * toExtract = memory;
            while(toExtract!=NULL)
            {
                
                if(toExtract->howOld==oldest&&!toExtract->isHole)
                {
                    
                    currentAmtProcesses--;
                    memoryTaken-=toExtract->size;
                    copy = push(copy,toExtract->pid,toExtract->size,toExtract->numTimes+1);
                    toExtract->isHole=1;
                    toExtract->pid = '-';
                    oldest++;
                    break;
                }
                toExtract=toExtract->next;
            }
        }while(!isGood);
        out:
        free(toAdd);
    }

    printf("Total loads = %d, average #processes = %.1f, average #holes = %.1f, cumulative %%mem = %d\n",
    totalLoads, ((float)(totalProcess)/(float)(i)),((float)(totalHoles)/(float)(i)),totalMemory/(i*128));
}

void first_fit(Queue * process)
{
    Queue * copy = NULL;
    Queue * temp = process;
    
    int i;
    Slot * memory = create1('-',128,1,0,0);
    while(temp!=NULL)
    {
        copy=push(copy,temp->pid,temp->size,0);
        temp=temp->next;
    }

    int j;
    int oldest=0;
    int totalProcess=0;
    int totalHoles=1;
    int currentAmtProcesses=0;
    int currentAmtHoles=1;
    int totalLoads=0;
    int totalMemory=0;
    int memoryTaken=0;
    
    for(i=0;keepLooping(copy);++i)
    {
        Queue * toAdd = pop(&copy);
        
        Slot * tempMem = memory;
        j=0;
        int isGood=0;
        
        do
        {
            tempMem = memory;

            tempMem = merge(tempMem);
            //printSlots(tempMem);
            //printf("\n\n");
            currentAmtHoles = countHoles(tempMem);
            if(tempMem->next==NULL)
            {
                if(!tempMem->isHole)
                {
                    copy = push(copy,tempMem->pid,tempMem->size,tempMem->numTimes+1);
                    memoryTaken-=tempMem->size;
                    currentAmtProcesses--;
                    /*Print something here*/
                }

                Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                totalLoads++;
                currentAmtProcesses++;
                newMemory->next=newHole;
                memory = newMemory;
                totalProcess+=currentAmtProcesses;
                totalHoles += currentAmtHoles;
                memoryTaken+=newMemory->size;
                totalMemory+=(memoryTaken*100);
                /*PRINT OFF SOMETHING HERE*/
                printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                goto out;
            }
            if(tempMem->isHole)
            {
                if(tempMem->size >=toAdd->size)
                {
                    Slot * newHole = create1('-',tempMem->size-toAdd->size,1,0,0);
                    Slot * newMemory = create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    if(tempMem->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next;
                    }


                    totalLoads++;
                    currentAmtProcesses++;
                    
                    memory = newMemory;
                    totalProcess+=currentAmtProcesses;
                    totalHoles += currentAmtHoles;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                    goto out;
                }
            }
            while(tempMem->next!=NULL)
            {
                
                if(tempMem->next->isHole && tempMem->next->size >=toAdd->size)
                {
                    currentAmtProcesses++;
                    totalHoles+=currentAmtHoles;
                    totalProcess+=currentAmtProcesses;
                    isGood=1;
                    
                    Slot * newHole = create1('-',tempMem->next->size-toAdd->size,1,0,0);
                    Slot * newMemory=create1(toAdd->pid,toAdd->size,0,toAdd->numTimes,i);
                    totalLoads++;
                    if(tempMem->next->size!=toAdd->size)
                    {
                        newMemory->next=newHole;
                        newHole->next = tempMem->next->next;
                    }
                    else
                    {
                        free(newHole);
                        newMemory->next=tempMem->next->next;
                    }
                    
                    free(tempMem->next);
                    tempMem->next = newMemory;
                    memoryTaken+=newMemory->size;
                    totalMemory+=(memoryTaken*100);
                    /*PRINT OFF SOMETHING HERE*/
                    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n",
                    currentAmtProcesses,currentAmtHoles,(memoryTaken*100)/128,totalMemory/((i+1)*128));
                
                    goto out;
                }
                tempMem = tempMem->next;
            }

            /*EXTRACT LONGEST*/
            Slot * toExtract = memory;
            while(toExtract!=NULL)
            {
                
                if(toExtract->howOld==oldest&&!toExtract->isHole)
                {
                    
                    currentAmtProcesses--;
                    memoryTaken-=toExtract->size;
                    copy = push(copy,toExtract->pid,toExtract->size,toExtract->numTimes+1);
                    toExtract->isHole=1;
                    oldest++;
                    break;
                }
                toExtract=toExtract->next;
            }
        }while(!isGood);
        out:
        free(toAdd);

    }

    printf("Total loads = %d, average #processes = %.1f, average #holes = %.1f, cumulative %%mem = %d\n",
    totalLoads, ((float)(totalProcess)/(float)(i)),((float)(totalHoles)/(float)(i)),totalMemory/(i*128));
}

int countHoles(Slot * memory)
{
    Slot * temp = memory;
    int count =0;
    while(temp!=NULL)
    {
        if(temp->isHole)count++;
        temp = temp->next;
    }
    return count;
}


/*Merges adjacent holes together*/
Slot * merge(Slot * memory)
{
    Slot * mem = memory;
    if(mem->next == NULL)return mem;
    if(mem->next->next ==NULL)
    {
        if(memory->isHole&&memory->next->isHole)
        {
            free(memory->next);
            free(memory);
            return create1('-',128,1,0,0);
        }
        return mem;
    }

    while(memory->next->next != NULL)
    {
        if(memory->next->isHole&&memory->next->next->isHole)
        {
            Slot * newElement = create1('-',memory->next->size+memory->next->next->size,1,0,0);
            newElement->next = memory->next->next->next;
            free(memory->next->next);
            free(memory->next);
            memory->next = newElement;
            continue;
        }
        memory=memory->next;
    }

    if(mem->next->next ==NULL)
    {
        if(memory->isHole&&memory->next->isHole)
        {
            free(memory->next);
            free(memory);
            mem= create1('-',128,1,0,0);
        }
    }
    else
    {
        if(mem->isHole && mem->next->isHole)
        {
            mem->size+=mem->next->size;
            Slot * mem11=mem->next;
            mem->next = mem->next->next;
            free(mem11);
        }
    }

    return mem;
}

int keepLooping(Queue * process)
{
    return (process)!=NULL&&process->numTimes < 3;
}

Slot * create1(char pid, int size, int isHole, int numTimes,int howOld)
{
    Slot * s = malloc(sizeof(Slot));
    s->pid=pid;
    s->size=size;
    s->isHole=isHole;
    s->numTimes=numTimes;
    s->howOld=howOld;
    s->next=NULL;
    return s;
}

Queue * create(char pid, int size,int numTimes)
{
    Queue * newElement = malloc(sizeof(Queue));
    newElement->pid=pid;
    newElement->size=size;
    newElement->numTimes=numTimes;
    newElement->next=NULL;
    return newElement;
}

Queue * push(Queue * process,int pid, int size, int numTimes)
{
    Queue * temp = process;
    if(temp == NULL)
    {
        return create(pid,size,numTimes);
    }

    while(temp->next!=NULL)
    {
        temp = temp->next;
    }
    temp->next = create(pid,size,numTimes);
    
    return process;
}

Queue * pop(Queue ** process)
{
    Queue * toReturn = (*process);
    if(!toReturn)
        return NULL;
    
    (*process)=(*process)->next;

    return toReturn;
}