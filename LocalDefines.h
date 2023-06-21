
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */


#ifndef RODOS_KIPU2_LOCALDEFINES_H
#define RODOS_KIPU2_LOCALDEFINES_H

#include <stdio.h>
#define STM32 1

#define STACK_SIZE 500
#define MAX_TIMERS 10
#define MAX_THREADS 10
#define THREAD_DEFAULT_PRIORITY 1



typedef long long TTime;
//typedef unsigned long long uint64_t;
//typedef long long int64_t;

#define END_OF_TIME -1
#define PRINTF 
//#define ERROR 
//extern "C" int tprintf(const char *format, ...);

#define SECONDS_NOW() ((long)TimeModel::getSeconds())
#define NOW() TimeModel::getTicks()


#ifndef NULL
#define NULL 0
#endif

#ifdef LINUX

#endif

#ifdef STM32
#define putchar(c)  USART_PutChar(c)
#endif


#endif //RODOS_KIPU2_LOCALDEFINES_H
