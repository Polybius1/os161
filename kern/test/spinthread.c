#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
#include <spinlock.h>

static struct semaphore *tsem = NULL;		//semaphore for threads

//spinlock methods for reference
/*void spinlock_init(struct spinlock *lk);
void spinlock_cleanup(struct spinlock *lk);

void spinlock_acquire(struct spinlock *lk);
void spinlock_release(struct spinlock *lk);

bool spinlock_do_i_hold(struct spinlock *lk);
*/
static struct spinlock the_lock;
int spin_shared_counter = 0; //shared counter for threads to increment
int spin_counter_max = 100; //hard coded counter max


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

/*static void the_lock_init(void) {
		if(the_lock == NULL) {
		the_lock = spinlock_init(&the_lock);

		if(the_lock == NULL) {
			panic("spinlock test: spinlock_init failed\n");
		}
	}
}*/

static void mattthread(void *junk, unsigned long num) {
	
	(void)junk;
	(void)num;
	
	for(int i = 0; i < spin_counter_max; i++) {
	
		spinlock_acquire(&the_lock);
		spin_shared_counter++; //critical section
		spinlock_release(&the_lock);	

	}

	V(tsem);
}

int spinthread(int nargs, char **args) {
	
	spinlock_init(&the_lock);					//initialize spinlock
	spin_shared_counter = 0;					//initialize shared counter
		
	tsem_init();									//initialize thread semaphore
	char name[16];
	int i, result, numthreads;

	if(nargs > 1) {								//if we have more than one arg
		numthreads = atoi(args[1]);			//first arg is number of threads to spawn
	} else numthreads = 10;

	if(nargs > 2) 
		spin_counter_max = atoi(args[2]);	//second arg is shared counter max
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

	//sem_destroy(tsem);
	//spinlock_cleanup(&the_lock);
	
	return 0;
}




