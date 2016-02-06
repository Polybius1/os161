/*
 * 
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
#include <spinlock.h>

static struct semaphore *lock_sem = NULL; //lock implemented as semaphore
static struct semaphore *tsem = NULL;		//semaphore for threads

/*void spinlock_init(struct spinlock *lk);
void spinlock_cleanup(struct spinlock *lk);

void spinlock_acquire(struct spinlock *lk);
void spinlock_release(struct spinlock *lk);

bool spinlock_do_i_hold(struct spinlock *lk);
*/

static struct spinlock *lock;
int spin_shared_counter = 0; //shared counter for threads to increment
int spin_counter_max = 100; //hard coded counter max

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

	for(int i = 0; i < spin_counter_max; i++) {
		spinlock_acquire(lock);
		spin_shared_counter++; //critical section
		spinlock_release(lock);

		
	}

	V(tsem);
}

int spinthread(int nargs, char **args) {
	
	spinlock_init(lock);
	
	spin_shared_counter = 0;	
	lock_sem_init();	
	tsem_init();
	char name[16];
	int i, result, numthreads;

	if(nargs > 1) {
		numthreads = atoi(args[1]);
	} else numthreads = 10;

	if(nargs > 2) 
		spin_counter_max = atoi(args[2]);
	else spin_counter_max = 100;

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
	
	kprintf("The shared counter is %d \n", spin_shared_counter);
	kprintf("The shared counter should be %d \n", spin_counter_max * numthreads);

	sem_destroy(lock_sem);
	sem_destroy(tsem);
	void spinlock_cleanup(struct spinlock *lk);
	return 0;
}




