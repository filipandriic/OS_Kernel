#include "KernSem.h"
#include "PCB.h"
#include "SCHEDULE.H"

//PROMENA --VAL I VAL++
//HARD LOK SVAKI DILIT

void KernelSem::~KernelSem() {
    LOCKED (
        while (!unlimitedWait.empty()) {
            PCB* temp = unlimitedWait.getHead();
            unlimitedWait.pop();
            temp->schedule();                
        }
        
        while (!limitedWait.empty()) {
            PCB* temp = limitedWait.getHead();
            limitedWait.pop();
            temp->schedule();
        }
    )
}

int KernelSem::wait(Time maxTimeToWait) {
    LOCKED (
        
        int ret = 1;
        
        if (--val < 0) {
            
            block(maxTimeToWait);
            
            if (running->semaphoreTime) {
                running->semaphoreTime = false;
                ret = 0;
            }
        }
        
    )
    return ret;
}

void KernelSem::block(Time maxTimeToWait) { 
    LOCKED ( 
        running->state = PCB::WAIT;

        if (maxTimeToWait == 0) {
            unlimitedWait.push_back(running);            
        }
        else {
            limitedWait.push_back(running, maxTimeToWait);            
        }
        dispatch();
    )
}

int KernelSem::signal(int n) {
    LOCKED (
        if (n < 0) return n;
        int ret = 0;
        int numUnblock;
        if (n == 0) {
            numUnblock = 1;
        }
        else {
            numUnblock = n;
        }

        val += numUnblock;
        ret = unblock(numUnblock);       
        
        if (n == 0) {
            ret = 0;
        }
    )
    return ret;
}

int KernelSem::unblock(int numUnblock) {
        int ret = 0;
        while (!unlimitedWait.empty() && numUnblock > 0) {
            PCB* temp = unlimitedWait.getHead();
            unlimitedWait.pop();
            temp->schedule();      
            numUnblock--;
            ret++;            
        }
        
        while (!limitedWait.empty() && numUnblock > 0) {
            PCB* temp = limitedWait.getHead();
            limitedWait.pop();
            temp->schedule();
            numUnblock--;
            ret++;
        }
    return ret;
}

void KernelSem::tickSem() {
    LOCKED (
        val += limitedWait.tickALL();
    )
    
}