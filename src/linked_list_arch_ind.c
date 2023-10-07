#include "linked_list_arch_ind.h"

enum linked_list_status linked_list_initailize_head(linked_list_t **head,
                                                    linked_list_t *elem) {
    if (!(head && *head == NULL && elem)) {
        return LINKED_LIST_INVALID;
    }
    *head = elem;

    return LINKED_LIST_OK;
}

enum linked_list_status linked_list_prepend(linked_list_t **head,
                                            linked_list_t *to_insert) {
    if (!(head && *head && to_insert)) {
        return LINKED_LIST_INVALID;
    }
    to_insert->next = *head;
    *head = to_insert;

    return LINKED_LIST_OK;
}

enum linked_list_status linked_list_append(linked_list_t *head,
                                           linked_list_t *to_insert) {
    if (!(head && to_insert)) {
        return LINKED_LIST_INVALID;
    }
    linked_list_t *last = linked_list_last(head);
    last->next = to_insert;

    return LINKED_LIST_OK;
}
enum linked_list_status linked_list_insert_after(linked_list_t *element,
                                                 linked_list_t *to_insert) {
    if (!(element && to_insert)) {
        return LINKED_LIST_INVALID;
    }
    to_insert->next = element->next;
    element->next = to_insert;

    return LINKED_LIST_OK;
}

enum linked_list_status linked_list_remove(linked_list_t **head,
                                           linked_list_t *to_remove) {
    if (!(head && *head && to_remove)) {
        return LINKED_LIST_INVALID;
    }

    if (*head == to_remove) {
        *head = (*head)->next;
        return LINKED_LIST_OK;
    }

    linked_list_t *temp = *head;
    while (temp->next != to_remove) {
        if (!temp->next) {
            return LINKED_LIST_INVALID;
        }
        temp = temp->next;
    }

    temp->next = temp->next->next;

    return LINKED_LIST_OK;
}

size_t linked_list_count(linked_list_t *head) {
    if (!head) {
        return 0;
    }

    size_t count = 0;
    while (head) {
        head = head->next;
        count++;
    }

    return count;
}

linked_list_t *linked_list_next(linked_list_t *elem) {
    return elem ? elem->next : NULL;
}

linked_list_t *linked_list_previous(linked_list_t *head, linked_list_t *elem) {
    if (!(head && elem) || head == elem) {
        return NULL;
    }

    while (head->next != elem) {
        if (!head->next) {
            return NULL;
        }
        head = head->next;
    }

    return head;
}

linked_list_t *linked_list_last(linked_list_t *head) {
    if (!head) {
        return NULL;
    }

    while (head->next) {
        head = head->next;
    }

    return head;
}
