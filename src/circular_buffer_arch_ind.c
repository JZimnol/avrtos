#include "circular_buffer_arch_ind.h"

enum circular_buffer_status circ_buff_initialize(
        struct circular_buffer *circ_buff, char *buffer, size_t size) {
    if (!(circ_buff && buffer && size > 0)) {
        return CIRC_BUFF_INVALID;
    }
    circ_buff->data = buffer;
    circ_buff->head = 0;
    circ_buff->capacity = size;
    circ_buff->occupancy = 0;

    return CIRC_BUFF_OK;
}

enum circular_buffer_status
circ_buff_insert_one(struct circular_buffer *circ_buff, char value) {
    if (!circ_buff) {
        return CIRC_BUFF_INVALID;
    }
    if (!circ_buff_has_space_for_one(circ_buff)) {
        return CIRC_BUFF_FULL;
    }
    circ_buff->data[(circ_buff->head + circ_buff->occupancy)
                    % circ_buff->capacity] = value;
    circ_buff->occupancy++;

    return CIRC_BUFF_OK;
}

enum circular_buffer_status circ_buff_insert_few(
        struct circular_buffer *circ_buff, char *values, size_t count) {
    if (!(circ_buff && values)) {
        return CIRC_BUFF_INVALID;
    }
    size_t space_left;
    circ_buff_get_space_left(circ_buff, &space_left);
    if (space_left < count) {
        return CIRC_BUFF_FULL;
    }
    for (size_t i = 0; i < count; i++) {
        circ_buff_insert_one(circ_buff, values[i]);
    }

    return CIRC_BUFF_OK;
}

enum circular_buffer_status circ_buff_get_one(struct circular_buffer *circ_buff,
                                              char *out_value) {
    if (!(circ_buff && out_value)) {
        return CIRC_BUFF_INVALID;
    }
    if (circ_buff_is_empty(circ_buff)) {
        return CIRC_BUFF_EMPTY;
    }
    *out_value = circ_buff->data[circ_buff->head++];
    circ_buff->head %= circ_buff->capacity;
    circ_buff->occupancy--;

    return CIRC_BUFF_OK;
}

enum circular_buffer_status circ_buff_get_few(struct circular_buffer *circ_buff,
                                              char *out_values,
                                              size_t count) {
    if (!(circ_buff && out_values)) {
        return CIRC_BUFF_INVALID;
    }
    size_t occupancy;
    circ_buff_get_occupancy(circ_buff, &occupancy);
    if (occupancy < count) {
        return CIRC_BUFF_EMPTY;
    }
    for (size_t i = 0; i < count; i++) {
        circ_buff_get_one(circ_buff, &out_values[i]);
    }

    return CIRC_BUFF_OK;
}

enum circular_buffer_status
circ_buff_get_capacity(struct circular_buffer *circ_buff,
                       size_t *out_capacity) {
    if (!(circ_buff && out_capacity)) {
        return CIRC_BUFF_INVALID;
    }

    *out_capacity = circ_buff->capacity;

    return CIRC_BUFF_OK;
}

enum circular_buffer_status
circ_buff_get_occupancy(struct circular_buffer *circ_buff,
                        size_t *out_occupancy) {
    if (!(circ_buff && out_occupancy)) {
        return CIRC_BUFF_INVALID;
    }

    *out_occupancy = circ_buff->occupancy;

    return CIRC_BUFF_OK;
}

enum circular_buffer_status
circ_buff_get_space_left(struct circular_buffer *circ_buff,
                         size_t *out_space_left) {
    if (!(circ_buff && out_space_left)) {
        return CIRC_BUFF_INVALID;
    }

    *out_space_left = circ_buff->capacity - circ_buff->occupancy;

    return CIRC_BUFF_OK;
}

enum circular_buffer_status circ_buff_reset(struct circular_buffer *circ_buff) {
    if (!circ_buff) {
        return CIRC_BUFF_INVALID;
    }
    circ_buff->head = 0;
    circ_buff->occupancy = 0;

    return CIRC_BUFF_OK;
}
