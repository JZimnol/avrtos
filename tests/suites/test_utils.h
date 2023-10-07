#pragma once

#include <assert.h>

#define TEST_SUITE_FINISH_CRITICAL(Message) \
    TEST_MESSAGE(Message);                  \
    assert(0);
