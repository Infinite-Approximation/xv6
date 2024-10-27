#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("YOU SHOULD INPUT ONE ARGUMENT\n");
        exit(0);
    }
    int stime = atoi(argv[1]);
    sleep(stime);
    exit(0);
}