#define PA_CLASS "PA"
#define PA_PATH "./exec/pa"
#define PB_CLASS "PB"
#define PB_PATH "./exec/pb"
#define PC_CLASS "PC"
#define PC_PATH "./exec/pc"
#define PD_CLASS "PD"
#define PD_PATH "./exec/pd"
#define N_PROCESS 4

enum ProcessClass_T {PA, PB, PC, PD};

struct TProcess_t {
    enum ProcessClass_T class;
    pid_t pid;
    char *str_process_class;
};