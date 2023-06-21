//
// Original RODOS
//
// https://www.montenegros.de/sergio/rodos/index.html
//
// Modified for Instant-RODOS 2019
//
//

#ifndef RODOS_KIPU2_COMMBUFFER_H
#define RODOS_KIPU2_COMMBUFFER_H

#include <csetjmp>
#include "LocalDefines.h"

template <typename Type> class CommBuffer {
private:
    jmp_buf env;
    int setjumpreturn;
    bool newDataAvailable;
    volatile bool readingNow;

    /// Data area consisting of three independent parts to avoid access conflicts.
    Type buffer[3];
    /// Pointer to buffer area that is currently written or will be written next.
    Type* writer;
    /// Pointer to buffer where last data is stored.
    Type* reader;
    /// Additional buffer, to avaoid conflicts between simultaneous read-write access.
    Type* intermediate;
public:
    long writeCnt;
    long readCnt;




public:
    CommBuffer() {
        writer = &buffer[0];
        reader = &buffer[1];
        intermediate = &buffer[2];
        readingNow = false;
        writeCnt = readCnt = 0;
        newDataAvailable = false;
    }

    void put(Type &data)
    {
        *writer = data;

        /* Swap reader <-> writer in order to make them read in the next call to get. */
        Type* swap = reader;
        reader = writer;
        writer = swap;

        /* Buffer being read now? Then take the intermediate buffer for the next write. */
        if(readingNow)
        {
            swap = writer;
            writer = intermediate;
            intermediate = swap;
        }
        writeCnt++;
        newDataAvailable = true;
    }


    void get(Type &data)
    {
        newDataAvailable = false;
        readingNow = true;
        Type* readerTmp = (Type*)reader; // This is assumed to be atomar
        data = *readerTmp;                // this is not atomar
        readingNow = false;
        readCnt++;
    }

    bool getOnlyIfNewData(Type &data) {
        if(newDataAvailable)
        {
            get(data);
            return true;
        }
        return false;
    }

};



#endif //RODOS_KIPU2_COMMBUFFER_H
