/*
 * Thread test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

//static struct semaphore *tsem = NULL;

int shared_counter = 0; //shared counter for threads to increment
int counter_max = 100; //hard coded counter max

/*static
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
*/
static void mattthread(void *junk, unsigned long num) {
	
	(void)junk;
	(void)num;

	for(int i = 0; i < counter_max; i++) 
		shared_counter++;

	//kprintf("%d \n", shared_counter);
	//V(tsem);

}

int unsafetest(int nargs, char **args) {
		
	//fun_sem();	
	char name[16];
	int i, result, numthreads;

	if(nargs > 1) {
		numthreads = atoi(args[1]);
	} else numthreads = 10;

	if(nargs > 2) 
		counter_max = atoi(args[2]);
	

	for (i=0; i<numthreads; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL, mattthread, NULL, i);

		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}

	/*for (i=0; i<numthreads; i++) {
		P(tsem);
	}*/
	
	kprintf("The shared counter is %d \n", shared_counter);
	kprintf("The shared counter should be %d \n", counter_max * numthreads);

	return 0;
}




