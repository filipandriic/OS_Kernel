#ifndef _MYIVT_H_
#define _MYIVT_H_

#include "locks.h"
#include "event.h"

extern InterruptRoutine oldTimerIvt;
void initInterrupts();
void restoreInterrupts();
void tick();
void interrupt timerInterrupt(...);



class IVTEntry {
public:
    IVTEntry(IVTNo ivtNo_, InterruptRoutine newIvt_);
    ~IVTEntry();
    void signal();

    void initialize();
    void restore();

    InterruptRoutine oldIvt;
    InterruptRoutine newIvt;

    static IVTEntry* ivTable[];
    IVTNo ivtNo;
    KernelEv* myEvent;
};

#define PREPAREENTRY(ivtNo, callOld)                    \
extern IVTEntry ivtEntry##ivtNo;                        \
void interrupt eventInterrupt##ivtNo(...) {             \
    ivtEntry##ivtNo.signal();                           \
    if (callOld) ivtEntry##ivtNo.oldIvt();              \
    dispatch();                                         \
}                                                       \
IVTEntry ivtEntry##ivtNo(ivtNo, eventInterrupt##ivtNo);


#endif