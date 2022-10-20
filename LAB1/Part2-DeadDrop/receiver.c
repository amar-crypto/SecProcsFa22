
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <time.h>
#define SAMPLES 10
 

 uint64_t get_index(ADDR_PTR phys_addr)
  {
	int index_and_offset_bits = 15;  
	int block_offset = 6;
        uint64_t mask = ((uint64_t) 1 << index_and_offset_bits) - 1;
	return (phys_addr & mask) >> block_offset;
  }


int main(int argc, char **argv)
{


 int index_number[8] = {0, 1, 2, 3, 4, 5, 6, 7};

 uint64_t eviction_set[8][8];

 volatile char tmp; 

 int n = 8;

 int o = 6;                          // log_2(64), where 64 is the line size
 int s = 14;                         // log_2(16384), where 16384 is the number of cache sets
 int two_o = 64;             // 64
 int two_o_s = 512;   // 1048576
 int b = 262144;

 int miss_threshold = 70;

 char *buffer = malloc((size_t) b);

 uint64_t addr;
 clock_t start_time, current_time;

 for (int index = 0; index < 8; index++) {
     int count = 0;
     for (int set = 0; set < 512; set++) {
       for (int way = 0; way < 8; way++) {
	  addr = (uint64_t) (buffer + set * two_o_s + way * two_o);
           if (get_index(addr) == index_number[index]) {
		 eviction_set[index][count] = addr;
		 count++;
	   }
         }	 
       }
     }



 int binary_number[8] = {0, 0, 0, 0, 0, 0, 0, 0};

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

            if (time > miss_threshold) {
             //misses[ind][way]++;
	     binary_number[ind] = 1;
	//     printf("For set %d we have miss\n", ind);
	    }
	   }	    
	    else
		    ;
	     //hits[ind][way]++;
  current_time = clock();      
           }		    
       }
    }

		// Put your covert channel code here

int flag_zero = 1;

  for (int i =1; i < 8; i++) {
    if (binary_number[i])
	  flag_zero = 0;  
  }

 if(flag_zero == 1)
   printf("The decoded number is 0\n");

 else {
 
   int num = 0;
   int pow_2 = 1;
   for (int i = 0; i < 8; i++) {
	num += binary_number[i] * pow_2;
        pow_2 = 2 * pow_2;	
//	printf("Power of 2 is %d and binary number is %d\n", pow_2, binary_number[i]); 
   
   
   }
   printf("The decoded number is %d\n", num); 
 
 }

  printf("Receiver finished.\n");
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


