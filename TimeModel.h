
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 * API from RODOS
 * Inspired by RODOS
 * https://www.montenegros.de/sergio/rodos/index.html
 *
 */

#ifndef RODOS_KIPU2_TIMEMODEL_H
#define RODOS_KIPU2_TIMEMODEL_H

#include <sys/time.h>
#include "LocalDefines.h"


#ifdef STM32
#define NANOSECONDS    1LL
#define MICROSECONDS   1/1000  //(1000LL * NANOSECONDS)
#define MILLISECONDS   1       //(1000LL * MICROSECONDS)
#define SECONDS        (1000LL * MILLISECONDS)
#define MINUTES        (60LL * SECONDS)
#endif

#ifdef LINUX
#define MILLISECONDS   1       //(1000LL * MICROSECONDS)
#define SECONDS        (1000LL * MILLISECONDS)
#define MINUTES        (60LL * SECONDS)
#endif

class TimeModel {
private:


public:

    static TTime getSeconds();
    static TTime getTicks();
    static TTime updateSystemTime();
};


#endif //RODOS_KIPU2_TIMEMODEL_H
