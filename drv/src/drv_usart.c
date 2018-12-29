#include <drv_usart.h>

static USART_TypeDef * const s_instances[DRV_USART_INST_COUNT] =
{
#if DRV_CONFIG_USART1_ENABLED
    USART1,
#endif
#if DRV_CONFIG_USART2_ENABLED
    USART2,
#endif
#if DRV_CONFIG_USART3_ENABLED
    USART3,
#endif
};

typedef struct
{
    drv_usart_handler_t     handler;
    uint8_t *               rx_primary_buffer;
    uint32_t                rx_primary_buffer_len;
    uint8_t *               rx_secondary_buffer;
    uint32_t                rx_secondary_buffer_len;
    uint8_t *               tx_buffer;
    uint32_t                tx_buffer_len;
    uint32_t                rx_idx;
    uint32_t                tx_idx;
} usart_cb_t;

static usart_cb_t s_cb[DRV_USART_INST_COUNT];

void drv_usart_init(drv_usart_t *           usart,
                    drv_usart_cfg_t const * cfg,
                    drv_usart_handler_t     handler)
{
    usart_cb_t * p_cb = &s_cb[usart->idx];
    USART_TypeDef * p_usart = s_instances[usart->idx];

    p_cb->handler = handler;

    hal_usart_init(p_usart);
    hal_usart_start(p_usart);

    hal_usart_baudrate_set(p_usart, cfg->baudrate);
    hal_usart_parity_set(p_usart, cfg->parity);
    hal_usart_stopbits_set(p_usart, cfg->stopbits);

    hal_usart_tx_enable(p_usart);
    hal_usart_evt_clear(p_usart, HAL_USART_EVT_TC);
    hal_usart_int_enable(p_usart, HAL_USART_INT_RX | HAL_USART_INT_TC);
}

void drv_usart_uninit(drv_usart_t * usart)
{
    USART_TypeDef * p_usart = s_instances[usart->idx];

    hal_usart_uninit(p_usart);
}

void drv_usart_tx(drv_usart_t * usart, uint8_t * buf, uint32_t len)
{
    usart_cb_t * p_cb = &s_cb[usart->idx];
    USART_TypeDef * p_usart = s_instances[usart->idx];

    p_cb->tx_buffer = buf;
    p_cb->tx_buffer_len = len;
    p_cb->tx_idx = 0;

    uint8_t byte = p_cb->tx_buffer[p_cb->tx_idx++];
    hal_usart_byte_tx(p_usart, byte);
}

void drv_usart_rx(drv_usart_t * usart, uint8_t * buf, uint32_t len)
{
    usart_cb_t * p_cb = &s_cb[usart->idx];
    USART_TypeDef * p_usart = s_instances[usart->idx];

    if (p_cb->rx_primary_buffer != NULL)
    {
        p_cb->rx_secondary_buffer = buf;
        p_cb->rx_secondary_buffer_len = len;
    }
    else
    {
        p_cb->rx_primary_buffer = buf;
        p_cb->rx_primary_buffer_len = len;
        p_cb->rx_idx = 0;

        hal_usart_rx_enable(p_usart);
    }
}

bool drv_usart_tx_ongoing_check(drv_usart_t * usart)
{
    usart_cb_t * p_cb = &s_cb[usart->idx];

    if (p_cb->tx_buffer_len == p_cb->tx_idx)
    {
        return false;
    }
    else
    {
        return true;
    }
}

static void usart_irq_handler(drv_usart_inst_idx_t idx)
{
    usart_cb_t * p_cb = &s_cb[idx];
    USART_TypeDef * p_usart = s_instances[idx];

    uint32_t evt_mask = hal_usart_evt_mask_get(p_usart);

    if (evt_mask & HAL_USART_EVT_RX)
    {
        /* RX flag will be cleared by byte read */
        uint8_t byte_received = hal_usart_byte_rx(p_usart);

        p_cb->rx_primary_buffer[p_cb->rx_idx++] = byte_received;

        if (p_cb->rx_idx == p_cb->rx_primary_buffer_len)
        {
            uint8_t * filled_buf = p_cb->rx_primary_buffer;

            if (p_cb->rx_secondary_buffer != NULL)
            {
                p_cb->rx_primary_buffer = p_cb->rx_secondary_buffer;
                p_cb->rx_primary_buffer_len = p_cb->rx_secondary_buffer_len;
                p_cb->rx_secondary_buffer = NULL;
                p_cb->rx_idx = 0;
            }
            else
            {
                hal_usart_rx_disable(p_usart);
                p_cb->rx_primary_buffer = NULL;
            }
            
            p_cb->handler(DRV_USART_EVT_RXDONE, filled_buf);
        }
    }

    if (evt_mask & HAL_USART_EVT_TC)
    {
        hal_usart_evt_clear(p_usart, HAL_USART_EVT_TC);
        if (p_cb->tx_idx == p_cb->tx_buffer_len)
        {
            p_cb->handler(DRV_USART_EVT_TXDONE, p_cb->tx_buffer);
        }
        else
        {
            uint8_t byte_to_transfer = p_cb->tx_buffer[p_cb->tx_idx++];
            hal_usart_byte_tx(p_usart, byte_to_transfer);
        }
    }
}

#if DRV_CONFIG_USART2_ENABLED
void USART2_Handler(void)
{
    usart_irq_handler(DRV_USART_INST2_IDX);
}
#endif
