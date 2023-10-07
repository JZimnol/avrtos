#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/atomic.h>

#include "avrtos_config.h"
#include "avrtos_core.h"
#include "avrtos_delay.h"
#include "boards/avrtos_board_impl.h"
#include "linked_list_arch_ind.h"

#ifdef AVRTOS_WITH_GPIO_TRACE
#include "avrtos_gpio_trace.h"
#endif // AVRTOS_WITH_GPIO_TRACE

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER
#include "avrtos_logger.h"
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

#define PUSH_TO_STACK(Register) __asm__ volatile("push " #Register " \n\t");
#define PUSH_MULTIPLE_TO_STACK(...) AVRTOS_MAP(PUSH_TO_STACK, __VA_ARGS__)

#define POP_FROM_STACK(Register) __asm__ volatile("pop " #Register " \n\t");
#define POP_MULTIPLE_FROM_STACK(...) AVRTOS_MAP(POP_FROM_STACK, __VA_ARGS__)

#define SET_TO_ZERO(Register) \
    __asm__ volatile("mov " #Register ", __zero_reg__ \n\t");
#define SET_MULTIPLE_TO_ZERO(...) AVRTOS_MAP(SET_TO_ZERO, __VA_ARGS__)

volatile uint16_t g_main_task_sp = 0x08ff;
volatile struct avrtos_task *HEAD = NULL;
volatile struct avrtos_task *g_current_task = NULL;

AVRTOS_TASK_DEFINE(_idle_task);
uint8_t _idle_task_stack[50];

static void _idle_thread(void *arg) {
    while (1) {
        /* consider putting CPU to sleep */
        avrtos_task_yield();
    }
}

static bool task_is_on_delay(volatile struct avrtos_task *task) {
    return (task->state == AVRTOS_WAITING);
}

static bool task_should_exit_delay(volatile struct avrtos_task *task) {
    return (task->delay_until < _avrtos_delay_get_microseconds());
}

static void task_mask_as_running_if_needed(void) {
    if (g_current_task->state != AVRTOS_NOT_INITIALIZED) {
        g_current_task->state = AVRTOS_RUNNING;
    }
}

static void task_mark_as_ready_if_needed(void) {
    if (g_current_task->state != AVRTOS_WAITING) {
        g_current_task->state = AVRTOS_READY;
    }
}

static void task_find_next_suitable(void) {
    /* @TODO: create a (better) scheduling algorithm, consider idle low-power
       task */
    while (true) {
        volatile struct avrtos_task *iterator;
        volatile struct avrtos_task *start_from =
                g_current_task->next ? g_current_task->next : HEAD;
        LINKED_LIST_FOREACH_FROM_SPECIFIED(HEAD, start_from, iterator) {
            if (task_is_on_delay(iterator)) {
                if (task_should_exit_delay(iterator)) {
                    g_current_task = iterator;
                    goto found;
                }
            } else {
                g_current_task = iterator;
                goto found;
            }
        }
    }
found:
    return;
}

static void task_select_next(void) {
#ifdef AVRTOS_WITH_GPIO_TRACE
    _avrtos_gpio_trace_clear((struct avrtos_task *) g_current_task);
#endif // AVRTOS_WITH_GPIO_TRACE

    task_mark_as_ready_if_needed();
    task_find_next_suitable();
    task_mask_as_running_if_needed();

#ifdef AVRTOS_WITH_GPIO_TRACE
    _avrtos_gpio_trace_set((struct avrtos_task *) g_current_task);
#endif // AVRTOS_WITH_GPIO_TRACE
}

static int task_add_to_list(struct avrtos_task *task) {
    if (linked_list_initailize_head((linked_list_t **) &HEAD, task)
        == LINKED_LIST_OK) {
        /* first task, initialize as HEAD and g_current_task */
        g_current_task = HEAD;
    } else if (linked_list_append((linked_list_t *) HEAD, task)) {
        return 1;
    }

    return 0;
}

static void task_sp_and_state_init(void) {
    /* decrement task SP by 2 == sizeof(task_fun_t)
       NOTE: this couldn't be done in avrtos_task_create() function due to the
       fact that "task" variable was just an argument and there is a problem in
       setting argument values using assembler manually */
    g_current_task->sp -= sizeof(g_current_task->function);
    g_current_task->state = AVRTOS_RUNNING;
}

struct avrtos_task *_avrtos_current_task_get(void) {
    return (struct avrtos_task *) g_current_task;
}

int avrtos_task_create(struct avrtos_task *task,
                       void (*function)(void *),
                       uint8_t *stack,
                       uint16_t stack_size,
                       void *arg) {
    static uint8_t id = 1;

    if (!(function && stack && stack_size != 0)) {
        return 1;
    }

    if (task_add_to_list(task)) {
        return 1;
    }

    task->id = id++;
    task->function = function;
    task->sp = (uint16_t) stack + stack_size - 1;
    task->sreg = 0x00;
    task->state = AVRTOS_NOT_INITIALIZED;
    task->next = NULL;
    task->arg = arg;

    PUSH_MULTIPLE_TO_STACK(r31, r30, r29, r28, r27, r26);
    __asm__ volatile(
            /* save current SP */
            "in r27, __SP_H__ \n\t"
            "in r26, __SP_L__ \n\t"
            /* change SP to new task stack */
            "out __SP_L__, %A1 \n\t"
            "out __SP_H__, %B1 \n\t"
            /* push task function address to the beginning of tasks stack
               this will be used with the first reti instruction */
            "push %A0 \n\t"
            "push %B0 \n\t"
            /* restore original SP */
            "out __SP_L__, r26 \n\t"
            "out __SP_H__, r27 \n\t"
            ""
            :
            : "z"(task->function), "y"(task->sp));
    POP_MULTIPLE_FROM_STACK(r26, r27, r28, r29, r30, r31);

    return 0;
}

void avrtos_task_yield(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        _avrtos_sched_timer_reset();
    }
    __asm__ volatile("call scheduler_interupt_start \n\t");
}

uint8_t _avrtos_current_task_id(void) {
    return g_current_task ? g_current_task->id : AVRTOS_INVALID_TASK_ID;
}

void avrtos_scheduler_start(void) {
#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER
    _avrtos_logger_init();
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

    (void) avrtos_task_create(&_idle_task, _idle_thread, _idle_task_stack,
                              sizeof(_idle_task_stack), NULL);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        _avrtos_sched_timer_init();
        _avrtos_delay_timer_init();
    }

    __asm__ volatile("JMP task_deploy_start \n\t");
}

ISR(TIMER0_COMPA_vect, ISR_NAKED) {
    /* This interrupt should either have generic vector name or be placed inside
       boards/avrtos_<board>.c file. Should be fixed in the future. */
    __asm__ volatile("scheduler_interupt_start: \n\t");

    cli();

    /* push current task registers onto its stack */
    PUSH_MULTIPLE_TO_STACK(r31, r30, r29, r28, r27, r26, r25, r24, r23, r22,
                           r21, r20, r19, r18, r17, r16, r15, r14, r13, r12,
                           r11, r10, r9, r8, r7, r6, r5, r4, r3, r2, r1, r0);

    /* save tasks SP and SREG to its struct */
    __asm__ volatile("in %A0, __SP_L__ \n\t"
                     "in %B0, __SP_H__ \n\t"
                     "in %A1, __SREG__ \n\t"
                     ""
                     : "=e"(g_current_task->sp), "=e"(g_current_task->sreg)
                     :);

    /* set SP to main stack for scheduling operations */
    __asm__ volatile("out __SP_L__, %A0 \n\t"
                     "out __SP_H__, %B0 \n\t"
                     :
                     : "e"(g_main_task_sp));

    sei();
    task_select_next();
    cli();

    /* save main SP to its variable */
    __asm__ volatile("in %A0, __SP_L__ \n\t"
                     "in %B0, __SP_H__ \n\t"
                     ""
                     : "=e"(g_main_task_sp)
                     :);

    if (g_current_task->state == AVRTOS_NOT_INITIALIZED) {
        __asm__ volatile("task_deploy_start: \n\t");

        task_sp_and_state_init();

        /* save main SP to its variable */
        __asm__ volatile("in %A0, __SP_L__ \n\t"
                         "in %B0, __SP_H__ \n\t"
                         ""
                         : "=e"(g_main_task_sp)
                         :);

        /* set SP to current task SP */
        __asm__ volatile("out __SP_L__, %A0 \n\t"
                         "out __SP_H__, %B0 \n\t"
                         ""
                         :
                         : "e"(g_current_task->sp));

        /* move task argument pointer into the r24 and r25 registers */
        __asm__ volatile("mov r24, %A0 \n\t"
                         "mov r25, %B0 \n\t"
                         ""
                         :
                         : "x"(g_current_task->arg));

        /* set all registers and SREG to zero
        (except r1 == __zero_reg__ and r25,r24 == argument registers) */
        SET_MULTIPLE_TO_ZERO(r0, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12,
                             r13, r14, r15, r16, r17, r18, r19, r20, r21, r22,
                             r23, r26, r27, r28, r29, r30, r31);
        __asm__ volatile("out __SREG__, __zero_reg__ \n\t");

        sei();
        __asm__ volatile("reti");
    }

    /* restore tasks SP and SREG */
    __asm__ volatile("out __SP_L__, %A0 \n\t"
                     "out __SP_H__, %B0 \n\t"
                     "out __SREG__, %A1 \n\t"
                     ""
                     :
                     : "e"(g_current_task->sp), "e"(g_current_task->sreg));

    /* pop current task registers from its stack */
    POP_MULTIPLE_FROM_STACK(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11,
                            r12, r13, r14, r15, r16, r17, r18, r19, r20, r21,
                            r22, r23, r24, r25, r26, r27, r28, r29, r30, r31);

    __asm__ volatile("reti \n\t");
}
