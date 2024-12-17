#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    struct sysinfo info;
    sysinfo(&info);
    printf("Free memory(M): %d, Number of active process: %d\n", info.freemem / 1024 / 1024, info.nproc);
    exit(0);
}