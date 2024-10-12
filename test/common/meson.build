logger_test_exe = executable(
    'logger_test_exe',
    'logger_test.c',
    dependencies: [
        common_dep,
        test_dep,
    ],
)

memory_test_exe = executable(
    'memory_test_exe',
    'memory_test.c',
    dependencies: [
        common_dep,
        test_dep,
    ],
)

test('logger_test', logger_test_exe)
test('memory_test', memory_test_exe)