#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../src/siphash.h"

void test(uint64_t expected, char key[16], char input[], int c, int d) {
		SipHash sipHash(key, c, d);

		sipHash.update(input);

		assert(expected == sipHash.finish());
}

int main () {
		test(4402678656023170274, (char *) "0123456789ABCDEF", (char *) "hello", 2, 4);			/* test default hash */
		test(4402678656023170274, (char *) "0123456789ABCDEFF", (char *) "hello", 2, 4); 		/* extra byte ignored */
		test(14986662229302055855UL, (char *) "0123456789ABCDEF", (char *) "hello", 4, 8);	/* check compression changes */
		return 0;
}
