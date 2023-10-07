#ifndef LINKED_LIST_ARCH_IND_H_
#define LINKED_LIST_ARCH_IND_H_

#include <stdbool.h>
#include <stddef.h>

#include "avrtos_config.h"
#include "avrtos_core.h"
#include "avrtos_utils.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Making such typedef makes this API semi universal and possible to test. The
 * only condition is that target struct should contain a field @ref
 * linked_list_t::next which is a pointer to the next linked_list_t element.
 */
typedef struct avrtos_task linked_list_t;

enum linked_list_status {
    LINKED_LIST_OK = 0,
    LINKED_LIST_INVALID,
};

/**
 * Initialize linked-list HEAD pointer. If given HEAD is not NULL, the function
 * will return LINKED_LIST_INVALID to avoid HEAD reinitialization.
 *
 * @param head Pointer to a @ref linked_list_t *HEAD variable. Should not be
 *             NULL. The HEAD pointer should be NULL.
 *
 * @param elem Pointer to element the HEAD should point to.
 *
 * @returns LINKED_LIST_INVALID if @p head is NULL or *head is not NULL or
 *          @p elem is NULL,
 *          LINKED_LIST_OK otherwise.
 */
enum linked_list_status linked_list_initailize_head(linked_list_t **head,
                                                    linked_list_t *elem);

/**
 * Prepends the linked-list with @p to_insert element. As a result, @p to_insert
 * will be the first element and new HEAD of the list.
 *
 * @param head      Pointer to a @ref linked_list_t *HEAD variable. Should not
 *                  be NULL.
 *
 * @param to_insert Pointer to element which will be the new first element of
 *                  the list.
 *
 * @returns LINKED_LIST_INVALID if @p head is NULL or *head is NULL or
 *          @p to_insert is NULL,
 *          LINKED_LIST_OK otherwise.
 */
enum linked_list_status linked_list_prepend(linked_list_t **head,
                                            linked_list_t *to_insert);

/**
 * Adds new element at the end of a linked-list.
 *
 * @param head      Pointer to the HEAD of the list. May also be a pointer to
 *                  "random" element of the same list.
 *
 * @param to_insert Pointer to element which will be the new last element of the
 *                  list.
 *
 * @returns LINKED_LIST_INVALID if @p head is NULL or @p to_insert is NULL,
 *          LINKED_LIST_OK otherwise.
 */
enum linked_list_status linked_list_append(linked_list_t *head,
                                           linked_list_t *to_insert);

/**
 * Insert a new element to the list after @p element element.
 *
 * @param element   Pointer to a linked-list element.
 *
 * @param to_insert Pointer to an element that will be inserted into the list.
 *
 * @returns LINKED_LIST_INVALID if @p element is NULL or @p to_insert is NULL,
 *          LINKED_LIST_OK otherwise.
 */
enum linked_list_status linked_list_insert_after(linked_list_t *element,
                                                 linked_list_t *to_insert);

/**
 * Removes @p to_remove element from the linked-list (if is in the list). If the
 * element which should be removed is the first element, the HEAD pointer will
 * be updated accordingly.
 *
 * @param head      Pointer to a @ref linked_list_t *HEAD variable. Should not
 *                  be NULL.
 *
 * @param to_remove Pointer to element which should be removed from the list.
 *
 * @returns LINKED_LIST_INVALID if @p head is NULL or *head is NULL or
 *          @p to_remove is NULL or @p to_remove is not in the list,
 *          LINKED_LIST_OK otherwise.
 */
enum linked_list_status linked_list_remove(linked_list_t **head,
                                           linked_list_t *to_remove);

/**
 * Returns the number of elements in the list (starting from @p head element).
 *
 * @param head Pointer to linked-list element from which the counting will
 *             begun.
 *
 * @returns Number of linked-list elements.
 */
size_t linked_list_count(linked_list_t *head);

/**
 * Returns the linked-list item that comes after @p elem element.
 *
 * @param elem Pointer to element from which we want to get the next element.
 *
 * @returns NULL if @p elem is NULL or @p elem is the last element in the list,
 *          pointer to the linked-list element otherwise.
 */
linked_list_t *linked_list_next(linked_list_t *elem);

/**
 * Returns the linked-list item that comes before @p elem element.
 *
 * @param head Pointer to the HEAD of the list.
 *
 * @param elem Pointer to the element that should be equal to
 *             @ref linked_list_t::next pointer of the element we're looking
 *             for.
 *
 * @returns NULL if @p head is NULL or @p elem is NULL or @p elem is the first
 *          element in the list or @p elem is not in the list,
 *          pointer to the linked-list element otherwise.
 */
linked_list_t *linked_list_previous(linked_list_t *head, linked_list_t *elem);

/**
 * Returns the last element of the linked-list.
 *
 * @param head Pointer to the HEAD of the list. May also be a pointer to
 *             "random" element of the same list.
 *
 * @returns NULL if @p head is NULL,
 *          pointer to the linked-list element otherwise.
 */
linked_list_t *linked_list_last(linked_list_t *head);

/**
 * Iterate through linked-list starting from @p Head.
 *
 * For a N-element linked-list, it will iterate from @p Head element to Nth
 * element.
 */
#define LINKED_LIST_FOREACH_BASIC(Head, Iterator) \
    for (Iterator = Head; Iterator != NULL;       \
         Iterator = linked_list_next(Iterator))

/**
 * Iterate through linked-list starting from @p Specified. Does not check if the
 * @p Specified element is in the linked-list that starts with @p Head.
 *
 * For a N-element linked-list, it will iterate from @p Specified element to Nth
 * element, and then from @p Head to @p Specified - 1 element.
 */
#define LINKED_LIST_FOREACH_FROM_SPECIFIED(Head, Specified, Iterator)  \
    Iterator = Specified;                                              \
    for (bool _first = true; Iterator != Specified || _first != false; \
         Iterator = Iterator->next ? Iterator->next : Head, _first = false)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* LINKED_LIST_ARCH_IND_H_ */
