/*Matthew Schroeder
 * inserted this on 10/22/15
 * for OS Lab 4
 */

#include <types.h>
#include <lib.h>
#include <test.h>

int lab4test(int argc, char *argv[]) {

	int i = 0;
	kprintf("\ncmdline args count=%d", argc);

	/* First argument is executable name only */
	kprintf("\nexe name=%s", argv[0]);

	for (i=1; i< argc; i++) {
		kprintf("\narg%d=%s", i, argv[i]);
	}

	kprintf("\n"); 
	return 0;
}
