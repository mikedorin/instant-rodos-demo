
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */


#ifndef RODOS_KIPU2_HELLOWORLD_H
#define RODOS_KIPU2_HELLOWORLD_H

#include "rodos.h"
#include "Thread.h"
class IdleTask : public Thread {
public:
    IdleTask();
    IdleTask(long priority);


public:
    void run();

};


#endif //RODOS_KIPU2_HELLOWORLD_H
