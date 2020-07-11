#ifndef _KERNELEV_H_
#define _KERNELEV_H_
#include "locks.h"
#include "event.h"
#include "KernSem.h"

class PCB;

class KernelEv {
public:
    KernelEv(IVTNo ivtNo_);
    ~KernelEv();

    void wait();
    void signal();

    PCB* myPCB;
    KernelSem mySem;
    IVTNo ivtNo;
};


#endif