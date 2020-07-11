#ifndef _TIMELIST_H_
#define _TIMELIST_H_
#include "locks.h"
class PCB;

struct timeNode {
	PCB* pcb;
	Time time;
	timeNode* next;
	timeNode(PCB* pcb_, Time time_, timeNode* next_ = nullptr) : pcb(pcb_), time(time_), next(next_) {}
};
class timeList {
private:
	timeNode* head;
	timeNode* tail;
public:
	timeList() : head(nullptr), tail(nullptr) {}
	~timeList() {
        while (head) {
            pop();
        }
		head = nullptr;
		tail = nullptr;
    }
	void push_back(PCB* pcb_, Time time_);
	void pop();
    bool empty() { return !head; }
    PCB* getHead() const { return head->pcb; }
    int tickALL();

	void remove(PCB* pcb_);
};

#endif