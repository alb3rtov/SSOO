#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <definitions.h>

void install_signal_handler();
void signal_handler(int signo);

int main(int argc, char *argv[]) {
    install_signal_handler();
    printf("[PB %d]\n", getpid());
    sleep(3);
    /*
    get_test_model();
    copy_test_model();
    */
    return EXIT_SUCCESS;
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[PA %d] Error installing singal handler: %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    printf("\n[PA %d] terminated (SIGINT).\n", getpid());
    exit(EXIT_SUCCESS);
}