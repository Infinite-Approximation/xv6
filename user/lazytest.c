#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

#define REGION_SZ (120 * 1024 * 1024)

void
sparse_memory(char *s)
{
  char *i, *prev_end, *new_end;
  
  prev_end = sbrk(REGION_SZ);
  if (prev_end == (char*)0xffffffffffffffffL) {
    printf("sbrk() failed\n");
    exit(1);
  }
  new_end = prev_end + REGION_SZ;

  for (i = prev_end + PGSIZE; i < new_end; i += 64 * PGSIZE)
    *(char **)i = i;

  for (i = prev_end + PGSIZE; i < new_end; i += 64 * PGSIZE) {
    if (*(char **)i != i) {
      printf("failed to read value from memory\n");
      exit(1);
    }
  }

  exit(0);
}

// run each test in its own process. run returns 1 if child's exit()
// indicates success.
int
run(void f(char *), char *s) {
  int pid;
  int xstatus;
  
  printf("running test %s\n", s);
  if((pid = fork()) < 0) {
    printf("runtest: fork error\n");
    exit(1);
  }
  if(pid == 0) {
    f(s);
    exit(0);
  } else {
    wait(&xstatus);
    if(xstatus != 0) 
      printf("test %s: FAILED\n", s);
    else
      printf("test %s: OK\n", s);
    return xstatus == 0;
  }
}

int
main(int argc, char *argv[])
{
  printf("lazytests starting\n");
  int fail = 0;
  uint64 time1 = r_time();
  if (!run(sparse_memory, "lazy alloc")) {
    fail = 1;
  }
  uint64 time2 = r_time();
  if(!fail) {
    printf("ALL TESTS PASSED\n");
    printf("Time cose: %d(ms)\n", (time2 - time1) / 10000);
  } else
    printf("SOME TESTS FAILED\n");
  exit(1);   // not reached.
}
