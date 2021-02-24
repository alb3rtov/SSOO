#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#include <definitions.h>

struct TProcess_t *g_process_table;

void init_process_table();
pid_t  create_single_process();
void get_str_process_info(enum ProcessClass_T class, char **path, char **str_process_class);
void create_process_by_class(enum ProcessClass_T class, int index_process_table);

void install_signal_handler();
void signal_handler(int signo);

void wait_processes();

void terminate_processes(void);
void free_resources();

void generate_log_termination();

int main(int argc, char *argv[]) {

    install_signal_handler();

    printf("[MANAGER] Start program...\n");
    init_process_table();
    create_process_by_class(PA, 0);

    wait_processes();

    terminate_processes();
    free_resources();

    return EXIT_SUCCESS;
}

void create_process_by_class(enum ProcessClass_T class, int index_process_table) {
    
    char *path = NULL, *str_process_class = NULL;
    pid_t pid;

    get_str_process_info(class, &path, &str_process_class);

    printf("[MANAGER] Waiting processes...\n");
    pid = create_single_process(path, str_process_class);

    printf("Process child %s created - PID: %d.\n", str_process_class, pid);
}

void init_process_table() {
    int i;

    g_process_table = malloc(N_PROCESS * sizeof(struct TProcess_t));

    for (i = 0; i < N_PROCESS; i++) {
        g_process_table[i].pid = 0;
    }
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
        case PD:
            *path = PD_PATH;
            *str_process_class = PD_PATH;
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

void wait_processes() {
    int i;
    pid_t pid;

    pid = wait(NULL);
    for (i = 0; i < N_PROCESS; i++) {
        if (pid == g_process_table[i].pid) {
            g_process_table[i].pid = 0;
        }
        break;
    }
}

void free_resources() {
    free(g_process_table);
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[MANAGER] Error installing singal handler: $s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    generate_log_termination();
    printf("\n[MANAGER] Program termination (Ctrl + C).\n");
    terminate_processes();
    free_resources();
    exit(EXIT_SUCCESS);
}

void generate_log_termination() {
    
    time_t rawtime;
    struct tm * timeinfo;

    FILE *log = fopen("log.txt","a");

    if (log == NULL) {
        printf("Unable to create file.\n");
    }
    
    char message[35] = "program termination (Ctrl + C) ";
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *str_time = asctime(timeinfo);

    /*strcat(message, " ");*/
    strcat(message, str_time);

    fputs(message, log);
    fclose(log);
}