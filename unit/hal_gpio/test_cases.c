#include <unity.h>
#include <string.h>

/* Define STM32 target for register structure access */
#define STM32F030x6
#define STM32_SERIES_F0
#include <hal_gpio.h>

#define UINT32_BIT_COUNT  32

#define DUMMY_BITS  4

#define DUMMY_BITS_MSK  0x0F

#define PINS_PER_PORT  16

#define BIT_TESTER_DEFAULT_CONFIG(_func, _reg, _msk)            \
{                                                               \
    .func           = _func,                                    \
    .reg            = (volatile uint32_t *)&g_dummy_gpio._reg,  \
    .starting_pin   = 0,                                        \
    .pins_amount    = PINS_PER_PORT,                            \
    .bitmask_to_clr = HAL_GPIO_ ## _reg ## _BITS_MSK,           \
    .bitmask_to_set = _msk,                                     \
}

typedef void (* hal_gpio_wrapper_t)(uint32_t pin, uint32_t setting);

typedef struct
{
    hal_gpio_wrapper_t  func;
    volatile uint32_t * reg;
    uint32_t            starting_pin;
    uint32_t            pins_amount;
    uint32_t            bitmask_to_clr;
    uint32_t            bitmask_to_set;
} bit_tester_cfg_t;

GPIO_TypeDef g_dummy_gpio;

uint32_t g_call_counter;

uint32_t g_dummy_reg;

void setUp(void)
{
    memset(&g_dummy_gpio, 0, sizeof(g_dummy_gpio));
}

void tearDown(void)
{
    ;
}

static void dummy_wrapper(uint32_t pin, uint32_t setting)
{
    g_call_counter++;
    uint32_t dummy_reg = g_dummy_reg & ~(DUMMY_BITS_MSK << pin * DUMMY_BITS);
    g_dummy_reg = dummy_reg | setting << pin * DUMMY_BITS;
}

static void mode_cfg_wrapper(uint32_t pin, uint32_t setting)
{
    hal_gpio_mode_cfg(&g_dummy_gpio, pin, (hal_gpio_mode_t)setting);
}

static void af_cfg_wrapper(uint32_t pin, uint32_t af)
{
    hal_gpio_af_cfg(&g_dummy_gpio, pin, (hal_gpio_af_t)af);
}

static void outtype_cfg_wrapper(uint32_t pin, uint32_t setting)
{
    hal_gpio_outtype_cfg(&g_dummy_gpio, pin, (hal_gpio_outtype_t)setting);
}

static void outspd_cfg_wrapper(uint32_t pin, uint32_t setting)
{
    hal_gpio_outspd_cfg(&g_dummy_gpio, pin, (hal_gpio_outspd_t)setting);
}

static void pull_cfg_wrapper(uint32_t pin, uint32_t setting)
{
    hal_gpio_pull_cfg(&g_dummy_gpio, pin, (hal_gpio_pull_t)setting);
}

static uint32_t bit_length_calculate(uint32_t msk)
{
    uint32_t count = 0;
    while(msk > 0)
    {
        count++;
        msk = msk >> 1;
    }
    return count;
}

static void bit_tester(const bit_tester_cfg_t * config)
{
    uint32_t old_reg;
    uint32_t expected;
    uint32_t shift_amount;
    uint32_t bitmask_len = bit_length_calculate(config->bitmask_to_clr);
    uint32_t last_pin = config->starting_pin + config->pins_amount;

    for (uint32_t init_val_idx = 0; init_val_idx < 2; init_val_idx++)
    {
        *(config->reg) = init_val_idx * 0xFFFFFFFF;
        for (uint32_t pin = config->starting_pin; pin < last_pin; pin++)
        {
            old_reg = *(config->reg);
            shift_amount = pin * bitmask_len;

            expected  = old_reg & ~((config->bitmask_to_clr) << shift_amount);
            expected |= (config->bitmask_to_set) << shift_amount;

            config->func(pin, config->bitmask_to_set);

            TEST_ASSERT_EQUAL_HEX32(expected, *(config->reg));
        }
    }
}

void test_bit_tester(void)
{
    bit_tester_cfg_t config;
    config.func           = dummy_wrapper;
    config.reg            = &(g_dummy_reg);
    config.starting_pin   = 0;
    config.pins_amount    = UINT32_BIT_COUNT / DUMMY_BITS;
    config.bitmask_to_clr = DUMMY_BITS_MSK;

    config.bitmask_to_set = 0x09;
    g_call_counter        = 0;
    g_dummy_reg           = 0;
    bit_tester(&config);
    TEST_ASSERT_EQUAL(config.pins_amount * 2, g_call_counter);
    TEST_ASSERT_EQUAL_HEX32(0x999999999, g_dummy_reg);

    config.pins_amount   /= 2;
    g_call_counter        = 0;
    g_dummy_reg           = 0;
    bit_tester(&config);
    TEST_ASSERT_EQUAL(config.pins_amount * 2, g_call_counter);
    TEST_ASSERT_EQUAL_HEX32(0xFFFF9999, g_dummy_reg);

    config.starting_pin   = config.pins_amount;
    g_call_counter        = 0;
    g_dummy_reg           = 0;
    bit_tester(&config);
    TEST_ASSERT_EQUAL(config.pins_amount * 2, g_call_counter);
    TEST_ASSERT_EQUAL_HEX32(0x9999FFFF, g_dummy_reg);
}

void test_set_pin_high(void)
{
    g_dummy_gpio.BSRR = 0;
    for(uint32_t pin = 0; pin < PINS_PER_PORT; pin++)
    {
        uint32_t expected = 1 << pin;
        hal_gpio_out_set(&g_dummy_gpio, pin);
        TEST_ASSERT_EQUAL_HEX32(expected, g_dummy_gpio.BSRR);
    }
}

void test_set_pin_low(void)
{
    g_dummy_gpio.BRR = 0;
    for(uint32_t pin = 0; pin < PINS_PER_PORT; pin++)
    {
        uint32_t expected = 1 << pin;
        hal_gpio_out_clr(&g_dummy_gpio, pin);
        TEST_ASSERT_EQUAL_HEX32(expected, g_dummy_gpio.BRR);
    }
}

void test_set_pin_as_output(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(mode_cfg_wrapper,
                                                        MODER,
                                                        HAL_GPIO_MODE_OUTPUT);
    bit_tester(&config);
}

void test_set_pin_as_input(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(mode_cfg_wrapper,
                                                        MODER,
                                                        HAL_GPIO_MODE_INPUT);
    bit_tester(&config);
}

void test_set_pin_in_alternate_function(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(mode_cfg_wrapper,
                                                        MODER,
                                                        HAL_GPIO_MODE_AF);
    bit_tester(&config);
}

void test_set_pin_as_analog_input(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(mode_cfg_wrapper,
                                                        MODER,
                                                        HAL_GPIO_MODE_ANALOG);
    bit_tester(&config);
}

void test_set_alternate_function_number(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(af_cfg_wrapper,
                                                        AFR,
                                                        HAL_GPIO_AFR_BITS_MSK);
    config.pins_amount    = HAL_GPIO_AF_IN_ONE_AFR;

    for (uint32_t afr_idx = 0; afr_idx < ARRAY_SIZE(g_dummy_gpio.AFR); afr_idx++)
    {
        config.starting_pin = afr_idx * HAL_GPIO_AF_IN_ONE_AFR;
        config.reg          = (volatile uint32_t *)&g_dummy_gpio.AFR[afr_idx];
        for (hal_gpio_af_t af = HAL_GPIO_AF_0; af <= HAL_GPIO_AF_15; af++)
        {
            config.bitmask_to_set = (uint32_t)af;
            bit_tester(&config);
        }
    }
}

void test_set_pullup(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(pull_cfg_wrapper,
                                                        PUPDR,
                                                        HAL_GPIO_PULL_PULLUP);
    bit_tester(&config);
}

void test_set_pulldown(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(pull_cfg_wrapper,
                                                        PUPDR,
                                                        HAL_GPIO_PULL_PULLDOWN);
    bit_tester(&config);
}

void test_set_nopull(void)
{

    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(pull_cfg_wrapper,
                                                        PUPDR,
                                                        HAL_GPIO_PULL_NOPULL);
    bit_tester(&config);
}

void test_set_pin_speed_to_low(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(outspd_cfg_wrapper,
                                                        OSPEEDR,
                                                        HAL_GPIO_OUTSPD_LOW);
    bit_tester(&config);
}

void test_set_pin_speed_to_medium(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(outspd_cfg_wrapper,
                                                        OSPEEDR,
                                                        HAL_GPIO_OUTSPD_MED);
    bit_tester(&config);
}

void test_set_pin_speed_to_high(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(outspd_cfg_wrapper,
                                                        OSPEEDR,
                                                        HAL_GPIO_OUTSPD_HIGH);
    bit_tester(&config);
}

void test_set_output_type_pull_push(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(outtype_cfg_wrapper,
                                                        OTYPER,
                                                        HAL_GPIO_OUTTYPE_PUSHPULL);
    bit_tester(&config);
}

void test_set_output_type_open_drain(void)
{
    bit_tester_cfg_t config = BIT_TESTER_DEFAULT_CONFIG(outtype_cfg_wrapper,
                                                        OTYPER,
                                                        HAL_GPIO_OUTTYPE_OPENDRAIN);
    bit_tester(&config);
}

void test_check_input(void)
{
    g_dummy_gpio.IDR = 0xFFFFFFFF;
    for (uint32_t pin; pin < PINS_PER_PORT; pin++)
    {
        TEST_ASSERT_TRUE(hal_gpio_input_check(&g_dummy_gpio, pin));
    }

    g_dummy_gpio.IDR = 0;
    for (uint32_t pin; pin < PINS_PER_PORT; pin++)
    {
        TEST_ASSERT_FALSE(hal_gpio_input_check(&g_dummy_gpio, pin));
    }

    g_dummy_gpio.IDR = 0xAAAAAAAA;
    for (uint32_t pin; pin < PINS_PER_PORT; pin++)
    {
        TEST_ASSERT_EQUAL(pin % 2, (uint32_t)hal_gpio_input_check(&g_dummy_gpio, pin));
    }
}

void test_lock_pin_configuration(void)
{
    uint32_t pin_msk = 0;
    for (uint32_t pin = 0; pin < PINS_PER_PORT; pin++)
    {
        pin_msk |= (1 << pin);
        g_dummy_gpio.LCKR = 0;

        hal_gpio_cfg_lock(&g_dummy_gpio, pin_msk);

        TEST_ASSERT_EQUAL_HEX32(pin_msk | GPIO_LCKR_LCKK, g_dummy_gpio.LCKR); 
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_bit_tester);
    RUN_TEST(test_set_pin_high);
    RUN_TEST(test_set_pin_low);
    RUN_TEST(test_set_pin_as_output);
    RUN_TEST(test_set_pin_as_input);
    RUN_TEST(test_set_pin_as_analog_input);
    RUN_TEST(test_set_pin_in_alternate_function);
    RUN_TEST(test_set_alternate_function_number);
    RUN_TEST(test_set_pullup);
    RUN_TEST(test_set_pulldown);
    RUN_TEST(test_set_pin_speed_to_low);
    RUN_TEST(test_set_pin_speed_to_medium);
    RUN_TEST(test_set_pin_speed_to_high);
    RUN_TEST(test_set_output_type_pull_push);
    RUN_TEST(test_set_output_type_open_drain);
    RUN_TEST(test_check_input);
    RUN_TEST(test_lock_pin_configuration);

    return UNITY_END();
}

