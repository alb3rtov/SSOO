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

pid_t  create_single_process();
void get_str_process_info(enum ProcessClass_T class, char **path);
void create_process_by_class(enum ProcessClass_T class);

void install_signal_handler();
void signal_handler(int signo);

void generate_log_termination();

int main(int argc, char *argv[]) {

    install_signal_handler();

    printf("[MANAGER %d] Starting manager program...\n",getpid());

    create_process_by_class(PA);

    printf("[MANAGER %d] Terminating manager program...\n", getpid());

    return EXIT_SUCCESS;
}
void create_process_by_class(enum ProcessClass_T class) {
    char *path = NULL;
    pid_t pid;

    get_str_process_info(class, &path);
    pid = create_single_process(path);

    printf("[MANAGER %d] Process child with PID %d created\n", getpid(), pid);
    wait(NULL);
}   

pid_t create_single_process(const char *path) {

    pid_t pid;

    switch (pid = fork()) {
        case -1:
            fprintf(stderr, "[MANAGER] Error creating process: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        case 0:
            if (execl(path, FILE_ESTUDIANTES ,NULL) == -1) {
                fprintf(stderr, "[MANAGER] Error using execl(): %s.\n",strerror(errno));
                exit(EXIT_FAILURE);
            }
    }

    return pid;
}

void get_str_process_info(enum ProcessClass_T class, char **path) {
    
    switch (class) {
        case PA:
            *path = PA_PATH;
            break;
        case PB:
            *path = PB_PATH;
            break;
        case PC:
            *path = PC_PATH;
            break;    
        case PD:
            *path = PD_PATH;
            break;        
    }
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[MANAGER] Error installing singal handler: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    generate_log_termination();
    printf("\n[MANAGER] Program termination (Ctrl + C).\n");
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