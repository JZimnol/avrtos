#include <avr/io.h>
#include <stdlib.h>

#include "avrtos_config.h"

#ifdef AVRTOS_WITH_GPIO_TRACE

#include "avrtos_core.h"
#include "avrtos_gpio_trace.h"

void avrtos_gpio_trace_install(struct avrtos_task *task,
                               struct avrtos_gpio_trace *gpio_trace) {
    uint8_t *ddrx_register = gpio_trace->port - AVRTOS_MMIO_DDRX_PORTX_OFFSET;
    AVRTOS_SET_BIT_IN_REGISTER(*ddrx_register, gpio_trace->pin);
    task->gpio_trace = gpio_trace;
}

static void gpio_trace_set(struct avrtos_gpio_trace *gpio_trace) {
    AVRTOS_SET_BIT_IN_REGISTER(*gpio_trace->port, gpio_trace->pin);
}

void _avrtos_gpio_trace_set(struct avrtos_task *task) {
    struct avrtos_gpio_trace *gpio_trace = task->gpio_trace;
    if (gpio_trace) {
        gpio_trace_set(gpio_trace);
    }
}

static void gpio_trace_clear(struct avrtos_gpio_trace *gpio_trace) {
    AVRTOS_CLEAR_BIT_IN_REGISTER(*gpio_trace->port, gpio_trace->pin);
}

void _avrtos_gpio_trace_clear(struct avrtos_task *task) {
    struct avrtos_gpio_trace *gpio_trace = task->gpio_trace;
    if (gpio_trace) {
        gpio_trace_clear(gpio_trace);
    }
}

#endif // AVRTOS_WITH_GPIO_TRACE
