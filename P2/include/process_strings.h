#include <string>
#include <algorithm>

/* Convert the word to lowercase and remove accents */
std::string convert_to_lowercase(std::string word) {
    std::string prueba = word;

    std::for_each(word.begin(), word.end(), [](char & c) {
            c = ::tolower(c);
    });
    return word;
}

/* Clean a given word of special characters */
std::string clean_word(std::string word) {    
    std::string fixed_word;
    
    for (int i = 0; i < word.size(); i++) {
        if (word[i] != '?' && 
            word[i] != '$' && 
            word[i] != '%' && 
            word[i] != '.' &&
            word[i] != ',' &&
            word[i] != ';' &&
            word[i] != ':' &&
            word[i] != '"' &&
            word[i] != '!' &&
            int(word[i] != -65) && /* open question mark */
            int(word[i] != -95) && /* open exclamation mark */
            int(word[i] != -85) && /* open special quotation mark */
            int(word[i] != -69)) /* close special quotation mark */
        {
            fixed_word += word[i];
        }
    }
    return fixed_word;
}

/* Compare two strings and returns boolean */
bool compare_strings(std::string selected_word, std::string fixed_word) {
    bool equal = false;

    std::string cleaned_word = clean_word(selected_word);

    if (cleaned_word.compare(fixed_word) == 0) {
        equal = true;
    }
    return equal;
}
