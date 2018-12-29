#ifndef _HAL_RCC_H_
#define _HAL_RCC_H_

#include <hal_common.h>

#define HAL_RCC_BUS_BASEADDR_MSK  0xFFFF0000

#define HAL_RCC_PERIPH_BASEADDR_MSK  0x0000FFFF

#define HAL_RCC_PERIPH_BASEADDR_OFFSET  0x0400

__STATIC_INLINE void hal_rcc_enable(hal_rcc_periph_t periph_msk);

__STATIC_INLINE void hal_rcc_apb_periph_enable(volatile uint32_t * p_periph);

#ifndef __MOCK_HAL

__STATIC_INLINE void hal_rcc_enable(hal_rcc_periph_t periph_msk)
{
    uint32_t bus_msk     = periph_msk >> HAL_RCC_BUS_BITPOS;
    uint32_t rcc_bit_msk = 1 << (periph_msk &  HAL_RCC_PERIPH_BITPOS_MSK);
    uint32_t volatile * rcc_reg;

    switch(bus_msk)
    {
#if defined(HAL_RCC_BUS_APB1)
        case HAL_RCC_BUS_APB1: rcc_reg = (uint32_t volatile *)&RCC->APB1ENR; break;
#endif
#if defined(HAL_RCC_BUS_APB2)
        case HAL_RCC_BUS_APB2: rcc_reg = (uint32_t volatile *)&RCC->APB2ENR; break;
#endif
#if defined(HAL_RCC_BUS_AHB)
        case HAL_RCC_BUS_AHB:  rcc_reg = (uint32_t volatile *)&RCC->AHBENR; break;
#endif
#if defined(HAL_RCC_BUS_AHB1)
        case HAL_RCC_BUS_AHB1: rcc_reg = (uint32_t volatile *)&RCC->AHB1ENR; break;
#endif
#if defined(HAL_RCC_BUS_AHB2)
        case HAL_RCC_BUS_AHB2: rcc_reg = (uint32_t volatile *)&RCC->AHB2ENR; break;
#endif
        default: rcc_reg = NULL;
    }

    ASSERT(rcc_reg);
    *rcc_reg |= rcc_bit_msk;
}

__STATIC_INLINE void hal_rcc_apb_periph_enable(volatile uint32_t * p_periph)
{
    uint32_t bus_baseaddr = (uint32_t)p_periph & HAL_RCC_BUS_BASEADDR_MSK;
    uint32_t rcc_bit_pos = ((uint32_t)p_periph & HAL_RCC_PERIPH_BASEADDR_MSK) /
                           HAL_RCC_PERIPH_BASEADDR_OFFSET;
    uint32_t volatile * rcc_reg;

    switch(bus_baseaddr)
    {
#if defined(APB1PERIPH_BASE)
        case APB1PERIPH_BASE: rcc_reg = (uint32_t volatile *)&RCC->APB1ENR; break;
#endif
#if defined(APB2PERIPH_BASE)
        case APB2PERIPH_BASE: rcc_reg = (uint32_t volatile *)&RCC->APB2ENR; break;
#endif
        default: rcc_reg = NULL;
    }

    ASSERT(rcc_reg);
    *rcc_reg |= 1 << rcc_bit_pos;
}

__STATIC_INLINE uint32_t hal_rcc_apb_clock_get(void)
{
#if defined(STM32_SERIES_F0)
    return 8000000;
#else
    return 16000000;
#endif
}

#endif // __MOCK_HAL

#endif // _HAL_RCC_H_
