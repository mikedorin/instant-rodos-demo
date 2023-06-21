// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 * Inspired by RODOS
 * API inspired by RODOS
 * https://www.montenegros.de/sergio/rodos/index.html
 *
 */

#include "Timer.h"

int Timer::timerCnt;


Timer::Timer()
{
    timerId = Timer::incTimerCnt();
    timeEvent = NULL;
    duration = 0;
    interval = 0;
}

Timer::Timer(TTime _duration)
{
    duration = _duration;
    interval = 0;
    timeEvent = NULL;
    current = TimeModel::getTicks()+duration;  //duraiton from now
    timerId = Timer::incTimerCnt();
}

Timer::Timer(TTime _duration, TTime _interval)
{
    duration = _duration;
    current = TimeModel::getTicks() + duration;
    interval = _interval;
    timeEvent = NULL;
    timerId = Timer::incTimerCnt();
}

void Timer::configure(TTime _duration, TTime _interval)
{
    duration = _duration;
    current = TimeModel::getTicks() + duration;
    interval = _interval;
    timeEvent = NULL;
}


