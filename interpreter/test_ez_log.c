#include "ez_log.h"

void
print_all_levels(void)
{
    printf("--- START: %s ---\n", EZ_LOG_LEVEL_STRINGS[ez_log_level]);
    EZ_LOG_VERBOSE("verbose %d", EZ_LOG_LEVEL_VERBOSE);
    EZ_LOG_TRACE("trace %d", EZ_LOG_LEVEL_TRACE);
    EZ_LOG_DEBUG("debug %d", EZ_LOG_LEVEL_DEBUG);
    EZ_LOG_INFO("info %d", EZ_LOG_LEVEL_INFO);
    EZ_LOG_WARN("warn %d", EZ_LOG_LEVEL_WARN);
    EZ_LOG_ERROR("error %d", EZ_LOG_LEVEL_ERROR);
    EZ_LOG_FATAL("fatal %d", EZ_LOG_LEVEL_FATAL);
    printf("--- END: %s ---\n", EZ_LOG_LEVEL_STRINGS[ez_log_level]);
}

int
main(void)
{
    print_all_levels();
    ez_log_level = EZ_LOG_LEVEL_ALL;
    print_all_levels();
    ez_log_level = EZ_LOG_LEVEL_INFO;
    print_all_levels();
    ez_log_level = EZ_LOG_LEVEL_WARN;
    print_all_levels();
    ez_log_level = EZ_LOG_LEVEL_ERROR;
    print_all_levels();
    ez_log_level = EZ_LOG_LEVEL_FATAL;
    print_all_levels();
    ez_log_level = EZ_LOG_LEVEL_NONE;
    print_all_levels();
    return 0;
}
