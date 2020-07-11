#include "PCB.h"
#include "thread.h"
#include "SCHEDULE.H"
#include "semList.h"
#include "myIVT.h"
#include "KernelEv.h"

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread) {
    LOCKED (
        state = NEW;
        threadID = threadIDs++;
        PCBlock = 0;
        this->timeSlice = timeSlice;
        this->timeLeft = timeSlice;
        if (timeSlice == 0) {
            unlimited = true;
        } else {
            unlimited = false;
        }
        semaphoreTime = false;
        
        if (stackSize > 0x1000) {
            stackSize = 0x1000;
        }
        sizeStack = stackSize / sizeof(word);
        this->thread = thread;
        
        killIt = false;
        /*init stack*/
        if (thread != nullptr) { 
            initStack(wrapper);
            parent = running;
            localSignals[0] = 1;
            handlers[0] = parent->handlers[0];
            for (int i = 1; i < 16; i++) {
                localSignals[i] = parent->localSignals[i];
                handlers[i] = parent->handlers[i];
            }
            signals = parent->signals;
            
        } else {
            parent = nullptr;
            localSignals[0] = 0;
            for (int i = 1; i < 16; i++) {
                localSignals[i] = 1;
            }
        }

        PCBlist.push_back(this);
    )
}

void PCB::initStack(WrapperFunc wrapper) {
    stack = new word[sizeStack];
    stack[sizeStack - 1] = 0x0200;
    stack[sizeStack - 2] = FP_SEG(wrapper);
    stack[sizeStack - 3] = FP_OFF(wrapper);

    ss = FP_SEG(stack + sizeStack - 12);
    sp = bp = FP_OFF(stack + sizeStack - 12);
}

PCB::~PCB() {
    this->scheduleAll();

    LOCKED (
        this->state = FINISHED;
        PCBlist.remove(this);
        PCBlist.removeFromWaitList(this);
        SemaphorList.remove(this);
        for (int i = 0; i < 256; i++) {
            KernelEv* ev = IVTEntry::ivTable[i]->myEvent;
            if (ev != nullptr) {
                ev->mySem.unlimitedWait.remove(this);
                ev->mySem.limitedWait.remove(this);
            }
        }
        parent = nullptr;
        if (stack) {
            delete[] stack;
            stack = nullptr;
        }
    )
}

void PCB::wrapper() {
    running->thread->run();

    LOCKED (
        running->removeFromLists();
        if (running->parent) {
            running->parent->signal(1);
        }
        
        running->signal(2);
        handler();
        running->state = FINISHED;

        dispatch();
    )
}

void PCB::schedule() {
    LOCKED (
        state = READY;       
        if (this != busyPCB_)
        {
            /*cout << endl << "PCB PUT ID : " << this->threadID << endl;*/
            Scheduler::put(this);
        } 
    )
}

void PCB::scheduleAll() {
    LOCKED (
        while (!this->wait.empty()) {
            PCB* temp = this->wait.getHead();
            this->wait.pop();          
            temp->schedule();            
        }
    )
}

bool PCB::deadlock() {
    bool ret = false;
    LOCKED(

        running->wait.setCurrent();
        PCB* temp = running->wait.getCurrent();

        while (temp) {
            /*moze i sa threadID i ovako*/
            if (temp == this) {
                ret = true;
                break;
            }

            running->wait.setNext();
            temp = running->wait.getCurrent();
        }

    )
    return ret;
}

void PCB::waitToComplete() {
    LOCKED (
        
        if (running != this && state != FINISHED && this != busyPCB_ && !deadlock()) {
            running->state = WAIT;
            wait.push_back(running);
            dispatch();
        }
    )
}

void PCB::blockSignal(SignalId signal) {
    LOCKED (
        if (signal >= 0 && signal <= 15) {
            localSignals[signal] = 0;
        }
    )
}

void PCB::unblockSignal(SignalId signal) {
    LOCKED (
        if (signal >= 0 && signal <= 15) {
            localSignals[signal] = 1;
        }
    )
}

void PCB::signal(SignalId signal) {
    LOCKED (
        if (signal >= 0 && signal <= 15) {
            signals.push_back(signal);
        }
    )
}
void PCB::registerHandler(SignalId signal, SignalHandler handler) {
    LOCKED (
        if (signal >= 0 && signal <= 15) {
            handlers[signal].push_back(handler);
        }
    )
}
void PCB::unregisterAllHandlers(SignalId id) {
    LOCKED (
        if (id >= 0 && id <= 15) {
            handlers[id].popAll();
        }
    )
}
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
    LOCKED (
        if (id >= 0 && id <= 15) {
            handlers[id].swap(hand1, hand2);
        }
    )
}

void PCB::handler() {
    LOCKED (
        signalNode* tempS = running->signals.getHead();

        while (tempS) {
            if (tempS->id == 0) {
                running->killIt = true;
            }
            else {
                handlerNode* tempH = running->handlers[tempS->id].getHead();
                while (tempH) {
                    (*(tempH->sh))();
                    tempH = tempH->next;
                }

                
            }
            tempS = tempS->next;
            running->signals.pop();
        }
    )
}

void PCB::killPCB(PCB* toKill) {
    LOCKED (
        if (toKill) {
            toKill->removeFromLists();
            if (toKill->parent) {
                toKill->parent->signal(1);
            }
            toKill->signal(2);
            toKill->state = FINISHED;
            toKill = nullptr;
        }
    )
}

void PCB::removeFromLists() {
    LOCKED (
        this->scheduleAll();
        PCBlist.remove(this);
        PCBlist.removeFromWaitList(this);
        SemaphorList.remove(this);
        for (int i = 0; i < 256; i++) {
            KernelEv* ev = IVTEntry::ivTable[i]->myEvent;
            if (ev != nullptr) {
                ev->mySem.unlimitedWait.remove(this);
                ev->mySem.limitedWait.remove(this);
            }
        }
    )
}


void PCB::deletePCB() {

    LOCKED (
        if (this->parent) {
            this->parent->signal(1);
        }
        this->signal(2);

        this->state = FINISHED;
        this->removeFromLists();

        if (stack) {
            delete[] stack;
            stack = nullptr;
        }
    )
}

ID PCB::getId() {
    ID ret = -1;
    LOCKED (
        ret = threadID;
    )
    return ret;
}
