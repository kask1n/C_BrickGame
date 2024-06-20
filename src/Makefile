#  src/Makefile
#
# REQUIREMENTS
#
#   * gcc
#   * clang-tidy
#   * clang-format
#   * cppcheck
#   * compiledb
#
# HOW TO USE
#
# $ make all   # builds all lib
# $ make test  # builds and runs all unittests
# $ make tests # builds all unittests
#
# $ make lint  # runs linters on all sources: clang-tidy cppcheck clang-format (in check mode)
# $ make fmt   # (or make format) formats all sources
#
# $ make clean # removes all binaries, compile_commands.json
#
# ADDITIONAL BUILD OPTIONS
#
# DEBUG_MODE
#
# If you what to build something with debug info enabled, add BUILD_CONFIG=DEBUG environment variable before run make.
#
# Example:
#
# $ BUILD_CONFIG=DEBUG make all   # builds lib with debug info enabled
# $ BUILD_CONFIG=DEBUG make test  # builds tests and run them with debug info enabled
#
# SANITIZERS
#
# If you what to build with sanitizers, add USE_SANITIZE=<name-of-sanitizer> environment variable before run make.
#
# Note: Currently only address sanitizer is available.
#
# EXAMPLE:
#
# $ USE_SANITIZE=ADDRESS make all   # builds lib with address sanitizer
# $ USE_SANITIZE=ADDRESS make test  # builds tests and run them with address sanitizer

PHONY := \
	default all build \
	format fmt lint \
	test tests \
	gcov_report \
	clean \
	install uninstall dist \
	dvi

ALL   :=
CLEAN :=

RM := rm -rf

THIS_MAKE_FILE     := $(abspath $(lastword $(MAKEFILE_LIST)))
THIS_MAKE_FILE_DIR := $(realpath $(patsubst %/,%,$(dir $(THIS_MAKE_FILE))))

SRCROOT  := $(THIS_MAKE_FILE_DIR)

default: all

# ============== [ COMPILATION ] ==============

CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -std=c11 -I $(SRCROOT) -I $(SRCROOT)/s21

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Windows_NT)
	$(error "We at S21 don't use Windows...")
else
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -D OS_OSX # OSX
		LDFLAGS :=
	else
		CFLAGS += -D OS_LINUX # LINUX
		LDFLAGS := -lm
	endif
endif

AR      := ar
ARFLAGS := rcs

ifeq ($(BUILD_CONFIG), DEBUG)
    CFLAGS += -O0 -g -D CONFIG_DEBUG
else
    CFLAGS += -Werror -O2 -D CONFIG_RELEASE
endif

ifeq ($(USE_SANITIZE), ADDRESS)
    CFLAGS += -fsanitize=address
endif

ifeq ($(USE_SANITIZE), LEAK)
    CFLAGS += -fsanitize=leak
endif

ifeq ($(WITH_COVERAGE), yes)
    # CFLAGS += --coverage
    CFLAGS += -fprofile-arcs
    CFLAGS += -ftest-coverage
endif

include $(SRCROOT)/make/tools.mk



# ================== [ S21_TETRIS ] ===================

TETRIS_DIR   := $(SRCROOT)
TETRIS_BIN   := tetris_game.a
BUILD_DIR    := build
DOCS_DIR     := docs
DIST_DIR     := dist


TETRIS_SRCS  := \
	$(TETRIS_DIR)/brick_game/tetris/tetris_logic.c \
	$(TETRIS_DIR)/gui/cli/tetris_cli.c \
	$(TETRIS_DIR)/tetris_main.c

TETRIS_OBJS  := $(patsubst $(TETRIS_DIR)/%.c, $(TETRIS_DIR)/%.o, $(TETRIS_SRCS))

$(TETRIS_BIN): $(TETRIS_OBJS)
	$(AR) $(ARFLAGS) $(LDFLAGS) $@ $^

$(TETRIS_DIR)/%.o: $(TETRIS_DIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

ALL     += $(TETRIS_BIN)
CLEAN   += $(TETRIS_OBJS) $(TETRIS_BIN)

# ================== [ UNIT TESTING ] ===================

TEST_DIR  := $(TETRIS_DIR)/tests

TEST_SRCS := \
    $(TEST_DIR)/tetris_test.c

TEST_BINS := $(patsubst $(TEST_DIR)/%.c, $(TEST_DIR)/%.bin, $(TEST_SRCS))

CLEAN += $(TEST_BINS)

TEST_CFLAGS :=

# NOTE: Unused
ifeq ($(TEST_MODE), NOFORK)
    TEST_CFLAGS += -D CK_NOFORK
endif

ifeq ($(UNAME_S), Darwin)
	TEST_LDFLAGS := $(shell pkg-config --static --cflags --libs check ncurses)
	TEST_LEAKS := $(leaks -atExit -- ./build/$(TETRIS_BIN) | grep LEAK:)
else
	TEST_LDFLAGS := $(shell pkg-config --static --cflags --libs check ncursesw)
	TEST_LEAKS := $(valgrind --tool=memcheck --leak-check=yes ./build/$(TETRIS_BIN))
endif

$(TEST_DIR)/%.bin: $(TEST_DIR)/%.c $(TETRIS_BIN)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LDFLAGS) $(LDFLAGS)

all: $(ALL)

build: $(TETRIS_BIN)

tests: $(TEST_BINS)

test: $(TEST_BINS)
	@for test in $(TEST_BINS); do $$test ; done

LCOV_REPORT  := $(SRCROOT)/s21_tetris.lcov_report
COV_HTML_OUT := $(SRCROOT)/out

CLEAN += \
	$(wildcard $(SRCROOT)/*.gcov $(TEST_DIR)/*.gcov) \
	$(wildcard $(SRCROOT)/*.gcda $(TEST_DIR)/*.gcda) \
	$(wildcard $(SRCROOT)/*.gcno $(TEST_DIR)/*.gcno) \
	$(TETRIS_DIR)/brick_game/tetris/*.gcov \
	$(TETRIS_DIR)/brick_game/tetris/*.gcda \
	$(TETRIS_DIR)/brick_game/tetris/*.gcno \
	$(TETRIS_DIR)/gui/cli/*.gcov \
	$(TETRIS_DIR)/gui/cli/*.gcda \
	$(TETRIS_DIR)/gui/cli/*.gcno \
	$(TETRIS_DIR)/*.o \
	$(TETRIS_DIR)/*.json \
	$(TETRIS_DIR)/data \
	$(TETRIS_DIR)/tetris_game \
	$(COV_HTML_OUT) \
	$(LCOV_REPORT)

gcov_report:
	$(MAKE) clean
	$(MAKE) BUILD_CONFIG=DEBUG WITH_COVERAGE=yes all tests
	$(MAKE) test
	lcov -t s21_tetris -o $(LCOV_REPORT) -c -d .
	genhtml -o $(COV_HTML_OUT) $(LCOV_REPORT)
	open out/index.html

install: build | build_dir
	$(CC) $(CFLAGS) $(TETRIS_BIN) -o $(BUILD_DIR)/tetris_game $(TEST_LDFLAGS)

uninstall: clean
	rm -rf $(BUILD_DIR) $(DOCS_DIR) $(DIST_DIR)

dist: install dist_dir
	doxygen
	tar cf $(DIST_DIR)/tetris_game.tar $(BUILD_DIR) $(DOCS_DIR)

dvi:
	doxygen
	open docs/html/index.html

build_dir:
	mkdir -p $(BUILD_DIR)

dist_dir:
	mkdir -p $(DIST_DIR)

clean:
	$(RM) $(CLEAN)

lint: run-clang-format run-cppcheck

format fmt: run-clang-reformat

leaks:
	$(TEST_LEAKS)

.PHONY: $(PHONY)
