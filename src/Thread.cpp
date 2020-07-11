#include "thread.h"
#include "PCB.h"
#include "myIVT.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
    LOCKED (
        myPCB = new PCB(stackSize, timeSlice, this);
    )
}

Thread::~Thread() {
    LOCKED (
        waitToComplete();
        if(myPCB != nullptr) {
           myPCB->deletePCB();
           myPCB = nullptr;
        }
    )
}

ID Thread::getId() {
    ID ret = -1;
    LOCKED (
        if (myPCB != nullptr) {
            ret = myPCB->getId();
        }
    )
    return ret;
}

ID Thread::getRunningId() {
    ID ret = -1;
    LOCKED (
        if (running != nullptr) {
            ret = running->getId();
        }
    )
    return ret;
}

Thread* Thread::getThreadById(ID id) {
    Thread* ret = nullptr;
    LOCKED (
        Node* temp = PCBlist.getNodeHead();

        while (temp) {
            if (temp->pcbNode->threadID == id) {
                ret = temp->pcbNode->thread;
                break;
            }

            temp = temp->next;
        }
    )
    return ret;
}

void Thread::start() {
    LOCKED (
        myPCB->schedule();
    )
}

void Thread::waitToComplete() {
    LOCKED (
        myPCB->waitToComplete();
    )
}

void Thread::blockSignal(SignalId signal) {
    LOCKED (
        myPCB->blockSignal(signal);
    )
}
void Thread::blockSignalGlobally(SignalId signal) {
    LOCKED (
        if (signal >= 0 && signal <= 15) {
            globalSignals[signal] = 0;
        }
    )
}
void Thread::unblockSignal(SignalId signal) {
    LOCKED (
        myPCB->unblockSignal(signal);
    )
}
void Thread::unblockSignalGlobally(SignalId signal) {
    LOCKED (
        if (signal >= 0 && signal <= 15) {
            globalSignals[signal] = 1;
        }
    )
}

void Thread::signal(SignalId signal) {
    LOCKED (
        myPCB->signal(signal);
    )
}
void Thread::registerHandler(SignalId signal, SignalHandler handler) {
    LOCKED (
        myPCB->registerHandler(signal, handler);
    )
}
void Thread::unregisterAllHandlers(SignalId id) {
    LOCKED (
        myPCB->unregisterAllHandlers(id);
    )
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
    LOCKED (
        myPCB->swap(id, hand1, hand2);
    )
}


void dispatch() {
    HARD_LOCKED (
        requestChange = true;
        timerInterrupt();
    )   
}