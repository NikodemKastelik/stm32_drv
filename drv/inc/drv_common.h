#ifndef _DRV_COMMON_H_
#define _DRV_COMMON_H_

#include <stm32drv_config.h>

/**
 * @brief Macro for checking if given symbol is defined and set to '1'.
 *
 * @param[in] cfg Symbol to check.
 *
 * @retval '1' If given symbol is defined and set to '1'.
 * @retval '0' If given symbol is undefined or set to different value than '1'.
 */
#define DRV_CHECK(cfg)   _DRV_CHECK(cfg)
#define _DUMMY_ARG_1 0,
#define _DRV_CHECK(cfg)  __DRV_CHECK(_DUMMY_ARG_ ## cfg)
#define __DRV_CHECK(cfg) ___DRV_CHECK(cfg 1, 0)
#define ___DRV_CHECK(_ignored, result, ...) result

#endif // _DRV_COMMON_H_
