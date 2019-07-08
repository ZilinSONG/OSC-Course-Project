// 16522063 zy22063 Zilin SONG
#include <stdio.h>
#include "coursework.h"
#define MAX_PROCESSES 5

void generateSJF(struct queue *my_arr, struct element *new_e)
{
	if (my_arr -> count == my_arr -> max)
	{
		printf("The queue is full\n");
		exit(1);
	}
	else
	{
		for (int i = 0; i <= my_arr -> count; i++)
		{
			if (my_arr -> count == 0)
			{
				printf("SJF: add first\n");
				my_arr -> e[i] = *new_e;
				my_arr -> count++;
				break;
			}
			else if (my_arr -> e[i].pid_time <= new_e -> pid_time)
			{
				printf("add new element\n");
				addHere(my_arr, new_e, i+1);
				break;
			}
		}
	}

}

double arTime[MAX_PROCESSES] = {0};
double atTime[MAX_PROCESSES] = {0};
void runSJF(struct queue *my_arr)
{
  printf("Running the processes using SJF ...\n");
  struct timeval start, end;
  for (int i = MAX_PROCESSES - 1; i >= 0; i--)
  {
  	gettimeofday(&start, NULL);
  	runNonPreemptiveJob(my_arr, i);
  	gettimeofday(&end, NULL);
    long int R = getDifferenceInMilliSeconds(my_arr -> e[i].created_time, start);
    long int T = getDifferenceInMilliSeconds(my_arr -> e[i].created_time, end);
    printf("C : %d, S: %d, E: %d, R: %d, T: %d\n", my_arr -> e[i].created_time.tv_sec, start.tv_sec, end.tv_sec, R, T);
    arTime[i] = R;
    arTime[MAX_PROCESSES] = arTime[MAX_PROCESSES] + R;
    atTime[i] = T;
    atTime[MAX_PROCESSES] = atTime[MAX_PROCESSES] + T;
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
	me_arr = (struct queue*)malloc(sizeof(struct queue));
	init(me_arr, MAX_PROCESSES);
	printAll(me_arr);
	printf("Generating processes for SJF ...\n");
	struct element e;
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		e = generateProcess();
		generateSJF(me_arr, &e);
		printAll(me_arr);
	}
	runSJF(me_arr);
	average();
	freeAll(me_arr);
}