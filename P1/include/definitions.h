#define PA_CLASS "PA"
#define PA_PATH "./exec/pa"
#define PB_CLASS "PB"
#define PB_PATH "./exec/pb"
#define PC_CLASS "PC"
#define PC_PATH "./exec/pc"
#define PD_CLASS "PD"
#define PD_PATH "./exec/pd"

#define ESTUDIANTES_FILE "estudiantes_p1.text"
#define SIZE    2
#define BUFFER  1024
#define LOG_FILE "log.txt"

enum ProcessClass_T {PA, PB, PC, PD};

FILE* open_file(char filename[]) {
    
    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "[PA %d] Error opening file '%s': %s\n",getpid(), filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return file;

}