#include <drv_exti.h>

#define UINT32_BIT_COUNT 32

typedef struct
{
    drv_exti_handler_t handler;
    uint32_t           lines_allocated;
} exti_cb_t;

static exti_cb_t s_cb;

static uint32_t exti_trigger_mask_create(uint8_t  line_idx,
                                         bool     enable,
                                         uint32_t current_trigger_mask)
{
    uint32_t mask = 0;
    if (enable)
    {
        mask = (1 << line_idx);
    }
    mask |= (current_trigger_mask & ~(1 << line_idx));
    return mask;
}

static void exti_line_trigger_set(uint8_t line_idx,
                                  bool    rising_enable,
                                  bool    falling_enable)
{
    uint32_t mask;

    mask = exti_trigger_mask_create(line_idx,
                                    rising_enable,
                                    hal_exti_rising_trigger_get(EXTI));
    hal_exti_rising_trigger_set(EXTI, mask);

    mask = exti_trigger_mask_create(line_idx,
                                    falling_enable,
                                    hal_exti_falling_trigger_get(EXTI));
    hal_exti_falling_trigger_set(EXTI, mask);
}

void drv_exti_init(drv_exti_handler_t handler)
{
    hal_syscfg_init(SYSCFG);

    s_cb.handler = handler;
    s_cb.lines_allocated = 0;
}

void drv_exti_line_alloc(uint8_t              line_idx,
                         drv_exti_line_port_t port)
{
    uint32_t line_mask = 1 << line_idx;
    if (!(line_mask & s_cb.lines_allocated))
    {
        s_cb.lines_allocated |= line_mask;
        hal_syscfg_exti_line_recfg(SYSCFG, line_idx, (hal_syscfg_exti_port_t)port);
        hal_exti_int_enable(EXTI, line_mask);
    }
}

void drv_exti_line_realloc(uint8_t              line_idx,
                           drv_exti_line_port_t port)
{
    uint32_t line_mask = 1 << line_idx;
    if (line_mask & s_cb.lines_allocated)
    {
        hal_syscfg_exti_line_recfg(SYSCFG, line_idx, (hal_syscfg_exti_port_t)port);
    }
}

void drv_exti_line_free(uint8_t line_idx)
{
    uint32_t line_mask = 1 << line_idx;
    if (line_mask & s_cb.lines_allocated)
    {
        hal_exti_int_disable(EXTI, line_mask);
        s_cb.lines_allocated &= ~line_mask;
        exti_line_trigger_set(line_idx, false, false);
    }
}

void drv_exti_line_trigger_set(uint8_t line_idx,
                               bool    rising_enable,
                               bool    falling_enable)
{
    exti_line_trigger_set(line_idx, rising_enable, falling_enable);
}

void drv_exti_irq_handler(uint32_t pending_mask)
{
    hal_exti_pending_clear(EXTI, pending_mask);
    s_cb.handler(pending_mask);
}
