// 16522063 zy22063 Zilin SONG
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "coursework.h"
#define MAX_BUFFER_SIZE  10
#define MAX_NUMBER_OF_JOBS 50

struct element e;
struct queue *me_arr;
sem_t p_sem, c_sem;
pthread_mutex_t mutex;
int P = 0;
int C = 0;
long int R = 0;
long int T = 0;
double arTime[MAX_NUMBER_OF_JOBS] = {0};
double atTime[MAX_NUMBER_OF_JOBS] = {0};
struct timeval start, end;

void *Producer()
{

	for (int i = 0; i < MAX_NUMBER_OF_JOBS; i++)
	{
		sem_wait(&p_sem);
		pthread_mutex_lock(&mutex);
		e = generateProcess();
	    if (me_arr -> count == me_arr -> max)
	    {
		    printf("The queue is full\n");
		    exit(1);
	    }
	    else
	    {
		    for (int i = 0; i <= me_arr -> count; i++)
		    {
			   if (i == me_arr -> count)
			    {
				   me_arr -> e[i] = e;
				   me_arr -> count++;
				   break;
			    }
			   else if (me_arr -> e[i].pid_time < e.pid_time)
			    {
				   addHere(me_arr, &e, i+1);
				   break;
			    }
		    }
	    }
	    P ++;
	    printf("P: job on buffer %d, job produced %d, job consumed %d\n",me_arr -> count, P, C);
	    pthread_mutex_unlock(&mutex);
		sem_post(&c_sem);
	}

}


void *Consumer()
{
	for (int i = 0; i < MAX_NUMBER_OF_JOBS; i++)
	{
		sem_wait(&c_sem);   
	    pthread_mutex_lock(&mutex);
	    printf("Product on index: %d, ", me_arr -> count -1);
	    gettimeofday(&start, NULL);
	    runNonPreemptiveJobv2(&me_arr ->e[me_arr -> count -1]);
	    gettimeofday(&end, NULL);
	    R = getDifferenceInMilliSeconds(me_arr -> e[me_arr -> count -1].created_time, start);
	    T = getDifferenceInMilliSeconds(me_arr -> e[me_arr -> count -1].created_time, end);
	    arTime[i] = R;
	    arTime[MAX_NUMBER_OF_JOBS] = arTime[MAX_NUMBER_OF_JOBS] + R;
	    atTime[i] = T;
	    atTime[MAX_NUMBER_OF_JOBS] = arTime[MAX_NUMBER_OF_JOBS] + T;
	    removeLast(me_arr);
	    C++;
	    printf("C: job on buffer %d, job produced %d, job consumed %d\n",me_arr -> count, P, C);
	    pthread_mutex_unlock(&mutex);
	    sem_post(&p_sem);      
	}
}


void average()
{
	printf("Average response time: %.2lf milliseconds\n", arTime[MAX_NUMBER_OF_JOBS]/MAX_NUMBER_OF_JOBS);
	printf("Average turn around time: %.2lf milliseconds\n", atTime[MAX_NUMBER_OF_JOBS]/MAX_NUMBER_OF_JOBS);
}


int main(int argc, char const *argv[])
{
	int err;
	printf("Max buffe: %d\nMax number of jobs: %d\n", MAX_BUFFER_SIZE, MAX_NUMBER_OF_JOBS);
	pthread_t tid1,tid2;
	sem_init(&p_sem,0,MAX_BUFFER_SIZE);
	sem_init(&c_sem,0,0);
	pthread_mutex_init(&mutex, NULL);
	me_arr = (struct queue*)malloc(sizeof(struct queue));
	init(me_arr, MAX_BUFFER_SIZE);
	err = pthread_create(&tid1,NULL,Producer,NULL);
	if (err != 0)
	{
		printf("Create Producer failed\n");
		exit(1);
	}
    err = pthread_create(&tid2,NULL,Consumer,NULL);
	if (err != 0)
	{
		printf("Create Consumer failed\n");
		exit(1);
	}
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    sem_destroy(&p_sem);
	sem_destroy(&c_sem);
	pthread_mutex_destroy(&mutex);
    freeAll(me_arr);
    average();
}