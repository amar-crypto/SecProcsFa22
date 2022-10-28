
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#include <math.h>
#include <time.h>

// TODO: define your own buffer size
#define BUFF_SIZE (1<< 21)

//Function to Calculate Index based on Address
 uint64_t get_index(ADDR_PTR phys_addr)
  {
	int index_and_offset_bits = 15;  
	int block_offset = 6;
        uint64_t mask = ((uint64_t) 1 << index_and_offset_bits) - 1;
	return (phys_addr & mask) >> block_offset;
  }


int main(int argc, char **argv)
{
  // Allocate a buffer using huge page
 // // See the handout for details about hugepage management
 void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);  //Allocate a Page of 2MB
  
  if (buf == (void*) - 1) {
     perror("mmap() error\n");
     exit(EXIT_FAILURE);
  }

 int index_number[8] = {100, 10, 50, 80, 200, 250, 150, 300};          ////Set number for each bit

 uint64_t eviction_set[8][8];          //Eviction set Array store the Address



 int L2_Block_size = 64;
 int L2_set_size = 512;   
 int L2_size = 262144;

 char *buffer = malloc((size_t) L2_size);    //Starting Address for L2 Access.

 uint64_t addr;
 clock_t start_time, current_time;       //TImer



//FInd the eviction Set
 for (int index = 0; index < 8; index++) {
     int count = 0;
     for (int set = 0; set < 512; set++) {
       for (int way = 0; way < 8; way++) {
	  addr = (uint64_t) (buffer + set * L2_set_size + way * L2_Block_size);
           if (get_index(addr) == index_number[index]) {
		 eviction_set[index][count] = addr;
		 count++;
	   }
         }	 
       }
     }


 *((char *)buf) = 1;
 
 char text_buf[128];

 bool sending = true;
 uint64_t *addr_pointer;
 uint64_t temp_point;

 /////Read the input and start eviction
 while (sending) {
    printf("Please type a message.\n");
    fgets(text_buf, sizeof(text_buf), stdin);
    int msg = string_to_int(text_buf);
    int msg_binary[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int i = 0;
  
    for ( ;msg > 0; ){
    msg_binary[i++] = msg % 2;
    msg /= 2;
  }
    for (int index = 0; index < 8; index++) {
       if (msg_binary[index] == 0) {
      
	       ;
       
       }
    else {
   for (int way = 0; way < 8; way++){
    start_time = clock();
    current_time = start_time;
     while (current_time - start_time < 100000) {                     ////For each line of particular set evict multiple times. 
       addr_pointer = (uint64_t *) eviction_set[index][way];
       temp_point = *addr_pointer;
//          measure_one_block_access_time(eviction_set[index][way]);
//	 clflush(eviction_set[index][way]);
     
         current_time = clock();
         }
       }
    
     }
   } 
    printf("Sender Access End\n");

   }

 return 0;

}









