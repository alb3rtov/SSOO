#include <vector>
#include "Result.h"

/* Class that store info for each thread */
class ThreadInfo {
private:
    int _id;
    int _begin;
    int _end;

public:
    std::vector<Result> m_results;
    
    ThreadInfo(int id, int begin, int end);
    int getId();
    int getBegin();
    int getEnd();
    bool isEmpty();
    void addResult(Result result);
};
