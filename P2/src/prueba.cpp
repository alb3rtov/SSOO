#include <iostream>
#include <string.h>

std::string clean_word(std::string word) {
    
    std::string fixed_word;
    
    for (int i = 0; i < word.size(); i++) {
        //std::cout << word[i] << " ";
        if (word[i] != '?' && 
            word[i] != '$' && 
            word[i] != '%' && 
            word[i] != '.' &&
            word[i] != ',' &&
            word[i] != ';' &&
            word[i] != ':' &&
            word[i] != '"' &&
            word[i] != '!') {
            
            fixed_word += word[i];
            
        }
    }
    return fixed_word;
}

int main() {
    std::string word = "hola,";
    std::string fixed_word;
    fixed_word = clean_word(word);
    std::cout << fixed_word << std::endl;
}