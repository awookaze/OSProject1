#include "kernel/sysinfo.h"
#include "user.h"
#include "stdio.h"

int
main(void)
{
  struct sysinfo s;
  
  // Call sysinfo system call and check for errors.
  if(sysinfo(&s) < 0){
    printf("sysinfo failed\n");
    exit();
  }
  
  // Print free memory and number of active processes.
  printf("Free Memory: %d bytes\n", s.freemem);
  printf("Active Processes: %d\n", s.nproc);
  
#ifdef CHALLENGE
  // If implementing the challenge, also print the load average.
  printf("Load Average: %f\n", s.loadavg);
#endif
  
  printf("sysinfo: OK\n");
  exit();
}
