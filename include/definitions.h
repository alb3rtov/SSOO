#define PA_CLASS "PA"
#define PA_PATH "./exec/pa"
#define PB_CLASS "PB"
#define PB_PATH "./exec/pb"
#define PC_CLASS "PC"
#define PC_PATH "./exec/pc"
#define N_PROCESS 3

enum ProcessClass_T {PA, PB, PC};

struct TProcess_t {
    enum ProcessClass_T class;
    pid_t pid;
    char *str_process_class;
};