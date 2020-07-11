#ifndef _KERNSEM_H_
#define _KERNSEM_H_
#include "Semaphor.h"
#include "locks.h"
#include "waitList.h"
#include "timeList.h"
#include "semList.h"

class KernelSem {
public:
    KernelSem(int init = 1) {
        if (init < 0) {
            init = 0;
        }
        val = init;
        SemaphorList.push_back(this);
    }
    ~KernelSem();
    int wait (Time maxTimeToWait);
    int signal(int n = 0);
    int val;
    
    
    void block(Time maxTimeToWait);
    int unblock(int numUnblock);
    void tickSem();
    waitList unlimitedWait;
    timeList limitedWait;
};

#endif