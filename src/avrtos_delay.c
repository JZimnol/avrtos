#include <util/atomic.h>

#include "avrtos_core.h"
#include "avrtos_delay.h"

void avrtos_delay_us(uint64_t delay_us) {
    if (delay_us == 0) {
        return;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        _avrtos_sched_timer_reset();
    }
    _avrtos_sched_timer_stop();

    struct avrtos_task *current = _avrtos_current_task_get();
    current->delay_until = _avrtos_delay_get_microseconds() + delay_us;
    current->state = AVRTOS_WAITING;

    _avrtos_sched_timer_resume();

    avrtos_task_yield();
}
