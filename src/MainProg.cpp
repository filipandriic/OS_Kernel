#include "MainProg.h"
#include "busyPCB.h"
#include "thread.h"
#include "locks.h"
#include "myIVT.h"
#include "PCB.h"
#include "SCHEDULE.H"

void MainProgram::start() {    
    mainPCB = new PCB(0, defaultTimeSlice);
    mainPCB->state = PCB::RUNNING;
    running = mainPCB;

    busyPCB_ = new busyPCB(defaultStackSize, defaultTimeSlice);
    initInterrupts();
}

void MainProgram::finish() {
    restoreInterrupts();

    busyPCB_->deletePCB();
    busyPCB_ = nullptr;
    
    
    mainPCB->deletePCB();
    mainPCB = nullptr;
}
