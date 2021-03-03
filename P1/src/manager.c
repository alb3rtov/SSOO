#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include <definitions.h>

int g_n_processes = 0;
pid_t g_pid_processes[N_TOTAL_PROCESS];

void create_multiple_process(const char process[NUM_P_PBPC], char wr_system_log_pipe[256], char wr_average_grade_pipe[256]);
pid_t create_single_process(const char *path, const char *str_process_class, char wr_system_log_pipe[256], char wr_average_grade_pipe[256]);

void get_str_process_info(enum ProcessClass_T class, char **path, char **str_process_class);
void create_process_by_class(enum ProcessClass_T class, char wr_system_log_pipe[256], char wr_average_grade_pipe[256]);

void wait_one_process();
void wait_processes(int system_log_pipe[2], int average_grade_pipe[2]);

void install_signal_handler();
void signal_handler(int signo);

void system_log_message(char *message);
void program_termination_log();
void terminate_processes();

int main(int argc, char *argv[]) {
    
    char process[NUM_P_PBPC] = {PB,PC};

    int system_log_pipe[2];
    int average_grade_pipe[2];
    
    char wr_system_log_pipe[256];
    char wr_average_grade_pipe[256];

    pipe(system_log_pipe);
    pipe(average_grade_pipe);

    sprintf(wr_system_log_pipe, "%d", system_log_pipe[WRITE]);
    sprintf(wr_average_grade_pipe, "%d", average_grade_pipe[WRITE]);

    install_signal_handler();

    printf("[MANAGER %d] Starting manager program...\n",getpid());

    system_log_message("************ Log del sistema ************\n");

    create_process_by_class(PA, NULL, NULL);
    wait_one_process();

    create_multiple_process(process, wr_system_log_pipe, wr_average_grade_pipe);
    wait_processes(system_log_pipe, average_grade_pipe);

    printf("[MANAGER %d] Terminating manager program...\n", getpid());

    return EXIT_SUCCESS;
}

void create_process_by_class(enum ProcessClass_T class, char wr_system_log_pipe[256], char wr_average_grade_pipe[256]) {
    char *path = NULL, *str_process_class = NULL;

    get_str_process_info(class, &path, &str_process_class);
    g_pid_processes[g_n_processes] = create_single_process(path, str_process_class, wr_system_log_pipe, wr_average_grade_pipe);
 
    printf("[MANAGER %d] Process child %s with PID %d created\n", getpid(), str_process_class, g_pid_processes[g_n_processes]);
    g_n_processes++;
}

void create_multiple_process(const char process[NUM_P_PBPC], char wr_system_log_pipe[256], char wr_average_grade_pipe[256]) {
    int i;

    for (i = 0; i < NUM_P_PBPC; i++) {
        create_process_by_class(process[i], wr_system_log_pipe, wr_average_grade_pipe);
    }
}

pid_t create_single_process(const char *path, const char *str_process_class, char wr_system_log_pipe[256], char wr_average_grade_pipe[256]) {

    pid_t pid;

    switch (pid = fork()) {
        case -1:
            fprintf(stderr, "[MANAGER %d] Error creating process: %s.\n", getpid(), strerror(errno));
            exit(EXIT_FAILURE);
        case 0:
            
            if (execl(path, str_process_class, wr_system_log_pipe, wr_average_grade_pipe, NULL) == -1) {
                fprintf(stderr, "[MANAGER %d] Error using execl(): %s.\n", getpid(),strerror(errno));
                exit(EXIT_FAILURE);
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
            *str_process_class = PD_CLASS;
            break;        
    }
}

void system_log_message(char *message) {
    FILE *log = open_file(LOG_FILE, "a");
    fputs(message, log);
    fclose(log);
}

void wait_one_process() {
    wait(NULL);
    system_log_message("Creación de directorios finalizada.\n");
}

void wait_processes(int system_log_pipe[2], int average_grade_pipe[2]) {
    int i;
    char buffer_grade[80];

    close(system_log_pipe[WRITE]);
    close(average_grade_pipe[WRITE]);

    for (i = 0; i < NUM_P_PBPC; i++) {
        char buffer[100] = "";
        read(system_log_pipe[READ], buffer, sizeof(buffer));
        system_log_message(buffer);
    }

    read(average_grade_pipe[READ], buffer_grade, sizeof(buffer_grade));
    system_log_message(buffer_grade);
    system_log_message("FIN DE PROGRAMA.\n");
}

void terminate_processes() {
    int i;

    for (i = 0; i < 3; i++) {
        if ((kill(g_pid_processes[i],0) == 0) && g_pid_processes[i] != 0) {
            if (kill(g_pid_processes[i],SIGINT) == -1) {
                fprintf(stderr,"[MANAGER %d] Error using kill() on process %d: %s.\n", getpid(), g_pid_processes[i], strerror(errno));
            } else {
                printf("[MANAGER %d] Terminating process %d\n", getpid(), g_pid_processes[i]);
            }
        }
    }
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[MANAGER %d] Error installing signal handler: %s.\n", getpid(),strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    program_termination_log();
    terminate_processes();
    create_process_by_class(PD, NULL, NULL);
    printf("\n[MANAGER %d] Program termination (Ctrl + C).\n",getpid());
    exit(EXIT_SUCCESS);
}

void program_termination_log() {
    time_t rawtime;
    struct tm * timeinfo;

    char message[35] = "Program termination (Ctrl + C) ";
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *str_time = asctime(timeinfo);

    strcat(message, str_time);
    system_log_message(message);
}