
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 * API from RODOS
 * Inspired by RODOS
 * https://www.montenegros.de/sergio/rodos/index.html
 *
 */

#ifndef RODOS_KIPU2_TIMER_H
#define RODOS_KIPU2_TIMER_H


#include "LocalDefines.h"
#include "rodos.h"


class Thread;




class TimeEvent;

class Timer {
private:
    TTime current;
    TTime interval;
    TTime duration;
    bool restartThread;
    Thread * thread;
    TimeEvent * timeEvent;
    static int timerCnt;

    int timerId;

public:
    static int incTimerCnt() {return Timer::timerCnt++;}
    static int getTimerCnt() {return Timer::timerCnt;}

    Timer();
    Timer(TTime duration);
    Timer(TTime duration, TTime reload);
    void configure(TTime duration, TTime reload);
    void setCurrent(TTime _current) { current = _current;}
    void setDuration(TTime _duration) { duration = _duration; }
    void setInterval(TTime _interval) { interval = _interval; }
    TTime getInterval() { return  interval;}
    TTime getDuration() { return duration; }
    TTime getCurrent() {return current;}
    Thread * getThread() {return thread;};
    void setThread(Thread * _thread) {thread = _thread;}
    void setTimeEvent(TimeEvent * _timeEvent) {timeEvent = _timeEvent;}
    TimeEvent * getTimeEvent() {return timeEvent;}
    bool restartTread() { return restartThread;}
    void enableRestart() { restartThread = true; }
    void disableRestart() { restartThread = false; }


    bool operator<(Timer other) const
    {
        return current < other.current;
    }

    bool operator>(Timer other) const
    {
        return current > other.current;
    }

    bool operator== (Timer other)
    {
        return timerId == other.timerId;
    }
};


#endif //RODOS_KIPU2_TIMER_H
