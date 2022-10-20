
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <time.h>
#define SAMPLES 10
 
uint64_t get_cache_set_index(ADDR_PTR phys_addr)
  {
	    uint64_t mask = ((uint64_t) 1 << 15) - 1;
	        return (phys_addr & mask) >> 6;
  }

int main(int argc, char **argv)
{

 int set_id[8] = {0, 1, 2, 3, 4, 5, 6, 7};

 uint64_t eviction_set[8][8];

 volatile char tmp; 

 int n = 8;
 int miss_threshold = 70;

 int o = 6;                          // log_2(64), where 64 is the line size
 int s = 14;                         // log_2(16384), where 16384 is the number of cache sets
 int two_o = 64;             // 64
 int two_o_s = 512;   // 1048576
 int b = 262144;

 long int misses[8][8];
 long int hits[8][8];

 char *buffer = malloc((size_t) b);

 uint64_t addr;

 clock_t current_time, start_time;

  for (int ind = 0; ind <8; ind++) {
	  for (int way = 0; way <8; way++) {
		  misses[ind][way] = 0;
	  }
  }

  for (int ind = 0; ind <8; ind++) {
	  for (int way = 0; way <8; way++) {
		  hits[ind][way] = 0;
	  }
  }


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


 for (int repeat=0; repeat < 100000; repeat++) {
	for (int ind = 0; ind < 8; ind++) {
          for (int way = 0; way < 8; way++){
              uint64_t *temp_pointer = (uint64_t *) eviction_set[ind][way];
	      tmp = *temp_pointer;
	    }
   	}
    }

 start_time = clock();
 current_time = start_time;

while (current_time - start_time < 10000000) {
current_time = clock();

}	
printf("Prime is done\n");       

        bool listening = true;
          for (int ind = 0; ind < 8; ind++) {
	    for (int way = 0; way < 8; way++){
         start_time = clock();
         current_time = start_time;
	    while (listening && current_time - start_time < 200) {
            int time = measure_one_block_access_time(eviction_set[ind][way]);
	    
	   if (time < 700) {

            if (time > miss_threshold)
             //misses[ind][way]++;
	     printf("For set %d we have miss\n", ind);		    
	    else
		    ;
	     //hits[ind][way]++;
           }		    
  current_time = clock();      
       }
    }

		// Put your covert channel code here
}

//  printf("Receiver finished.\n");
//  for (int ind = 0; ind < 8; ind++) {
//	  for (int way = 1; way <8; way++) {
//		   misses[ind][0] += misses[ind][way];
//	  }
//		  printf("Misses are %ld for index %d is\n", misses[ind][0], ind);
//  }
//
//  for (int ind = 0; ind <8; ind++) {
//	  for (int way = 1; way <8; way++) {
//		   hits[ind][0] += hits[ind][way];
//	  }
//            printf("Hits are %ld for index %d is\n", hits[ind][0], ind);
//  }
	return 0;
}


