#ifndef AVRTOS_CORE_H_
#define AVRTOS_CORE_H_

#include <inttypes.h>
#include <stddef.h>

#include "avrtos_init.h"
#include "boards/avrtos_board_impl.h"

#ifdef AVRTOS_WITH_GPIO_TRACE
#include "avrtos_gpio_trace.h"
#endif // AVRTOS_WITH_GPIO_TRACE

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Specifies task current state.
 */
enum avrtos_task_state {
    AVRTOS_NOT_INITIALIZED,
    AVRTOS_READY,
    AVRTOS_WAITING,
    AVRTOS_RUNNING
};

/**
 * Struct containing all required task information.
 */
struct avrtos_task {
    uint8_t id;
    void (*function)(void *);
    uint16_t sp;
    uint8_t sreg;
    enum avrtos_task_state state;
    void *arg;
#ifdef AVRTOS_WITH_GPIO_TRACE
    struct avrtos_gpio_trace *gpio_trace;
#endif // AVRTOS_WITH_GPIO_TRACE
    uint64_t delay_until;
    struct avrtos_task *next;
};

/**
 * Creates avrtos task. Links task function, task stack and task arguments with
 * specified @ref struct avrtos_task. Appends linked list with the initialized
 * task.
 *
 * @param task       Pointer to the static @ref struct avrtos_task structure.
 *                   This structure will be added to the global task linked
 *                   list.
 *
 * @param function   Pointer to the already existing task function.
 *
 * @param stack      Pointer to the beginning of the task's stack.
 *
 * @param stack_size Size of the task's stack size. Should be equal to
 *                   sizeof(stack).
 *
 * @param arg        Pointer to a generic task argument.
 *
 * @returns non-zero value if @p task is NULL,
 *          0 otherwise.
 */
int avrtos_task_create(struct avrtos_task *task,
                       void (*function)(void *),
                       uint8_t *stack,
                       uint16_t stack_size,
                       void *arg);

/**
 * Sets proper values to the required timer(s). Turns on interrupts. Deploys
 * first task.
 */
void avrtos_scheduler_start(void);

/**
 * Yields the current task.
 */
void avrtos_task_yield(void);

/**
 * Returns the ID of the current task. Should be a "private" function.
 */
uint8_t _avrtos_current_task_id(void);

/**
 * Initializes scheduler's periodic timer. Should be a "private" function.
 */
static inline void _avrtos_sched_timer_init(void) {
    avrtos_sched_timer_init_impl();
}

/**
 * Reset scheduler's timer to 0, prevents current task to be yielded in the next
 * few clock cycles. Should be a "private" function.
 */
static inline void _avrtos_sched_timer_reset(void) {
    avrtos_sched_timer_reset_impl();
}

/**
 * Stop the scheduler's Timer. Useful for time-sensitive kernel operations.
 * Should be a "private" function.
 */
static inline void _avrtos_sched_timer_stop(void) {
    avrtos_sched_timer_stop_impl();
}

/**
 * Resumes the scheduler's Timer. Should be a "private" function.
 */
static inline void _avrtos_sched_timer_resume(void) {
    avrtos_sched_timer_resume_impl();
}

/**
 * Returns pointer to the current task. Should be a "private" function.
 */
struct avrtos_task *_avrtos_current_task_get(void);

/**
 * Minimal stack size that does not crashes the basic application.
 */
#define AVRTOS_STACK_REGISTERS_SIZE 32
#define AVRTOS_STACK_BASIC_BYTES 32
#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER
#define STACK_ADDITIONAL_BYTES AVRTOS_SINGLE_LOG_MAX_SIZE + 100
#else // AVRTOS_WITH_ASYNCHRONOUS_LOGGER
#define STACK_ADDITIONAL_BYTES 0
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

#define AVRTOS_MINIMAL_STACK_SIZE                           \
    (AVRTOS_STACK_REGISTERS_SIZE + AVRTOS_STACK_BASIC_BYTES \
     + STACK_ADDITIONAL_BYTES)

/**
 * Simple task declaration. Does not initialize the struct
 *
 * @param TaskName Name of the avrtos task that will be used as task identifier.
 */
#define AVRTOS_TASK_DEFINE(TaskName) struct avrtos_task TaskName

/**
 * Simple stack declaration. When used as a global variable, will be initialized
 * with zeros.
 *
 * @param StackName Name of the buffer that will be used as stack identifier.
 *
 * @param StackSize Size of the stack. Should be at least 32 bytes, preferably
 *                  >= @ref AVRTOS_MINIMAL_STACK_SIZE.
 */
#define AVRTOS_STACK_DEFINE(StackName, StackSize)                      \
    AVRTOS_STATIC_ASSERT(                                              \
            StackSize >= AVRTOS_MINIMAL_STACK_SIZE,                    \
            SpecifiedStackSizeIsSmallerThanAVRTOS_MINIMAL_STACK_SIZE); \
    uint8_t StackName[StackSize]

/**
 * Invalid task_id for a mutex. There should be no task with id == 0.
 */
#define AVRTOS_INVALID_TASK_ID (0)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* AVRTOS_CORE_H_ */
