// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */


#include "Fifo.h"//
// Created by Michael Dorin on 8/20/19.
//

#ifndef RODOS_KIPU2_WATCHER_H
#define RODOS_KIPU2_WATCHER_H

#include "Semaphore.h"
#include "TimeEvent.h"
#include "Fifo.h"

extern long systemTicks;

Semaphore onlyOne;
char globalCnt = 'A';

class Watcher : public Thread {
public:

  void run()
  {
    this->priority=9000;
    char myId;

    //distribute the ids
    onlyOne.enter(this);
    myId = globalCnt++;
      yield();
    onlyOne.leave(this);

    while(1)
    {
        onlyOne.enter(this);
        PRINTF("Only one, I am -- %c -- \n", myId);
        yield(); // Just to show that the semaphore protects
        PRINTF("time %ld %c\n", TimeModel::getTicks(), myId);
        onlyOne.leave(this);
        //printf("****\n");
    }
  }

};
//team[3];

class TimeEventTest : public TimeEvent
{
public:

    TimeEventTest() : TimeEvent()
    {
        init();
    }

    void handle ()
    {
        PRINTF (" Time Event at %d\n", SECONDS_NOW ());
    }

    void init(void)
    {
        activatePeriodic (5* SECONDS , 3* SECONDS );
    }
};
TimeEventTest te01 ;


CommBuffer<int> buf;

class Sender : public Thread
{
    void run () {
        int  cnt = 0;
        while(1)
        {
            cnt++;
            PRINTF("Combuff Writing %d\n", cnt);
            buf.put(cnt);
            AT(NOW() + 3*SECONDS);
        }
    }
};

class Receiver : public Thread
{
    void run ()
    {
        int cnt;
        while(1)
        {
            buf.get(cnt);
            PRINTF("Combuff Reading %d\n", cnt);
            AT(NOW() + 2*SECONDS);
        }
    }
};

//Sender   sender;
//Receiver receiver;


SyncFifo<int, 10> fifo;

class FifoSender : public Thread
{
    void run () {
        int  cnt = 0;
        PRINTF("sender\n");
        while(1)
        {
            cnt++;
            bool ok = fifo.put(cnt);
            if (ok)
            {
                PRINTF("Fifo Sending %d\n", cnt);
            }
            else
            {
               // PRINTF("Fifo Fifo full\n");
            }
            if ((cnt % 15) == 0)
            {
                PRINTF("Fifo waiting 3 seconds \n");
                AT(NOW() + 3*SECONDS);
                PRINTF("WAITED 3 seconds ");
            }
        }
    }
};

class FifoReceiver : public Thread
{
    void run ()
    {
        int cnt;
        PRINTF("receiver\n");

        while(1)
        {
            bool ok = fifo.get(cnt);
            if (ok)
            {
                PRINTF("Fifo reading %d\n", cnt);
            }
            else
            {
                AT(NOW() + 1*SECONDS);
            }
        }
    }
};


//FifoSender   FifoSender;
//FifoReceiver FifoReceiver;

#endif //RODOS_KIPU2_WATCHER_H
