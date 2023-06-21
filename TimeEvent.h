// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

#ifndef RODOS_KIPU2_TIMEEVENT_H
#define RODOS_KIPU2_TIMEEVENT_H

#include "LocalDefines.h"

#include "Timer.h"


class TimeEvent {
private:
    Timer timer;
public:
    TimeEvent();
    void activateAt(const TTime time);
    void activatePeriodic(const TTime startAt, const TTime period);
    virtual void handle(void) { }
    virtual void init(void) { }
};


#endif //RODOS_KIPU2_TIMEEVENT_H
