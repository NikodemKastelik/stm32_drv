#ifndef _HAL_COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if defined(STM32_SERIES_F0)
    #include <stm32f0xx.h>
#elif defined(STM32_SERIES_F4)
    #include <stm32f4xx.h>
#else
    #error "No STM32 series defined"
#endif

/**
 * @brief Macro for calculating array element count.
 *
 * @param array  Name of array to check.
 *
 * @retval  Specified array element count.
 */
#define ARRAY_SIZE(array)  (sizeof(array) / sizeof(array[0]))

/**
 * @brief Macro for assertion checking.
 *
 * If specified expression evaluates to false, code will be stuck in infinite loop,
 * for debugger to examine. Asssertions can be disabled in release version for
 * code maximum performance.
 *
 * @param expr  Expression to check for validity.
 */
#if defined(HAL_DEBUG) && (HAL_DEBUG == 1)
#define ASSERT(expr)        \
    if(!(expr)) while(1){};
#else
#define ASSERT(expr)
#endif

#endif // _HAL_COMMON_H_
