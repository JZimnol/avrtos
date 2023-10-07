#ifndef CIRCULAR_BUFFER_ARCH_IND_H_
#define CIRCULAR_BUFFER_ARCH_IND_H_

#include <stdbool.h>
#include <stddef.h>

#include "avrtos_config.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Variable size circular buffer.
 */
struct circular_buffer {
    char *data;
    size_t head;
    size_t capacity;
    size_t occupancy;
};

/**
 * Specifies function return code.
 */
enum circular_buffer_status {
    CIRC_BUFF_OK,
    CIRC_BUFF_FULL,
    CIRC_BUFF_EMPTY,
    CIRC_BUFF_INVALID,
};

/**
 * Initializes circular buffer. Requires a pointer to non NULL circular buffer,
 * data buffer and data buffer's size.
 *
 * @param circ_buff Pointer to non NULL circular buffer.
 *
 * @param buffer    Pointer to nun NULL data buffer.
 *
 * @param size      Size of @ref buffer. Must be greater than zero less than or
 *                  equal sizeof( @ref buffer ).
 *
 * @returns CIRC_BUFF_INVALID if circ_buff or buffer are NULL or size is less or
 *          equal 0,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status circ_buff_initialize(
        struct circular_buffer *circ_buff, char *buffer, size_t size);

/**
 * Inserts one element into the buffer.
 *
 * @param circ_buff Pointer to non NULL circular buffer.
 *
 * @param value     A value we want to insert into the buffer.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff is NULL,
 *          CIRC_BUFF_FULL if there is no space in the buffer,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status
circ_buff_insert_one(struct circular_buffer *circ_buff, char value);

/**
 * Insert a various number of elements into the buffer. If there is no space for
 * all @ref count elements, does not insert any element and returns.
 *
 * @param circ_buff Pointer to non NULL circular buffer.
 *
 * @param values    Pointer to non NULL value array.
 *
 * @param count     Number of elements we want to insert into the buffer.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff or values are NULL,
 *          CIRC_BUFF_FULL if there is no space for all the elements,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status circ_buff_insert_few(
        struct circular_buffer *circ_buff, char *values, size_t count);

/**
 * Gets one value from the buffer.
 *
 * @param circ_buff Pointer to non NULL circular buffer.
 *
 * @param out_value Pointer to the non NULL variable to which the result will be
 *                  copied.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff or out_value are NULL,
 *          CIRC_BUFF_EMPTY if there is no value to retrieve,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status circ_buff_get_one(struct circular_buffer *circ_buff,
                                              char *out_value);

/**
 * Gets a various number of elements from the buffer. If there is less elements
 * in the buffer that @ref count, does not modify the @ref out_values array.
 *
 * @param circ_buff  Pointer to non NULL circular buffer.
 *
 * @param out_values Pointer to the non NULL arary to which the result will be
 *                   copied.
 *
 * @param count      Number of elements we want to retrieve from the buffer.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff or out_values are NULL,
 *          CIRC_BUFF_EMPTY if there is not enough values in the buffer to
 *          retrieve,
 *          CIRC_BUFF_OK otherwise
 */
enum circular_buffer_status circ_buff_get_few(struct circular_buffer *circ_buff,
                                              char *out_values,
                                              size_t count);

/**
 * Gets the capacity (size, number of elements it can hold) of the buffer.
 *
 * @param circ_buff    Pointer to non NULL circular buffer.
 *
 * @param out_capacity Pointer to the non NULL variable to which the capacity
 *                     will be copied.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff or out_capacity are NULL,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status
circ_buff_get_capacity(struct circular_buffer *circ_buff, size_t *out_capacity);

/**
 * Gets the occupancy (number of elements that are currently in the buffer) of
 * the buffer.
 *
 * @param circ_buff     Pointer to non NULL circular buffer.
 *
 * @param out_occupancy Pointer to the non NULL variable to which the occupancy
 *                      will be copied.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff or out_occupancy are NULL,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status
circ_buff_get_occupancy(struct circular_buffer *circ_buff,
                        size_t *out_occupancy);

/**
 * Gets the number of elements that can be inserted into the buffer at the
 * moment.
 *
 * @param circ_buff      Pointer to non NULL circular buffer.
 *
 * @param out_space_left Pointer to the non NULL variable to which the space
 *                       left will be copied.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff or out_space_left are NULL,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status
circ_buff_get_space_left(struct circular_buffer *circ_buff,
                         size_t *out_space_left);

/**
 * Checks wether the buffer is full.
 *
 * @param circ_buff Pointer to circular buffer.
 *
 * @returns true if there is no space left in the buffer or the circ_buff is
 *          NULL,
 *          false otherwise.
 */
static inline bool circ_buff_is_full(struct circular_buffer *circ_buff) {
    return circ_buff ? circ_buff->capacity == circ_buff->occupancy : true;
}

/**
 * Checks wether there is a space for at least one element.
 *
 * @param circ_buff Pointer to circular buffer.
 *
 * @returns true if there a space for at least one element in the buffer,
 *          false when there is no space in the buffer or circ_buff is NULL
 */
static inline bool
circ_buff_has_space_for_one(struct circular_buffer *circ_buff) {
    return !circ_buff_is_full(circ_buff);
}

/**
 * Checks wether the buffer is empty.
 *
 * @param circ_buff Pointer to circular buffer.
 *
 * @returns true if there are no elements in the buffer or the circ_buff is NULL
 *          false otherwise.
 */
static inline bool circ_buff_is_empty(struct circular_buffer *circ_buff) {
    return circ_buff ? circ_buff->occupancy == 0 : true;
}

/**
 * Removes all elements from the buffer. Does not clear the buffer.
 *
 * @param circ_buff Pointer to non NULL circular buffer.
 *
 * @returns CIRC_BUFF_INVALID if circ_buff is NULL,
 *          CIRC_BUFF_OK otherwise.
 */
enum circular_buffer_status circ_buff_reset(struct circular_buffer *circ_buff);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* CIRCULAR_BUFFER_ARCH_IND_H_ */
