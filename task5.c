// 16522063 zy22063 Zilin SONG
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "coursework.h"
#define MAX_BUFFER_SIZE 10
#define MAX_NUMBER_OF_JOBS 50

struct queue *me_arr;
struct element e;
sem_t p_sem, c_sem;
pthread_mutex_t mutex;
int P, C;
long int R = 0;
long int T = 0;
int S[MAX_NUMBER_OF_JOBS];
struct timeval start[MAX_NUMBER_OF_JOBS], end[MAX_NUMBER_OF_JOBS];
double arTime[MAX_NUMBER_OF_JOBS] = {0};
int check = 1;                                                             

void *Producer()
{
	for (int i = 0; i < MAX_NUMBER_OF_JOBS; i++)
	{
		sem_wait(&p_sem);
		pthread_mutex_lock(&mutex);
		while(1)
		{
			if (check == 1)
			{
				e = generateProcess();
			}
		    if (e.pid_priority == 0 && me_arr[0].count != me_arr[0].max)
		    {
		    	check = 1;
			    addFirst(&me_arr[0], &e);
			    pthread_mutex_unlock(&mutex);
			    sem_post(&c_sem);
			    break;
		    }
		    else if (e.pid_priority == 1 && me_arr[1].count != me_arr[1].max)
		    {
		    	check = 1;
			    addFirst(&me_arr[1], &e);
			    pthread_mutex_unlock(&mutex);
			    sem_post(&c_sem);
			    break;
		    }
		    else if(e.pid_priority == 2 && me_arr[2].count != me_arr[2].max)
		    {
		    	check = 1;
			    addFirst(&me_arr[2], &e);
			    pthread_mutex_unlock(&mutex);
			    sem_post(&c_sem);
			    break;
		    }
		    else
		    {
		    	check = 0;                                  // fail to add element
		    	pthread_mutex_unlock(&mutex);
		    	break;
		    }
		}
		if (check == 1)              // When buffer add element successful
		{
		P ++;
	    printf("Producer : job on buffer1 %d, job on buffer2 %d, job on buffer3 %d, job produced %d, job consumed %d\n", me_arr[0].count, me_arr[1].count, me_arr[2].count, P, C);
		}
	}
}

void *Consumer(void *arg)
{
	int CID = *(int *)arg;
	while(1)
	{
		sem_wait(&c_sem);
	    pthread_mutex_lock(&mutex);
	    if (me_arr[0].count != 0)
	    {
	    	struct element e0;
	    	e0 = me_arr[0].e[me_arr[0].count - 1];
	    	printf("Product on index: %d, ", me_arr[0].count -1);
	    	if (arTime[e0.pid] == 0)
	    	{
	    		arTime[e0.pid] = 1;
	    		gettimeofday(&start[e0.pid], NULL);
	    	}
	    	removeLast(&me_arr[0]);
	    	runPreemptiveJobv2(&e0);
	    	pthread_mutex_unlock(&mutex);
	    	if (e0.pid_time > 0)
	    	{
                addFirst(&me_arr[0], &e0);                     // add to the queue again
                sem_post(&c_sem);
	    	}
	    	else
	    	{
	    		C++;
	    		printf("Consumer %d: job on buffer1 %d, job on buffer2 %d, job on buffer3 %d, job produced %d, job consumed %d\n", CID, me_arr[0].count, me_arr[1].count, me_arr[2].count, P, C);
	    		gettimeofday(&end[e0.pid], NULL);
	    		R = getDifferenceInMilliSeconds(e0.created_time, start[e0.pid]) + R;
	    		T = getDifferenceInMilliSeconds(e0.created_time, end[e0.pid]) + T;
	    		if (C == MAX_NUMBER_OF_JOBS -1 || C == MAX_NUMBER_OF_JOBS -2 || C == MAX_NUMBER_OF_JOBS)
	    		{
	    			return 0;
	    		}
	    		sem_post(&p_sem);
	    	}
	    }
	    else if (me_arr[1].count != 0)
	    {
	    	struct element e1;
	    	e1 = me_arr[1].e[me_arr[1].count - 1];
	    	printf("Product on index: %d, ", me_arr[1].count -1);
	    	removeLast(&me_arr[1]);
	    	if (arTime[e1.pid] == 0)
	    	{
	    		arTime[e1.pid] = 1;
	    		gettimeofday(&start[e1.pid], NULL);
	    	}
	    	runPreemptiveJobv2(&e1);
	    	pthread_mutex_unlock(&mutex);
	    	if (e1.pid_time > 0)
	    	{
                addFirst(&me_arr[1], &e1);
                sem_post(&c_sem);
	    	}
	    	else
	    	{
	    		C++;
	    		printf("Consumer %d: job on buffer1 %d, job on buffer2 %d, job on buffer3 %d, job produced %d, job consumed %d\n", CID, me_arr[0].count, me_arr[1].count, me_arr[2].count, P, C);
	    		gettimeofday(&end[e1.pid], NULL);
	    		R = getDifferenceInMilliSeconds(e1.created_time, start[e1.pid]) + R;
	    		T = getDifferenceInMilliSeconds(e1.created_time, end[e1.pid]) + T;
	    		if (C == MAX_NUMBER_OF_JOBS -1 || C == MAX_NUMBER_OF_JOBS -2 || C == MAX_NUMBER_OF_JOBS)
	    		{
	    			return 0;
	    		}
	    		sem_post(&p_sem);
	    	}
	    }
	    else if (me_arr[2].count != 0)
	    {
	    	struct element e2;
	    	e2 = me_arr[2].e[me_arr[2].count - 1];
	    	printf("Product on index: %d, ", me_arr[2].count -1);
	    	removeLast(&me_arr[2]);
	    	if (arTime[e2.pid] == 0)
	    	{
	    		arTime[e2.pid] = 1;
	    		gettimeofday(&start[e2.pid], NULL);
	    	}
	    	runPreemptiveJobv2(&e2);
	    	pthread_mutex_unlock(&mutex);
	    	if (e2.pid_time > 0)
	    	{
                addFirst(&me_arr[2], &e2);
                sem_post(&c_sem);
	    	}
	    	else
	    	{
	    		C++;
	    		printf("Consumer %d: job on buffer1 %d, job on buffer2 %d, job on buffer3 %d, job produced %d, job consumed %d\n", CID, me_arr[0].count, me_arr[1].count, me_arr[2].count, P, C);
	    		gettimeofday(&end[e2.pid], NULL);
	    		R = getDifferenceInMilliSeconds(e2.created_time, start[e2.pid]) + R;
	    		T = getDifferenceInMilliSeconds(e2.created_time, end[e2.pid]) + T;
	    		if (C == MAX_NUMBER_OF_JOBS -1 || C == MAX_NUMBER_OF_JOBS -2 || C == MAX_NUMBER_OF_JOBS)
	    		{
	    			return 0;
	    		}
	    		sem_post(&p_sem);
	    	}
	    } 
	}
}

void average()
{
	double AR = R/MAX_NUMBER_OF_JOBS;
    double AT = T/MAX_NUMBER_OF_JOBS;

	printf("Average response time: %.2lf milliseconds\n", AR);
	printf("Average turn around time: %.2lf milliseconds\n", AT);
}

int main(int argc, char const *argv[])
{
	int err;                                // to check if the thread was successfully created
	P = C = 0;
	pthread_t p, c[3];
	printf("Producer: 1\nConsumer: 3\nMAX_BUFFER_SIZE: %d\nMAX_NUMBER_OF_JOBS: %d\n", MAX_BUFFER_SIZE, MAX_NUMBER_OF_JOBS);
	me_arr = (struct queue*)malloc(sizeof(struct queue)*3);
	for (int i = 0; i < 3; i++)
	{
		init(&me_arr[i], MAX_BUFFER_SIZE);
	}
	sem_init(&p_sem,0,MAX_BUFFER_SIZE*3);
	pthread_mutex_init(&mutex, NULL);
	sem_init(&c_sem,0,0);
	err = pthread_create(&p,NULL,Producer,NULL);
	if (err != 0)                                         
	{
		printf("Create Producer failed\n");
		exit(1);
	}
	for (int i = 0; i < 3; i++)
	{
		err = pthread_create(&c[i],NULL,Consumer,&i);
		if (err != 0)
	{
		printf("Create Consumer %d failed\n", i);
		exit(1);
	}
	}
	pthread_join(p,NULL);
    for (int i = 0; i < 3; i++)
    {
    	 pthread_join(c[i],NULL);
    }
    sem_destroy(&p_sem);
    sem_destroy(&c_sem);
	pthread_mutex_destroy(&mutex);
	average();
}