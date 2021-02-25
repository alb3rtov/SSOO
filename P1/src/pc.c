#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <definitions.h>

float read_grade(FILE *file);
int calculate_minimum_grade(const char *grade);
void generate_file_grade(const char *dni, int minimum_grade);
void write_grades(FILE *file, int minimum_grade);

void parse_argv(char *argv[], int *wr_pipe);
void install_signal_handler();
void signal_handler(int signo);

int main(int argc, char *argv[]) {

    int wr_pipe;
    float average_grade;
    char a_grade[80];
    char message[] = "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n";

    install_signal_handler();
    parse_argv(argv, &wr_pipe);
    sleep(5);
    FILE *file = open_file(ESTUDIANTES_FILE);
    average_grade = read_grade(file);
    send_message_to_manager(wr_pipe, message);
    sprintf(a_grade, "La nota media de la clase es: %.2f\n",average_grade);
    send_message_to_manager(wr_pipe, a_grade);
    return EXIT_SUCCESS;
}

float read_grade(FILE *file) {
    char buffer[BUFFER];
    int counter = 0, sum_grades = 0;

    while (fgets(buffer, BUFFER, file) != NULL) { 
        const char *dni = strtok(buffer, " ");
        strtok(NULL, " ");
        const char *grade = strtok(NULL, " ");
        int minimum_grade = calculate_minimum_grade(grade);
        generate_file_grade(dni, minimum_grade);
        counter++;
        sum_grades += atoi(grade);
    }

    return (sum_grades/counter);
}

int calculate_minimum_grade(const char *grade) {
    return (10 - atoi(grade));
}

void write_grades(FILE *file, int minimum_grade) {
    char message[] = "La nota que debes obtener en este nuevo examen para superar la prueba es ";
    char m_grade[10];
    sprintf(m_grade, "%d.\n", minimum_grade);
    strcat(message, m_grade);
    fputs(message, file);
    fclose(file);
}

void generate_file_grade(const char *dni, int minimum_grade) {
    char destination_path[40];

    sprintf(destination_path,"%s/%s/%s", DIR_ESTUDIANTES, dni, GRADE_FILE);
    /*printf("%s\n",destination_path);*/
    FILE *file = create_file(destination_path);
    write_grades(file, minimum_grade);
}

void parse_argv(char *argv[], int *wr_pipe) {
    *wr_pipe = atoi(argv[1]);
}

void install_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[PC %d] Error installing signal handler: %s.\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    printf("\n[PC %d] terminated (SIGINT).\n", getpid());
    exit(EXIT_SUCCESS);
}