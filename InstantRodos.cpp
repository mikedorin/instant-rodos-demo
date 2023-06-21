// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#include "InstantRodos.h"
#include "ThreadWithTimer.h"
#include "IdleTask.h"
extern int tprintf(const char *format, ...) ;


int InstantRodos::instantMain()
{

    ThreadWithTimer twt;
    ThreadWithTimer tw2;
    IdleTask  hw,hw2 ;

    Thread::startThreads();
    while(true) { PRINTF("NEVER GET HERE");}
    return 1;
}

