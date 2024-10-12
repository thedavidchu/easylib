[![Lint](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml/badge.svg)](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml)
[![Test](https://github.com/thedavidchu/easylib/workflows/Test/badge.svg)](https://github.com/thedavidchu/easylib/actions)

# easylib
An easily understood generic library for C.

## Notable Features

Objects are passed by value exclusively. This means that every "modification" is actually a new object.
This simplifies ownership and resource management.
Recall: my primary goal is eas of implementation and understanding.

## Notes

I use the Meson build system for ease of development. Ideally, I would
also test with a Makefile, because this should be more widely available.
