// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */


#include "IdleTask.h"
#include "Semaphore.h"

IdleTask::IdleTask():Thread(9) {
    PRINTF("CONSTRUCTING IDLE TASK\r\n");
}

IdleTask::IdleTask(long priority):Thread(priority)
{
    PRINTF("CONSTRUCTING IDLE TASK\r\n");
}


void IdleTask::run() {
    static long counter = 0;
    priority = 9000;

    while(1)
    {
        counter++;
        yield();
    }
}


