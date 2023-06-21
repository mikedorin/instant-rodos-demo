cmake_minimum_required(VERSION 3.7)
project(rodos_kipu2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp InstantRodos.cpp Thread.cpp IdleTask.cpp IdleTask.h rodos.h CommBuffer.h Fifo.h TinyHeap.h ThreadHeap.cpp ThreadHeap.h Timer.cpp Timer.h TimerHeap.cpp TimerHeap.h ThreadWithTimer.cpp ThreadWithTimer.h LocalDefines.h Semaphore.cpp Semaphore.h Watcher.cpp Watcher.h TimeEvent.cpp TimeEvent.h TimeModel.cpp TimeModel.h printf.c)
add_executable(rodos_kipu2 ${SOURCE_FILES})