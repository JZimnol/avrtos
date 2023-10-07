#include "test_utils.h"
#include <string.h>
#include <unity.h>

#include <logger_arch_ind.h>

char unit_test_global_buffer[AVRTOS_UNIT_TEST_SINGLE_LOG_MAX_SIZE];

#define GLOBAL_BUFFER_RESET() unit_test_global_buffer[0] = '\0'
#define UNIT_TEST_LOG(...) \
    GLOBAL_BUFFER_RESET(); \
    avrtos_log(__VA_ARGS__)
#define TEST_ASSERT_LOG_EQUAL_TO(...) \
    TEST_ASSERT_EQUAL_STRING(__VA_ARGS__, unit_test_global_buffer);

void avrtos_handle_log_buffer_impl(char *buffer, size_t buf_len) {
    (void) buf_len;
    strcpy(unit_test_global_buffer, buffer);
}

void setUp(void) {
    GLOBAL_BUFFER_RESET();
}

void tearDown(void) {}

void TestCheckLogLevels(void) {
#undef AVRTOS_LOG_DEFAULT_LEVEL
#define AVRTOS_LOG_DEFAULT_LEVEL NONEXISTING
    UNIT_TEST_LOG(test, ERROR, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");
    UNIT_TEST_LOG(test, WARNING, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");
    UNIT_TEST_LOG(test, INFO, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");
    UNIT_TEST_LOG(test, DEBUG, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");

#undef AVRTOS_LOG_DEFAULT_LEVEL
#define AVRTOS_LOG_DEFAULT_LEVEL ERROR
    UNIT_TEST_LOG(test, ERROR, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("ERROR [test] test message\n");
    UNIT_TEST_LOG(test, WARNING, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");
    UNIT_TEST_LOG(test, INFO, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");
    UNIT_TEST_LOG(test, DEBUG, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");

#undef AVRTOS_LOG_DEFAULT_LEVEL
#define AVRTOS_LOG_DEFAULT_LEVEL WARNING
    UNIT_TEST_LOG(test, ERROR, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("ERROR [test] test message\n");
    UNIT_TEST_LOG(test, WARNING, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("WARNING [test] test message\n");
    UNIT_TEST_LOG(test, INFO, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");
    UNIT_TEST_LOG(test, DEBUG, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");

#undef AVRTOS_LOG_DEFAULT_LEVEL
#define AVRTOS_LOG_DEFAULT_LEVEL INFO
    UNIT_TEST_LOG(test, ERROR, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("ERROR [test] test message\n");
    UNIT_TEST_LOG(test, WARNING, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("WARNING [test] test message\n");
    UNIT_TEST_LOG(test, INFO, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] test message\n");
    UNIT_TEST_LOG(test, DEBUG, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("");

#undef AVRTOS_LOG_DEFAULT_LEVEL
#define AVRTOS_LOG_DEFAULT_LEVEL DEBUG
    UNIT_TEST_LOG(test, ERROR, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("ERROR [test] test message\n");
    UNIT_TEST_LOG(test, WARNING, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("WARNING [test] test message\n");
    UNIT_TEST_LOG(test, INFO, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] test message\n");
    UNIT_TEST_LOG(test, DEBUG, "test message");
    TEST_ASSERT_LOG_EQUAL_TO("DEBUG [test] test message\n");
}

void TestCheckNullTermination(void) {
#undef AVRTOS_LOG_DEFAULT_LEVEL
#define AVRTOS_LOG_DEFAULT_LEVEL INFO

#define TEST_STRING_1 "Less than max size"
#define TEST_STRING_2 "Equal to max size ABC"
#define TEST_STRING_PADDING "PADDING"

    TEST_ASSERT_TRUE(strlen("INFO [test] ") + strlen(TEST_STRING_1)
                             + strlen("\n") + 1
                     < AVRTOS_UNIT_TEST_SINGLE_LOG_MAX_SIZE);
    TEST_ASSERT_TRUE(strlen("INFO [test] ") + strlen(TEST_STRING_2)
                             + strlen("\n") + 1
                     == AVRTOS_UNIT_TEST_SINGLE_LOG_MAX_SIZE);
    TEST_ASSERT_TRUE(strlen("INFO [test] ") + strlen(TEST_STRING_2)
                             + strlen("\n") + strlen(TEST_STRING_PADDING) + 1
                     > AVRTOS_UNIT_TEST_SINGLE_LOG_MAX_SIZE);

    UNIT_TEST_LOG(test, INFO, TEST_STRING_1);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] " TEST_STRING_1 "\n");

    UNIT_TEST_LOG(test, INFO, TEST_STRING_2);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] " TEST_STRING_2 "\n");

    UNIT_TEST_LOG(test, INFO, TEST_STRING_2 TEST_STRING_PADDING);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] " TEST_STRING_2 "\n");
}

void TestBasicSnprintfFormatting(void) {
#undef AVRTOS_LOG_DEFAULT_LEVEL
#define AVRTOS_LOG_DEFAULT_LEVEL INFO

    int int_value_15 = 15;
    UNIT_TEST_LOG(test, INFO, "int is %d", int_value_15);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] int is 15\n");

    char char_value_s = 's';
    UNIT_TEST_LOG(test, INFO, "char is %c", char_value_s);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] char is s\n");

    float float_value_pi = 3.14159f;
    UNIT_TEST_LOG(test, INFO, "float is %.2f", float_value_pi);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] float is 3.14\n");
    UNIT_TEST_LOG(test, INFO, "float is %.3f", float_value_pi);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] float is 3.142\n");
    UNIT_TEST_LOG(test, INFO, "float is %.4f", float_value_pi);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] float is 3.1416\n");

    char *string_value_unit_test = "unit test";
    UNIT_TEST_LOG(test, INFO, "string is %s", string_value_unit_test);
    TEST_ASSERT_LOG_EQUAL_TO("INFO [test] string is unit test\n");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(TestCheckLogLevels);
    RUN_TEST(TestCheckNullTermination);
    RUN_TEST(TestBasicSnprintfFormatting);

    return UNITY_END();
}
