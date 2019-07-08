// 16522063 zy22063 Zilin SONG
#include <stdio.h>
#include "coursework.h"
#define MAX_PROCESSES 5


void generatePQ(struct queue *my_arr, struct element *new_e)
{

	printf("New process has priority %d\n", new_e -> pid_priority);
	if (new_e-> pid_priority == 0)
	{
		printf("PQ 0: add new\nQ ");
		addFirst(&my_arr[0], new_e);
	}
	else if (new_e-> pid_priority == 1)
	{
		printf("PQ 1: add new\nQ ");
		addFirst(&my_arr[1], new_e);
	}
	else
	{
		printf("PQ 2: add new\nQ ");
		addFirst(&my_arr[2], new_e);
	}
	for (int p = 0; p < 3; p++)
    {
    	printf("Q #%d\n", p);
		printAll(&my_arr[p]);
	}
}


double arTime[MAX_PROCESSES] = {0};
double atTime[MAX_PROCESSES] = {0};


void runPQ(struct queue *my_arr)
{
	struct element temp_e;
	printf("Running the processes using PQ ...\n");
	for (int i = 0; i < 3; i++)
	{   
		int c = 0;
		while(my_arr[i].e[0].pid_time != 0)
		{   
			for (int b = my_arr[i].count - 1; b >= 0; b--)
			{
				struct timeval start[my_arr[i].count - 1];
				struct timeval end[my_arr[i].count - 1];
                long int R[my_arr[i].count - 1],T[my_arr[i].count - 1];
                temp_e = my_arr[i].e[my_arr[i].count -1];
                removeLast(&my_arr[i]);
				if (c==0)
				{
					gettimeofday(&start[b], NULL);
			        
			        R[b] = getDifferenceInMilliSeconds(temp_e.created_time, start[b]);
			        printf("Q: %d P: %d C: %d S: %d R: %d\n", i, temp_e.pid, temp_e.created_time.tv_sec, start[b].tv_sec, R[b]);
			        arTime[temp_e.pid] = R[b];
			        arTime[MAX_PROCESSES] = R[b] + arTime[MAX_PROCESSES];                 
				}
				runPreemptiveJobv2(&temp_e);
				if (temp_e.pid_time == 0)
				{
					gettimeofday(&end[b], NULL);
                    T[b] = getDifferenceInMilliSeconds(temp_e.created_time, end[b]);
                    printf("Q: %d P: %d C: %d E: %d T: %d\n", i, temp_e.pid, temp_e.created_time.tv_sec, end[b].tv_sec, T[b]);
                    atTime[temp_e.pid] = T[b];
                    atTime[MAX_PROCESSES] = T[b] + atTime[MAX_PROCESSES];
				}
				else
				{
					addFirst(&my_arr[i], &temp_e);
				}	
			}
			c++;
		}
	}
}



void average()
{

	printf("Average response time: %.2lf milliseconds\n", arTime[MAX_PROCESSES]/MAX_PROCESSES);
	printf("Average turn around time: %.2lf milliseconds\n", atTime[MAX_PROCESSES]/MAX_PROCESSES);
}



int main(int argc, char const *argv[])
{
    struct queue *me_arr;
    struct element e;
	me_arr = (struct queue*)malloc(sizeof(struct queue)*3);
	for (int i = 0; i < 3; i++)
	{
		init(&me_arr[i], MAX_PROCESSES);
		printf("Q #%d\n", i);
		printAll(&me_arr[i]);
	}
	printf("Generating processes for PQ ...\n");
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		e = generateProcess();
		generatePQ(me_arr, &e);
	}
	runPQ(me_arr);
    average();
    freeAll(me_arr);

}
