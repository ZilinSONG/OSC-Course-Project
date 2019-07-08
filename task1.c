// 16522063 zy22063 Zilin SONG
#include <stdio.h>
#include "coursework.h"
#define MAX_PROCESSES 5

int main(int argc, char const *argv[])
{
	struct queue *me_arr;
	me_arr = (struct queue*)malloc(sizeof(struct queue));
	init(me_arr, MAX_PROCESSES);
	printAll(me_arr);
	printf("Performing FIFO ... \n");
	struct element e;
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
    	e = generateProcess();
    	addFirst(me_arr, &e);
        printf("Add: pid %d into the queue with time %d\n",e.pid, e.pid_time);

    }
    printAll(me_arr);
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        printf("Remove: %d from the queue index %d\n",me_arr -> e[me_arr -> count - 1].pid, me_arr -> count - 1);
    	removeLast(me_arr);
    	printAll(me_arr);
    }
    printf("Performing LIFO ...\n");
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
    	e = generateProcess();
    	addLast(me_arr, &e);
        printf("Add: pid %d into the queue with time %d\n", e.pid, e.pid_time);
    }
    printAll(me_arr);
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        printf("Remove: %d from the queue index %d\n",me_arr -> e[me_arr -> count - 1].pid, me_arr -> count - 1);
    	removeLast(me_arr);
    	printAll(me_arr);
    }
    freeAll(me_arr);
}
