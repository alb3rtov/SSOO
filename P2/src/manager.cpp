#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream> 
#include <iterator>
#include <queue>

#include <colors.h>

class Result {
private:
    int line_number;
    std::string line;

public:
    Result(int line_number, std::string line) {
        this->line_number = line_number;
        this->line = line;
    }
};

class ThreadInfo {
private:
    int id;
    int begin;
    int end;

public:
    ThreadInfo( int id, int begin, int end) {
        this->id = id;
        this->begin = begin;
        this->end = end;
    }
};

std::string DIR_BOOKS = "books/";
std::vector<ThreadInfo> threads_info;

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

/* Search the word into the file */
void search_word(std::string word, int begin, int end, int id, std::string filename) {
    //std::cout << "[Hilo " << id << " inicio: " << begin << " - final: " << end << "]" << std::endl;

    filename = DIR_BOOKS + filename;

    std::ifstream file(filename);
    std::string line, original_line, find_word, previous_last_word, token, previous_word;
    int number_line;

    for (int i = begin; i <= end; i++) {
        std::getline(file, line);

        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin,end);
        
        for (int j = 0; j < vstrings.size(); j++) {
            if (vstrings[j].compare(word) == 0) {
                if (vstrings[j-1].empty()) {
                    previous_word =  previous_last_word;
                } else {
                    previous_word = vstrings[j-1];
                }

                std::cout << "... " << previous_word << " " << vstrings[j] 
                            << " " << vstrings[j+1] << " ..." << std::endl;
            }
        }

        /* Keep the previous line */
        previous_last_word = vstrings[vstrings.size()-1];
        //std::cout << previous_last_word << std::endl;
    }

    file.close();
}

/* Create all threads and */
void create_and_distribute_threads(int number_threads, std::vector<std::thread> &threads, 
                    int number_of_lines, std::string word, std::string filename) {

    int size_task = number_of_lines/number_threads;
    std::cout << "Tamaño divison: " << size_task << std::endl;
    
    for (int i = 0; i < number_threads; i++) {
        int begin = i*size_task;
        int end = (begin + size_task)-1;
        if (i == number_threads-1) end = number_of_lines-1;

        ThreadInfo ithread(i, begin, end);
        threads_info.push_back(ithread);
        threads.push_back(std::thread(search_word, word, begin, end, i, filename));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

/* Main function */
int main(int argc, char *argv[]) {

    int number_threads, number_of_lines, size_task;
    std::string word, filename;
    std::vector<std::thread> threads;

    parse_argv(argc, argv, &filename, &word, &number_threads);
    number_of_lines = get_number_of_lines(filename);
    
    std::cout << "Numero de lineas: " << number_of_lines << std::endl;

    create_and_distribute_threads(number_threads, threads, number_of_lines, word, filename);

    return 0;
}