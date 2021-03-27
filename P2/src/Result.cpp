#include "Result.h"

Result::Result(int line_number, std::string previous_word, 
            std::string word, std::string next_word) {
    this->line_number = line_number;
    this->previous_word = previous_word;
    this->word = word;
    this->next_word = next_word;
}

std::string Result::getWord() {
    return this->word;
}

std::string Result::getPreviousWord() {
    return this->previous_word;
}

std::string Result::getNextWord() {
    return this->next_word;
}