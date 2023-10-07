#include <util/atomic.h>

#include "avrtos_config.h"
#include "avrtos_core.h"
#include "avrtos_mutex.h"

#ifdef AVRTOS_WITH_MUTEX
static bool mutex_is_locked(struct avrtos_mutex *mutex) {
    return (mutex->locked == true);
}

static bool current_task_locked_the_mutex(struct avrtos_mutex *mutex) {
    bool ret = mutex->task_id == _avrtos_current_task_id();
    return ret;
}

bool avrtos_mutex_lock(struct avrtos_mutex *mutex) {
    while (true) {
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            if (!mutex_is_locked(mutex)) {
                mutex->locked = true;
                mutex->task_id = _avrtos_current_task_id();
                goto locked;
            }
        }
        avrtos_task_yield();
    }

locked:
    return true;
}

bool avrtos_mutex_unlock(struct avrtos_mutex *mutex) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        if (current_task_locked_the_mutex(mutex) && mutex_is_locked(mutex)) {
            mutex->locked = false;
            mutex->task_id = AVRTOS_INVALID_TASK_ID;
            return true;
        }
    }

    return false;
}
#endif // AVRTOS_WITH_MUTEX
