#ifndef _DRV_USART_H_
#define _DRV_USART_H_

#define DRV_CONFIG_USART1_ENABLED  1

#include <hal_usart.h>

#define DRV_USART_INSTANCE_GET(x)             \
{                                             \
    .idx =      DRV_USART_INST ## x ## _IDX,  \
}

#define DRV_USART_DEFAULT_CONFIG              \
{                                             \
    .baudrate = HAL_USART_BAUD_115200,        \
    .stopbits = HAL_USART_STOPBITS_ONE,       \
    .parity   = HAL_USART_PARITY_DISABLED,    \
}

typedef enum
{
#if DRV_CONFIG_USART1_ENABLED
    DRV_USART_INST1_IDX,
#endif
#if DRV_CONFIG_USART2_ENABLED
    DRV_USART_INST2_IDX,
#endif
#if DRV_CONFIG_USART3_ENABLED
    DRV_USART_INST3_IDX,
#endif
    DRV_USART_INST_COUNT
} drv_usart_inst_idx_t;

typedef enum
{
    DRV_USART_EVT_RXDONE,
    DRV_USART_EVT_TXDONE,
    DRV_USART_EVT_ERROR,
} drv_usart_evt_t;

typedef struct
{
    drv_usart_inst_idx_t idx;
} drv_usart_t;

typedef void (* drv_usart_handler_t)(drv_usart_evt_t evt,
                                     const uint8_t * buf);

typedef struct
{
    hal_usart_baud_t     baudrate;
    hal_usart_stopbits_t stopbits;
    hal_usart_parity_t   parity;
} drv_usart_cfg_t;

void drv_usart_init(drv_usart_t *           usart,
                    drv_usart_cfg_t const * cfg,
                    drv_usart_handler_t     handler);

void drv_usart_uninit(drv_usart_t * usart);

void drv_usart_tx(drv_usart_t * usart, uint8_t * buf, uint32_t len);

void drv_usart_rx(drv_usart_t * usart, uint8_t * buf, uint32_t len);

bool drv_usart_tx_ongoing_check(drv_usart_t * usart);

#endif // _DRV_USART_H_
