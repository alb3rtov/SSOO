/************************************************************
 * Project          : Práctica 1 de Sistemas Operativos II
 * Program name     : backup.c
 * Author           : Alberto Vázquez Martínez
 * Date created     : 17/02/2021
 * Purpose          : Perform a back up of estudiantes
 *                    directory and log file each 1 minute
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include <files.h>

void copy_files();

/******************* Main function *******************/
int main(void) {
    
    printf("[PBK %d] Back Up daemon created.\n", getpid());
    copy_files();

    return EXIT_SUCCESS;
}

/******************* Make a backup estudiantes directory and log file into backup directory *******************/
void copy_files() {
    char files_to_copy[40];
    char command[] = "yes | cp -r";

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
        sleep(60);
    }
}