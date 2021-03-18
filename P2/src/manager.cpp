#include <iostream>

void parse_argv(char *argv[], std::string *filename ,
            std::string *word, int *thread_number) {
    
    *filename = argv[1];
    *word = argv[2];
    *thread_number = std::stoi(argv[3]);
}


int main(int argc, char *argv[]) {

    int thread_number;
    std::string word;
    std::string filename;

    parse_argv(argv, &word, &filename, &thread_number);
    std::cout << word << std::endl;
    std::cout << filename << std::endl;
    std::cout << thread_number << std::endl;

    return 0;
}