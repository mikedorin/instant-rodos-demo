
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#ifndef RODOS_KIPU2_SEMAPHORE_H
#define RODOS_KIPU2_SEMAPHORE_H

#include "Thread.h"
// #include <mach/boolean.h>
#include "rodos.h"

class Semaphore {
private:
    bool inuse;
    Thread * volatile owner;
    int size;
    TinyHeap<Thread, MAX_THREADS>  priorityThreadQueue;


public:
    Semaphore();
    void enter(Thread * thread);
    void leave(Thread * leave);
};




#endif //RODOS_KIPU2_SEMAPHORE_H
