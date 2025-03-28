type_inc = include_directories('include')

ez_number_lib = library('ez_number_lib', 'ez_number.c',
    dependencies: [
        common_dep,
    ],
    include_directories: type_inc)

type_dep = declare_dependency(
    dependencies: [
        common_dep,
    ],
    include_directories: type_inc,
    link_with: [
        ez_number_lib,
    ],
)