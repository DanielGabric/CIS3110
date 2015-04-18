/*
Name: Daniel Gabric
ID: ******
Assignment #3 PART 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <string.h>
#include <unistd.h>



#define FOR(i,j) for(i=0;i<j;++i) 

typedef struct philData 
{
    pthread_mutex_t *right;
    pthread_mutex_t *left;
    pthread_t thread;
    int num;
    int fail;
    int numTimesToEat;
} Philosopher;
void *simulation(void *p) 
{
    Philosopher *phil = (Philosopher*)p;
    int failed;
    int tries_left;
    pthread_mutex_t *left, *right, *temp;
    int i;
    for(i=0;i<phil->numTimesToEat;++i)
    {
        printf("Philosopher %d is thinking\n", phil->num+1);
        sleep( 1+ rand()%8);
 
        left = phil->left;
        right = phil->right;
        printf("Philosopher %d is hungry\n", phil->num+1);
        tries_left = 2;
        do 
        {
            failed = pthread_mutex_lock( left);
            if(tries_left>0)failed = pthread_mutex_trylock(right);
            else failed = pthread_mutex_lock(right);

            if (failed) 
            {
                pthread_mutex_unlock(left);
                temp = left;
                left = right;
                right = temp;
                tries_left--;
            }
        } while(failed);
 
        if (!failed) 
        {
            printf("Philosopher %d is eating\n", phil->num+1);
            if(i==phil->numTimesToEat-1)
                printf("Philosopher %d is done eating.\n",phil->num+1);
            sleep( 1+ rand() % 8);
            pthread_mutex_unlock( right);
            pthread_mutex_unlock( left);
        }
    }
    return NULL;
}
 

int main(int argc, char * argv[])
{
    if(argc!=3)
    {
        printf("Error you need arguments to run this program\n");
        return -1;
    }
    sem_t mutual_exclusion;

    int numPhil = atoi(argv[1]);
    int numTimesToEat = atoi(argv[2]);
    int total = 0;
    int wait_status;
    if(numPhil<=2 || (numTimesToEat <1||numTimesToEat>1000))
    {
        printf("Error you need more than 2 philosophers and they need to eat 1-1000 times\n");
        return -2;
    }


    pthread_mutex_t forks[numPhil];
    Philosopher philosophers[numPhil];
    Philosopher *phil;
    int i;
 
    for (i=0;i<numPhil; i++)
        pthread_mutex_init(&forks[i], NULL);
 
    for (i=0;i<numPhil; i++) 
    {
        phil = &philosophers[i];
        phil->left = &forks[i];
        phil->right = &forks[(i+1)%numPhil];
        phil->num=i;
        phil->numTimesToEat=numTimesToEat;
        phil->fail = pthread_create( &phil->thread, NULL, simulation, phil);
    }
 
    sleep(1);
 
    for(i=0; i<numPhil; i++) 
    {
        phil = &philosophers[i];
        pthread_join( phil->thread, NULL);
    }
    return 0;
}
    