#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

void install_signal_handler();
void signal_handler(int signo);
void parse_argv(char *argv[], char **filename);
void read_filename(char filename[]);
void f_execle(const char *args[]);

int main(int argc, char *argv[]) {
    char *filename;

    install_signal_handler();
    parse_argv(argv, &filename);

    read_filename(filename);

    return EXIT_SUCCESS;
}

void read_filename(char filename[]) {
    FILE *file;
    char dir_name[] = "estudiantes";
    char dir_complete[40];

    file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "[PA %d] Error opening file '%s': %s\n",getpid(), filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    DIR* dir = opendir(dir_name);

    if (dir) {
        closedir(dir);
    } else if(ENOENT == errno) {
        const char *args[] = {"/bin/mkdir", dir_name, NULL};
        f_execle(args);
        closedir(dir);
    } else {
        fprintf(stderr, "[PA %d] Error creating the main directory: %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (fgets(buffer, 256 - 1, file)) {
        buffer[strcspn(buffer, " ")] = 0;
        sprintf(dir_complete, "%s/%s", dir_name, buffer);
        printf("%s\n",dir_complete);
    }

    fclose(file);
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

void parse_argv(char *argv[], char **filename) {
    *filename = argv[0];
}

void f_execle(const char *args[]) {
    
    if (execl(args[0], args, NULL) == -1) {
        fprintf(stderr, "[PA %d] Error with execve: %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}