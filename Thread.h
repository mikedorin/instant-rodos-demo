// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 * 
 *
 */


#ifndef RODOS_KIPU1_THREAD_H
#define RODOS_KIPU1_THREAD_H

#include <csetjmp>
#include "LocalDefines.h"
#include "rodos.h"
#include "Timer.h"

//class Timer;

class Thread  {

private:
    Timer timer;
    jmp_buf  context;
    static int threadCnt;
    static Thread * threadList[MAX_THREADS];
    static Thread * currentThread;
    char * stack;
    int processTimers();
    TTime ticks;

public:
    enum rodosState {READY, WAITING, IDLE, SUSPENDED};

    static int getCount() {return Thread::threadCnt;}
    static void incCount() {Thread::threadCnt++;}
    static Thread * getThread(int index);
    rodosState  state;

    Thread();
    Thread(long priority);

    void setState(rodosState _state) { state = _state; }
    void setTime(long time);
    void setPriority(long priority);
    int nextToInit();
    rodosState getState() {return state;}

    long getTime() const;
    long getPriority() const;

    void resume(); //moves this thread back into the ready queue
    bool suspendCallerUntil(const TTime reactivationTime = END_OF_TIME);


    void yield();
    void suspendUntilNextBeat();
    void setPeriodicBeat(const TTime begin, const TTime period);
    static Thread* getCurrentThread();

    void init();
    void static startThreads() {threadList[0]->init();}

    TTime  UTC2LocalTime(TTime _time);


    inline void AT(int64_t _time)     { suspendCallerUntil(_time); }
    inline void AT_UTC(int64_t _time) { suspendCallerUntil(UTC2LocalTime(_time)); }
    inline void BUSY_WAITING_UNTIL(int64_t endWaitingTime) { while(time < (endWaitingTime)) ; }


    bool operator<(Thread other) const
    {
        if (priority == other.priority)
        {
            return ticks < other.ticks;
        }
        else
        {
            return priority < other.priority;
        }
    }

    bool operator>(Thread other) const
    {
        if (priority == other.priority)
        {
            return ticks > other.ticks;
        }
        else
        {
            return priority > other.priority;
        }
    }

    bool operator==(Thread other) const
    {
        return threadId == other.threadId;
    }

    virtual void run() {};

protected:
    long time;
    int threadId;
    long priority;
};




#endif //RODOS_KIPU1_THREAD_H
