#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void install_signal_handler();
void signal_handler(int signo);

int main(int argc, char *argv[]) {
    int i;
    install_signal_handler();

    printf("\n[PA %d] Hello World!\n", getpid());
    
    for (i = 0; i < 5; i++) {
        printf("[PA %d] Doing things... (%d)\n", getpid(), i+1);
        sleep(1);
    }

    return EXIT_SUCCESS;
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[MANAGER] Error installing singal handler: $s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    printf("\n[PA %d] terminated (SIGINT).\n", getpid());
    exit(EXIT_SUCCESS);
}