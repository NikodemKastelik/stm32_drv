#ifndef _HAL_DWT_H_
#define _HAL_DWT_H_

#include <hal_common.h>

__STATIC_INLINE void hal_dwt_init()
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

__STATIC_INLINE void hal_dwt_delay_us(uint32_t us)
{
    uint32_t target_tick = DWT->CYCCNT + us * (16000000 / 1000000);
    while (DWT->CYCCNT <= target_tick);
}

#endif // _HAL_DWT_H_
