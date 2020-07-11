#ifndef _types_h_
#define _types_h_
#include <iostream.h>
#include <dos.h>

typedef unsigned int word;
typedef unsigned long StackSize;
typedef unsigned int Time;
typedef int ID;

typedef int bool;
#define true 1
#define false 0
#define nullptr NULL

const Time defaultTimeSlice = 2;
const StackSize defaultStackSize = 4096;

class PCB;
class busyPCB;
class waitList;
class semList;
extern int tickCnt;
extern PCB* running;
extern PCB* mainPCB;
extern PCB* userPCB;
extern busyPCB* busyPCB_;
extern bool requestChange;
extern bool timedOut;
extern ID threadIDs;
extern waitList PCBlist;
extern semList SemaphorList;

typedef void interrupt(*InterruptRoutine)(...);
typedef void (*WrapperFunc)();

typedef void (*SignalHandler)();
typedef unsigned SignalId;

extern int globalSignals[16];

#endif