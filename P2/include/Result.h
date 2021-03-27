#include <string>

/* Class for info found that match with the word */
class Result {
private:
    std::string previous_word;
    std::string word;
    std::string next_word;

public:
    int line_number;
    Result(int line_number, std::string previous_word, 
            std::string word, std::string next_word);
    std::string getWord();
    std::string getPreviousWord();
    std::string getNextWord();
};