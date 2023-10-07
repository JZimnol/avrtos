#ifndef AVRTOS_DELAY_H_
#define AVRTOS_DELAY_H_

#include <inttypes.h>

#include "avrtos_config.h"
#include "boards/avrtos_board_impl.h"

#define AVRTOS_SECONDS_TO_MICROSECONDS (1000000)
#define AVRTOS_MILLISECONDS_TO_MICROSECONDS (1000)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Prepares task for non-blocking delay. Assumes that the delay timer has been
 * implemented properly.
 *
 * @param delay_us The number of microseconds that the task should wait. A
 *                 smaller value will result in lower delay accuracy.
 */
void avrtos_delay_us(uint64_t delay_us);

/**
 * Prepares task for non-blocking delay. Assumes that the delay timer has been
 * implemented properly.
 *
 * @param delay_us The number of milliseconds that the task should wait.
 */
static inline void avrtos_delay_ms(uint64_t delay_ms) {
    avrtos_delay_us(delay_ms * (uint64_t) AVRTOS_MILLISECONDS_TO_MICROSECONDS);
}

/**
 * Prepares task for non-blocking delay. Assumes that the delay timer has been
 * implemented properly.
 *
 * @param delay_us The number of seconds that the task should wait.
 */
static inline void avrtos_delay_s(uint64_t delay_s) {
    avrtos_delay_us(delay_s * (uint64_t) AVRTOS_SECONDS_TO_MICROSECONDS);
}

/**
 * Returns the number of microseconds that have elapsed since the delay timer
 * was started. Should be a "private" function.
 */
static inline uint64_t _avrtos_delay_get_microseconds(void) {
    return avrtos_delay_get_microseconds_impl();
}

/**
 * Initializes delay timer. This function should use
 * @ref AVRTOS_CPU_CLOCK_FREQUENCY. Should be a "private" function.
 */
static inline void _avrtos_delay_timer_init(void) {
    avrtos_delay_timer_init_impl();
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* AVRTOS_DELAY_H_ */
