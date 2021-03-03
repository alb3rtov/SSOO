#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include <definitions.h>

int main(void) {
    char files_to_copy[40];
    char command[] = "yes | cp -r";

    printf("[PBK %d] Back Up demon created.\n", getpid());
    mkdir(DIR_BACK_UP, 0777);

    sprintf(files_to_copy," %s %s ", DIR_ESTUDIANTES, LOG_FILE);
    strcat(command, files_to_copy);
    strcat(command, DIR_BACK_UP);
    
    while (1) {

        if (check_directory(DIR_ESTUDIANTES) == 0) {
            if (system(command) == -1) {
                fprintf(stderr, "[PBK %d] Error using system(): %s.\n", getpid(), strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        sleep(5);
    }

    return EXIT_SUCCESS;
}