#define PA_CLASS "PA"
#define PA_PATH "./exec/pa"
#define PB_CLASS "PB"
#define PB_PATH "./exec/pb"
#define PC_CLASS "PC"
#define PC_PATH "./exec/pc"
#define PD_CLASS "PD"
#define PD_PATH "./exec/pd"
#define PBK_PATH "./exec/backup"

#define N_TOTAL_PROCESS 4
#define NUM_P_PBPC  2
#define READ     0
#define WRITE   1

/******************* Enumeration of each kind of process *******************/
enum ProcessClass_T {PA, PB, PC, PD};

/******************* Write a given message into a given pipe *******************/
void send_message_to_manager(int wr_pipe, char message[]) {
    write(wr_pipe, message, strlen(message)+1);
}