#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    printf("[PC %d]\n",getpid());
    return 0;
}