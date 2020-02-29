#include <drv_common.h>
#include <drv_usart.h>
#include <drv_exti.h>

#if DRV_CHECK(DRV_CONFIG_USART1_ENABLED)
void USART1_IRQHandler(void)
{
    drv_usart_irq_handler(DRV_USART_INST1_IDX);
}
#endif

#if DRV_CHECK(DRV_CONFIG_USART2_ENABLED)
void USART2_IRQHandler(void)
{
    drv_usart_irq_handler(DRV_USART_INST2_IDX);
}
#endif

#if DRV_CHECK(DRV_CONFIG_EXTI_ENABLED)
void EXTI0_1_IRQHandler(void)
{
    drv_exti_irq_handler(hal_exti_pending_get(EXTI) & 0x3ul);
}

void EXTI2_3_IRQHandler(void)
{
    drv_exti_irq_handler(hal_exti_pending_get(EXTI) & 0xCuL);
}

void EXTI4_15_IRQHandler(void)
{
    drv_exti_irq_handler(hal_exti_pending_get(EXTI) & 0xFFF0uL);
}
#endif
