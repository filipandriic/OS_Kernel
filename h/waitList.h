#ifndef _WAITLIST_H_
#define _WAITLIST_H_
#include "locks.h"

class PCB;
struct Node {
    PCB* pcbNode;
    Node* next;

    Node(PCB* pcb_, Node* next_ = nullptr) : pcbNode(pcb_), next(next_) {}
};

class waitList {
private:
    Node* head;
    Node* tail;
    Node* current;

public:
    waitList() : head(nullptr), tail(nullptr), current(nullptr) {}
    ~waitList() {
        while (head) {
            pop();
        }
        head = nullptr;
        tail = nullptr;
        current = nullptr;       
    }
    void push_back(PCB* pcb_);
    PCB* pop();
    bool empty() {
        if (head == nullptr) {
            return true;
        }
        return false;
    }
    PCB* getHead() const { return head->pcbNode; }
    Node* getNodeHead() const { return head; }
    PCB* getTail() const { return tail->pcbNode; }
    void setCurrent() { current = head; }
    PCB* getCurrent() const { return current->pcbNode; }
    void setNext() { current = current->next; }

    void remove(PCB* pcb_);
    void removeFromWaitList(PCB* pcb_);
};

#endif