// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 
 *
 * API and inspiration derived from RODOS
 * https://www.montenegros.de/sergio/rodos/index.html
 *
 */

#pragma GCC optimize("O0")
#include "Thread.h"
#include "ThreadHeap.h"
#include "TimerHeap.h"
#include "TimeEvent.h"



int Thread::threadCnt;
Thread * Thread::threadList[MAX_THREADS];
Thread * Thread::currentThread;


Thread::Thread() {
    threadId = Thread::getCount();
    Thread::incCount();
    threadList[threadId] = this;
    ThreadHeap * th =  ThreadHeap::Instance();
    priority = THREAD_DEFAULT_PRIORITY;
    //tprintf("NEW THREAD %d \r\n", threadId);
    th->threadHeap.insert(this);
}

Thread::Thread(long _priority) {
    threadId = Thread::getCount();
    Thread::incCount();
    threadList[threadId] = this;
    priority = _priority;
    ThreadHeap * th =  ThreadHeap::Instance();
    th->threadHeap.insert(this);
}

int Thread::nextToInit() {
    int i=threadId+1;
    return (i<Thread::getCount()) ? i:0;
}

Thread * Thread::getThread(int index)
{
    return threadList[index];
}

Thread * Thread::getCurrentThread() {
    return currentThread;
}


void Thread::init()
{
    volatile char stackForPredecssor[STACK_SIZE];
    int nextToInit;
    for(int i=0;i<STACK_SIZE;i++)
    {
        stackForPredecssor[i] = (char) threadId;
    }
    stack = (char *)&stackForPredecssor[0];
    nextToInit = this->nextToInit();
    state = READY;
    if(!setjmp(context))
    {
        if(nextToInit !=0)
        {
            Thread * initThread = Thread::threadList[nextToInit];
            initThread->init();
        }
    }
    while(1)
    {
        run();
    }
}


void Thread::setTime(long time) {
    Thread::time = time;
}

void Thread::setPriority(long priority) {
    Thread::priority = priority;
}

long Thread::getTime() const {
    return time;
}

long Thread::getPriority() const {
    return priority;
}



void Thread::setPeriodicBeat(const TTime begin, const TTime period)
{
    TimerHeap * th = TimerHeap::Instance();
    timer.configure(begin, period);
    timer.setThread(this);
    timer.disableRestart();
    th->timerHeap.insert(&timer);
}

void Thread::suspendUntilNextBeat()
{
    if (timer.getInterval() != 0)
    {
        timer.enableRestart();
        state = WAITING;
    }
    yield();

}

bool Thread::suspendCallerUntil(const TTime reactivationTime) {
    if (reactivationTime == END_OF_TIME)
    {
        setState(Thread::SUSPENDED);
    }
    else
    {
        setState(Thread::WAITING);
    }
    timer.setCurrent(reactivationTime);
    timer.enableRestart();
    timer.setThread(this);
    TimerHeap::Instance()->timerHeap.insert(&timer);
    yield();
    return true;
}



void Thread::resume()
{
    ThreadHeap * readyQueue =  ThreadHeap::Instance();
    state=READY;
    readyQueue->threadHeap.insert(this);
}

void Thread::yield()
{
    ThreadHeap * readyQueue =  ThreadHeap::Instance();
    Thread * kipuThread;
    static TTime  systemTicks, lastSystemTicks;

    kipuThread = this;
    static int val=1;
    static long long startTicks = 0;
    static long long endTicks;

    endTicks = TimeModel::getTicks();

    //* if this thread is still ready, return it to the ready queue before
    // returning
    if (state == READY)
    {
        readyQueue->threadHeap.insert(kipuThread);
    }

    systemTicks = TimeModel::updateSystemTime();

    if (systemTicks != lastSystemTicks)
    {
        processTimers();
        lastSystemTicks = systemTicks;
    }

    ticks = endTicks - startTicks;

    if (!setjmp(context))
    {
        kipuThread = readyQueue->threadHeap.extract();
        startTicks = TimeModel::getTicks();

        if (kipuThread != NULL)  // remember, we always need an idle task
        {
            longjmp(kipuThread->context,val++);
        }
    }

}

int Thread::processTimers()
{
    TimerHeap * timerHeap = TimerHeap::Instance();
    ThreadHeap * readyQueue =  ThreadHeap::Instance();

    Timer * timer;

    static long time;
    TTime systemTicks;

    systemTicks = TimeModel::updateSystemTime();
    timer = timerHeap->timerHeap.peek();

    while (timer != NULL)
    {
        if (timer->getCurrent() <= systemTicks)
        {
            timer = timerHeap->timerHeap.extract();
            if (timer->restartTread())
            {
                Thread * thread = timer->getThread();
                if (thread == NULL)
                {
                    continue;
                }
                readyQueue->threadHeap.insert(thread);
            }
            else
            {
                TimeEvent *timeEvent = timer->getTimeEvent();
                if (timeEvent != NULL)
                {
                    timeEvent->handle();
                }
            }
            if (timer->getInterval())  // check if periodic timer
            {
                timer->setCurrent(timer->getInterval() + systemTicks);
                timerHeap->timerHeap.insert(timer);
            }
        }
        else
        {
            break;
        }
        timer = timerHeap->timerHeap.peek();
    }
    return 1;
}


