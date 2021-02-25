#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>


#include <definitions.h>

void install_signal_handler();
void signal_handler(int signo);

void read_test_model(FILE *file);
void copy_test_model(const char *dni, const char *filename_model);
char* get_filename_test_model(const char *test_model);

int main(int argc, char *argv[]) {

    char message[] = "Copia de modelos de examen, finalizada.\n";

    install_signal_handler();
    printf("[PB %d]\n", getpid());

    FILE *file = open_file(ESTUDIANTES_FILE);
    read_test_model(file);

    write(atoi(argv[1]), message, strlen(message));

    return EXIT_SUCCESS;
}

void copy_test_model(const char *dni, const char *filename_model) {
    char destination_path[40];
    char source_path[40];
    char command[] = "cp";

    sprintf(source_path," %s/%s",DIR_TEST_MODELS,filename_model);
    sprintf(destination_path," %s/%s", DIR_ESTUDIANTES, dni);
    
    strcat(command, source_path);
    strcat(command, destination_path);  

    if (system(command) == -1) {
        fprintf(stderr, "[PB %d] Error using execl(): %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void read_test_model(FILE * file) {
    char buffer[BUFFER];
    
    while (fgets(buffer, BUFFER, file) != NULL) { 
        const char *dni = strtok(buffer, " ");
        const char *test_model = strtok(NULL, " ");
        char *filename_model = get_filename_test_model(test_model);
        /*printf("%s: %s -> %s\n", dni, test_model, filename_model);*/
        copy_test_model(dni, filename_model);
    }
}

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