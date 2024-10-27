#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

void find(char *dir, char *filename) {
    int fd;
    struct stat st;
    struct dirent de;
    // 打开路径
    if ((fd = open(dir, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }
    // 分析一下这个文件/目录的信息
    if (fstat(fd, &st) < 0) {
        close(fd);
        return;
    }
    // 如果不是目录的话直接退出
    if (st.type != T_DIR) {
        // 一定要关闭fd，不然打开其他文件时发现fd不够了就会导致打开失败！
        close(fd);
        return;
    }
    // buf是用于存放完整的路径名，p用于往buf里面存放文件名
    // 注意每次调用find_recur时，都会往对应的函数栈里面新建 buf 和 p, 因此
    // 不会造成被调用者函数和调用者函数的buf一样
    char buf[512], *p;
    strcpy(buf, dir);
    p = buf + strlen(buf);
    *p++  = '/';
    // 读取目录中的内容
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        // 不能对 . 和 .. 进行递归。
        // 目录下还有一些空文件名，这也不能进行递归
        if (!strcmp(de.name, ".") || !strcmp(de.name, "..")
        || !strcmp(de.name, "")) {
            continue;
        }
        // 拼接完整的路径名
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        // 如果目录中有文件的名字等于filename，则输出
        if (strcmp(de.name, filename) == 0) {
            printf("%s\n", buf);
        }
        find(buf, filename);
    }
    close(fd);
    return;
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        printf("Should include a dir tree and a specific name\n");
        exit(0);
    } else {
        find(argv[1], argv[2]);
    }
    exit(0);
}