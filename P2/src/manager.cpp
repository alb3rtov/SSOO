#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>
#include <functional>

#include <colors.h>

std::string DIR_BOOKS = "books/";

/* Parse arguments */
void parse_argv(int argc, char *argv[], std::string *filename ,
            std::string *word, int *number_threads) {
    
    if (argc != 4) {
        std::cout << "Not enough arguments, 4 arguments are needed" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    *filename = argv[1];
    *word = argv[2];
    *number_threads = std::stoi(argv[3]);
}

/* Returns the number of lines of a given file */
int get_number_of_lines(std::string filename) {
    
    int number_of_lines = 0;
    std::string complete_filename = DIR_BOOKS + filename;
    std::string line;
    std::ifstream myfile(complete_filename);

    while (std::getline(myfile, line)) {
        ++number_of_lines;
    }

    return number_of_lines;
}


void search_word(std::string word, int begin, int end, int id) {
    std::cout << "[Hilo " << id << " inicio: " << begin << " - final: " << end << "]" << std::endl;
}

/* Create all threads and */
void create_and_distribute_threads(int number_threads, std::vector<std::thread> &v_hilos, 
                    int number_of_lines, std::string word) {

    int size_task = number_of_lines/number_threads;
    std::cout << "Tamaño divison: " << size_task << std::endl;
    for (int i = 0; i < number_threads; i++) {
        int begin = i*size_task;
        int end = (begin + size_task)-1;
        if (i == number_threads-1) end = number_of_lines-1;

        v_hilos.push_back(std::thread(search_word, word, begin, end, i));
    }
    std::for_each(v_hilos.begin(), v_hilos.end(), std::mem_fn(&std::thread::join));
}

/* Main function */
int main(int argc, char *argv[]) {

    int number_threads, number_of_lines, size_task;
    std::string word, filename;
    std::vector<std::thread> v_hilos;

    parse_argv(argc, argv, &filename, &word, &number_threads);
    number_of_lines = get_number_of_lines(filename);
    
    std::cout << "Numero de lineas: " << number_of_lines << std::endl;

    
    create_and_distribute_threads(number_threads, v_hilos, number_of_lines, word);
    //std::cout << word << std::endl;
    //std::cout << filename << std::endl;
    //std::cout << thread_number << std::endl;
    return 0;
}