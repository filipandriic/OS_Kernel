#include "busyPCB.h"

busyPCB::busyPCB(StackSize stackSize, Time timeSlice) : PCB(stackSize, timeSlice) {
    state = READY;
    initStack(busyWrapper);
    for (int i = 0; i < 16; i++) {
        localSignals[i] = 0;
    }
    parent = mainPCB;
}

void busyPCB::busyWrapper() {
    while(true) {        
        //syncPrintf("GLUPI IDLE\n");        
    }
}