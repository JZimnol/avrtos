#ifndef AVRTOS_MUTEX_H_
#define AVRTOS_MUTEX_H_

#include <stdbool.h>

#include "avrtos_config.h"
#include "avrtos_core.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Simple mutex struct. Field "task_id" is equal to the id of the task that
 * currently keeps the mutex locked.
 */
struct avrtos_mutex {
    bool locked;
    uint8_t task_id;
};

#ifdef AVRTOS_WITH_MUTEX
/**
 * Locks the mutex. If the mutex is already locked, yields the current task and
 * tries again to lock the mutex. Does not have a timeout.
 *
 * @param mutex Pointer to the mutex we want to lock.
 *
 * @returns true on mutex lock.
 */
bool avrtos_mutex_lock(struct avrtos_mutex *mutex);
/**
 * Unlocks the mutex if the current task is the one that locked the mutex.
 *
 * @param mutex Pointer to the mutex we want to unlock.
 *
 * @returns false if current task did not locked the mutex or the mutex has not
 *          been locked, true otherwise
 */
bool avrtos_mutex_unlock(struct avrtos_mutex *mutex);
#else // AVRTOS_WITH_MUTEX
#define avrtos_mutex_lock(...) true
#define avrtos_mutex_unlock(...) true
#endif // AVRTOS_WITH_MUTEX

#ifdef AVRTOS_WITH_MUTEX
/**
 * Simple mutex definition.
 *
 * @param MutexName Name of the mutex that will be used as a mutex identifier.
 */
#define AVRTOS_MUTEX_DEFINE(MutexName)                \
    struct avrtos_mutex MutexName = {.locked = false, \
                                     .task_id = AVRTOS_INVALID_TASK_ID}
#else // AVRTOS_WITH_MUTEX
#define AVRTOS_MUTEX_DEFINE(...)
#endif // AVRTOS_WITH_MUTEX

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* AVRTOS_MUTEX_H_ */
