#include "KernelEv.h"
#include "PCB.h"
#include "myIVT.h"


KernelEv::KernelEv(IVTNo ivtNo_) : ivtNo(ivtNo_), myPCB(running), mySem(0) {

    IVTEntry::ivTable[ivtNo]->myEvent = this;
}

KernelEv::~KernelEv() {
    IVTEntry::ivTable[ivtNo]->myEvent = nullptr;
}

void KernelEv::wait() {
    LOCKED (
        if (myPCB == running) {
            mySem.wait(0);
        }
    )
}

void KernelEv::signal() {
    LOCKED (
        if (mySem.val < 0) {
            mySem.signal();
        }
    )
}