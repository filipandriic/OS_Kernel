#include "Semaphor.h"
#include "KernSem.h"

Semaphore::Semaphore(int init) {
    LOCKED (
        myImpl = new KernelSem(init);
    )
}

Semaphore::~Semaphore() {
    LOCKED (
        delete myImpl;
    )
}

int Semaphore::wait(Time maxTimeToWait) {
    LOCKED (
        int ret = myImpl->wait(maxTimeToWait);
    )
    return ret;
}

int Semaphore::signal(int n) {
    LOCKED (
        int ret =  myImpl->signal(n);
    )
    return ret;
}

int Semaphore::val() const {
    LOCKED(
        int ret = myImpl->val;
    )
    return ret;
}