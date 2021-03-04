/************************************************************
 * Project          : Práctica 1 de Sistemas Operativos II
 * Program name     : pd.c
 * Author           : Alberto Vázquez Martínez
 * Date created     : 17/02/2021
 * Purpose          : Delete estudiantes directory if a 
 *                    SIGINT signal happens (Ctrl + C)
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

#include <files.h>

void delete_directory();

/******************* Main function *******************/
int main(int argc, char *argv[]) {

    delete_directory();
    return EXIT_SUCCESS;
}

/******************* Delete estudiantes directory if exists *******************/
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