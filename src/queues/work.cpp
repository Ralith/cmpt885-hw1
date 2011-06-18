#include "work.h"

void workKernel(int n) {
	__asm__ __volatile__ (
                "movl %0, %%eax;"
                "loopA:"
                "dec %%eax;"
                "jnz loopA;"
                : /* no outputs */
				: "g"(n)
				: "eax"
        );

}



