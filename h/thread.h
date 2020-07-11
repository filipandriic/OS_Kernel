#ifndef _thread_h_
#define _thread_h_
#include "locks.h"

class PCB;
class Thread {
public:
    void start();
    void waitToComplete();
    virtual ~Thread();
    ID getId();
    static ID getRunningId();
    static Thread * getThreadById(ID id);

    void signal(SignalId signal);
    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);


    void blockSignal(SignalId signal);
    static void blockSignalGlobally(SignalId signal);
    void unblockSignal(SignalId signal);
    static void unblockSignalGlobally(SignalId signal);

protected:
    friend class PCB;
    friend class userMain;
    Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    virtual void run() {}

    PCB* myPCB;
};
void dispatch ();
#endif
