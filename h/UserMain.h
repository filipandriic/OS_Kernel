#ifndef _USERMAIN_H_
#define _USERMAIN_H_
#include "locks.h"
#include "thread.h"
#include "PCB.h"

class userThread : public Thread {
private:
    int argc;
	char **argv;
	int ret;

public:
	userThread(int argc, char** argv) : Thread() {
		this->argc = argc;
		this->argv = argv;
		ret = 0;
        userPCB = myPCB;
	}
	~userThread() {
		LOCKED (
			waitToComplete();
			if (myPCB) {
				myPCB->deletePCB();
				myPCB = nullptr;
			}
		)
	}
    int getRet() { return ret; }

protected:
    void run();
};

#endif
