#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream> 
#include <iterator>
#include <mutex>

#include "ThreadInfo.h"
#include "colors.h"

/* Declare global variables */
std::string DIR_BOOKS = "books/";
std::vector<ThreadInfo> g_threads_info;
std::mutex g_mutex;

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

/* Search the next word of the next line */
std::string search_next_word(std::string filename, int line_number) {
        
    int number_of_lines = 0;
    std::string next_word, line;
    std::ifstream file(filename);

    while (std::getline(file, line)) {
        if (number_of_lines == line_number) {
            break;
        } else {
            number_of_lines++;
        }
    }

    if (line.length() == 1) {
        std::getline(file, line);
    }
    
    if (line == "\0") {
            next_word = "...";
    } else {
        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin,end);

        next_word = vstrings[0];
    }
    return next_word;
}

/* Set the edges words of the found word */
std::string set_edge_words(int pos, int desp, int j, std::string edge_word, std::vector<std::string> vstrings) {
    std::string word;
    
    if (j == pos) { /* Start of the line */
        word = edge_word;
    } else {
        word = vstrings[j+desp];
    }

    return word;
}

/* Search in the given line if match with the word */
void search_in_line(std::string filename, std::string line, std::string word, std::string &next_first_word,
                std::string &previous_last_word, int i, int cnt, int id) {

    int line_number;
    std::string previous_word, current_word, next_word;
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin,end);
    line_number = i+1+cnt;

    for (int j = 0; j < vstrings.size(); j++) {
        if (vstrings[j].find(word) != std::string::npos) {

            previous_word = set_edge_words(0, -1, j, previous_last_word, vstrings);  
            current_word = vstrings[j];
            next_word = set_edge_words(vstrings.size()-1, 1, j, next_first_word, vstrings);

            Result result(line_number, previous_word, current_word, next_word);
            
            std::unique_lock<std::mutex> g_lk(g_mutex);
            g_threads_info[id].addResult(result);
            g_lk.unlock();

            cnt = 0;
        }
    }
    /* Set the last word in the previous line */
    previous_last_word = vstrings[vstrings.size()-1];
    /* Set the first word in the next line */
    next_first_word = search_next_word(filename, line_number+1);
}

/* Search the word into the file */
void search_word(std::string word, int begin, int end, int id, std::string filename) {
    filename = DIR_BOOKS + filename;

    std::ifstream file(filename);
    std::string line, previous_last_word, next_first_word;
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
        search_in_line(filename, line, word, next_first_word, previous_last_word, i, cnt, id);
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
        g_threads_info.push_back(ithread);
        threads.push_back(std::thread(search_word, word, begin, end, i, filename));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

/* Print results, line number and words found */
void print_results(int number_threads) {
    std::cout << "\n";

    for (int i = 0; i < number_threads; i++) {
        for (int j = 0; j < g_threads_info[i].m_results.size(); j++) {
            std::cout << BHIYELLOW << "[Hilo " << g_threads_info[i].getId() << 
                " inicio: " << g_threads_info[i].getBegin()+1 << 
                " - final: " << g_threads_info[i].getEnd()+1 << 
                "] " << BHIWHITE << ":: " << BHICYAN << "línea " << 
                g_threads_info[i].m_results[j].m_line_number << BHIWHITE <<
                " :: ... " << BHIRED << g_threads_info[i].m_results[j].getPreviousWord() << 
                " " << g_threads_info[i].m_results[j].getWord() << 
                " " << g_threads_info[i].m_results[j].getNextWord() << BHIWHITE <<
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