#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void install_signal_handler();
void signal_handler(int signo);

int main(int argc, char *argv[]) {

    char message[] = "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n";

    install_signal_handler();
    printf("[PC %d]\n",getpid());

    write(atoi(argv[1]), message, strlen(message));

    return 0;
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