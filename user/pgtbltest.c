#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

#define TEST_SIZE (4096)  // 测试数据量：1KB
#define ITERATIONS 5    // 重复次数，稳定测试结果

char src[TEST_SIZE];  // 用户空间数据缓冲区

void measure_copyin() {
  int fd;

  printf("Testing copyin performance with %d KB of data...\n", TEST_SIZE / 1024);

  // 初始化测试数据
  for (int i = 0; i < TEST_SIZE; i++) {
    src[i] = (char)(i % 256);
  }

  // 测试写入 / copyin 的性能
  for (int i = 0; i < ITERATIONS; i++) {
    fd = open("dummy", O_CREATE | O_WRONLY);
    if (fd < 0) {
      printf("Failed to open file\n");
      exit(1);
    }
    if (write(fd, src, TEST_SIZE) != TEST_SIZE) {
      printf("write failed at iteration %d\n", i);
      exit(1);
    }
    // 输出结果
    close(fd);
    unlink("dummy");  // 删除文件
  }
}

int main() {
  measure_copyin();
  exit(0);
}