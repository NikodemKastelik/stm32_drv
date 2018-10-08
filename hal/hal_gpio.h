/**
 * @brief HAL for managing GPIO peripheral.
 */

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#include <hal_common.h>

/**
 * 
 */
#define HAL_GPIO_MODER_BITS  2

#define HAL_GPIO_MODER_BITS_MSK  0x03

#define HAL_GPIO_AFR_BITS  4

#define HAL_GPIO_AFR_BITS_MSK  0x0F

#define HAL_GPIO_AF_IN_ONE_AFR  8

#define HAL_GPIO_OTYPER_BITS  1

#define HAL_GPIO_OTYPER_BITS_MSK  0x01

#define HAL_GPIO_OSPEEDR_BITS  2

#define HAL_GPIO_OSPEEDR_BITS_MSK  0x03

#define HAL_GPIO_PUPDR_BITS  2

#define HAL_GPIO_PUPDR_BITS_MSK  0x03

typedef enum
{
    HAL_GPIO_AF_0,
    HAL_GPIO_AF_1,
    HAL_GPIO_AF_2,
    HAL_GPIO_AF_3,
    HAL_GPIO_AF_4,
    HAL_GPIO_AF_5,
    HAL_GPIO_AF_6,
    HAL_GPIO_AF_7,
    HAL_GPIO_AF_8,
    HAL_GPIO_AF_9,
    HAL_GPIO_AF_10,
    HAL_GPIO_AF_11,
    HAL_GPIO_AF_12,
    HAL_GPIO_AF_13,
    HAL_GPIO_AF_14,
    HAL_GPIO_AF_15
} hal_gpio_af_t;

typedef enum
{
    HAL_GPIO_MODE_INPUT,
    HAL_GPIO_MODE_OUTPUT,
    HAL_GPIO_MODE_AF,
    HAL_GPIO_MODE_ANALOG
} hal_gpio_mode_t;

typedef enum
{
    HAL_GPIO_OUTTYPE_PUSHPULL,
    HAL_GPIO_OUTTYPE_OPENDRAIN
} hal_gpio_outtype_t;

typedef enum
{
    HAL_GPIO_OUTSPD_LOW  = 0x00,
    HAL_GPIO_OUTSPD_MED  = 0x01,
    HAL_GPIO_OUTSPD_HIGH = 0x03
} hal_gpio_outspd_t;

typedef enum
{
    HAL_GPIO_PULL_NOPULL,
    HAL_GPIO_PULL_PULLUP,
    HAL_GPIO_PULL_PULLDOWN
} hal_gpio_pull_t;

__STATIC_INLINE void hal_gpio_mode_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_mode_t mode);

__STATIC_INLINE void hal_gpio_af_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_af_t af);

__STATIC_INLINE void hal_gpio_out_write(GPIO_TypeDef * p_port, uint32_t pin, uint32_t state);

__STATIC_INLINE void hal_gpio_out_set(GPIO_TypeDef * p_port, uint32_t pin);

__STATIC_INLINE void hal_gpio_out_clr(GPIO_TypeDef * p_port, uint32_t pin);

__STATIC_INLINE void hal_gpio_out_toggle(GPIO_TypeDef * p_port, uint32_t pin);

__STATIC_INLINE void hal_gpio_outtype_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_outtype_t outtype);

__STATIC_INLINE void hal_gpio_outspd_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_outspd_t outspd);

__STATIC_INLINE void hal_gpio_pull_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_pull_t pull);

__STATIC_INLINE bool hal_gpio_input_check(GPIO_TypeDef const * p_port, uint32_t pin);

__STATIC_INLINE uint32_t hal_gpio_input_mask_get(GPIO_TypeDef const * p_port);

__STATIC_INLINE void hal_gpio_cfg_lock(GPIO_TypeDef * p_port, uint32_t pin_msk);

#ifndef __MOCK_HAL

__STATIC_INLINE void hal_gpio_mode_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_mode_t mode)
{
    uint32_t shift_amount = HAL_GPIO_MODER_BITS * pin;
    uint32_t moder = p_port->MODER & ~(HAL_GPIO_MODER_BITS_MSK << (shift_amount));
    p_port->MODER = moder | (mode << shift_amount);
}

__STATIC_INLINE void hal_gpio_af_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_af_t af)
{
    /* Get proper AFR register according to the pin number being higher or lower
     * than maximum number of pins configuration that fit in one AFR register.*/
    uint32_t afr_idx = pin / HAL_GPIO_AF_IN_ONE_AFR;

    /* Wrap around pin value to fit in one register. */
    pin %= HAL_GPIO_AF_IN_ONE_AFR;

    /* Clear corresponding bit mask before applying new setting. */
    uint32_t shift_amount = HAL_GPIO_AFR_BITS * pin;
    uint32_t afr = p_port->AFR[afr_idx] & ~(HAL_GPIO_AFR_BITS_MSK << (shift_amount));
    p_port->AFR[afr_idx] = afr | (af << (shift_amount));
}

__STATIC_INLINE void hal_gpio_out_write(GPIO_TypeDef * p_port, uint32_t pin, uint32_t state)
{
    uint32_t odr = p_port->ODR & ~(1 << pin);
    p_port->ODR = odr | (state << pin);
}

__STATIC_INLINE void hal_gpio_out_set(GPIO_TypeDef * p_port, uint32_t pin)
{
    p_port->BSRR = (1 << pin);
}

__STATIC_INLINE void hal_gpio_out_clr(GPIO_TypeDef * p_port, uint32_t pin)
{
#if defined(GPIO_BRR_BR_0)
    p_port->BRR = (1 << pin);
#else
    p_port->BSRR = (1 << pin) << 16;
#endif
}

__STATIC_INLINE void hal_gpio_out_toggle(GPIO_TypeDef * p_port, uint32_t pin)
{
    p_port->ODR ^= (1 << pin);
}

__STATIC_INLINE void hal_gpio_outtype_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_outtype_t outtype)
{
    uint32_t otyper = p_port->OTYPER & ~(HAL_GPIO_OTYPER_BITS_MSK << pin);
    p_port->OTYPER = otyper | (outtype << pin);
}

__STATIC_INLINE void hal_gpio_outspd_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_outspd_t outspd)
{
    uint32_t shift_amount = HAL_GPIO_OSPEEDR_BITS * pin;
    uint32_t ospeedr = p_port->OSPEEDR & ~(HAL_GPIO_OSPEEDR_BITS_MSK << shift_amount);
    p_port->OSPEEDR = ospeedr | (outspd << shift_amount);
}

__STATIC_INLINE void hal_gpio_pull_cfg(GPIO_TypeDef * p_port, uint32_t pin, hal_gpio_pull_t pull)
{
    uint32_t shift_amount = HAL_GPIO_PUPDR_BITS * pin;
    uint32_t pupdr = p_port->PUPDR & ~(HAL_GPIO_PUPDR_BITS_MSK << shift_amount);
    p_port->PUPDR = pupdr | (pull << shift_amount);
}

__STATIC_INLINE bool hal_gpio_input_check(GPIO_TypeDef const * p_port, uint32_t pin)
{
    return (bool)(p_port->IDR & (1 << pin));
}

__STATIC_INLINE uint32_t hal_gpio_input_mask_get(GPIO_TypeDef const * p_port)
{
    return p_port->IDR;
}

__STATIC_INLINE void hal_gpio_cfg_lock(GPIO_TypeDef * p_port, uint32_t pin_msk)
{
    /* Apply locking sequence */
    p_port->LCKR = pin_msk | GPIO_LCKR_LCKK;
    p_port->LCKR = pin_msk;
    p_port->LCKR = pin_msk | GPIO_LCKR_LCKK;
    uint32_t volatile dummy_read = p_port->LCKR;
    (void)dummy_read;
}

#endif // __MOCK_HAL

#endif // _HAL_GPIO_H_

