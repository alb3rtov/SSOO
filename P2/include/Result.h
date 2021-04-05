#include <string>

/* Class for info found that match with the word */
class Result {
private:
    std::string _previous_word;
    std::string _word;
    std::string _next_word;

public:
    int m_line_number;
    
    Result(int line_number, std::string previous_word, 
            std::string word, std::string next_word);
    std::string getWord();
    std::string getPreviousWord();
    std::string getNextWord();
};