PREFIX = arm-none-eabi-

CPP=$(PREFIX)g++
CC=$(PREFIX)gcc
AR = $(PREFIX)ar

MCFLAGS=-mcpu=cortex-m4 -mthumb -mlittle-endian \
-mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork


CFLAGS=-g -I. -I ../STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I ../STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc  \
-I ../STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include  -mfloat-abi=hard $(MCFLAGS)


DEPS = CommBuffer.h Fifo.h IdleTask.h  LocalDefines.h Semaphore.h Thread.h ThreadHeap.h  TimeEvent.h TimeModel.h Timer.h TimerHeap.h TinyHeap.h  rodos.h

OBJ = IdleTask.o  Semaphore.o Thread.o ThreadHeap.o  TimeEvent.o TimeModel.o Timer.o TimerHeap.o  


%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)


instant-rodos.a: $(OBJ) 
	$(AR) rcs $@ $^  

clean:
	rm *.o
	rm *.a
