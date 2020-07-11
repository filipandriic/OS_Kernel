#include "types.h"
#include "PCB.h"
#include "busyPCB.h"
#include "semList.h"

PCB* running = nullptr;
PCB* mainPCB = nullptr;
PCB* userPCB = nullptr;
busyPCB* busyPCB_ = nullptr;
bool requestChange = false;
bool timedOut = false;
ID threadIDs = 0;
waitList PCBlist;
semList SemaphorList;
int tickCnt = 0;
int globalSignals[16] = { 1 };