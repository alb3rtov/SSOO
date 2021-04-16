#include <vector>
#include "ThreadInfo.h"

ThreadInfo::ThreadInfo(int id, int begin, int end) {
    this->_id = id;
    this->_begin = begin;
    this->_end = end;
}

int ThreadInfo::getId() {
    return this->_id;
}

int ThreadInfo::getBegin() {
    return this->_begin;
}

int ThreadInfo::getEnd() {
    return this->_end;
}

bool ThreadInfo::isEmpty() {
    return this->m_results.empty();
}

void ThreadInfo::addResult(Result result) {
    this->m_results.push_back(result);
}