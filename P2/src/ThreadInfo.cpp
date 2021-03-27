#include <vector>
#include "ThreadInfo.h"

ThreadInfo::ThreadInfo(int id, int begin, int end) {
    this->id = id;
    this->begin = begin;
    this->end = end;
}

int ThreadInfo::getId() {
    return this->id;
}

int ThreadInfo::getBegin() {
    return this->begin;
}

int ThreadInfo::getEnd() {
    return this->end;
}

void ThreadInfo::addResult(Result result) {
    this->results.push_back(result);
}