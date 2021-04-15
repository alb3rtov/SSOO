#include "User.h"

User::User(std::string word, int identifier) {
    this->_word = word;
    this->_identifier = identifier;
}

std::string User::getWord() {
    return this->_word;
}

int User::getIdentifier() {
    return this->_identifier;
}