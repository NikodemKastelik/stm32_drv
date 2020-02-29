#ifndef _HAL_EXTI_H_
#define _HAL_EXTI_H_

#include <hal_common.h>
#include <hal_rcc.h>

#define HAL_EXTI_LINE_COUNT 16

__STATIC_INLINE void hal_exti_int_set(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE void hal_exti_int_enable(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE void hal_exti_int_disable(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE void hal_exti_rising_trigger_set(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE uint32_t hal_exti_rising_trigger_get(EXTI_TypeDef const * p_exti);

__STATIC_INLINE void hal_exti_rising_trigger_enable(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE void hal_exti_rising_trigger_disable(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE void hal_exti_falling_trigger_set(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE uint32_t hal_exti_falling_trigger_get(EXTI_TypeDef const * p_exti);

__STATIC_INLINE void hal_exti_falling_trigger_enable(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE void hal_exti_falling_trigger_disable(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE uint32_t hal_exti_pending_get(EXTI_TypeDef * p_exti);

__STATIC_INLINE void hal_exti_pending_clear(EXTI_TypeDef * p_exti, uint32_t mask);

__STATIC_INLINE uint32_t hal_exti_pending_get_and_clear(EXTI_TypeDef * p_exti);

#ifndef __MOCK_HAL

__STATIC_INLINE void hal_exti_int_set(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->IMR = mask;
}

__STATIC_INLINE void hal_exti_int_enable(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->IMR |= mask;
}

__STATIC_INLINE void hal_exti_int_disable(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->IMR &= ~mask;
}

__STATIC_INLINE void hal_exti_rising_trigger_set(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->RTSR = mask;
}

__STATIC_INLINE uint32_t hal_exti_rising_trigger_get(EXTI_TypeDef const * p_exti)
{
    return p_exti->RTSR;
}

__STATIC_INLINE void hal_exti_rising_trigger_enable(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->RTSR |= mask;
}

__STATIC_INLINE void hal_exti_rising_trigger_disable(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->RTSR &= ~mask;
}

__STATIC_INLINE void hal_exti_falling_trigger_set(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->FTSR = mask;
}

__STATIC_INLINE uint32_t hal_exti_falling_trigger_get(EXTI_TypeDef const * p_exti)
{
    return p_exti->FTSR;
}

__STATIC_INLINE void hal_exti_falling_trigger_enable(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->FTSR |= mask;
}

__STATIC_INLINE void hal_exti_falling_trigger_disable(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->FTSR &= ~mask;
}

__STATIC_INLINE uint32_t hal_exti_pending_get(EXTI_TypeDef * p_exti)
{
    return p_exti->PR;
}

__STATIC_INLINE void hal_exti_pending_clear(EXTI_TypeDef * p_exti, uint32_t mask)
{
    p_exti->PR = mask;
}

__STATIC_INLINE uint32_t hal_exti_pending_get_and_clear(EXTI_TypeDef * p_exti)
{
    uint32_t mask = hal_exti_pending_get(p_exti);
    hal_exti_pending_clear(p_exti, mask);
    return mask;
}

#endif // __MOCK_HAL

#endif // _HAL_EXTI_H_
