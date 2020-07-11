#include "timeList.h"
#include "PCB.h"
#include "SCHEDULE.H"

void timeList::push_back(PCB* pcb_, Time time_) {
    LOCKED (
        timeNode* myPCB = new timeNode(pcb_, time_);
        if (head == nullptr) {
            head = tail = myPCB;
        }
        else {
            timeNode* tempNext = head;
            timeNode* tempPrev = nullptr;
            while (tempNext && myPCB->time > tempNext->time) {
                tempPrev = tempNext;
                tempNext = tempNext->next;
            }
            if (tempPrev == nullptr) {
                myPCB->next = head;
                head = myPCB;
            }
            else {
                tempPrev->next = myPCB;
                myPCB->next = tempNext;
                if (tempNext == nullptr) {
                    tail = myPCB;
                }
            }
        }
    )
}

void timeList::pop() {
    LOCKED (
        if (head) {
            timeNode* old = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
            delete old;
        }
    )
}

int timeList::tickALL() {
    int ret = 0;
    LOCKED (
        timeNode* temp = head;
        while (temp) {
            temp->time--;
            temp = temp->next;           
        }
        
        while (head->time == 0) {
            
            head->pcb->semaphoreTime = true;
            head->pcb->schedule();
            pop();
            ret++;
        }
    )
    return ret;
}

void timeList::remove(PCB* pcb_) {
    LOCKED (
        timeNode* temp = head;
        timeNode* prev = nullptr;

        while (temp && temp->pcb != pcb_) {
            prev = temp;
            temp = temp->next;
        }
        if (temp != nullptr) {

            if (temp == head) {
                head = head->next;
                
            } else if (temp == tail) {
                prev->next = nullptr;
                tail = prev;
                
            } else {
                prev->next = temp->next;
                
            }
            delete temp;
        }
    )
}