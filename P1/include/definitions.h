#include <dirent.h>

#define PA_CLASS "PA"
#define PA_PATH "./exec/pa"
#define PB_CLASS "PB"
#define PB_PATH "./exec/pb"
#define PC_CLASS "PC"
#define PC_PATH "./exec/pc"
#define PD_CLASS "PD"
#define PD_PATH "./exec/pd"
#define PBK_PATH "./exec/backup"

#define ESTUDIANTES_FILE "estudiantes_p1.text"
#define LOG_FILE "log.txt"
#define GRADE_FILE "min_grade.txt"

#define DIR_ESTUDIANTES "estudiantes"
#define DIR_TEST_MODELS "test_models"
#define DIR_BACK_UP "backup"

#define N_TOTAL_PROCESS 4
#define NUM_P_PBPC  2
#define BUFFER  1024
#define READ     0
#define WRITE   1

enum ProcessClass_T {PA, PB, PC, PD};

FILE* open_file(char filename[], char *permissions) {
    FILE *file;
    file = fopen(filename, permissions);

    if (file == NULL) {
        fprintf(stderr, "[%d] Error opening file '%s': %s\n",getpid(), filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return file;
}

void send_message_to_manager(int wr_pipe, char message[]) {
    write(wr_pipe, message, strlen(message)+1);
}

int check_directory(char dir_name[]) {
    int exists;
    DIR* dir = opendir(dir_name);

    if (dir) {
        exists = 0;
        closedir(dir);
    } else if(ENOENT == errno) {
        exists = -1;
    }
    
    return exists;
}