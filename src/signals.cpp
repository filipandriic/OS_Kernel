#include "signals.h"

void signalList::push_back(SignalId signal) {
    LOCKED (
        signalNode* node = new signalNode(signal);

        if (head == nullptr) {
            head = node;
        } else {
            tail->next= node;
        }
        tail = node;
    )
}

void signalList::pop() {
    LOCKED (
        if (head) {
            signalNode* node = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
            delete node;
        }
    )
}