#include "test_utils.h"
#include <unity.h>

#include <circular_buffer_arch_ind.h>

#define UNIT_TEST_CIRC_BUFF_SIZE 10
char test_data_buffer[UNIT_TEST_CIRC_BUFF_SIZE];
struct circular_buffer test_circ_buff;

void setUp(void) {
    /* set all global buffer values to zero */
    for (size_t i = 0; i < UNIT_TEST_CIRC_BUFF_SIZE; i++) {
        test_data_buffer[i] = '\0';
    }

    /* initialize buffer before each test, critical part*/
    enum circular_buffer_status ret =
            circ_buff_initialize(&test_circ_buff, test_data_buffer,
                                 UNIT_TEST_CIRC_BUFF_SIZE);
    if (!(ret == CIRC_BUFF_OK && test_circ_buff.data == test_data_buffer
          && test_circ_buff.occupancy == 0
          && test_circ_buff.capacity == UNIT_TEST_CIRC_BUFF_SIZE
          && test_circ_buff.head == 0)) {
        TEST_SUITE_FINISH_CRITICAL(
                "Buffer initialization failed, abort all test cases");
    }
}

void tearDown(void) {
    test_circ_buff.data = NULL;
    test_circ_buff.head = 0;
    test_circ_buff.occupancy = 0;
    test_circ_buff.occupancy = 0;
}

void TestInsertElements(void) {
    char single_to_insert[3] = {1, 3, 6};
    char few_to_insert[5] = {10, 12, 14, 16, 18};
    TEST_ASSERT_TRUE(sizeof(single_to_insert) + sizeof(few_to_insert)
                     <= UNIT_TEST_CIRC_BUFF_SIZE);

    for (size_t i = 0; i < sizeof(single_to_insert); i++) {
        TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                              circ_buff_insert_one(&test_circ_buff,
                                                   single_to_insert[i]));
    }

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_insert_few(&test_circ_buff, few_to_insert,
                                               sizeof(few_to_insert)));
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_FULL,
                          circ_buff_insert_few(&test_circ_buff, few_to_insert,
                                               sizeof(few_to_insert)));

    for (size_t i = 0; i < sizeof(single_to_insert); i++) {
        TEST_ASSERT_EQUAL_CHAR(single_to_insert[i], test_circ_buff.data[i]);
    }
    for (size_t i = 0; i < sizeof(few_to_insert); i++) {
        TEST_ASSERT_EQUAL_CHAR(
                few_to_insert[i],
                test_circ_buff.data[i + sizeof(single_to_insert)]);
    }
}

void TestGetElements(void) {
    char few_to_insert[UNIT_TEST_CIRC_BUFF_SIZE - 2] = {1,  3,  6,  10,
                                                        12, 14, 16, 18};
    TEST_ASSERT_TRUE(sizeof(few_to_insert) <= UNIT_TEST_CIRC_BUFF_SIZE);
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_insert_few(&test_circ_buff, few_to_insert,
                                               sizeof(few_to_insert)));

    char out_one;
    for (size_t i = 0; i < 3; i++) {
        TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                              circ_buff_get_one(&test_circ_buff, &out_one));
        TEST_ASSERT_EQUAL_CHAR(few_to_insert[i], out_one);
    }

    char out_few[4];
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_few(&test_circ_buff, out_few,
                                            sizeof(out_few)));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(&few_to_insert[3], out_few, sizeof(out_few));

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_EMPTY,
                          circ_buff_get_few(&test_circ_buff, out_few,
                                            sizeof(out_few)));
}

void TestGetCapacity(void) {
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_INVALID,
                          circ_buff_get_capacity(&test_circ_buff, NULL));

    size_t capacity;
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_capacity(&test_circ_buff, &capacity));
    TEST_ASSERT_EQUAL_size_t(UNIT_TEST_CIRC_BUFF_SIZE, capacity);
}

void TestGetOccupancy(void) {
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_INVALID,
                          circ_buff_get_occupancy(&test_circ_buff, NULL));
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_INVALID,
                          circ_buff_get_space_left(&test_circ_buff, NULL));

    size_t occupancy;
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(0, occupancy);

    size_t space_left;
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK, circ_buff_get_space_left(
                                                &test_circ_buff, &space_left));
    TEST_ASSERT_EQUAL_size_t(UNIT_TEST_CIRC_BUFF_SIZE, space_left);

    char few_to_insert[UNIT_TEST_CIRC_BUFF_SIZE - 2] = {1,  3,  6,  10,
                                                        12, 14, 16, 18};
    TEST_ASSERT_TRUE(sizeof(few_to_insert) <= UNIT_TEST_CIRC_BUFF_SIZE);
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_insert_few(&test_circ_buff, few_to_insert,
                                               sizeof(few_to_insert)));

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK, circ_buff_get_space_left(
                                                &test_circ_buff, &space_left));
    TEST_ASSERT_EQUAL_size_t(2, space_left);

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(sizeof(few_to_insert), occupancy);

    for (size_t i = 0; i < 2; i++) {
        TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                              circ_buff_insert_one(&test_circ_buff, 100));
    }
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(sizeof(few_to_insert) + 2, occupancy);

    for (size_t i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(CIRC_BUFF_FULL,
                              circ_buff_insert_one(&test_circ_buff, 100));
    }
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(sizeof(few_to_insert) + 2, occupancy);

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK, circ_buff_get_space_left(
                                                &test_circ_buff, &space_left));
    TEST_ASSERT_EQUAL_size_t(0, space_left);
}

void TestHasSpaceIsFullOrEmpty(void) {
    TEST_ASSERT_TRUE(circ_buff_is_empty(&test_circ_buff));
    TEST_ASSERT_FALSE(circ_buff_is_full(&test_circ_buff));

    char out_one;
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_EMPTY,
                          circ_buff_get_one(&test_circ_buff, &out_one));
    TEST_ASSERT_TRUE(circ_buff_is_empty(&test_circ_buff));

    char few_to_insert[UNIT_TEST_CIRC_BUFF_SIZE - 2] = {1,  3,  6,  10,
                                                        12, 14, 16, 18};
    TEST_ASSERT_TRUE(sizeof(few_to_insert) <= UNIT_TEST_CIRC_BUFF_SIZE);
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_insert_few(&test_circ_buff, few_to_insert,
                                               sizeof(few_to_insert)));
    TEST_ASSERT_FALSE(circ_buff_is_empty(&test_circ_buff));
    TEST_ASSERT_TRUE(circ_buff_has_space_for_one(&test_circ_buff));
    TEST_ASSERT_FALSE(circ_buff_is_full(&test_circ_buff));

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_insert_one(&test_circ_buff, 100));
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_insert_one(&test_circ_buff, 200));

    size_t occupancy;
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(UNIT_TEST_CIRC_BUFF_SIZE, occupancy);
    TEST_ASSERT_FALSE(circ_buff_is_empty(&test_circ_buff));
    TEST_ASSERT_FALSE(circ_buff_has_space_for_one(&test_circ_buff));
    TEST_ASSERT_TRUE(circ_buff_is_full(&test_circ_buff));

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_FULL,
                          circ_buff_insert_one(&test_circ_buff, 100));
    TEST_ASSERT_FALSE(circ_buff_is_empty(&test_circ_buff));
    TEST_ASSERT_FALSE(circ_buff_has_space_for_one(&test_circ_buff));
    TEST_ASSERT_TRUE(circ_buff_is_full(&test_circ_buff));
}

void TestReset(void) {
    size_t capacity;
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_capacity(&test_circ_buff, &capacity));
    TEST_ASSERT_EQUAL_size_t(UNIT_TEST_CIRC_BUFF_SIZE, capacity);

    size_t occupancy;
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(0, occupancy);
    TEST_ASSERT_TRUE(circ_buff_is_empty(&test_circ_buff));

    char few_to_insert[UNIT_TEST_CIRC_BUFF_SIZE - 2] = {1,  3,  6,  10,
                                                        12, 14, 16, 18};
    TEST_ASSERT_TRUE(sizeof(few_to_insert) <= UNIT_TEST_CIRC_BUFF_SIZE);
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_insert_few(&test_circ_buff, few_to_insert,
                                               sizeof(few_to_insert)));

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(sizeof(few_to_insert), occupancy);
    TEST_ASSERT_FALSE(circ_buff_is_empty(&test_circ_buff));

    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK, circ_buff_reset(&test_circ_buff));
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_capacity(&test_circ_buff, &capacity));
    TEST_ASSERT_EQUAL_size_t(UNIT_TEST_CIRC_BUFF_SIZE, capacity);
    TEST_ASSERT_EQUAL_INT(CIRC_BUFF_OK,
                          circ_buff_get_occupancy(&test_circ_buff, &occupancy));
    TEST_ASSERT_EQUAL_size_t(0, occupancy);
    TEST_ASSERT_TRUE(circ_buff_is_empty(&test_circ_buff));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(TestInsertElements);
    RUN_TEST(TestGetElements);
    RUN_TEST(TestGetCapacity);
    RUN_TEST(TestGetOccupancy);
    RUN_TEST(TestHasSpaceIsFullOrEmpty);
    RUN_TEST(TestReset);

    return UNITY_END();
}
