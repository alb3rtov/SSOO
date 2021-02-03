#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <definitions.h>

struct TProcess_t *g_process_table;

pid_t  create_single_process();
void init_process_table();
void terminate_processes(void);
void free_resources();
void get_str_process_info(enum ProcessClass_T class, char **path, char **str_process_class);
void create_process_by_class(enum ProcessClass_T class, int index_process_table);

int main(int argc, char *argv[]) {

    init_process_table();
    create_process_by_class(PA, 0);

    printf("\nSleeping 5 seconds...");
    sleep(5);

    terminate_processes();
    free_resources();

    return EXIT_SUCCESS;
}

void create_process_by_class(enum ProcessClass_T class, int index_process_table) {
    
    char *path = NULL, *str_process_class = NULL;
    pid_t pid;

    get_str_process_info(class, &path, &str_process_class);
    pid = create_single_process(path, str_process_class);

    printf("PID: %d",pid);
}

void init_process_table() {
    int i;

    g_process_table = malloc(N_PROCESS * sizeof(struct TProcess_t));

    for (i = 0; i < N_PROCESS; i++) {
        g_process_table[i].pid = 0;
    }

}

void free_resources() {
    free(g_process_table);
}

pid_t create_single_process(const char *path, const char *str_process_class) {

    pid_t pid;

    switch (pid = fork()) {
        case -1:
            fprintf(stderr, "[MANAGER] Error creating process: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
            terminate_processes();
            free_resources();
            
        case 0:
            if (execl(path, str_process_class, NULL) == -1) {
                fprintf(stderr, "[MANAGER] Error using execl(): %s.\n",strerror(errno));
                exit(EXIT_FAILURE);
                terminate_processes();
                free_resources();
            }
    }

    return pid;
}

void get_str_process_info(enum ProcessClass_T class, char **path, char **str_process_class) {
    
    switch (class) {
        case PA:
            *path = PA_PATH;
            *str_process_class = PA_CLASS;
            break;
        case PB:
            *path = PB_PATH;
            *str_process_class = PB_CLASS;
            break;
        case PC:
            *path = PC_PATH;
            *str_process_class = PC_CLASS;
            break;            
    }
}

void terminate_processes(void) {
    int i;

    printf("\n----  [MANAGER] Terminating running child processes ----\n");
    for (i = 0; i < N_PROCESS; i++) {
        if (g_process_table[i].pid != 0) {
            if (kill(g_process_table[i].pid, SIGINT) == -1) {
                fprintf(stderr, " [MANAGER] Error using kill(): %s", strerror(errno));
            }
        }
    }

}


