#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_

#include <hal_common.h>

typedef enum
{
    HAL_SPI_MODE_SLAVE  = 0,
    HAL_SPI_MODE_MASTER = SPI_CR1_MSTR_Msk
} hal_spi_mode_t;

typedef enum
{
    HAL_SPI_BAUD_PCLK_2   = 0,
    HAL_SPI_BAUD_PCLK_4   = SPI_CR1_BR_0,
    HAL_SPI_BAUD_PCLK_8   = SPI_CR1_BR_1,
    HAL_SPI_BAUD_PCLK_16  = SPI_CR1_BR_1 | SPI_CR1_BR_0,
    HAL_SPI_BAUD_PCLK_32  = SPI_CR1_BR_2,
    HAL_SPI_BAUD_PCLK_64  = SPI_CR1_BR_2 | SPI_CR1_BR_0,
    HAL_SPI_BAUD_PCLK_128 = SPI_CR1_BR_2 | SPI_CR1_BR_1,
    HAL_SPI_BAUD_PCLK_256 = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0
} hal_spi_baud_t;

typedef enum
{
    HAL_SPI_CLK_POL0_PHASE0 = 0,
    HAL_SPI_CLK_POL1_PHASE0 = SPI_CR1_CPOL_Msk,
    HAL_SPI_CLK_POL0_PHASE1 = SPI_CR1_CPHA_Msk,
    HAL_SPI_CLK_POL1_PHASE1 = SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk
} hal_spi_clk_t;

typedef enum
{
    HAL_SPI_DUPLEX_FULL = 0,
    HAL_SPI_DUPLEX_HALF = SPI_CR1_BIDIMODE_Msk,
    HAL_SPI_SIMPLEX_RX  = SPI_CR1_RXONLY_Msk,
} hal_spi_duplex_t;

typedef enum
{
    HAL_SPI_ORDER_MSB_FIRST = 0,
    HAL_SPI_ORDER_LSB_FIRST = SPI_CR1_LSBFIRST_Msk,
} hal_spi_order_t;

typedef enum
{
    HAL_SPI_NSS_HW_IN  = 0,
    HAL_SPI_NSS_HW_OUT = SPI_CR2_SSOE_Msk,
    HAL_SPI_NSS_SW     = SPI_CR1_SSM_Msk,
} hal_spi_nss_t;

typedef struct
{
    hal_spi_mode_t    mode;
    hal_spi_baud_t    baud;
    hal_spi_clk_t     clk;
    hal_spi_duplex_t  duplex;
    hal_spi_order_t   order;
    hal_spi_nss_t     nss;
} hal_spi_cfg_t;

typedef enum
{
    HAL_SPI_EVT_RX   = SPI_SR_RXNE_Msk,
    HAL_SPI_EVT_TX   = SPI_SR_TXE_Msk,
    HAL_SPI_EVT_BSY  = SPI_SR_BSY_Msk,
    HAL_SPI_EVT_OVR  = SPI_SR_OVR_Msk,
    HAL_SPI_EVT_MODF = SPI_SR_MODF_Msk
} hal_spi_evt_t;

typedef enum
{
    HAL_SPI_INT_RX  = SPI_CR2_RXNEIE_Msk,
    HAL_SPI_INT_TX  = SPI_CR2_TXEIE_Msk,
    HAL_SPI_INT_ERR = SPI_CR2_ERRIE_Msk
} hal_spi_int_t;

__STATIC_INLINE void hal_spi_cfg(SPI_TypeDef * const p_periph, const hal_spi_cfg_t * const config);

__STATIC_INLINE void hal_spi_int_enable(SPI_TypeDef * const p_periph, hal_spi_int_t int_msk);

__STATIC_INLINE void hal_spi_int_disable(SPI_TypeDef * const p_periph, hal_spi_int_t int_msk);

__STATIC_INLINE bool hal_spi_evt_check(const SPI_TypeDef * const p_periph, hal_spi_evt_t evt);

__STATIC_INLINE uint8_t hal_spi_byte_rx(const SPI_TypeDef * const p_periph);

__STATIC_INLINE void hal_spi_byte_tx(SPI_TypeDef * const p_periph, uint8_t byte);

__STATIC_INLINE void hal_spi_nss_internal_set(SPI_TypeDef * const p_periph);

__STATIC_INLINE void hal_spi_nss_internal_clr(SPI_TypeDef * const p_periph);

#ifndef __MOCK_HAL

__STATIC_INLINE void hal_spi_cfg(SPI_TypeDef * const p_periph, const hal_spi_cfg_t * const config)
{
    p_periph->CR2 = config->nss;

    p_periph->CR1 = config->mode   |
                    config->baud   |
                    config->clk    |
                    config->duplex |
                    config->order  |
                    (config->nss & HAL_SPI_NSS_SW) |
                    SPI_CR1_SPE_Msk;
}

__STATIC_INLINE void hal_spi_int_enable(SPI_TypeDef * const p_periph, hal_spi_int_t int_msk)
{
    p_periph->CR2 |= (uint32_t)int_msk;
}

__STATIC_INLINE void hal_spi_int_disable(SPI_TypeDef * const p_periph, hal_spi_int_t int_msk)
{
    p_periph->CR2 &= ~(uint32_t)int_msk;
}

__STATIC_INLINE bool hal_spi_evt_check(const SPI_TypeDef * const p_periph, hal_spi_evt_t evt)
{
    return (bool)(p_periph->SR & evt);
}

__STATIC_INLINE uint8_t hal_spi_byte_rx(const SPI_TypeDef * const p_periph)
{
    return p_periph->DR;
}

__STATIC_INLINE void hal_spi_byte_tx(SPI_TypeDef * const p_periph, uint8_t byte)
{
    p_periph->DR = byte;
}

__STATIC_INLINE void hal_spi_nss_internal_set(SPI_TypeDef * const p_periph)
{
    p_periph->CR1 |= SPI_CR1_SSI_Msk;
}

__STATIC_INLINE void hal_spi_nss_internal_clr(SPI_TypeDef * const p_periph)
{
    p_periph->CR1 &= ~SPI_CR1_SSI_Msk;
}

#endif // __MOCK_HAL

#endif // _HAL_SPI_H_

