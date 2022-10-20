
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <math.h>
#include <time.h>

// TODO: define your own buffer size
#define BUFF_SIZE (1<< 21)
//#define BUFF_SIZE [TODO]

 uint64_t get_cache_set_index(ADDR_PTR phys_addr)
  {
	    uint64_t mask = ((uint64_t) 1 << 15) - 1;
	        return (phys_addr & mask) >> 6;
  }


int main(int argc, char **argv)
{
  // Allocate a buffer using huge page
 // // See the handout for details about hugepage management
 void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  
  if (buf == (void*) - 1) {
     perror("mmap() error\n");
     exit(EXIT_FAILURE);
  }

 int set_id[8] = {0, 1, 2, 3, 4, 5, 6, 7};

 uint64_t eviction_set[8][8];

 volatile char tmp; 

 int n = 8;

 int o = 6;                          // log_2(64), where 64 is the line size
 int s = 14;                         // log_2(16384), where 16384 is the number of cache sets
 int two_o = 64;             // 64
 int two_o_s = 512;   // 1048576
 int b = 262144;

 char *buffer = malloc((size_t) b);

 uint64_t addr;
 clock_t start_time, current_time;

 for (int set_id_index = 0; set_id_index < 8; set_id_index++) {
     int address_count = 0;
     for (int set_index = 0; set_index < 512; set_index++) {
       for (int way_index = 0; way_index < 8; way_index++) {
	  addr = (uint64_t) (buffer + set_index * two_o_s + way_index * two_o);
           if (get_cache_set_index(addr) == set_id[set_id_index]) {
		 eviction_set[set_id_index][address_count] = addr;
		 address_count++;
	   }
         }	 
       }
     }


 *((char *)buf) = 1;

 printf("Please type a message.\n");
 bool sending = true;
 uint64_t *temp_pointer;
 uint64_t tmp_point;

 while (sending) {
    char text_buf[128];
    fgets(text_buf, sizeof(text_buf), stdin);
    int msg = string_to_int(text_buf);
    int msg_bin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int i = 0;
  for ( ;msg > 0; ){
    msg_bin[i++] = msg % 2;
    msg /= 2;
  }
    size_t msg_len = 8;
    for (int ind = 0; ind < msg_len; ind++) {
       if (msg_bin[ind] == 0) {
          ;//No access
      }
    else {
   for (int way = 0; way < 8; way++){
    start_time = clock();
    current_time = start_time;
     while (current_time - start_time < 250000) { 
        uint64_t *temp_pointer = (uint64_t *) eviction_set[ind][way];
	uint64_t tmp_point = *temp_pointer;
        //measure_one_block_access_time(eviction_set[ind][way]);
	 //clflush(eviction_set[ind][way]);
     
         current_time = clock();
         }
       }
    
     }
   } 
    printf("Sender Access End\n");

   }

 return 0;

}









