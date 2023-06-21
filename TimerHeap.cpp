// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#include "TimerHeap.h"


static TimerHeap timerHeapInstance;

TimerHeap * TimerHeap::Instance()
{
    return &timerHeapInstance;
}
