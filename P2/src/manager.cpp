#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream> 
#include <iterator>

#include "ThreadInfo.h"
#include "colors.h"

/* Declare global variables */
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

/* Search in the given line if match with the word */
void search_in_line(std::string line, std::string word,
                std::string &previous_last_word, int i, int cnt, int id) {

    int line_number;
    std::string previous_word, next_word, current_word;
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin,end);
    line_number = i+1+cnt;
    for (int j = 0; j < vstrings.size(); j++) {
        if (vstrings[j].find(word) != std::string::npos) {
            //std::cout << vstrings[j-1].empty()   << std::endl;
            /* If the word is greater than "The chemical composition of titin" then is pointing to a NULL value */
            if (vstrings[j-1].size() > 189819) { 
                previous_word =  previous_last_word;
            } else {
                previous_word = vstrings[j-1];
            }

            
            current_word = vstrings[j];
            next_word = vstrings[j+1];
            
            //std::cout << "hola " << line_number << std::endl;

            Result result(line_number, previous_word, current_word, next_word);
            threads_info[id].addResult(result);
            
            cnt = 0;
        }
    }
    /* Return the word in the previous line */

    previous_last_word = vstrings[vstrings.size()-1];
}

/* Search the word into the file */
void search_word(std::string word, int begin, int end, int id, std::string filename) {
    //std::cout << "hilo " << id << " begin: " << begin << " end: " << end << std::endl;
    filename = DIR_BOOKS + filename;

    std::ifstream file(filename);
    std::string line, previous_last_word, previous_word, current_word, next_word;
    int line_number, number_of_lines = 0, cnt = 0;

    /* Set the getline in the right line where start reading */
    while (std::getline(file, line)) {
        if (number_of_lines == begin) {
            break;
        } else {
            number_of_lines++;
        }
    }
    /* Read line by line the file */
    for (int i = begin; i <= end; i++) {
        if (line.length() == 1) {
            std::getline(file, line);
            cnt++; /* Characters of new lines "\n" */
            if (line == "\0") { /* End of file */
                break;
            }
        }
        search_in_line(line, word, previous_last_word, i, cnt, id);
        std::getline(file, line);
    }
    file.close();
}

/* Create all threads and set the part of the file to process */
void create_and_distribute_threads(int number_threads, std::vector<std::thread> &threads, 
                    int number_of_lines, std::string word, std::string filename) {

    int size_task = number_of_lines/number_threads;

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

/* Print results, line number and words found */
void print_results(int number_threads) {
    std::cout << "\n";

    for (int i = 0; i < number_threads; i++) {
        for (int j = 0; j < threads_info[i].results.size(); j++) {
            std::cout << BHIYELLOW << "[Hilo " << threads_info[i].getId() << 
                " inicio: " << threads_info[i].getBegin()+1 << 
                " - final: " << threads_info[i].getEnd()+1 << 
                "] " << BHIWHITE << ":: " << BHICYAN << "línea " << 
                threads_info[i].results[j].line_number << BHIWHITE <<
                " :: ... " << BHIRED << threads_info[i].results[j].getPreviousWord() << 
                " " << threads_info[i].results[j].getWord() << 
                " " << threads_info[i].results[j].getNextWord() << BHIWHITE <<
                " ..." << std::endl;
        }
    }
    std::cout << "\n";
}

/* Main function */
int main(int argc, char *argv[]) {

    int number_threads, number_of_lines, size_task;
    std::string word, filename;
    std::vector<std::thread> threads;

    parse_argv(argc, argv, &filename, &word, &number_threads);
    number_of_lines = get_number_of_lines(filename);
    
    create_and_distribute_threads(number_threads, threads, number_of_lines, word, filename);
    print_results(number_threads);

    return 0;
}