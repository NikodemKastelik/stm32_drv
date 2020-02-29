#ifndef _DRV_EXTI_H_
#define _DRV_EXTI_H_

#include <hal_exti.h>
#include <hal_syscfg.h>

typedef enum
{
    DRV_EXTI_LINE_PA = HAL_SYSCFG_EXTI_LINE_PA,
    DRV_EXTI_LINE_PB = HAL_SYSCFG_EXTI_LINE_PB,
    DRV_EXTI_LINE_PC = HAL_SYSCFG_EXTI_LINE_PC,
    DRV_EXTI_LINE_PD = HAL_SYSCFG_EXTI_LINE_PD,
#if defined(GPIOE)
    DRV_EXTI_LINE_PE = HAL_SYSCFG_EXTI_LINE_PE,
#endif
    DRV_EXTI_LINE_PF = HAL_SYSCFG_EXTI_LINE_PF,
} drv_exti_line_port_t;

typedef void (* drv_exti_handler_t)(uint32_t lines_triggered_mask);

void drv_exti_init(drv_exti_handler_t handler);

void drv_exti_line_alloc(uint8_t              line_idx,
                         drv_exti_line_port_t port);

void drv_exti_line_realloc(uint8_t              line_idx,
                           drv_exti_line_port_t port);

void drv_exti_line_free(uint8_t line_idx);

void drv_exti_line_trigger_set(uint8_t line_idx,
                               bool    trigger_on_rising,
                               bool    trigger_on_falling);

void drv_exti_irq_handler(uint32_t pending_mask);

#endif // _DRV_EXTI_H_
