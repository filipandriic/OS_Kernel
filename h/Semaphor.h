#ifndef _SEMAPHOR_H_
#define _SEMAPHOR_H_
#include "locks.h"

typedef unsigned int Time;
class KernelSem;
class Semaphore {
public:
    Semaphore (int init=1);
    virtual ~Semaphore ();
    virtual int wait (Time maxTimeToWait);
    virtual int signal(int n=0);
    int val () const;
private:
    KernelSem* myImpl;
};

#endif