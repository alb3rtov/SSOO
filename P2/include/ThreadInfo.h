#include <vector>
#include "Result.h"

/* Class that store info for each thread */
class ThreadInfo {
private:
    int id;
    int begin;
    int end;

public:
    std::vector<Result> results;
    ThreadInfo(int id, int begin, int end);
    int getId();
    int getBegin();
    int getEnd();
    void addResult(Result result);
};
