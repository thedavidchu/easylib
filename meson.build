project(
    'easylib',
    'c',
    version: '0.0.0',
    license: 'none',
    default_options: [
        'c_std=c99',
        # This is equivalent to "-Wall -Werror -Wextra -Wpedantic" according to
        # this: https://mesonbuild.com/Builtin-options.html#details-for-warning_level
        'warning_level=3',
        # I link the libraries statically (default is dynamic) because I want to
        # see them when I use gprof.
        'default_library=static',
        'buildtype=release',
    ],
)

# For some reason, the <math.h> library needs to be added to the compiler
# separately in Linux. Source: https://mesonbuild.com/howtox.html#add-math-library-lm-portably
cc = meson.get_compiler('c', native: true)
math_dep = cc.find_library('m', required: false)
thread_dep = dependency('threads')

subdir('src')
subdir('test')