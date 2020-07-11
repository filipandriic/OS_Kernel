#ifndef _SIGNALS_H_
#define _SIGNALS_H_
#include "locks.h"

struct signalNode {
    SignalId id;
    signalNode* next;

    signalNode(SignalId myID) {
        id = myID;
        next = nullptr;
    }
};

class signalList {
private:
    signalNode* head;
    signalNode* tail;
public:
    signalList() : head(nullptr), tail(nullptr) {}
    ~signalList() {
        while (head) {
            pop();
        }
        head = nullptr;
        tail = nullptr;
    }

    void push_back(SignalId signal);
    void pop();
    signalNode* getHead() const { return head; }
};


#endif