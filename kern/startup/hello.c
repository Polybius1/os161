/* hello.c
 * for making os161 say hello!
 * Matthew Schroeder
 * 10/14/15
 * SOS
 * OS
 * Lab3
 */

#include <types.h>
#include <lib.h>
#include <test.h>

int sayhello(void) {
	kprintf("Hello Asshole\n");
	return 0;	
}
