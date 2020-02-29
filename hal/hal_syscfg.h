#ifndef _HAL_SYSCFG_H_
#define _HAL_SYSCFG_H_

#include <hal_common.h>
#include <hal_rcc.h>

#define HAL_SYSCFG_EXTICR_BITS_MSK 0x0F

#define HAL_SYSCFG_EXTICR_BITS_CNT 4

typedef enum
{
    HAL_SYSCFG_EXTI_LINE_PA = SYSCFG_EXTICR1_EXTI0_PA,
    HAL_SYSCFG_EXTI_LINE_PB = SYSCFG_EXTICR1_EXTI0_PB,
    HAL_SYSCFG_EXTI_LINE_PC = SYSCFG_EXTICR1_EXTI0_PC,
    HAL_SYSCFG_EXTI_LINE_PD = SYSCFG_EXTICR1_EXTI0_PD,
#if defined(SYSCFG_EXTICR1_EXTI0_PE)
    HAL_SYSCFG_EXTI_LINE_PE = SYSCFG_EXTICR1_EXTI0_PE,
#endif
    HAL_SYSCFG_EXTI_LINE_PF = SYSCFG_EXTICR1_EXTI0_PF,
} hal_syscfg_exti_port_t;

__STATIC_INLINE void hal_syscfg_init(SYSCFG_TypeDef * p_syscfg);

__STATIC_INLINE void hal_syscfg_exti_line_cfg(SYSCFG_TypeDef *       p_syscfg,
                                              uint8_t                line_idx,
                                              hal_syscfg_exti_port_t port);

__STATIC_INLINE void hal_syscfg_exti_line_recfg(SYSCFG_TypeDef *       p_syscfg,
                                                uint8_t                line_idx,
                                                hal_syscfg_exti_port_t port);

#ifndef __MOCK_HAL

__STATIC_INLINE void hal_syscfg_init(SYSCFG_TypeDef * p_syscfg)
{
    hal_rcc_apb_periph_enable(RCC, p_syscfg);
}

__STATIC_INLINE void hal_syscfg_exti_line_cfg(SYSCFG_TypeDef *       p_syscfg,
                                              uint8_t                line_idx,
                                              hal_syscfg_exti_port_t port)
{
    uint32_t line_reg_idx = line_idx / HAL_SYSCFG_EXTICR_BITS_CNT;
    uint32_t line_pos     = (line_idx % HAL_SYSCFG_EXTICR_BITS_CNT) * HAL_SYSCFG_EXTICR_BITS_CNT;
    p_syscfg->EXTICR[line_reg_idx] |= (port << line_pos);
}

__STATIC_INLINE void hal_syscfg_exti_line_recfg(SYSCFG_TypeDef *       p_syscfg,
                                                uint8_t                line_idx,
                                                hal_syscfg_exti_port_t port)
{
    uint32_t line_reg_idx = line_idx / HAL_SYSCFG_EXTICR_BITS_CNT;
    uint32_t line_nibble  = line_idx % HAL_SYSCFG_EXTICR_BITS_CNT;
    uint32_t line_pos     = line_nibble * HAL_SYSCFG_EXTICR_BITS_CNT;
    uint32_t exticr = p_syscfg->EXTICR[line_reg_idx] &
                      ~(HAL_SYSCFG_EXTICR_BITS_MSK << line_pos);
    p_syscfg->EXTICR[line_reg_idx] = exticr | (port << line_pos);
}

#endif // __MOCK_HAL

#endif // _HAL_SYSCFG_H_
