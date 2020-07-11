#ifndef _PCB_h_
#define _PCB_h_

#include "locks.h"
#include "waitList.h"
#include "signals.h"
#include "handlers.h"

class Thread;

class PCB {
public:
    enum States {NEW, RUNNING, READY, WAIT, FINISHED};
    
    Thread* thread;
    ID threadID;
    //lock
    word PCBlock;

    //stack
    StackSize sizeStack;
    word sp, ss, bp;
    word* stack;

    //time
    Time timeSlice;
    Time timeLeft;
    StackSize stackSize;
    States state;
    bool unlimited;
    static void wrapper();

    //thread functions
    ID getId();

    //deadlock fun
    bool deadlock();

    //waiting list
    waitList wait;

    //semaphore
    bool semaphoreTime;
    
    //signals
    void signal(SignalId signal);
    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

    void blockSignal(SignalId signal);
    void unblockSignal(SignalId signal);
    static void handler();
    bool killIt;
    int localSignals[16];
    PCB* parent;
    handlerList handlers[16];
    signalList signals;
    
    

    void removeFromLists();
    void deletePCB();
    static void killPCB(PCB* toKill);

    PCB(StackSize stackSize, Time timeSlice, Thread* thread = nullptr);

    ~PCB();
    void initStack(WrapperFunc wrapper);
    void schedule();
    void waitToComplete();
    void scheduleAll();
};

#endif