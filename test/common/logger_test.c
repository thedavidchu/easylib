#include <stdlib.h>

#include "common/logger.h"
#include "test/test.h"

int
main(void)
{
    TEST_ASSERT_OK(
        LOGGER_VERBOSE("verbose - logger message: %d", LOGGER_LEVEL_VERBOSE));
    TEST_ASSERT_OK(
        LOGGER_TRACE("trace - logger message: %d", LOGGER_LEVEL_TRACE));
    TEST_ASSERT_OK(
        LOGGER_DEBUG("debug - logger message: %d", LOGGER_LEVEL_DEBUG));
    TEST_ASSERT_OK(LOGGER_INFO("info - logger message: %d", LOGGER_LEVEL_INFO));
    TEST_ASSERT_OK(LOGGER_WARN("warn - logger message: %d", LOGGER_LEVEL_WARN));
    TEST_ASSERT_OK(
        LOGGER_ERROR("error - logger message: %d", LOGGER_LEVEL_ERROR));
    TEST_ASSERT_OK(
        LOGGER_FATAL("fatal - logger message: %d", LOGGER_LEVEL_FATAL));
    return EXIT_SUCCESS;
}
