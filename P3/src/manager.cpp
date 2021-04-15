#include <iostream>
#include <fstream>
#include <thread>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream> 
#include <iterator>
#include <vector>

std::vector<std::string> g_dictionary;

void generate_dictionary() {
    std::string book_name = "books/prueba.txt";
    std::string line;
    std::ifstream myfile(book_name);

    while (std::getline(myfile, line)) {
        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin,end);

        for (int j = 0; j < vstrings.size(); j++) {
            if (vstrings[j].size() >= 2) {
                g_dictionary.push_back(vstrings[j]);
            }
        }
    }
}

void parse_args(int argc, char *argv[], int *number_of_customers) {
    
    if (argc != 1) {
        std::cout << "Not enough arguments, 1 arguments are needed" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    *number_of_customers = std::stoi(argv[1]);
}

void generate_threads(int number_of_customers) {
    std::vector<std::thread> threads;

    for (int i = 0; i < number_of_customers; i++) {
        threads.push_back(std::thread());
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main (int argc, char *argv[]) {
    int number_of_customers;

    parse_args(argc, argv ,&number_of_customers);
    generate_dictionary();
    generate_threads(number_of_customers);
}