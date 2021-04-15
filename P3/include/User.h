#include <string>

class User {
private:
    std::string _word;
    int _identifier;

public:
    User(std::string word, int identifier);
    std::string getWord();
    int getIdentifier();
};