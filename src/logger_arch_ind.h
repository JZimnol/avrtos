#ifndef LOGGER_ARCH_IND_H_
#define LOGGER_ARCH_IND_H_

#include <stddef.h>

#include "avrtos_config.h"
#include "avrtos_utils.h"
#include "boards/avrtos_board_impl.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER

/**
 * Creates a log message and appends it to the global log buffer. Should be a
 * "private" function.
 *
 * @param module Stringified module name.
 *
 * @param level  Stringified log level.
 *
 * @param msg    Message to be added to the buffer.
 *
 * @param vaargs Specified formatting of the message.
 */
void _avrtos_log_buffer_append(const char *module,
                               const char *level,
                               const char *msg,
                               ...);

/**
 * Initialize asynchronous logger. This function should be defined separately
 * for each AVR board (if no abstraction-layer defines have been created).
 * Should be a "private" function.
 */
static inline void _avrtos_logger_init(void) {
    avrtos_logger_init_impl();
}

/**
 * Handles created log message. This function should be defined separately for
 * each AVR board (if no abstraction-layer defines have been created). Should be
 * a "private" function.
 *
 * @param buffer Pointer to the buffer containing formatted log message.
 *
 * @param buf_len Length of the log buffer.
 */
static inline void _avrtos_handle_log_buffer(char *buffer, size_t buf_len) {
    avrtos_handle_log_buffer_impl(buffer, buf_len);
}

#define LOG_LEVEL_ERROR_CONTAIN_ERROR 1

#define LOG_LEVEL_WARNING_CONTAIN_ERROR 1
#define LOG_LEVEL_WARNING_CONTAIN_WARNING 1

#define LOG_LEVEL_INFO_CONTAIN_ERROR 1
#define LOG_LEVEL_INFO_CONTAIN_WARNING 1
#define LOG_LEVEL_INFO_CONTAIN_INFO 1

#define LOG_LEVEL_DEBUG_CONTAIN_ERROR 1
#define LOG_LEVEL_DEBUG_CONTAIN_WARNING 1
#define LOG_LEVEL_DEBUG_CONTAIN_INFO 1
#define LOG_LEVEL_DEBUG_CONTAIN_DEBUG 1

#define _LOG_TYPE(Level, TrueValue, FalseValue)                                \
    AVRTOS_IS_DEFINED(                                                         \
            AVRTOS_CONCAT(AVRTOS_CONCAT(LOG_LEVEL_, AVRTOS_LOG_DEFAULT_LEVEL), \
                          AVRTOS_CONCAT(_CONTAIN_, Level)),                    \
            TrueValue, FalseValue)

#define _LOG_EMPTY(...) \
    do {                \
    } while (0)

#define _LOG_NOT_EMPTY(Module, Level, ...)              \
    _avrtos_log_buffer_append(AVRTOS_STRINGIFY(Module), \
                              AVRTOS_STRINGIFY(Level), __VA_ARGS__)

#define _AVRTOS_LOG(Module, Level, ...)                      \
    AVRTOS_CONCAT(_LOG_, _LOG_TYPE(Level, NOT_EMPTY, EMPTY)) \
    (Module, Level, __VA_ARGS__)

/**
 * Prepares proper log message and passes it to @ref
 * avrtos_handle_log_buffer_impl.
 */
#define avrtos_log(Module, Level, ...) _AVRTOS_LOG(Module, Level, __VA_ARGS__)
#else // AVRTOS_WITH_ASYNCHRONOUS_LOGGER
#define avrtos_log(...) \
    do {                \
    } while (0)
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* LOGGER_ARCH_IND_H_ */
