#include <dirent.h>

#define ESTUDIANTES_FILE "estudiantes_p1.text"
#define LOG_FILE "log.txt"
#define GRADE_FILE "min_grade.txt"

#define DIR_ESTUDIANTES "estudiantes"
#define DIR_TEST_MODELS "test_models"
#define DIR_BACK_UP "backup"

#define BUFFER  1024

/******************* Open a file with a given permissions and return a File pointer *******************/
FILE* open_file(char filename[], char *permissions) {
    FILE *file;
    file = fopen(filename, permissions);

    if (file == NULL) {
        fprintf(stderr, "[%d] Error opening file '%s': %s\n",getpid(), filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return file;
}

/******************* Check if a given directory exists *******************/
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