#ifndef _HAL_USART_H_
#define _HAL_USART_H_

#include <hal_common.h>
#include <hal_rcc.h>

#if defined(USART_RDR_RDR) && defined(USART_TDR_TDR)
    #define HAL_USART_TX_REG TDR
    #define HAL_USART_RX_REG RDR
#elif defined(USART_DR_DR)
    #define HAL_USART_TX_REG DR
    #define HAL_USART_RX_REG DR
#endif

#if defined(USART_ISR_RXNE_Msk)
    #define HAL_USART_STATUS_REG ISR
#elif defined(USART_SR_RXNE_Msk)
    #define HAL_USART_STATUS_REG SR
#endif

typedef enum
{
    HAL_USART_BAUD_9600   = 9600U,
    HAL_USART_BAUD_19200  = 19200U,
    HAL_USART_BAUD_38400  = 38400U,
    HAL_USART_BAUD_56700  = 56700U,
    HAL_USART_BAUD_115200 = 115200U,
} hal_usart_baud_t;

typedef enum
{
    HAL_USART_MODE_RX   = USART_CR1_RE_Msk,
    HAL_USART_MODE_TX   = USART_CR1_TE_Msk,
    HAL_USART_MODE_RXTX = USART_CR1_RE_Msk | USART_CR1_TE_Msk
} hal_usart_mode_t;

typedef enum
{
    HAL_USART_STOPBITS_ONE = 0,
    HAL_USART_STOPBITS_TWO = USART_CR2_STOP_1
} hal_usart_stopbits_t;

typedef enum
{
    HAL_USART_PARITY_DISABLED = 0,
    HAL_USART_PARITY_EVEN     = USART_CR1_PCE_Msk,
    HAL_USART_PARITY_ODD      = USART_CR1_PCE_Msk | USART_CR1_PS_Msk
} hal_usart_parity_t;

#define HAL_USART_PARITY_Msk  (USART_CR1_PCE_Msk | USART_CR1_PS_Msk)

typedef enum
{
    HAL_USART_HFCTRL_DISABLED = 0,
    HAL_USART_HFCTRL_ENABLED  = USART_CR3_CTSE_Msk | USART_CR3_RTSE_Msk
} hal_usart_hfctrl_t;

typedef enum
{
    HAL_USART_LENGTH_8BIT = 0,
    HAL_USART_LENGTH_9BIT = USART_CR1_M_Msk
} hal_usart_len_t;

typedef enum
{
    HAL_USART_CLK_POL0_PHASE0 = 0,
    HAL_USART_CLK_POL1_PHASE0 = USART_CR2_CPOL_Msk,
    HAL_USART_CLK_POL0_PHASE1 = USART_CR2_CPHA_Msk,
    HAL_USART_CLK_POL1_PHASE1 = USART_CR2_CPOL_Msk | USART_CR2_CPHA_Msk,
} hal_usart_clk_t;

typedef enum
{
    HAL_USART_INT_RX = USART_CR1_RXNEIE_Msk,
    HAL_USART_INT_TX = USART_CR1_TXEIE_Msk,
    HAL_USART_INT_TC = USART_CR1_TCIE_Msk,
} hal_usart_int_t;

typedef enum
{
#if defined(USART_ISR_RXNE_Msk)
    HAL_USART_EVT_RX      = USART_ISR_RXNE_Msk,
    HAL_USART_EVT_TX      = USART_ISR_TXE_Msk,
    HAL_USART_EVT_TC      = USART_ISR_TC_Msk,
    HAL_USART_EVT_OVERRUN = USART_ISR_ORE_Msk
#elif defined(USART_SR_RXNE_Msk)
    HAL_USART_EVT_RX      = USART_SR_RXNE_Msk,
    HAL_USART_EVT_TX      = USART_SR_TXE_Msk,
    HAL_USART_EVT_TC      = USART_SR_TC_Msk,
    HAL_USART_EVT_OVERRUN = USART_SR_ORE_Msk
#endif
} hal_usart_evt_t;

__STATIC_INLINE void hal_usart_init(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_uninit(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_baudrate_set(USART_TypeDef * p_usart, hal_usart_baud_t baud);

__STATIC_INLINE void hal_usart_parity_set(USART_TypeDef * p_usart, hal_usart_parity_t parity);

__STATIC_INLINE void hal_usart_stopbits_set(USART_TypeDef * p_usart, hal_usart_stopbits_t stopbits);

__STATIC_INLINE void hal_usart_start(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_stop(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_tx_enable(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_tx_disable(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_rx_enable(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_rx_disable(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_hfctrl_enable(USART_TypeDef * p_usart);

__STATIC_INLINE void hal_usart_cfg(USART_TypeDef *     p_periph,
                                  hal_usart_baud_t     baud,
                                  hal_usart_stopbits_t stopbits,
                                  hal_usart_parity_t   parity);

__STATIC_INLINE void hal_usart_int_enable(USART_TypeDef * p_usart, hal_usart_int_t int_msk);

__STATIC_INLINE void hal_usart_int_disable(USART_TypeDef * p_usart, hal_usart_int_t int_msk);

__STATIC_INLINE bool hal_usart_evt_check(USART_TypeDef const * p_usart, hal_usart_evt_t evt);

__STATIC_INLINE uint32_t hal_usart_evt_mask_get(USART_TypeDef const * p_usart);

__STATIC_INLINE void hal_usart_evt_clear(USART_TypeDef * p_usart, hal_usart_evt_t evt);

__STATIC_INLINE uint8_t hal_usart_byte_rx(USART_TypeDef const * p_usart);

__STATIC_INLINE void hal_usart_byte_tx(USART_TypeDef * p_usart, uint8_t byte);

#ifndef __MOCK_HAL

__STATIC_INLINE void hal_usart_init(USART_TypeDef * p_usart)
{
    hal_rcc_apb_periph_enable(RCC, p_usart);
}

__STATIC_INLINE void hal_usart_uninit(USART_TypeDef * p_usart)
{
//    hal_rcc_apb_periph_disable(p_usart);
}

__STATIC_INLINE void hal_usart_baudrate_set(USART_TypeDef * p_usart, hal_usart_baud_t baud)
{
    p_usart->BRR = hal_rcc_apb_clock_get(RCC) / (uint32_t)baud;
}

__STATIC_INLINE void hal_usart_parity_set(USART_TypeDef * p_usart, hal_usart_parity_t parity)
{
    p_usart->CR1 = (p_usart->CR1 & ~HAL_USART_PARITY_Msk) | (uint32_t)parity;
}

__STATIC_INLINE void hal_usart_stopbits_set(USART_TypeDef * p_usart, hal_usart_stopbits_t stopbits)
{
    p_usart->CR2 = (p_usart->CR2 & ~USART_CR2_STOP_Msk) | (uint32_t)stopbits;
}

__STATIC_INLINE void hal_usart_start(USART_TypeDef * p_usart)
{
    p_usart->CR1 |= USART_CR1_UE_Msk;
}

__STATIC_INLINE void hal_usart_stop(USART_TypeDef * p_usart)
{
    p_usart->CR1 &= ~USART_CR1_UE_Msk;
}

__STATIC_INLINE void hal_usart_tx_enable(USART_TypeDef * p_usart)
{
    p_usart->CR1 |= HAL_USART_MODE_TX;
}

__STATIC_INLINE void hal_usart_tx_disable(USART_TypeDef * p_usart)
{
    p_usart->CR1 &= ~HAL_USART_MODE_TX;
}

__STATIC_INLINE void hal_usart_rx_enable(USART_TypeDef * p_usart)
{
    p_usart->CR1 |= HAL_USART_MODE_RX;
}

__STATIC_INLINE void hal_usart_rx_disable(USART_TypeDef * p_usart)
{
    p_usart->CR1 &= ~HAL_USART_MODE_RX;
}

__STATIC_INLINE void hal_usart_hfctrl_enable(USART_TypeDef * p_usart)
{
    p_usart->CR3 |= HAL_USART_HFCTRL_ENABLED;
}

__STATIC_INLINE void hal_usart_cfg(USART_TypeDef *     p_usart,
                                  hal_usart_baud_t     baud,
                                  hal_usart_stopbits_t stopbits,
                                  hal_usart_parity_t   parity)
{
    p_usart->BRR = hal_rcc_apb_clock_get(RCC) / baud;

    p_usart->CR2 = (uint32_t)stopbits;

    p_usart->CR1 = (uint32_t)parity |
                   HAL_USART_MODE_RXTX |
                   USART_CR1_UE_Msk;
}

__STATIC_INLINE void hal_usart_int_enable(USART_TypeDef * p_usart, hal_usart_int_t int_msk)
{
    p_usart->CR1 |= (uint32_t)int_msk;
}

__STATIC_INLINE void hal_usart_int_disable(USART_TypeDef * p_usart, hal_usart_int_t int_msk)
{
    p_usart->CR1 &= ~(uint32_t)int_msk;
}

__STATIC_INLINE bool hal_usart_evt_check(USART_TypeDef const * p_usart, hal_usart_evt_t evt)
{
    return (bool)(p_usart->HAL_USART_STATUS_REG & (uint32_t)evt);
}

__STATIC_INLINE uint32_t hal_usart_evt_mask_get(USART_TypeDef const * p_usart)
{
    return (uint32_t)p_usart->HAL_USART_STATUS_REG;
}

__STATIC_INLINE void hal_usart_evt_clear(USART_TypeDef * p_usart, hal_usart_evt_t evt)
{
#if defined(USART_ICR_TCCF_Msk)
    p_usart->ICR = (uint32_t)evt;
#else
    switch(evt)
    {
        case HAL_USART_EVT_TC:
        {
            // cleared by writing '0'
            p_usart->HAL_USART_STATUS_REG &= ~HAL_USART_EVT_TC;
            break;
        }
        case HAL_USART_EVT_RX:
        {
            // cleared by writing '0'
            p_usart->HAL_USART_STATUS_REG &= ~HAL_USART_EVT_RX;
            break;
        }
        case HAL_USART_EVT_OVERRUN:
        {
            // cleared by software sequence
            uint32_t volatile dummy_read = hal_usart_evt_mask_get(p_usart);
            dummy_read = hal_usart_byte_rx(p_usart);
            (void)dummy_read;
        }
        default: break;
    }
#endif
}

__STATIC_INLINE uint8_t hal_usart_byte_rx(USART_TypeDef const * p_usart)
{
    return (uint8_t)(p_usart->HAL_USART_RX_REG);
}

__STATIC_INLINE void hal_usart_byte_tx(USART_TypeDef * p_usart, uint8_t byte)
{
    p_usart->HAL_USART_TX_REG = byte;
}

#endif // __MOCK_HAL

#endif // _HAL_USART_H_
