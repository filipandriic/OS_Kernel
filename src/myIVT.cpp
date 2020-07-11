#include "myIVT.h"
#include "PCB.h"
#include "busyPCB.h"
#include "SCHEDULE.H"
#include "semList.h"
#include "KernelEv.h"
#include "KernSem.h"

InterruptRoutine oldTimerIvt = nullptr;
static word volatile tss;
static word volatile tsp;
static word volatile tbp;
static PCB* volatile kill = nullptr;

void initInterrupts() {
    HARD_LOCKED (
        oldTimerIvt = getvect(0x8);
        setvect(0x8, timerInterrupt);
    )
}
void restoreInterrupts() {
    HARD_LOCKED (
        setvect(0x8, oldTimerIvt);
    )
}

void interrupt timerInterrupt(...) {
    if (requestChange == false) {
        oldTimerIvt();
        tick();
        
        SemaphorList.tickAllSems();        
        /*if (lock) {
            tickCnt++;
        } else {
            while (tickCnt-- >= 0) SemaphorList.tickAllSems();
            tickCnt = 0;
        }*/

        if (running->timeLeft > 0) {
            running->timeLeft--;
            if (running->timeLeft == 0 && lock) {
                timedOut = true;
            }
        }
    }
    else if (timedOut) {
        timedOut = false;
    }

    if (requestChange || (running->unlimited == false && running->timeLeft == 0 && !lock)) {

        asm {
            mov tss, ss
            mov tsp, sp
            mov tbp, bp
        }
        running->ss = tss;
        running->sp = tsp;
        running->bp = tbp;
        running->PCBlock = lock;

        if ((running->state == PCB::RUNNING && running != busyPCB_)) {
            running->state = PCB::READY;
            Scheduler::put(running);
        }
        kill = nullptr;
        do {
            running = Scheduler::get();
            if (!running)
            {
                running = busyPCB_;
            }
            
            running->state = PCB::RUNNING;         
            running->timeLeft = running->timeSlice;
        
            tss = running->ss;
            tsp = running->sp;
            tbp = running->bp;
            asm {
                mov ss, tss
                mov sp, tsp
                mov bp, tbp
            }
            lock = running->PCBlock;

            PCB::handler();
            if (kill != nullptr) {
                PCB::killPCB(kill);
                kill = nullptr;
            }
            
            if (running->killIt == true) {
                kill = running;
            }
            
        } while (kill != nullptr);
        requestChange = false;
    }

    
}

IVTEntry* IVTEntry::ivTable[256] = { 0 };
IVTEntry::IVTEntry(IVTNo ivtNo_, InterruptRoutine newIvt_) {
    ivtNo = ivtNo_;
    newIvt = newIvt_;
    myEvent = nullptr;

    ivTable[ivtNo] = this;

    initialize();    
}

void IVTEntry::initialize() {
    HARD_LOCKED (
        oldIvt = getvect(ivtNo);
        setvect(ivtNo, newIvt);
    )
}

IVTEntry::~IVTEntry() {
    restore();
    ivTable[ivtNo] = nullptr;
}

void IVTEntry::restore() {
    HARD_LOCKED (
        setvect(ivtNo, oldIvt);
    )
}

void IVTEntry::signal() {
    if (myEvent) {
        myEvent->signal();
    }
}