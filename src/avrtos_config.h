#ifndef AVRTOS_CONFIG_H_
#define AVRTOS_CONFIG_H_

/**
 * Enables task gpio-tracing using AVRTOS_GPIO_TRACE_DEFINE() and
 * avrtos_gpio_trace_install() functions.
 */
#define AVRTOS_WITH_GPIO_TRACE

/**
 * Enables usage of atomic mutexes.
 */
#define AVRTOS_WITH_MUTEX

/**
 * Enables usage of compile-time asserts. May increase code size by a few bytes.
 */
#define AVRTOS_WITH_STATIC_ASSERTS

/**
 * CPU frequency in Hz. It's required to set proper register values for
 * specified baud rate and Timer periods.
 */
#define AVRTOS_CPU_CLOCK_FREQUENCY 8000000UL

/**
 * Enables usage of asynchronous logger using avrtos_log() macro. If
 * AVRTOS_WITH_MUTEX is disabled, may produce strange and mixed log messages.
 */
#define AVRTOS_WITH_ASYNCHRONOUS_LOGGER

#ifdef AVRTOS_WITH_ASYNCHRONOUS_LOGGER

/**
 * Size of the global log buffer.
 */
#define AVRTOS_LOG_BUFFER_SIZE 120

/**
 * Maximum size of single log message. If the formatted message is larger than
 * @p AVRTOS_SINGLE_LOG_MAX_SIZE, it will be trimmed.
 */
#define AVRTOS_SINGLE_LOG_MAX_SIZE 60

/**
 * Default level of log messages. If the log message is below default log level,
 * it won't be compiled into code. Possible values are:
 * DEBUG, INFO, WARNING, ERROR.
 */
#define AVRTOS_LOG_DEFAULT_LEVEL INFO

/**
 * Baud rate of asynchronous serial logger.
 */
#define AVRTOS_ASYNCHRONOUS_LOGGER_BAURATE 9600

#endif // AVRTOS_WITH_ASYNCHRONOUS_LOGGER

#endif /* AVRTOS_CONFIG_H_ */
