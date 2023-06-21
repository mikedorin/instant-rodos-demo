
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 * Inspired by RODOS
 * API Derived from RODOS
 * https://www.montenegros.de/sergio/rodos/index.html
 *
 */

#include "TimeEvent.h"
#include "TimerHeap.h"
#include "rodos.h"

TimeEvent::TimeEvent()
{
    //call local init();
}

void TimeEvent::activateAt(const TTime time)
{
    TimerHeap * th = TimerHeap::Instance();
    timer.disableRestart();
    timer.setTimeEvent(this);
    th->timerHeap.insert(&timer);
}

void TimeEvent::activatePeriodic(const TTime startAt, const TTime period)
{
    TimerHeap * th = TimerHeap::Instance();
    timer.configure(startAt, period);
    timer.disableRestart();
    timer.setTimeEvent(this);
    th->timerHeap.insert(&timer);
    PRINTF("HEAP DONE\r\n");
}

