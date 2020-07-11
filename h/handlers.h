#ifndef _HANDLERS_H_
#define _HANDLERS_H_
#include "locks.h"

struct handlerNode {
    SignalHandler sh;
    handlerNode* next;

    handlerNode(SignalHandler mySH) {
        sh = mySH;
        next = nullptr;
    }
};

class handlerList {
private:
    handlerNode* head;
    handlerNode* tail;
public:
    handlerList() : head(nullptr), tail(nullptr) {}
    ~handlerList() {
        while (head) {
            pop();
        }
        head = nullptr;
        tail = nullptr;
    }

    void push_back(SignalHandler mySH);
    void pop();
    void popAll();
    void swap(SignalHandler hand1, SignalHandler hand2);
    handlerNode* getHead() const { return head; }
};


#endif