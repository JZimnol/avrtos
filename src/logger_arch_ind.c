#include <stdarg.h>
#include <stdio.h>

#include "logger_arch_ind.h"

#ifdef AVRTOS_UNIT_TEST
#undef AVRTOS_SINGLE_LOG_MAX_SIZE
#define AVRTOS_SINGLE_LOG_MAX_SIZE AVRTOS_UNIT_TEST_SINGLE_LOG_MAX_SIZE
#endif // AVRTOS_UNIT_TEST

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER

void _avrtos_log_buffer_append(const char *module,
                               const char *level,
                               const char *msg,
                               ...) {
    char buffer[AVRTOS_SINGLE_LOG_MAX_SIZE];
    char *pointer = buffer;

    int message_header_expected_size =
            snprintf(pointer, AVRTOS_SINGLE_LOG_MAX_SIZE, "%s [%s] ", level,
                     module);

    if (message_header_expected_size + 1 >= AVRTOS_SINGLE_LOG_MAX_SIZE) {
        /* buffer overflow, last characters should be equal to "\n\0" */
        buffer[AVRTOS_SINGLE_LOG_MAX_SIZE - 2] = '\n';
    } else {
        pointer += message_header_expected_size;
        va_list argptr;
        va_start(argptr, msg);
        int message_content_expected_size = vsnprintf(
                pointer,
                AVRTOS_SINGLE_LOG_MAX_SIZE - message_header_expected_size, msg,
                argptr);
        va_end(argptr);

        if (message_header_expected_size + message_content_expected_size + 1
            >= AVRTOS_SINGLE_LOG_MAX_SIZE) {
            /* buffer overflow, last characters should be equal to "\n\0" */
            buffer[AVRTOS_SINGLE_LOG_MAX_SIZE - 2] = '\n';
        } else {
            pointer += message_content_expected_size;
            *(pointer++) = '\n';
            *pointer = '\0';
        }
    }

    _avrtos_handle_log_buffer(buffer, AVRTOS_SINGLE_LOG_MAX_SIZE);
}

#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER
