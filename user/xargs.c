#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

void handle(char *buf, int argc, char *argv[]) {
    char *argp[MAXARG];
    int j;
    for (j = 1; j < argc; ++j) {
        argp[j - 1] = argv[j]; 
    }
    argp[j - 1] = buf;
    argp[j] = 0; // 标记着参数列表的结束
    if (fork() == 0) {
        exec(argp[0], argp);
    } else {
        wait(0);
    }
    return;
}

int main(int argc, char *argv[]) {
    char buf[100];
    int i = 0;
    // 从标准输入读取一行，放到buf中，到 "\n" 结束
    while (read(0, &buf[i], 1) > 0) {
        if (buf[i] == '\n') {
            buf[i] = 0;
            handle(buf, argc, argv);
            i = 0; 
        } else {
            ++i;
        }
    }
    // 最后还要处理一次，因为最后一个参数后面没有 \n
    buf[i] = 0;
    handle(buf, argc, argv);
    exit(0);
}