/*
 * 
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static struct lock *thelock = NULL; 
static struct semaphore *tsem = NULL;		//semaphore for threads

int shared_counterL = 0; //shared counter for threads to increment
int counter_maxL = 100; //hard coded counter max

//lock methods for reference
//struct lock *lock_create(const char *name);
//void lock_acquire(struct lock *);

/*
 * Operations:
 *    lock_acquire - Get the lock. Only one thread can hold the lock at the
 *                   same time.
 *    lock_release - Free the lock. Only the thread holding the lock may do
 *                   this.
 *    lock_do_i_hold - Return true if the current thread holds the lock; 
 *                   false otherwise.
 *
 * These operations must be atomic. You get to write them.
 */
//void lock_release(struct lock *);
//bool lock_do_i_hold(struct lock *);
//void lock_destroy(struct lock *);


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

static void lock_init(void) {
	if(thelock == NULL) {
		thelock = lock_create("thelock");

		if(thelock == NULL) {
			panic("locktest: lock initialization failed\n");
		}
	}
}


static void mattthread(void *junk, unsigned long num) {
	
	(void)junk;
	(void)num;

	for(int i = 0; i < counter_maxL; i++) {
		lock_acquire(thelock);
		shared_counterL++; //critical section
		lock_release(thelock);

		
	}

	V(tsem);
}

int lockthread(int nargs, char **args) {
		
	shared_counterL = 0;	
	lock_init();
	tsem_init();
	char name[16];
	int i, result, numthreads;

	if(nargs > 1) {
		numthreads = atoi(args[1]);
	} else numthreads = 10;

	if(nargs > 2) 
		counter_maxL = atoi(args[2]);
	else counter_maxL = 100;

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
	
	kprintf("The shared counter is %d \n", shared_counterL);
	kprintf("The shared counter should be %d \n", counter_maxL * numthreads);

	//lock_destroy(thelock);
	//sem_destroy(tsem);

	return 0;
}




