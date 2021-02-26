#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

#include <definitions.h>

void delete_directory();

int main(int argc, char *argv[]) {

    delete_directory();
    return EXIT_SUCCESS;
}

void delete_directory() {

    if (check_directory(DIR_ESTUDIANTES) == 0) {
        char command[] = "rm -r ";
        strcat(command, DIR_ESTUDIANTES);
        
        if (system(command) == -1) {
            fprintf(stderr, "[PD %d] Error using system(): %s.\n", getpid(), strerror(errno));
            exit(EXIT_FAILURE);
        }

    } else {
        fprintf(stderr, "[PD %d] Directory %s doesn't exists.\n", getpid(), DIR_ESTUDIANTES);
    }
}