/************************************************************
 * Project          : Práctica 1 de Sistemas Operativos II
 * Program name     : pa.c
 * Author           : Alberto Vázquez Martínez
 * Date created     : 17/02/2021
 * Purpose          : Create a directory for each student
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include <files.h>

void create_main_directory(char dir_name[]);
void create_directories(FILE *file);

void parse_argv(char *argv[], int system_log_message_pipe[2]);
void install_signal_handler();
void signal_handler(int signo);

/******************* Main function *******************/
int main(int argc, char *argv[]) {

    install_signal_handler();
    sleep(5);
    FILE *file = open_file(ESTUDIANTES_FILE, "r");
    create_directories(file);

    return EXIT_SUCCESS;
}

/******************* Create the directory ESTUDIANTES *******************/
void create_main_directory(char dir_name[]) {
    if (check_directory(dir_name) != 0) {
        mkdir(dir_name, 0777);
    } 
}

/******************* Create directory of each student *******************/
void create_directories(FILE *file) {
    char buffer[BUFFER];
    char dir_complete[40];
    
    create_main_directory(DIR_ESTUDIANTES);

    while (fgets(buffer, BUFFER, file) != NULL) {
        const char *dni = strtok(buffer, " ");
        sprintf(dir_complete,"%s/%s",DIR_ESTUDIANTES, dni);
        mkdir(dir_complete, 0777);
    }
    
    fclose(file);
}

/******************* Install a signal handler for CTRL+C signal *******************/
void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[PA %d] Error installing signal handler: %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    printf("\n[PA %d] terminated (SIGINT).\n", getpid());
    exit(EXIT_SUCCESS);
}