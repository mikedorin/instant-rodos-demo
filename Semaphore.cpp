// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 * API based derrived from RODOS
 * https://www.montenegros.de/sergio/rodos/index.html
 *
 */

#include "Semaphore.h"
#include "ThreadHeap.h"

Semaphore::Semaphore()
{
    //priorityThreadQueue =  new TinyHeap<Thread>(MAX_THREADS);
    inuse = false;
    owner = NULL;
}

void Semaphore::enter(Thread * thread)
{
    if (inuse && thread != owner)
    {
        thread->setState(Thread::WAITING);
        priorityThreadQueue.insert(thread);
        thread->yield();
    }
    inuse = true;
    owner = thread;
}

void Semaphore::leave(Thread * thread)
{
    Thread * nextThread = NULL;
    if (thread != owner)
    {
        return;
    } // only the owner can leave a semaphore

    nextThread = priorityThreadQueue.extract();

    if (nextThread && nextThread->getState() == Thread::WAITING)
    {
        owner = nextThread;
        inuse = true;
        nextThread->setState(Thread::READY);
        ThreadHeap::Instance()->threadHeap.insert(nextThread);
    }
    else
    {
        owner = NULL;
        inuse = false;
    }
}
