#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#include "../avrtos_config.h"
#include "../avrtos_mutex.h"
#include "../avrtos_utils.h"
#include "../circular_buffer_arch_ind.h"

#if defined(__AVR_ATmega328P__)

#define ONE_MHZ 1000000UL
#define OCR0A_VAL_FOR_ONE_MHZ 4
#define OCR2A_VAL_FOR_ONE_MHZ 12.5

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER
#if AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE > 38400
#define UBRR_REG_VAL \
    ((AVRTOS_CPU_CLOCK_FREQUENCY / (8UL * AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE)))
#else // AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE > 38400
#define UBRR_REG_VAL                                 \
    ((AVRTOS_CPU_CLOCK_FREQUENCY                     \
      / (16UL * AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE)) \
     - 1)
#endif // AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE > 38400
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER
struct circular_buffer g_logger_circ_buff;
char g_logger_buffer[AVRTOS_LOG_BUFFER_SIZE];
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

volatile uint64_t g_microseconds_counter;

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER
AVRTOS_MUTEX_DEFINE(logger_mutex);

void avrtos_logger_init_impl(void) {
    (void) circ_buff_initialize(&g_logger_circ_buff, g_logger_buffer,
                                AVRTOS_LOG_BUFFER_SIZE);

    /* Set the baud rate registers */
    UBRR0H = (uint8_t)(UBRR_REG_VAL >> 8);
    UBRR0L = (uint8_t) UBRR_REG_VAL;

    /* Enable transmitter and USART Tx interrupt */
    AVRTOS_SET_BIT_IN_REGISTER(UCSR0B, TXEN0);
    AVRTOS_SET_BIT_IN_REGISTER(UCSR0B, UDRIE0);

#if AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE > 38400
    /* double transmission speed */
    AVRTOS_SET_BIT_IN_REGISTER(UCSR0A, U2X0);
#endif // AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE > 38400

    /* Set data frame format (8-bit data, no parity, 1 stop bit) */
    AVRTOS_SET_BIT_IN_REGISTER(UCSR0C, UCSZ00);
    AVRTOS_SET_BIT_IN_REGISTER(UCSR0C, UCSZ01);
}

void avrtos_handle_log_buffer_impl(char *buffer, size_t buf_len) {
    if (!buffer) {
        return;
    }

    avrtos_mutex_lock(&logger_mutex);
    size_t index = 0;
    while (buffer[index] && index < buf_len) {
        enum circular_buffer_status ret;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ret = circ_buff_insert_one(&g_logger_circ_buff, buffer[index]);
        }

        if (ret == CIRC_BUFF_OK) {
            index++;
            AVRTOS_SET_BIT_IN_REGISTER(UCSR0B, UDRIE0);
        } else if (ret == CIRC_BUFF_INVALID) {
            break;
        }
    }
    avrtos_mutex_unlock(&logger_mutex);
}
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

void avrtos_sched_timer_reset_impl(void) {
    TCNT0 = 0;
}

void avrtos_sched_timer_init_impl(void) {
    /* Not gonna lie, I'm lazy on that one. I've set it to fixed value based on
       (1 MHz CPU clock) * (multiplier). This should be configurable in a
       prettier way. */
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = 0;
    AVRTOS_SET_BIT_IN_REGISTER(TCCR0A, WGM01);
    AVRTOS_SET_BIT_IN_REGISTER(TCCR0B, CS01);
    AVRTOS_SET_BIT_IN_REGISTER(TCCR0B, CS00);
    OCR0A = OCR0A_VAL_FOR_ONE_MHZ
            * (uint8_t)(AVRTOS_CPU_CLOCK_FREQUENCY / ONE_MHZ);
    AVRTOS_SET_BIT_IN_REGISTER(TIMSK0, OCIE0A);
}

void avrtos_sched_timer_stop_impl() {
    AVRTOS_CLEAR_BIT_IN_REGISTER(TIMSK0, OCIE0A);
}
void avrtos_sched_timer_resume_impl() {
    AVRTOS_SET_BIT_IN_REGISTER(TIMSK0, OCIE0A);
}

void avrtos_delay_timer_init_impl(void) {
    /* Not gonna lie, I'm lazy on that one. I've set it to fixed value based on
       (1 MHz CPU clock) * (multiplier). This should be configurable in a
       prettier way. */
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    AVRTOS_SET_BIT_IN_REGISTER(TCCR2A, WGM21);
    AVRTOS_SET_BIT_IN_REGISTER(TCCR2B, CS21);
    OCR2A = OCR2A_VAL_FOR_ONE_MHZ
            * (uint8_t)(AVRTOS_CPU_CLOCK_FREQUENCY / ONE_MHZ);
    AVRTOS_SET_BIT_IN_REGISTER(TIMSK2, OCIE2A);
}

uint64_t avrtos_delay_get_microseconds_impl(void) {
    uint64_t ret;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ret = g_microseconds_counter;
    }

    return ret;
}

ISR(TIMER2_COMPA_vect) {
    /* Not gonna lie, I'm lazy on that one. I've set it to fixed value based on
       (1 MHz CPU clock) * (multiplier). This should be configurable in a
       prettier way. */
    g_microseconds_counter += 100;
}

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER
ISR(USART_UDRE_vect) {
    char value;
    if (circ_buff_get_one(&g_logger_circ_buff, &value) == CIRC_BUFF_OK) {
        UDR0 = value;
    } else {
        AVRTOS_CLEAR_BIT_IN_REGISTER(UCSR0B, UDRIE0);
    }
}
#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

#endif // defined(__AVR_ATmega328P__)
