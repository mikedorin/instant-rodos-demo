
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#ifndef RODOS_KIPU2_TIMERHEAP_H
#define RODOS_KIPU2_TIMERHEAP_H

#include "LocalDefines.h"
#include "Timer.h"



class TimerHeap {
public:
    static TimerHeap * Instance();
    TinyHeap<Timer, MAX_TIMERS>  timerHeap;
    void processTimers();
};


#endif //RODOS_KIPU2_TIMERHEAP_H
