#include "Result.h"

Result::Result(int line_number, std::string previous_word, 
            std::string word, std::string next_word) {
    this->m_line_number = line_number;
    this->_previous_word = previous_word;
    this->_word = word;
    this->_next_word = next_word;
}

std::string Result::getWord() {
    return this->_word;
}

std::string Result::getPreviousWord() {
    return this->_previous_word;
}

std::string Result::getNextWord() {
    return this->_next_word;
}