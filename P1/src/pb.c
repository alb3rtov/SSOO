/************************************************************
 * Project          : Práctica 1 de Sistemas Operativos II
 * Program name     : pb.c
 * Author           : Alberto Vázquez Martínez
 * Date created     : 17/02/2021
 * Purpose          : Read the test model of each student and
 *                    copy to their directory
 ***********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

#include <definitions.h>
#include <files.h>

void read_test_model(FILE *file);
char* get_filename_test_model(const char *test_model);
void copy_test_model(const char *dni, const char *filename_model);

void parse_argv(char *argv[], int *wr_pipe);
void install_signal_handler();
void signal_handler(int signo);

/******************* Main function *******************/
int main(int argc, char *argv[]) {

    int wr_pipe;
    char message[] = "Copia de modelos de examen, finalizada.\n";
    
    install_signal_handler();
    parse_argv(argv, &wr_pipe);
    sleep(5);
    FILE *file = open_file(ESTUDIANTES_FILE, "r");
    read_test_model(file);

    send_message_to_manager(wr_pipe, message);

    return EXIT_SUCCESS;
}

/******************* Read estudiantes file and split in dni and test model *******************/
void read_test_model(FILE *file) {
    char buffer[BUFFER];
    
    while (fgets(buffer, BUFFER, file) != NULL) { 
        const char *dni = strtok(buffer, " ");
        const char *test_model = strtok(NULL, " ");
        char *filename_model = get_filename_test_model(test_model);
        copy_test_model(dni, filename_model);
    }

    fclose(file);
}

/******************* Get the name pdf model of test *******************/
char* get_filename_test_model(const char *test_model) {

    char *filename_model = "";
    
    if (strcmp("A", test_model) == 0) {
        filename_model = "MODELOA.pdf";
    } else if (strcmp("B", test_model) == 0) {
        filename_model = "MODELOB.pdf";
    } else {
        filename_model = "MODELOC.pdf";
    }

    return filename_model;
}

/******************* Copy into the student directory the pdf test model file *******************/
void copy_test_model(const char *dni, const char *filename_model) {
    char destination_path[40];
    char source_path[40];
    char command[] = "cp";

    sprintf(source_path," %s/%s",DIR_TEST_MODELS,filename_model);
    sprintf(destination_path," %s/%s", DIR_ESTUDIANTES, dni);
    
    strcat(command, source_path);
    strcat(command, destination_path);  

    if (system(command) == -1) {
        fprintf(stderr, "[PB %d] Error using system(): %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void parse_argv(char *argv[], int *wr_pipe) {
    *wr_pipe = atoi(argv[1]);
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[PB %d] Error installing signal handler: %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    printf("\n[PB %d] terminated (SIGINT).\n", getpid());
    exit(EXIT_SUCCESS);
}