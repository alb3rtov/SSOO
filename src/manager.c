#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <definitions.h>

struct TProcess_t *g_process_table;

int main(int argc, char *argv[]) {
    printf("Hello world!\n");

    pid_t pid;

    init_process_table();

    pid = create_single_process();
    print(pid);

    return EXIT_SUCCESS;
}

void init_process_table() {
    int i;

    g_process_table = malloc(N_PROCESS * sizeof(struct TProcess_t));

    for (i = 0; i < N_PROCESS; i++) {
        g_process_table[i].pid = 0; //Inicialize pids 0
    }

}

pid_t create_single_process() {

    pid_t pid;

    switch (pid = fork()) {
        case -1:
            fprintf(stderr, "[MANAGER] Error creating process: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        case 0:
            if (execl(NULL == -1)) {
                fprintf(stderr, "[MANAGER] Error using execl(): %s.\n",strerror(errno));
                exit(EXIT_FAILURE);
            }
    }

    return pid;
}


