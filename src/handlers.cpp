#include "handlers.h"

void handlerList::push_back(SignalHandler mySH) {
    LOCKED (
        handlerNode* node = new handlerNode(mySH);

        if (head == nullptr) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    )
}

void handlerList::pop() {
    LOCKED (
        if (head) {
            handlerNode* node = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
            delete node;
        }
    )
}

void handlerList::popAll() {
    LOCKED (
        while (head) {
            pop();
        }
        head = nullptr;
        tail = nullptr;
    )
}

void handlerList::swap(SignalHandler hand1, SignalHandler hand2) {
    LOCKED (
        handlerNode* temp1 = head;
        handlerNode* temp2 = head;

        while (temp1 && temp1->sh != hand1) {
            temp1 = temp1->next;
        }
        if (temp1) {
            while (temp2 && temp2->sh != hand2) {
                temp2 = temp2->next;
            }
            if (temp2) {
                SignalHandler temp = temp1->sh;
                temp1->sh = temp2->sh;
                temp2->sh = temp;
            }
        }
    )
}