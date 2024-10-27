#include "kernel/types.h"
#include "user/user.h"
void Pipeline(int p_read) { 
    // 在父进程中处理数据，然后通过管道的形式给子进程。
    // 然后子进程同样的处理即可。
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        Pipeline(p[0]);
        close(p[0]);
    } else {
        close(p[0]);
        int prime_num, flow_num;
        if (read(p_read, &prime_num, sizeof(prime_num)) > 0) {
            printf("prime %d\n", prime_num);
        }
        while (read(p_read, &flow_num, sizeof(flow_num)) > 0) {
            if (flow_num % prime_num != 0) {
                write(p[1], &flow_num, sizeof(flow_num));
            }
        }
        close(p[1]);
        wait((int *)0);
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        Pipeline(p[0]);
        close(p[0]);
    } else { // 这个是root进程，用来创建2-35的数字。
        close(p[0]);
        for (int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait((int *)0);
    }
    exit(0);
}