#ifndef AVRTOS_BOARD_IMPL_H_
#define AVRTOS_BOARD_IMPL_H_

#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void avrtos_sched_timer_init_impl(void);
void avrtos_sched_timer_reset_impl(void);
void avrtos_sched_timer_stop_impl();
void avrtos_sched_timer_resume_impl();

void avrtos_logger_init_impl(void);
void avrtos_handle_log_buffer_impl(char *buffer, size_t buf_len);

void avrtos_delay_timer_init_impl(void);
uint64_t avrtos_delay_get_microseconds_impl(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* AVRTOS_BOARD_IMPL_H_ */
