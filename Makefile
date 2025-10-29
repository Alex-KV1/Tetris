OS_NAME = $(shell uname)
ifeq ($(OS_NAME),Linux)
OPEN = xdg-open
endif

ifeq ($(OS_NAME),Darwin)
OPEN = open
endif

DEBUG_FLAGS := valgrind -q -s --leak-check=full --trace-children=yes --show-leak-kinds=all --track-origins=yes  --log-file=RESULT_VALGRIND.txt ./tests/tests

CC := gcc
CFLAGS := -Wextra -Wall -Werror -std=c99 -Wpedantic
# -Wpedantic
SOURCE_FILES := $(wildcard brick_game/tetris/*.c) $(wildcard gui/cli/*.c)
OBJECT_FILES := $(SOURCE_FILES:.c=.o)
TEST_SOURCES := $(wildcard tests/*.c) $(wildcard gui/cli/*.c) brick_game/tetris/fsm_matrix.c brick_game/tetris/tetris_backend.c
TEST_OBJECTS := $(TEST_SOURCES:.c=.o)

TESTFLAGS:= -Wextra -Wall -Werror -std=c99 -Wpedantic -fprofile-arcs -ftest-coverage


.PHONY: all clean clean_all
all : install

install: build clean


build: $(OBJECT_FILES)
	mkdir -p Tetris || true
	$(CC) $(CFLAGS) $^ -o Tetris/tetris -lncurses

%.o: %.с
	$(CC) $(CFLAGS) -c $< -o $@



clean_all: clean clean_tests

clean:
	rm $(OBJECT_FILES) || true
	
uninstall: clean_all 
	rm ./Tetris/* || true
	rmdir Tetris || true

rebuild: clean_all tetris

test: o_test
	./o_test

.PHONY: gcov_report
gcov_report: test
	$(CC) --coverage $(TEST_SOURCES) -o tests/s21_test -lcheck -lm -lsubunit  -lncurses -lgcov
	./tests/s21_test
	lcov -t "s21_test" -o tests/s21_test.info -c -d .
	genhtml -o report tests/s21_test.info

dist: install
	mkdir tetris-1.0.0 || true
	cp Tetris/tetris tetris-1.0.0
	tar -czvf tetris-1.0.0.tar.gz tetris-1.0.0
	rm -rf tetris-1.0.0 || true


dvi: 
	doxygen
	open docs/html/index.html

o_test:
	$(CC) $(TESTFLAGS) $(TEST_SOURCES) -o $@ -lcheck -lncurses -lm -lsubunit -lm 

clean_tests:
	rm -f o_te*.* || true
	rm -f o_test || true
	rm -fr ./report/* || true
	rm -f ./tests/s21* || true
	rm -f score.txt || true
	rmdir report || true
