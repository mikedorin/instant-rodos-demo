 /* 
 * Inspired by RODOS
 * API derived from RODOS
 * https://www.montenegros.de/sergio/rodos/index.html
 */


#include "TimeModel.h"

#ifdef STM32
extern "C" {
#include"stm32f4xx.h"
#include "stm32f4xx_tim.h"
//uint32_t TIM_GetCounter(TIM_TypeDef* TIMx);
}
#endif

static TTime systemTicks = 0;

TTime TimeModel::getSeconds()
{
#ifdef STM32
    systemTicks = TIM_GetCounter(TIM2);
    return systemTicks / SECONDS;
#endif
#ifdef LINUX
    return systemTicks/SECONDS;
#endif

}

TTime TimeModel::updateSystemTime()
{
#ifdef LINUX
    struct timeval tp;
    TTime  ms;
    gettimeofday(&tp,NULL);
    ms = tp.tv_sec*1000;
    systemTicks = ms ;
#endif
#ifdef STM32
    systemTicks = TIM_GetCounter(TIM2);
#endif
    return systemTicks;
}

TTime TimeModel::getTicks()
{
#ifdef LINUX
    struct timeval tp;
    TTime  ms;
    gettimeofday(&tp,NULL);
    ms = tp.tv_sec*1000;
    systemTicks = ms ;
#endif
#ifdef STM32
    systemTicks = TIM_GetCounter(TIM2);
#endif
    return systemTicks;
}
