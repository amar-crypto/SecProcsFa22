
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <math.h>

// TODO: define your own buffer size
#define BUFF_SIZE (1<< 21)
//#define BUFF_SIZE [TODO]



int main(int argc, char **argv)
{
  // Allocate a buffer using huge page
 // // See the handout for details about hugepage management
 void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  
  if (buf == (void*) - 1) {
     perror("mmap() error\n");
     exit(EXIT_FAILURE);
  }

 uint64_t *sender_buffer = (uint64_t *)malloc(4096*8*sizeof(uint64_t));
 // // The first access to a page triggers overhead associated with
  // page allocation, TLB insertion, etc.
  // Thus, we use a dummy write here to trigger page allocation
  // so later access will not suffer from such overhead.
  //*((char *)buf) = 1; // dummy write to trigger page allocation


 volatile char tmp;
  // TODO:
  // Put your covert channel setup code here

  printf("Please type a message.\n");
  bool sending = true;
  while (sending) {
      char text_buf[128];
      fgets(text_buf, sizeof(text_buf), stdin);
      printf("sender sending now"); 
      int index = string_to_int(text_buf);
       

      for (int i=0; i<1000; i++ )  
      tmp = sender_buffer[index*8];
      // TODO:
      // Put your covert channel code here
  }

  printf("Sender finished.\n");
  return 0;
}


