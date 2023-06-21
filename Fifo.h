

/**
* @file fifo.h
* @date 2008/05/11 16:13
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief FIFO (first in first out) communication (header)
*
* Modified for Instant-RODOS 2019
* https://www.montenegros.de/sergio/rodos/index.html
*
*/

#pragma once
#include "Thread.h"
#include "rodos.h"


/**
*  @class Fifo
*  @brief simple FIFO (realized as ring buffer)
*
*  A fifo where both sides (reader & writer) can work asynchronously.
*  If one side has more than one owner, the fifo has to be
*  protected using priority_ceiling, or semaphores.
*
*  @param Type    data type of fifo entries
*  @param len     maximal number of entries must be at least 2
*
*/
template < typename Type, int len > class Fifo  {

protected:
    Type buffer[len];
    volatile int writeX;
    volatile int readX;

    /** advance index to next position
    * with overflow to 0 to implement a ring
    */
    int advanceIndex(int index) {
        return ((index < (len-1)) ? (index+1) : 0);
    }


public:

    Fifo() {
        readX = 0;
        writeX = 0;
    }

    /**  returns true == ok, false == fifo full */
    bool put(const Type& val) {
        int index =  advanceIndex(writeX);
        if(index == readX)
        {
            return false; /* full! */
        }

        buffer[writeX] = val;
        writeX = index;
        return true;
    }

    /** return true == ok, false = fifo empty, val not modified */
    bool get(Type& val)
    {
        if(readX == writeX)
        {
            return false;
        }
        val = buffer[readX];
        readX = advanceIndex(readX);
        return true;
    }

    int getLen()
    {
        return len;
    }

    int getElementCount()
    { ///< warning: not absolutelly threadsafe
        int r = readX;
        int w = writeX;
        return (r <= w) ? (w-r) : (len-r+w);
    }
    int getFreeSpaceCount() { ///< warning: not absolutelly threadsafe
        return len - getElementCount() - 1;
    }
    bool isFull()  { return advanceIndex(writeX)==readX; } ///< warning: not absolutelly threadsafe
    bool isEmpty() { return readX == writeX;}              ///< warning: not absolutelly threadsafe
    void clear()   { readX = writeX = 0; }                 ///< erases all content
};

/**********************************************************************/

/**
* @class SyncFifo
* @brief A synchronized fifo.
*
* Like Fifo, but it provides syncPut which block if the fifo is
* full and syncRead which block if the fifo is empty.
* Each function may have a timeout. If no timeout is given, or timeout == 0
* it will wait util the fifo is not empty/full.
* Warning:
* Only for one writer and one reader. If more than that is required it has
* to be protected using semaphores.
*
*/
template <class Type, int len> class SyncFifo : public Fifo<Type, len> {



protected:
    Thread* suspendedReader;
    Thread* suspendedWriter;

public:

    SyncFifo() {
        suspendedReader = 0;
        suspendedWriter = 0;
    }



    /**  suspends while fifo is full. returns true == ok, false == timeout, */
    bool syncPut(Type &val, const int64_t timeout = END_OF_TIME)
    {
        bool ok = false;

            ok = this->put(val);
            if (!ok)
            {
                suspendedWriter = Thread::getCurrentThread();
                switch (timeout)
                {
                    case 0:
                        break;
                    case END_OF_TIME:
                        // Thread::getCurrentThread()->suspendCallerUntil(END_OF_TIME);
                        suspendedWriter->suspendCallerUntil(END_OF_TIME);
                        break;

                    default:
                        Thread::getCurrentThread()->suspendCallerUntil(NOW() + timeout);
                        break;
                }
                suspendedWriter = 0; // after suspend, after resume
            }


        if (!ok && timeout > 0)
        {
            ok = this->put(val);
        }

        if (ok)
        {
                if(suspendedReader)
                {
                    suspendedReader->resume();
                }
        }
        return ok;
    }

    /** return true == ok, false = fifo empty, val not modified */
    bool syncGet(Type &val, const int64_t timeout = END_OF_TIME)
    {
        bool ok = false;

            ok= this->get(val);
            if (!ok)
            {
                suspendedReader = Thread::getCurrentThread();
                switch (timeout)
                {
                    case 0:
                        break;
                    case END_OF_TIME:
                        Thread::getCurrentThread()->suspendCallerUntil(END_OF_TIME);
                        break;
                    default:
                        Thread::getCurrentThread()->suspendCallerUntil(NOW() + timeout);
                }
                suspendedReader = 0;
            }


        if (!ok && timeout > 0)
        {
            ok = this->get(val);
        }

        if(ok)
        {

                if (suspendedWriter)
                {
                    suspendedWriter->resume();
                }

        }
        return ok;
    }

};


template <class Type, int len>  class BlockFifo : public Fifo<Type,len> {
protected:
    int advanceIndexEx(int index,int advanceSize)
    {
        int newIndex = index + advanceSize;
        while(newIndex >= len)
        {
            newIndex -= len;
        }
        return newIndex;
    }

public:
    BlockFifo()
    {

    }


    /// returns pointer to write and maximal number of entries you may write
    Type* getBufferToWrite(int& maxLen)
    {

        int r = this->readX;
        int w = this->writeX;

        if(r == this->advanceIndex(w))
        { //full
            maxLen =0;
            return 0;
        }

        if(r > w)
        { // put till r-1 possible
            maxLen = r - w - 1;
        } else
        { // put unitil end of buffer possible
            maxLen = len - w;
            if(r == 0)
            {
                maxLen--;
            }
        }
        return &(this->buffer[w]);
    }

    /// call this after you have written in the buffer you got from getBufferToWrite
    void writeConcluded(int numOfWrittenElements)
    {
        this->writeX = advanceIndexEx(this->writeX, numOfWrittenElements);
    }

    // returns pointer to read and maximal number of entries you may read
    Type* getBufferToRead(int& maxLen)
    {
        int r = this->readX;
        int w = this->writeX;

        if(r == w)
        { //empty
            maxLen =0;
            return 0;
        }

        if(r < w)
        { // get till w-1 possible
            maxLen = w - r;
        }
        else
        { // get unitil end of buffer possible
            maxLen = len - r;
        }
        return &(this->buffer)[r];
    }

    /// call this after you have read in the buffer you got from getBufferToRead
    void readConcluded(int sizeRed)
    {
        this->readX=advanceIndexEx(this->readX,sizeRed);
    }

};



