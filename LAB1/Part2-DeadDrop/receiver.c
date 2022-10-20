
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define SAMPLES 10

int main(int argc, char **argv)
{
        uint64_t l2_latency[4096] = {0};
	volatile char tmp;
        uint64_t *prime_buffer = (uint64_t *)malloc(4096*8*sizeof(uint64_t));
	// Put your covert channel setup code here

       for (int j = 0; j < 4096; j++) {
        tmp = prime_buffer[j*8];
       }
       printf("receiver prime is done\n");
	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	bool listening = true;
	while (listening) {
       for (int j = 0; j < 4096; j++) {
       
        l2_latency[j] = measure_one_block_access_time((uint64_t)&prime_buffer[j*8]);
	if (l2_latency[j] > 200)
	printf("L2 latency for index %d is %ld\n", j, l2_latency[j]);
       
   }
		

		// Put your covert channel code here

	}

	printf("Receiver finished.\n");

	return 0;
}


