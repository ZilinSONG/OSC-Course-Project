// 16522063 zy22063 Zilin SONG
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct element
{
	int pid;
	int pid_time;
	int pid_priority;

	struct timeval created_time;
};

struct queue
{
	struct element *e;
	int max;
	int count;
};

int addFirst(struct queue *my_arr, struct element *new_e)
{
	if (my_arr -> max == my_arr -> count)
	{
		printf("The queue is full.\n");
		return 1;
	}
	my_arr -> count = my_arr -> count + 1;
	for (int i = my_arr -> count-1; i >=0; i--)
	{
		if (i == 0)
		{
			my_arr -> e[0] = *new_e;
		}
		else
		{
		    my_arr -> e[i] = my_arr -> e[i-1];
		}
	}
	return 0;
}


int addHere(struct queue *my_arr, struct element *new_e, int index)
{
	if (my_arr -> max == my_arr -> count)
	{
		printf("The queue is full.\n");
		return 1;
	}
    my_arr -> count = my_arr -> count + 1;
	for (int i = my_arr -> count-1; i >=0; i--)
	{
		if (my_arr -> count == 1)
        {
    	    my_arr -> e[0] = *new_e;
        }
		else if (i == index-1)
		{
			my_arr -> e[i] = *new_e;
			break;
		}
		else
		{
		    my_arr -> e[i] = my_arr -> e[i-1];
		}
	}
	return 0;
}


int addLast(struct queue *my_arr, struct element *new_e)
{
	if (my_arr -> max == my_arr -> count)
	{
		printf("The queue is full.\n");
		return 1;
	}
	my_arr -> count = my_arr -> count + 1;
	my_arr -> e[my_arr -> count - 1] = *new_e;
	return 0;
}


void freeAll(struct queue *my_arr)
{
	free(my_arr -> e);
	my_arr -> e = NULL;
	free(my_arr);
    my_arr = NULL;
}

int getCount(struct queue *my_arr)
{
	return my_arr -> count;
}


int init(struct queue *my_arr, int arr_size)
{
	printf("Initialising ...\n");
	if (my_arr == NULL)
	{
		printf("Failed to allocate memory to queue\n");
		return 1;
	}
	my_arr -> count = 0;
	my_arr -> max = arr_size;
	my_arr -> e = (struct element*)malloc(sizeof(struct element)*arr_size);
	if (my_arr -> e == NULL)
	{
		printf("Failed to allocate memory to element\n");
		free(my_arr);
		return 1;
	}
	for (int i = 0; i < arr_size; i++)
	{
		my_arr -> e[i].pid = 0;
		my_arr -> e[i].pid_time = 0;
		my_arr -> e[i].pid_priority = 0;
		my_arr -> e[i].created_time.tv_sec = 0;
		my_arr -> e[i].created_time.tv_usec = 0;
	}
	printf("Init: successfully malloc element with size of %d ...\n", arr_size);
	return 0;
}


void printAll(struct queue *my_arr)
{
	printf("There are %d elements in total\n", my_arr -> count);
	for (int i = 0; i < my_arr -> max; i++)
	{
		printf("#[%d]: %d running-time %d created-time %ld sec %d usec %d priority\n", i, my_arr -> e[i].pid, my_arr -> e[i].pid_time, my_arr -> e[i].created_time.tv_sec, my_arr -> e[i].created_time.tv_usec, my_arr -> e[i].pid_priority  );
	}

}


void removeLast(struct queue *my_arr)
{
	if (my_arr -> count == 0)
	{
		printf("The queue is empty.\n");
	}
	my_arr -> count = my_arr -> count -1;
	my_arr -> e[my_arr -> count].pid = 0;
	my_arr -> e[my_arr -> count].pid_time = 0;
	my_arr -> e[my_arr -> count].pid_priority = 0;
	my_arr -> e[my_arr -> count].created_time.tv_sec = 0;
	my_arr -> e[my_arr -> count].created_time.tv_usec = 0;
}