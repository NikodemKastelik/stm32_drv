#include <drv_common.h>
#include <drv_usart.h>

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
