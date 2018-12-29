#ifndef _HAL_TIM_H_
#define _HAL_TIM_H_

#include <hal_common.h>
#include <hal_rcc.h>

#define HAL_TIM_POL_BITS  4

#define HAL_TIM_POL_BITS_MSK  0x0F

enum
{
    HAL_TIM_FREQ_16MHz  = 16000000U,
    HAL_TIM_FREQ_8MHz   =  8000000U,
    HAL_TIM_FREQ_2MHz   =  2000000U,
    HAL_TIM_FREQ_1MHz   =  1000000U,
    HAL_TIM_FREQ_100kHz =   100000U,
    HAL_TIM_FREQ_10kHz  =    10000U,
    HAL_TIM_FREQ_1kHz   =     1000U,
};

enum
{
    HAL_TIM_PERIOD_8M_TICKS   = 8000000U,
    HAL_TIM_PERIOD_1M_TICKS   = 1000000U,
    HAL_TIM_PERIOD_100K_TICKS =  100000U,
    HAL_TIM_PERIOD_10K_TICKS  =   10000U,
    HAL_TIM_PERIOD_1K_TICKS   =    1000U,
};

enum
{
    HAL_TIM_PRESCALER_DISABLED = 1,
};

enum
{
    HAL_TIM_CAPACITY_MAXIMUM = 0xFFFFFFFF,
};

typedef enum
{
    HAL_TIM_INT_UPDATE = TIM_DIER_UIE_Msk,
    HAL_TIM_INT_CC_CH1 = TIM_DIER_CC1IE_Msk,
    HAL_TIM_INT_CC_CH2 = TIM_DIER_CC2IE_Msk,
    HAL_TIM_INT_CC_CH3 = TIM_DIER_CC3IE_Msk,
    HAL_TIM_INT_CC_CH4 = TIM_DIER_CC4IE_Msk,
} hal_tim_int_t;

typedef enum
{
    HAL_TIM_EVT_CC_CH1   = TIM_SR_CC1IF_Msk,
    HAL_TIM_EVT_CC_CH2   = TIM_SR_CC2IF_Msk,
    HAL_TIM_EVT_CC_CH3   = TIM_SR_CC3IF_Msk,
    HAL_TIM_EVT_CC_CH4   = TIM_SR_CC4IF_Msk,
    HAL_TIM_EVT_UPDATE   = TIM_SR_UIF_Msk,
} hal_tim_evt_t;

typedef enum
{
    HAL_TIM_CH1 = 0,
    HAL_TIM_CH2 = 1,
    HAL_TIM_CH3 = 2,
    HAL_TIM_CH4 = 3,
} hal_tim_ch_t;

typedef enum
{
    HAL_TIM_PWM_POL_H0L1 = TIM_CCER_CC1E_Msk | TIM_CCER_CC1P_Msk,
    HAL_TIM_PWM_POL_H1L0 = TIM_CCER_CC1E_Msk,
} hal_tim_pwm_pol_t;

typedef enum
{
    HAL_TIM_ENC_MODE1 = TIM_SMCR_SMS_0,
    HAL_TIM_ENC_MODE2 = TIM_SMCR_SMS_1,
    HAL_TIM_ENC_MODE3 = TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
} hal_tim_enc_mode_t;

typedef enum
{
    HAL_TIM_ENC_POL_H0L1 = TIM_CCER_CC2P_Msk | TIM_CCER_CC1P_Msk,
    HAL_TIM_ENC_POL_H1L0 = 0,
} hal_tim_enc_pol_t;

typedef enum
{
    HAL_TIM_ICP_POL_FALLING = TIM_CCER_CC1E_Msk | TIM_CCER_CC1P_Msk,
    HAL_TIM_ICP_POL_RISING  = TIM_CCER_CC1E_Msk,
    HAL_TIM_ICP_POL_BOTH    = TIM_CCER_CC1E_Msk | TIM_CCER_CC1P_Msk | TIM_CCER_CC1NP_Msk,
} hal_tim_icp_pol_t;

typedef enum
{
    HAL_TIM_SLAVE_TRIG_ITR0 = 0,
    HAL_TIM_SLAVE_TRIG_ITR1 = TIM_SMCR_TS_0,
    HAL_TIM_SLAVE_TRIG_ITR2 = TIM_SMCR_TS_1,
    HAL_TIM_SLAVE_TRIG_ITR3 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0,
    HAL_TIM_SLAVE_TRIG_ED   = TIM_SMCR_TS_2,
    HAL_TIM_SLAVE_TRIG_TI1  = TIM_SMCR_TS_2 | TIM_SMCR_TS_0,
    HAL_TIM_SLAVE_TRIG_TI2  = TIM_SMCR_TS_2 | TIM_SMCR_TS_1,
} hal_tim_slave_trig_t;

typedef enum
{
    HAL_TIM_SLAVE_MODE_DISABLED = 0,
    HAL_TIM_SLAVE_MODE_RESET    = TIM_SMCR_SMS_2,
    HAL_TIM_SLAVE_MODE_GATED    = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0,
    HAL_TIM_SLAVE_MODE_TRIGGER  = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1,
    HAL_TIM_SLAVE_MODE_EXTERNAL = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
} hal_tim_slave_mode_t;

#ifndef __MOCK_HAL

__STATIC_INLINE void hal_tim_timer_cfg(TIM_TypeDef * p_tim,
                                       uint32_t      freq_hz,
                                       uint32_t      period_ticks)
{
    uint32_t apb_clock = hal_rcc_apb_clock_get();
    p_tim->PSC = (apb_clock / freq_hz) - 1;
    p_tim->ARR = period_ticks - 1;
}

__STATIC_INLINE void hal_tim_counter_cfg(TIM_TypeDef * p_tim,
                                         uint32_t      prescaler,
                                         uint32_t      counter_capacity)
{
    p_tim->PSC = prescaler - 1;
    p_tim->ARR = counter_capacity - 1;
}

__STATIC_INLINE void hal_tim_pwm_cfg(TIM_TypeDef *     p_tim,
                                     hal_tim_ch_t      ch,
                                     hal_tim_pwm_pol_t polarity)
{
    uint32_t ch_reg_idx = ch >> 1;
    uint32_t ch_nibble  = ch &  0x01;

    (&p_tim->CCMR1)[ch_reg_idx] |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1) << (ch_nibble * 8);
    p_tim->CCER |= polarity << (ch * 4);
    p_tim->BDTR = TIM_BDTR_MOE_Msk;
}

__STATIC_INLINE void hal_tim_encoder_cfg(TIM_TypeDef *      p_tim,
                                         hal_tim_enc_mode_t mode,
                                         hal_tim_enc_pol_t  polarity)
{
    p_tim->SMCR  = mode;
    p_tim->CCMR1 = (TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_0) | TIM_CCMR1_CC2S_0 |
                   (TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0) | TIM_CCMR1_CC1S_0;
    p_tim->CCER  = (p_tim->CCER & 0xFF00) | polarity;
}

__STATIC_INLINE void hal_tim_icp_cfg(TIM_TypeDef *     p_tim,
                                     hal_tim_ch_t      ch,
                                     hal_tim_icp_pol_t polarity)
{
    uint32_t ch_reg_idx = ch >> 1;
    uint32_t ch_nibble  = ch &  0x01;

    (&p_tim->CCMR1)[ch_reg_idx] |= ((TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_2
                                   | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0)
                                   | TIM_CCMR1_CC1S_0) << (ch_nibble * 8);
    p_tim->CCER |= polarity << (ch * 4);
}

__STATIC_INLINE void hal_tim_icp_polarity_set(TIM_TypeDef *     p_tim,
                                              hal_tim_ch_t      ch,
                                              hal_tim_icp_pol_t polarity)
{
    uint32_t shift_amount = HAL_TIM_POL_BITS * ch;
    uint32_t ccer = p_tim->CCER & ~(HAL_TIM_POL_BITS_MSK << (shift_amount));
    p_tim->CCER = ccer | (polarity << shift_amount);
}

__STATIC_INLINE hal_tim_icp_pol_t hal_tim_icp_polarity_get(TIM_TypeDef * p_tim,
                                                           hal_tim_ch_t  ch)
{
    uint32_t shift_amount = HAL_TIM_POL_BITS * ch;
    uint32_t ccer = p_tim->CCER & (HAL_TIM_POL_BITS_MSK << (shift_amount));
    return (hal_tim_icp_pol_t)(ccer >> shift_amount);
}

__STATIC_INLINE void hal_tim_cc_set(TIM_TypeDef * p_tim, hal_tim_ch_t ch, uint32_t cc_val)
{
    (&p_tim->CCR1)[ch] = cc_val;
}

__STATIC_INLINE uint32_t hal_tim_cc_get(TIM_TypeDef * p_tim, hal_tim_ch_t ch)
{
    return (uint32_t)(&p_tim->CCR1)[ch];
}

__STATIC_INLINE void hal_tim_cc_clear(TIM_TypeDef * p_tim, hal_tim_ch_t ch)
{
    (&p_tim->CCR1)[ch] = 0;
}

__STATIC_INLINE void hal_tim_slave_cfg(TIM_TypeDef *        p_tim,
                                       hal_tim_slave_mode_t mode,
                                       hal_tim_slave_trig_t trigger)
{
    p_tim->SMCR = mode | trigger;
}

__STATIC_INLINE void hal_tim_count_set(TIM_TypeDef * p_tim, uint32_t cnt_val)
{
    p_tim->CNT = cnt_val;
}

__STATIC_INLINE uint32_t hal_tim_count_get(TIM_TypeDef * p_tim)
{
    return p_tim->CNT;
}

__STATIC_INLINE void hal_tim_count_clear(TIM_TypeDef *p_tim)
{
    p_tim->CNT = 0;
}

__STATIC_INLINE void hal_tim_start(TIM_TypeDef * p_tim)
{
    p_tim->CR1 |= TIM_CR1_CEN_Msk;
}

__STATIC_INLINE void hal_tim_stop(TIM_TypeDef * p_tim)
{
    p_tim->CR1 &= ~TIM_CR1_CEN_Msk;
}

__STATIC_INLINE void hal_tim_int_enable(TIM_TypeDef * p_tim, hal_tim_int_t int_msk)
{
    p_tim->DIER |= int_msk;
}

__STATIC_INLINE void hal_tim_int_disable(TIM_TypeDef * p_tim, hal_tim_int_t int_msk)
{
    p_tim->DIER &= ~int_msk;
}

__STATIC_INLINE bool hal_tim_evt_check(TIM_TypeDef * p_tim, hal_tim_evt_t evt)
{
    return (bool)(p_tim->SR & evt);
}

__STATIC_INLINE uint32_t hal_tim_evt_mask_get(TIM_TypeDef * p_tim)
{
    return p_tim->SR;
}

__STATIC_INLINE void hal_tim_evt_clear(TIM_TypeDef *p_tim, hal_tim_evt_t evt)
{
    p_tim->SR &= ~evt;
}

#endif // __MOCK_HAL

#endif // _HAL_TIM_H_
