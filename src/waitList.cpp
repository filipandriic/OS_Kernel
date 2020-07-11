#include "waitList.h"
#include "PCB.h"

void waitList::push_back(PCB* pcb_) {
    LOCKED (
        Node* node = new Node(pcb_);
        if (head == nullptr) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    )
}

PCB* waitList::pop() {
    LOCKED (
        PCB* ret = nullptr;
        if (head) {
            ret = head->pcbNode;
            Node* old = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
                current = nullptr;
            }
            delete old;
        }
    )
    return ret;
}

void waitList::remove(PCB* pcb_) {
    LOCKED (
        Node* temp = head;
        Node* prev = nullptr;

        while (temp) {
            if (temp->pcbNode->parent == pcb_) {
                temp->pcbNode->parent = nullptr;
            }
            
            temp = temp->next;
        }

        temp = head;
        while (temp && temp->pcbNode != pcb_) {
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

void waitList::removeFromWaitList(PCB* pcb_) {
    LOCKED (
        Node* temp = head;

        while (temp) {
            if (temp->pcbNode != pcb_) {
                temp->pcbNode->wait.remove(pcb_);
            }
            temp = temp->next;
        }
    )
}