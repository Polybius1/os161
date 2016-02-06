/*
 * Thread test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
//#include <stdlib.h>

//#define NTHREADS  8

static struct semaphore *tsem = NULL;

static
void
fun_sem(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("threadtest: sem_create failed\n");
		}
	}
}

static void mattthread(void *junk, unsigned long num) {
	
	(void)junk;
	kprintf("%lu ", num);
	V(tsem);

}

int lab5threadtest(int nargs, char **args) {
	
	//(void)nargs;
	//(void)args;
	
	fun_sem();	
	char name[16];
	int i, result, numthreads;

	if(nargs > 1) {
		numthreads = atoi(args[1]);
	} else numthreads = 10;

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

	return 0;
}




