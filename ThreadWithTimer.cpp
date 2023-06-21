// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#include "ThreadWithTimer.h"

void ThreadWithTimer::run() {
    int counter = 1;

    PRINTF("Started, my id is %d\r\n", threadId);
    setPeriodicBeat(10 * SECONDS, 15 * SECONDS);
    PRINTF("WAITING 10 seconds\r\n");
    suspendUntilNextBeat();
    PRINTF("WAITED 10 seconds, NOW WAITING at 15 intervals \r\n");
    while(true)
    {
        suspendUntilNextBeat();
        PRINTF("Thread With Timer.  Counter = %d ID %d\r\n", counter++, threadId);
    }
}

