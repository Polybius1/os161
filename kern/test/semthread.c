/*
 * 
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static struct semaphore *lock_sem = NULL; //lock implemented as semaphore
static struct semaphore *tsem = NULL;		//semaphore for threads

int shared_counterS = 0; //shared counter for threads to increment
int counter_maxS = 100; //hard coded counter max

static
void
lock_sem_init(void)
{
	if (lock_sem==NULL) {
		lock_sem = sem_create("lock_sem", 1); //lock is semaphore with initial value at 1
		if (lock_sem == NULL) {
			panic("threadtest: sem_create failed\n");
		}
	}
}

static
void
tsem_init(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0); 
		if (tsem == NULL) {
			panic("threadtest: tsem failed\n");
		}
	}
}

static void mattthread(void *junk, unsigned long num) {
	
	(void)junk;
	(void)num;

	for(int i = 0; i < counter_maxS; i++) {
		P(lock_sem);
		shared_counterS++; //critical section
		V(lock_sem);

		
	}

	V(tsem);
}

int semthread(int nargs, char **args) {
		
	shared_counterS = 0;	
	lock_sem_init();	
	tsem_init();
	char name[16];
	int i, result, numthreads;

	if(nargs > 1) {
		numthreads = atoi(args[1]);
	} else numthreads = 10;

	if(nargs > 2) 
		counter_maxS = atoi(args[2]);
	else counter_maxS = 100;

	for (i=0; i<numthreads; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL, mattthread, NULL, i);

		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}

	for (i=0; i<numthreads; i++) {
		P(tsem);
	}
	
	kprintf("The shared counter is %d \n", shared_counterS);
	kprintf("The shared counter should be %d \n", counter_maxS * numthreads);

	return 0;
}




