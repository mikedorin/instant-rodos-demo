
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#include "ThreadHeap.h"
#include "rodos.h"


ThreadHeap* ThreadHeap::instance = NULL;
static ThreadHeap th;

ThreadHeap * ThreadHeap::Instance()
{
    return &th;
}
