/*
 * Thread test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS  8

static struct semaphore *tsem = NULL;

static
void
string_sem(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("threadtest: sem_create failed\n");
		}
	}
}

static void stringthread(void *junk, unsigned long num) {
	
	(void)junk;
	(void)num;
	kprintf("1234 ");
	V(tsem);

}

int stringthreadtest(int nargs, char **args) {
	
	(void)nargs;
	(void)args;
	string_sem();	
	char name[16];
	int i, result;

	for (i=0; i<10; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL, stringthread, NULL, i);

		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}

	for (i=0; i<10; i++) {
		P(tsem);
	}

	return 0;
}




