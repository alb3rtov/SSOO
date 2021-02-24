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


void create_multiple_process(const char process[SIZE]);
pid_t create_single_process();

void get_str_process_info(enum ProcessClass_T class, char **path);
void create_process_by_class(enum ProcessClass_T class);

void wait_one_process();
void wait_processes();

void install_signal_handler();
void signal_handler(int signo);

void system_log_message(char *message);
FILE* create_log_file(char *filename);
void generate_log_termination();

int main(int argc, char *argv[]) {

    char process[SIZE] = {PB,PC};

    install_signal_handler();

    printf("[MANAGER %d] Starting manager program...\n",getpid());

    create_process_by_class(PA);
    wait_one_process();

    
    create_multiple_process(process);
    wait_processes();
    
    printf("[MANAGER %d] Terminating manager program...\n", getpid());

    return EXIT_SUCCESS;
}


void create_process_by_class(enum ProcessClass_T class) {
    char *path = NULL;
    pid_t pid;

    get_str_process_info(class, &path);
    pid = create_single_process(path);

    printf("[MANAGER %d] Process child with PID %d created\n", getpid(), pid);

}

void create_multiple_process(const char process[SIZE]) {

    int i;

    for (i = 0; i < SIZE; i++) {
        create_process_by_class(process[i]);
    }
}

pid_t create_single_process(const char *path) {

    pid_t pid;

    switch (pid = fork()) {
        case -1:
            fprintf(stderr, "[MANAGER] Error creating process: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        case 0:
            if (execl(path, ESTUDIANTES_FILE ,NULL) == -1) {
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

void system_log_message(char *message) {
    FILE *log = create_log_file(LOG_FILE);
    fputs(message, log);
    fclose(log);
}

void wait_one_process() {
    wait(NULL);
    system_log_message("************ Log del sistema ************\n");
    system_log_message("Creación de directorios finalizada.\n");
}

void wait_processes() {
    int i;
    for (i = 0; i < SIZE; i++) {
        wait(NULL);
    }
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[MANAGER %d] Error installing singal handler: %s.\n", getpid(),strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    generate_log_termination();
    printf("\n[MANAGER %d] Program termination (Ctrl + C).\n",getpid());
    exit(EXIT_SUCCESS);
}

FILE* create_log_file(char *filename) {
    FILE *log = fopen(filename,"a");
    
    if (log == NULL) {
        fprintf(stderr, "[MANAGER %d] Error creating the system log file.\n", getpid());
        exit(EXIT_FAILURE);
    }

    return log;
}

void generate_log_termination() {
    
    time_t rawtime;
    struct tm * timeinfo;

    FILE *log = create_log_file(LOG_FILE);
    
    char message[35] = "Program termination (Ctrl + C) ";
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *str_time = asctime(timeinfo);

    /*strcat(message, " ");*/
    strcat(message, str_time);

    fputs(message, log);
    fclose(log);
}