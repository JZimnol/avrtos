#include "test_utils.h"
#include <unity.h>

#include <linked_list_arch_ind.h>

#define TEST_LINKED_LIST_SIZE 5
linked_list_t *head;
linked_list_t list_elements[TEST_LINKED_LIST_SIZE];

void setUp(void) {
    for (size_t i = 0; i < TEST_LINKED_LIST_SIZE; i++) {
        list_elements[i].next = NULL;
    }
    head = NULL;
    enum linked_list_status ret =
            linked_list_initailize_head(&head, &list_elements[0]);
    if (!(ret == LINKED_LIST_OK && head && head == &list_elements[0])) {
        TEST_SUITE_FINISH_CRITICAL(
                "Head initialization failed, abort all test cases");
    }
}

void tearDown(void) {
    head = NULL;
}

void TestInitializeHead(void) {
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_initailize_head(&head, NULL));
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_initailize_head(&head,
                                                      &list_elements[1]));
}

void TestAppend(void) {
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID, linked_list_append(head, NULL));
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_append(NULL, &list_elements[0]));

    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
    }

    for (size_t i = 0; i < TEST_LINKED_LIST_SIZE - 1; i++) {
        TEST_ASSERT_EQUAL_PTR(&list_elements[i + 1], list_elements[i].next);
    }
    TEST_ASSERT_EQUAL_PTR(NULL, list_elements[TEST_LINKED_LIST_SIZE - 1].next);
}

void TestPrepend(void) {
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_prepend(&head, NULL));
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_prepend(NULL, &list_elements[0]));
    linked_list_t *fake = NULL;
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_prepend(&fake, &list_elements[0]));

    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_prepend(&head, &list_elements[i]));
        TEST_ASSERT_EQUAL_PTR(&list_elements[i], head);
    }
    for (size_t i = 0; i < TEST_LINKED_LIST_SIZE - 1; i++) {
        TEST_ASSERT_EQUAL_PTR(
                &list_elements[TEST_LINKED_LIST_SIZE - 1 - i - 1],
                list_elements[TEST_LINKED_LIST_SIZE - 1 - i].next);
    }
    TEST_ASSERT_EQUAL_PTR(list_elements[0].next, NULL);
}

void TestInsertNextTo(void) {
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_insert_after(head, NULL));
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_insert_after(NULL, &list_elements[0]));
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE - 1; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
    }
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                          linked_list_insert_after(
                                  &list_elements[2],
                                  &list_elements[TEST_LINKED_LIST_SIZE - 1]));
    /* check the list manually */
    TEST_ASSERT_EQUAL_PTR(&list_elements[1], list_elements[0].next);
    TEST_ASSERT_EQUAL_PTR(&list_elements[2], list_elements[1].next);
    TEST_ASSERT_EQUAL_PTR(&list_elements[TEST_LINKED_LIST_SIZE - 1],
                          list_elements[2].next);
    TEST_ASSERT_EQUAL_PTR(&list_elements[3],
                          list_elements[TEST_LINKED_LIST_SIZE - 1].next);
    TEST_ASSERT_EQUAL_PTR(NULL, list_elements[3].next);
}

void TestRemove(void) {
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID, linked_list_remove(&head, NULL));
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_remove(NULL, &list_elements[0]));
    linked_list_t *fake = NULL;
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_remove(&fake, &list_elements[0]));
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
    }
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                          linked_list_remove(&head, &list_elements[2]));
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_INVALID,
                          linked_list_remove(&head, &list_elements[2]));
    /* check the list manually */
    TEST_ASSERT_EQUAL_PTR(&list_elements[1], list_elements[0].next);
    TEST_ASSERT_EQUAL_PTR(&list_elements[3], list_elements[1].next);
    TEST_ASSERT_EQUAL_PTR(&list_elements[4], list_elements[3].next);
}

void TestCount(void) {
    TEST_ASSERT_EQUAL_size_t(1, linked_list_count(head));
    TEST_ASSERT_EQUAL_size_t(0, linked_list_count(NULL));
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
        TEST_ASSERT_EQUAL_size_t(i + 1, linked_list_count(head));
    }
    TEST_ASSERT_EQUAL_size_t(TEST_LINKED_LIST_SIZE, linked_list_count(head));
    TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                          linked_list_remove(&head, &list_elements[2]));
    TEST_ASSERT_EQUAL_size_t(TEST_LINKED_LIST_SIZE - 1,
                             linked_list_count(head));
}

void TestReturnNext(void) {
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_next(head));
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_next(NULL));
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
    }
    for (size_t i = 0; i < TEST_LINKED_LIST_SIZE - 1; i++) {
        TEST_ASSERT_EQUAL_PTR(list_elements[i].next,
                              linked_list_next(&list_elements[i]));
    }
    TEST_ASSERT_EQUAL_PTR(list_elements[TEST_LINKED_LIST_SIZE - 1].next,
                          linked_list_next(
                                  &list_elements[TEST_LINKED_LIST_SIZE - 1]));
}

void TestReturnPrevious(void) {
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_previous(head, NULL));
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_previous(NULL, head));
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_previous(head, head));
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
    }
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_PTR(&list_elements[i - 1],
                              linked_list_previous(head, &list_elements[i]));
    }
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_previous(head, &list_elements[0]));

    linked_list_t fake;
    linked_list_t *prev_head = head;
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_previous(head, &fake));
    TEST_ASSERT_EQUAL_PTR(prev_head, head);
}

void TestReturnLast(void) {
    TEST_ASSERT_EQUAL_PTR(head, linked_list_last(head));
    TEST_ASSERT_EQUAL_PTR(NULL, linked_list_last(NULL));
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
        TEST_ASSERT_EQUAL_PTR(&list_elements[i], linked_list_last(head));
    }
}

void TestForeachBasic(void) {
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
    }
    linked_list_t *iter;
    int i = 0;
    LINKED_LIST_FOREACH_BASIC(head, iter) {
        TEST_ASSERT_EQUAL_PTR(&list_elements[i], iter);
        i++;
    }
}

void TestForeachFromSpecified(void) {
    for (size_t i = 1; i < TEST_LINKED_LIST_SIZE; i++) {
        TEST_ASSERT_EQUAL_INT(LINKED_LIST_OK,
                              linked_list_append(head, &list_elements[i]));
    }

    linked_list_t *iter = NULL;
    size_t i;
    linked_list_t *middle;

    for (i = 0; i < TEST_LINKED_LIST_SIZE; i++) {
        middle = &list_elements[i];
        LINKED_LIST_FOREACH_FROM_SPECIFIED(head, middle, iter) {
            TEST_ASSERT_EQUAL_PTR(&list_elements[i], iter);
            i = ++i % TEST_LINKED_LIST_SIZE;
        }
    }
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(TestInitializeHead);
    RUN_TEST(TestAppend);
    RUN_TEST(TestPrepend);
    RUN_TEST(TestInsertNextTo);
    RUN_TEST(TestRemove);
    RUN_TEST(TestCount);
    RUN_TEST(TestReturnNext);
    RUN_TEST(TestReturnPrevious);
    RUN_TEST(TestReturnLast);
    RUN_TEST(TestForeachBasic);
    RUN_TEST(TestForeachFromSpecified);

    return UNITY_END();
}
