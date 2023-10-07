#ifndef AVRTOS_GPIO_TRACE_H_
#define AVRTOS_GPIO_TRACE_H_

#include <inttypes.h>
#include <stdlib.h>

#include "avrtos_init.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct avrtos_gpio_trace {
    uint8_t *port;
    uint8_t pin;
};

/**
 * Offsets between PINx, DDRx, PORTx registers.
 */
#define AVRTOS_MMIO_PINX_DDRX_OFFSET (0x01)
#define AVRTOS_MMIO_DDRX_PORTX_OFFSET (0x01)
#define AVRTOS_MMIO_PINX_PORTX_OFFSET (0x02)

/**
 * MMIO addresses of PINx, DDRx, PORTx registers.
 */
#define AVRTOS_MMIO_PINA_ADDRESS (0x00)
#define AVRTOS_MMIO_DDRA_ADDRESS \
    (AVRTOS_MMIO_PINA_ADDRESS + AVRTOS_MMIO_PINX_DDRX_OFFSET)
#define AVRTOS_MMIO_PORTA_ADDRESS \
    (AVRTOS_MMIO_PINA_ADDRESS + AVRTOS_MMIO_PINX_PORTX_OFFSET)

#define AVRTOS_MMIO_PINB_ADDRESS (0x03)
#define AVRTOS_MMIO_DDRB_ADDRESS \
    (AVRTOS_MMIO_PINB_ADDRESS + AVRTOS_MMIO_PINX_DDRX_OFFSET)
#define AVRTOS_MMIO_PORTB_ADDRESS \
    (AVRTOS_MMIO_PINB_ADDRESS + AVRTOS_MMIO_PINX_PORTX_OFFSET)

#define AVRTOS_MMIO_PINC_ADDRESS (0x06)
#define AVRTOS_MMIO_DDRC_ADDRESS \
    (AVRTOS_MMIO_PINC_ADDRESS + AVRTOS_MMIO_PINX_DDRX_OFFSET)
#define AVRTOS_MMIO_PORTC_ADDRESS \
    (AVRTOS_MMIO_PINC_ADDRESS + AVRTOS_MMIO_PINX_PORTX_OFFSET)

#define AVRTOS_MMIO_PIND_ADDRESS (0x09)
#define AVRTOS_MMIO_DDRD_ADDRESS \
    (AVRTOS_MMIO_PIND_ADDRESS + AVRTOS_MMIO_PINX_DDRX_OFFSET)
#define AVRTOS_MMIO_PORTD_ADDRESS \
    (AVRTOS_MMIO_PIND_ADDRESS + AVRTOS_MMIO_PINX_PORTX_OFFSET)

#define AVRTOS_MMIO_PINE_ADDRESS (0x0c)
#define AVRTOS_MMIO_DDRE_ADDRESS \
    (AVRTOS_MMIO_PINE_ADDRESS + AVRTOS_MMIO_PINX_DDRX_OFFSET)
#define AVRTOS_MMIO_PORTE_ADDRESS \
    (AVRTOS_MMIO_PINE_ADDRESS + AVRTOS_MMIO_PINX_PORTX_OFFSET)

/**
 * Offset between MMIO and SRAM addresses.
 */
#define AVRTOS_MMIO_OFFSET (0x20)
#define AVRTOS_MMIO_ADDRESS_WITH_OFFSTET(Address) \
    ((Address) + AVRTOS_MMIO_OFFSET)

/**
 * Pointer type of PINx, DDRx, PORTx registers.
 */
#define AVRTOS_MMIO_ADDRESS_POINTER_TYPE (uint8_t *)

/**
 * SRAM addresses of PORTx registers.
 */
#define AVRTOS_GPIO_TRACE_ADDRESS_PORTA                                \
    AVRTOS_MMIO_ADDRESS_POINTER_TYPE AVRTOS_MMIO_ADDRESS_WITH_OFFSTET( \
            AVRTOS_MMIO_PORTA_ADDRESS)
#define AVRTOS_GPIO_TRACE_ADDRESS_PORTB                                \
    AVRTOS_MMIO_ADDRESS_POINTER_TYPE AVRTOS_MMIO_ADDRESS_WITH_OFFSTET( \
            AVRTOS_MMIO_PORTB_ADDRESS)
#define AVRTOS_GPIO_TRACE_ADDRESS_PORTC                                \
    AVRTOS_MMIO_ADDRESS_POINTER_TYPE AVRTOS_MMIO_ADDRESS_WITH_OFFSTET( \
            AVRTOS_MMIO_PORTC_ADDRESS)
#define AVRTOS_GPIO_TRACE_ADDRESS_PORTD                                \
    AVRTOS_MMIO_ADDRESS_POINTER_TYPE AVRTOS_MMIO_ADDRESS_WITH_OFFSTET( \
            AVRTOS_MMIO_PORTD_ADDRESS)
#define AVRTOS_GPIO_TRACE_ADDRESS_PORTE                                \
    AVRTOS_MMIO_ADDRESS_POINTER_TYPE AVRTOS_MMIO_ADDRESS_WITH_OFFSTET( \
            AVRTOS_MMIO_PORTE_ADDRESS)

#define AVRTOS_GPIO_TRACE_ADDRESS_A AVRTOS_GPIO_TRACE_ADDRESS_PORTA
#define AVRTOS_GPIO_TRACE_ADDRESS_B AVRTOS_GPIO_TRACE_ADDRESS_PORTB
#define AVRTOS_GPIO_TRACE_ADDRESS_C AVRTOS_GPIO_TRACE_ADDRESS_PORTC
#define AVRTOS_GPIO_TRACE_ADDRESS_D AVRTOS_GPIO_TRACE_ADDRESS_PORTD
#define AVRTOS_GPIO_TRACE_ADDRESS_E AVRTOS_GPIO_TRACE_ADDRESS_PORTE

/**
 * Simple gpio_trace struct definition.
 *
 * @param TraceName Name of the gpio_trace identifier.
 *
 * @param PortName  Name of the GPIO Port. Can be a single letter (e.g. B) or
 *                  whole port name (e.g. PORTB).
 *
 * @param PinNumber Number of GPIO Pin.
 */
#ifdef AVRTOS_WITH_GPIO_TRACE
#define AVRTOS_GPIO_TRACE_DEFINE(TraceName, PortName, PinNumber)              \
    AVRTOS_STATIC_ASSERT(PinNumber >= 0 && PinNumber <= 7, InvalidPinNumber); \
    struct avrtos_gpio_trace TraceName = {                                    \
            .port = AVRTOS_CONCAT(AVRTOS_GPIO_TRACE_ADDRESS_, PortName),      \
            .pin = PinNumber}
#else // AVRTOS_WITH_GPIO_TRACE
#define AVRTOS_GPIO_TRACE_DEFINE(...)
#endif // AVRTOS_WITH_GPIO_TRACE

/**
 * Arduino UNO pin mapping.
 */
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_0(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 0);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_1(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 1);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_2(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 2);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_3(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 3);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_4(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 4);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_5(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 5);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_6(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 6);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_7(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, D, 7);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_8(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, B, 0);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_9(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, B, 1);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_10(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, B, 2);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_11(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, B, 3);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_12(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, B, 4);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_13(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, B, 5);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_A0(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, C, 0);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_A1(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, C, 1);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_A2(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, C, 2);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_A3(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, C, 3);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_A4(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, C, 4);
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_A5(TraceName) \
    AVRTOS_GPIO_TRACE_DEFINE(TraceName, C, 5);

/**
 * Simple gpio_trace struct definition for Arduino UNO.
 *
 * @param TraceName Name of the gpio_trace identifier.
 *
 * @param PinNumber Number of Arduino Uno Pin (e.g. 1 or A0).
 */
#define AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO(TraceName, PinNumber) \
    AVRTOS_CONCAT(AVRTOS_GPIO_TRACE_DEFINE_ARDUINO_UNO_, PinNumber)(TraceName)

struct avrtos_task;

/**
 * Links gpio_trace struct to specified task.
 *
 * @param task       Name of the avrtos task that the gpio_trace struct should
 *                   be associated with.
 *
 * @param gpio_trace Name of the gpio_trace struct with properly initialized
 *                   Port and Pin fields.
 */
#ifdef AVRTOS_WITH_GPIO_TRACE
void avrtos_gpio_trace_install(struct avrtos_task *task,
                               struct avrtos_gpio_trace *gpio_trace);
#else // AVRTOS_WITH_GPIO_TRACE
#define avrtos_gpio_trace_install(...) \
    do {                               \
    } while (0)
#endif // AVRTOS_WITH_GPIO_TRACE

/**
 * Sets/clears specified task's gpio_trace pin if specified. Should be a
 * "private" function.
 *
 * @param Task which GPIO pin should be set/cleared.
 */
#ifdef AVRTOS_WITH_GPIO_TRACE
void _avrtos_gpio_trace_set(struct avrtos_task *task);
void _avrtos_gpio_trace_clear(struct avrtos_task *task);
#endif // AVRTOS_WITH_GPIO_TRACE

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* AVRTOS_GPIO_TRACE_H_ */
