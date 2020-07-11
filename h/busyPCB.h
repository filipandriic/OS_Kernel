#ifndef _BUSYPCB_H_
#define _BUSYPCB_H_

#include "PCB.h"

class busyPCB : public PCB {
public:
    busyPCB(StackSize stackSize, Time timeSlice);
    static void busyWrapper();
};

#endif