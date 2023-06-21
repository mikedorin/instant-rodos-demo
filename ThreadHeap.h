
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#ifndef RODOS_KIPU2_THREADHEAP_H
#define RODOS_KIPU2_THREADHEAP_H

#include "Thread.h"

class ThreadHeap {
private:
    static int size;
public:
    static ThreadHeap * Instance();
    static ThreadHeap * instance;
    TinyHeap<Thread,MAX_THREADS>  threadHeap;
};


#endif //RODOS_KIPU2_THREADHEAP_H
